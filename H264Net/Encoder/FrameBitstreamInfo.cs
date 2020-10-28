using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace H264Net.Encoder
{
    public enum VideoFrameType
    {
        VideoFrameTypeInvalid,
        VideoFrameTypeIDR,
        VideoFrameTypeI,
        VideoFrameTypeP,
        VideoFrameTypeSkip,
        VideoFrameTypeIPMixed
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct LayerBitstreamInfo
    {
        public byte TemporalId;
        public byte SpatialId;
        public byte QualityId;
        public VideoFrameType FrameType;
        public byte LayerType;
        public int SubSeqId;
        public int NalCount;
        public IntPtr NalLengthInByte;
        public IntPtr BsBuffer;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct FrameBitstreamInfo
    {
        public int LayerNum;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 128)]
        public LayerBitstreamInfo[] LayerInfo;
        public VideoFrameType FrameType;
        public int FrameSizeInBytes;
        public ulong TimeStamp;
    }
}
