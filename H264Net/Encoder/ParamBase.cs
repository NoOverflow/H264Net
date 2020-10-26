using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace H264Net.Encoder
{
    public enum UsageType
    {
        CAMERA_VIDEO_REAL_TIME,
        SCREEN_CONTENT_REAL_TIME,
        CAMERA_VIDEO_NON_REAL_TIME
    }

    public enum RateControlModes
    {
        RC_QUALITY_MODE = 0,
        RC_BITRATE_MODE = 1,
        RC_BUFFERBASED_MODE = 2,
        RC_TIMESTAMP_MODE = 3,
        RC_BITRATE_MODE_POST_SKIP = 4,
        RC_OFF_MODE = -1
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct ParamBase
    {
        public UsageType UsageType;
        public int PicWidth;
        public int PicHeight;
        public int TargetBitrate;
        public RateControlModes RateControl;
        public float MaxFrameRate;
    }
}
