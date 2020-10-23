using System;
using Xunit;
using H264Net;

namespace H264NetTests
{
    public class BasicFeatures
    {
        [Fact]
        public void CreateEncoder()
        {
            Encoder encoder = new Encoder(Shared.CiscoDLLPath);

            Assert.NotNull(encoder);
            encoder.Dispose();
        }

        [Fact]
        public void CreateEncoderFromInvalidDLL()
        {
            Assert.Throws<DllNotFoundException>(() => new Encoder("thisdlldoesnotexisthopefully.dll"));
        }

        [Fact]
        public void SetupEncoder()
        {
            Encoder encoder = new Encoder(Shared.CiscoDLLPath);
            Encoder.EncoderBaseParameters param = new Encoder.EncoderBaseParameters();

            param.RCMode = Encoder.RateControlModes.RC_BITRATE_MODE_POST_SKIP;
            param.UsageType = Encoder.UsageType.CAMERA_VIDEO_REAL_TIME;
            encoder.Setup(param);
            encoder.Dispose();
        }

        [Fact]
        public void SetupSourceParams()
        {
            Encoder encoder = new Encoder(Shared.CiscoDLLPath);

            encoder.SetSourcePictureParameters(1920, 1080, Encoder.VideoFormatType.videoFormatI420);
            encoder.Dispose();
        }
    }
}
