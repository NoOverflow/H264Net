using H264Net.Encoder;
using System;

namespace H264NetExample
{
    class Program
    {
        static void Main(string[] args)
        {
            ParamBase encoderParams = new ParamBase();

            encoderParams.MaxFrameRate = 30.0F;
            encoderParams.PicWidth = 1920;
            encoderParams.PicHeight = 1080;
            encoderParams.RateControl = RateControlModes.RC_BITRATE_MODE;
            encoderParams.TargetBitrate = 5000000;
            encoderParams.UsageType = UsageType.SCREEN_CONTENT_REAL_TIME;

            Encoder encoder = new Encoder();
            encoder.Initialize(encoderParams);
            Console.ReadLine();
        }
    }
}
