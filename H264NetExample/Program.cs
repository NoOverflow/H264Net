using H264Net;
using H264Net.Encoder;
using SIPSorcery.Media;
using SIPSorcery.Net;
using SIPSorceryMedia.Abstractions.V1;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Threading;
using System.Threading.Tasks;
using WebSocketSharp.Server;

namespace H264NetExample
{
    class Program
    {
        private const int WEBSOCKET_PORT = 8081;
        private const string STUN_URL = "stun:stun.sipsorcery.com";

        private static int _frameCount = 0;
        private static DateTime _startTime;
        private static Encoder encoder;
        private static RTCPeerConnection peer = null;

        private static Task<RTCPeerConnection> CreatePeerConnection()
        {
            RTCConfiguration config = new RTCConfiguration
            {
                iceServers = new List<RTCIceServer> { new RTCIceServer { urls = STUN_URL } }
            };
            //var pc = new RTCPeerConnection(config);
            var pc = peer = new RTCPeerConnection(null);

            var testPatternSource = new VideoTestPatternSource();
            
            testPatternSource.SetFrameRate(60);
            testPatternSource.SetMaxFrameRate(true);
            var videoEndPoint = new SIPSorceryMedia.FFmpeg.FFmpegVideoEndPoint();
            videoEndPoint.RestrictFormats(format => format.Codec == VideoCodecsEnum.H264);
            //var videoEndPoint = new SIPSorceryMedia.Windows.WindowsVideoEndPoint(true);
            //var videoEndPoint = new SIPSorceryMedia.Windows.WindowsEncoderEndPoint();
            //var videoEndPoint = new SIPSorceryMedia.Encoders.VideoEncoderEndPoint();

            MediaStreamTrack track = new MediaStreamTrack(videoEndPoint.GetVideoSourceFormats(), MediaStreamStatusEnum.SendOnly);
            pc.addTrack(track);

            testPatternSource.OnVideoSourceRawSample += TestPatternSource_OnVideoSourceRawSample;
            
            pc.OnVideoFormatsNegotiated += (formats) =>
            {
                videoEndPoint.SetVideoSourceFormat(formats.First());
            };


            pc.onconnectionstatechange += async (state) =>
            {
           
                if (state == RTCPeerConnectionState.failed)
                {
                    pc.Close("ice disconnection");
                }
                else if (state == RTCPeerConnectionState.closed)
                {
                    await testPatternSource.CloseVideo();
                    await videoEndPoint.CloseVideo();
                }
                else if (state == RTCPeerConnectionState.connected)
                {
                    await videoEndPoint.StartVideo();
                    await testPatternSource.StartVideo();
                }
            };

            return Task.FromResult(pc);
        }

        private static void TestPatternSource_OnVideoSourceRawSample(uint durationMilliseconds, int width, int height, byte[] sample, SIPSorceryMedia.Abstractions.V1.VideoPixelFormatsEnum pixelFormat)
        {
            if (_startTime == DateTime.MinValue)
            {
                _startTime = DateTime.Now;
            }

            _frameCount++;

            if (DateTime.Now.Subtract(_startTime).TotalSeconds > 5)
            {
                double fps = _frameCount / DateTime.Now.Subtract(_startTime).TotalSeconds;
                Console.WriteLine($"Frame rate {fps:0.##}fps.");
                _startTime = DateTime.Now;
                _frameCount = 0;
            }

            var encodeResult = encoder.EncodeFrameH264(sample);

            peer.SendVideo(3000, encodeResult.frameBuffer);
        }

        static void Main(string[] args)
        {
            Console.WriteLine("WebRTC Test Pattern Server Demo");
            CancellationTokenSource cts = new CancellationTokenSource();

            // Start web socket.
            Console.WriteLine("Starting web socket server...");
            var webSocketServer = new WebSocketServer(IPAddress.Any, WEBSOCKET_PORT);
            webSocketServer.AddWebSocketService<WebRTCWebSocketPeer>("/", (peer) => peer.CreatePeerConnection = CreatePeerConnection);
            webSocketServer.Start();

            Console.WriteLine($"Waiting for web socket connections on {webSocketServer.Address}:{webSocketServer.Port}...");
            Console.WriteLine("Press ctrl-c to exit.");

            ManualResetEvent exitMre = new ManualResetEvent(false);
            Console.CancelKeyPress += delegate (object sender, ConsoleCancelEventArgs e)
            {
                e.Cancel = true;
                cts.Cancel();
                exitMre.Set();
            };
            
            ParamBase encoderParams = new ParamBase();

            encoderParams.MaxFrameRate = 60.0F;
            encoderParams.PicWidth = 640;
            encoderParams.PicHeight = 480;
            encoderParams.RateControl = RateControlModes.RC_BITRATE_MODE;
            encoderParams.TargetBitrate = 5000000;
            encoderParams.UsageType = UsageType.CAMERA_VIDEO_REAL_TIME;

            SourcePicture sourcePicture = new SourcePicture();

            sourcePicture.PicWidth = 640;
            sourcePicture.PicHeight = 480;
            sourcePicture.ColorFormat = (int)VideoFormats.VideoFormatI420;
            encoder = new Encoder();

            encoder.KeyframeInterval = 5;
            encoder.Initialize(encoderParams);
            encoder.SetSourcePictureParameters(sourcePicture);

            exitMre.WaitOne();
            encoder.Dispose();
        }
    }
}
