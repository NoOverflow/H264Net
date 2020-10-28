using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace H264Net.Encoder
{
    public class Encoder : IDisposable
    {
        #region DLL Imports
        [DllImport("H264NetInvokeLayer", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern int InitializeEncoder(IntPtr encoder, IntPtr pParam);

        [DllImport("H264NetInvokeLayer", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern int CreateEncoder(ref IntPtr encoder);

        [DllImport("H264NetInvokeLayer", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern int EncodeFrame(IntPtr encoder, ref SourcePicture SourcePicture, ref FrameBitstreamInfo FrameInfo);

        [DllImport("H264NetInvokeLayer", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern int ForceIntraFrame(IntPtr encoder, bool bIDR, int iLayerId = -1);

        [DllImport("H264NetInvokeLayer", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern int UnInitializeEncoder(IntPtr encoder);
        #endregion

        private GCHandle EncoderHandle;
        private IntPtr EncoderPointer = IntPtr.Zero;
        private static SourcePicture SourcePicture;
        private IntPtr ImageBufferHandle = IntPtr.Zero;

        #region Exposed Properties
        /// <summary>
        /// Generate an IDR Frames every KeyframeInterval frames, set to -1 to not generate key frames automatically
        /// </summary>
        public int KeyframeInterval { get; set; } = -1;
        #endregion

        public Encoder()
        {
            EncoderHandle = GCHandle.Alloc(EncoderPointer, GCHandleType.Pinned);
            int retCode = 0;

            if ((retCode = CreateEncoder(ref EncoderPointer)) != 0)
                throw new Exception(String.Format("Couldn't initialize encoder. Return code: {0}", retCode));
        }

        // TODO: Simplify using ref
        public void Initialize(ParamBase baseParameters)
        {
            IntPtr structPtr = Marshal.AllocCoTaskMem(Marshal.SizeOf(baseParameters));
            Marshal.StructureToPtr(baseParameters, structPtr, false);
            int retCode = InitializeEncoder(EncoderPointer, structPtr);
            Marshal.FreeCoTaskMem(structPtr);

            if (retCode != 0)
                throw new Exception(String.Format("Failed to initialize encoder. Return code: {0}", retCode));
        }

        public void SetSourcePictureParameters(SourcePicture sourcePicture)
        {
            int width = sourcePicture.PicWidth;
            int height = sourcePicture.PicHeight;
            byte[] buffer = new byte[width * height * 3 / 2];

            if (ImageBufferHandle != IntPtr.Zero)
                Marshal.FreeHGlobal(ImageBufferHandle);
            ImageBufferHandle = Marshal.AllocHGlobal(Marshal.SizeOf(buffer[0]) * width * height * 3 / 2);
            SourcePicture = sourcePicture;

            SourcePicture.Stride = Marshal.AllocHGlobal(Marshal.SizeOf((int)0) * 4);
            var imageStrides = new int[4];
            imageStrides[0] = sourcePicture.PicWidth;
            imageStrides[1] = imageStrides[2] = sourcePicture.PicWidth >> 1;
            Marshal.Copy(imageStrides, 0, SourcePicture.Stride, 4);

            SourcePicture.Data = Marshal.AllocHGlobal(Marshal.SizeOf(IntPtr.Zero) * 4);
            var imageData = new IntPtr[4];
            imageData[0] = ImageBufferHandle;
            imageData[1] = imageData[0] + width * height;
            imageData[2] = imageData[1] + (width * height >> 2);
            Marshal.Copy(imageData, 0, SourcePicture.Data, 4);
            return;
        }

        private byte[] GetFrameBuffer(FrameBitstreamInfo info)
        {
            int currentArrayIndex = 0;
            byte[] ret = new byte[info.FrameSizeInBytes];

            for (int iLayer = 0; iLayer < info.LayerNum; iLayer++)
            {
                LayerBitstreamInfo pLayerBsInfo = info.LayerInfo[iLayer];
                int iLayerSize = 0;
                int iNalIdx = pLayerBsInfo.NalCount - 1;
                int[] nalLengths = new int[4];

                Marshal.Copy(pLayerBsInfo.NalLengthInByte, nalLengths, 0, 4);
                do
                {
                    iLayerSize += nalLengths[iNalIdx];
                    --iNalIdx;
                } while (iNalIdx >= 0);
                Marshal.Copy(pLayerBsInfo.BsBuffer, ret, currentArrayIndex, iLayerSize);
                currentArrayIndex += iLayerSize;
            }
            return (ret);
        }

        private static int FrameIntervalHolder = 0;
        public (byte[] frameBuffer, FrameBitstreamInfo frameInfo) EncodeFrameH264(byte[] imageBuffer)
        {
            FrameBitstreamInfo ret = new FrameBitstreamInfo();

            if (imageBuffer.Length != (SourcePicture.PicWidth * SourcePicture.PicHeight * 3 / 2))
                throw new H264NetEncodingError(String.Format("The input buffer size must be equal to Width * Image * 3 / 2 ({0} bytes)", SourcePicture.PicWidth * SourcePicture.PicHeight * 3 / 2));
            if (ImageBufferHandle == IntPtr.Zero)
                throw new H264NetEncodingError("The image buffer handle has not been initialized did you call SetSourcePictureParameters ?");
            if (KeyframeInterval >= 0 && (FrameIntervalHolder++ > KeyframeInterval))
            {
                ForceIntraFrame(EncoderPointer, true);
                FrameIntervalHolder = 0;
            }
            Marshal.Copy(imageBuffer, 0, ImageBufferHandle, imageBuffer.Length);
            var result = EncodeFrame(EncoderPointer, ref SourcePicture, ref ret);

            if (result != 0)
                throw new H264NetEncodingError(String.Format("Couldn't encode frame. Return code: {0}", result));
            byte[] retFrameBuffer;

            try
            {
                retFrameBuffer = GetFrameBuffer(ret);
            }
            catch (Exception ex)
            {
                throw new H264NetFramebufferError("Failed to retrieve encoded frame buffer", ex);
            }
            return (retFrameBuffer, ret);
        }

        public void ForceIntraFrame(bool IDR, int layerID = -1)
        {
            int result = 0;

            if ((result = ForceIntraFrame(EncoderPointer, true, layerID)) != 0)
                throw new H264NetForceIDRError(String.Format("Couldn't force intra frame. Return code: {0}", result));
        }

        private void FreeSourcePicture(SourcePicture picture)
        {
            try
            {
                if (picture.Stride != IntPtr.Zero)
                    Marshal.FreeHGlobal(SourcePicture.Stride);
                if (picture.Stride != IntPtr.Zero)
                    Marshal.FreeHGlobal(SourcePicture.Data);
            }
            catch (Exception)
            {

            }
        }

        #region IDisposable Support
        private bool disposedValue = false;

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    
                }
                UnInitializeEncoder(EncoderPointer);
                Marshal.FreeHGlobal(ImageBufferHandle);
                EncoderHandle.Free();
                FreeSourcePicture(SourcePicture);
                disposedValue = true;
            }
        }

        ~Encoder()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
        }
        #endregion
    }
}
