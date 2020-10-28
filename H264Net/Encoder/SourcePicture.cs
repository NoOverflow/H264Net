using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace H264Net.Encoder
{
    [StructLayout(LayoutKind.Sequential)]
    public struct SourcePicture
    {
        public int ColorFormat;
        public IntPtr Stride;
        public IntPtr Data;
        public int PicWidth;
        public int PicHeight;
        public ulong TimeStamp;
    }
}
