using System;
using Xunit;
using H264Net;
using H264Net.Encoder;

namespace H264NetTests
{
    public class BasicFeatures
    {
        [Fact]
        public void CreateEncoder()
        {
            Encoder encoder = new Encoder();

            Assert.NotNull(encoder);
            encoder.Dispose();
        }

        [Fact]
        public void SetSourcePictureParameters()
        {
            /*Encoder encoder = new Encoder();
            SourcePicture sourcePicture = new SourcePicture();

            sourcePicture.PicWidth = 640;
            sourcePicture.PicHeight = 480;
            sourcePicture.ColorFormat = (int)VideoFormats.VideoFormatI420;
            encoder.SetSourcePictureParameters(sourcePicture);
            encoder.Dispose();*/
        }

        [Fact]
        public void InitializeEncoder()
        {
            /* Encoder encoder = new Encoder();
            SourcePicture sourcePicture = new SourcePicture();

            sourcePicture.PicWidth = 640;
            sourcePicture.PicHeight = 480;
            sourcePicture.ColorFormat = (int)VideoFormats.VideoFormatI420;
            encoder.SetSourcePictureParameters(sourcePicture);

            ParamBase encoderParams = new ParamBase();

            encoderParams.MaxFrameRate = 60.0F;
            encoderParams.PicWidth = 640;
            encoderParams.PicHeight = 480;
            encoderParams.RateControl = RateControlModes.RC_BITRATE_MODE;
            encoderParams.TargetBitrate = 5000000;
            encoderParams.UsageType = UsageType.CAMERA_VIDEO_REAL_TIME;

            encoder.Initialize(encoderParams);
            encoder.Dispose();*/
        }
    }
}
