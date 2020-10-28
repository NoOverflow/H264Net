using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace H264Net.Encoder
{
    public class Encoder
    {
        #region DLL Imports
        [DllImport("H264NetInvokeLayer", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern int InitializeEncoder(IntPtr encoder, IntPtr pParam);

        [DllImport("H264NetInvokeLayer", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern int CreateEncoder(ref IntPtr encoder);
        #endregion

        private IntPtr EncoderHandle = IntPtr.Zero;

        public Encoder()
        {
            GCHandle retHandle = GCHandle.Alloc(EncoderHandle, GCHandleType.Pinned);
            int retCode = 0;

            if ((retCode = CreateEncoder(ref EncoderHandle)) != 0)
                throw new Exception("Couldn't initialize encoder.");
            Console.WriteLine("Encoder handle is: {0}", retHandle.AddrOfPinnedObject().ToString());
        }

        public void Initialize(ParamBase baseParameters)
        {
            Console.WriteLine("Initializing encoder...");
            IntPtr structPtr = Marshal.AllocCoTaskMem(Marshal.SizeOf(baseParameters));

            Marshal.StructureToPtr(baseParameters, structPtr, false);
            int retCode = InitializeEncoder(EncoderHandle, structPtr);
            Marshal.FreeCoTaskMem(structPtr);

            if (retCode != 0)
                throw new Exception("Failed to initialize encoder.");
        }
    }
}
