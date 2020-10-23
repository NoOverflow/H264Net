#pragma once

#include "Cisco/codec_api.h"
#include "Cisco/codec_app_def.h"

using namespace System;

namespace H264Net
{
	public ref class Encoder
	{
	public:
		#pragma region Cisco Managed Structures
#define MAX_SLICES_NUM_TMP              ( ( MAX_NAL_UNITS_IN_LAYER - SAVED_NALUNIT_NUM_TMP ) / 3 )

		enum class VideoFrameType {
			videoFrameTypeInvalid,    ///< encoder not ready or parameters are invalidate
			videoFrameTypeIDR,        ///< IDR frame in H.264
			videoFrameTypeI,          ///< I frame type
			videoFrameTypeP,          ///< P frame type
			videoFrameTypeSkip,       ///< skip the frame based encoder kernel
			videoFrameTypeIPMixed     ///< a frame where I and P slices are mixing, not supported yet
		};

		ref struct EncodedFrame {
			int LayerNum;
			array<Byte>^ FrameData;
			VideoFrameType FrameType;
			int FrameSizeInBytes;
			long long TimeStamp;
		};

		enum class UsageType {
			CAMERA_VIDEO_REAL_TIME,      ///< camera video for real-time communication
			SCREEN_CONTENT_REAL_TIME,    ///< screen content signal
			CAMERA_VIDEO_NON_REAL_TIME
		};

		enum class VideoFormatType {
			videoFormatRGB = 1,             ///< rgb color formats
			videoFormatRGBA = 2,
			videoFormatRGB555 = 3,
			videoFormatRGB565 = 4,
			videoFormatBGR = 5,
			videoFormatBGRA = 6,
			videoFormatABGR = 7,
			videoFormatARGB = 8,
			videoFormatYUY2 = 20,            ///< yuv color formats
			videoFormatYVYU = 21,
			videoFormatUYVY = 22,
			videoFormatI420 = 23,            ///< the same as IYUV
			videoFormatYV12 = 24,
			videoFormatInternal = 25,            ///< only used in SVC decoder testbed
			videoFormatNV12 = 26,            ///< new format for output by DXVA decoding
			videoFormatVFlip = 0x80000000
		};

		value struct SliceArgument {
			SliceModeEnum SliceMode;    ///< by default, uiSliceMode will be SM_SINGLE_SLICE
			unsigned int SliceNum;     ///< only used when uiSliceMode=1, when uiSliceNum=0 means auto design it with cpu core number
			array<unsigned int>^ SliceMbNum; ///< only used when uiSliceMode=2; when =0 means setting one MB row a slice
			unsigned int  uiSliceSizeConstraint; ///< now only used when uiSliceMode=4
		};

		value struct SpatialLayerConfig {
			int VideoWidth;           ///< width of picture in luminance samples of a layer
			int VideoHeight;          ///< height of picture in luminance samples of a layer
			float FrameRate;            ///< frame rate specified for a layer
			int SpatialBitrate;       ///< target bitrate for a spatial layer, in unit of bps
			int MaxSpatialBitrate;    ///< maximum  bitrate for a spatial layer, in unit of bps
			EProfileIdc ProfileIdc;   ///< value of profile IDC (PRO_UNKNOWN for auto-detection)
			ELevelIdc LevelIdc;     ///< value of profile IDC (0 for auto-detection)
			int DLayerQp;      ///< value of level IDC (0 for auto-detection)

			SliceArgument SliceArguments;

			// Note: members bVideoSignalTypePresent through uiColorMatrix below are also defined in SWelsSPS in parameter_sets.h.
			bool VideoSignalTypePresent;	// false => do not write any of the following information to the header
			unsigned char VideoFormat;				// EVideoFormatSPS; 3 bits in header; 0-5 => component, kpal, ntsc, secam, mac, undef
			bool FullRange;					// false => analog video data range [16, 235]; true => full data range [0,255]
			bool ColorDescriptionPresent;	// false => do not write any of the following three items to the header
			unsigned char ColorPrimaries;			// EColorPrimaries; 8 bits in header; 0 - 9 => ???, bt709, undef, ???, bt470m, bt470bg,
													  //    smpte170m, smpte240m, film, bt2020
			unsigned char TransferCharacteristics;	// ETransferCharacteristics; 8 bits in header; 0 - 15 => ???, bt709, undef, ???, bt470m, bt470bg, smpte170m,
													  //   smpte240m, linear, log100, log316, iec61966-2-4, bt1361e, iec61966-2-1, bt2020-10, bt2020-12
			unsigned char ColorMatrix;				// EColorMatrix; 8 bits in header (corresponds to FFmpeg "colorspace"); 0 - 10 => GBR, bt709,
													  //   undef, ???, fcc, bt470bg, smpte170m, smpte240m, YCgCo, bt2020nc, bt2020c
			bool AspectRatioPresent; ///< aspect ratio present in VUI
			ESampleAspectRatio AspectRatio; ///< aspect ratio idc
			unsigned short AspectRatioExtWidth; ///< use if aspect ratio idc == 255
			unsigned short AspectRatioExtHeight; ///< use if aspect ratio idc == 255
		};

		enum class RateControlModes {
			RC_QUALITY_MODE = 0,     ///< quality mode
			RC_BITRATE_MODE = 1,     ///< bitrate mode
			RC_BUFFERBASED_MODE = 2, ///< no bitrate control,only using buffer status,adjust the video quality
			RC_TIMESTAMP_MODE = 3, //rate control based timestamp
			RC_BITRATE_MODE_POST_SKIP = 4, ///< this is in-building RC MODE, WILL BE DELETED after algorithm tuning!
			RC_OFF_MODE = -1,         ///< rate control off mode
		};

		value struct EncoderBaseParameters {
			UsageType UsageType;                 ///< application type;1.CAMERA_VIDEO_REAL_TIME:camera video signal; 2.SCREEN_CONTENT_REAL_TIME:screen content signal;
			int PicWidth;        ///< width of picture in luminance samples (the maximum of all layers if multiple spatial layers presents)
			int PicHeight;       ///< height of picture in luminance samples((the maximum of all layers if multiple spatial layers presents)
			int TargetBitrate;   ///< target bitrate desired, in unit of bps
			RateControlModes RCMode;          ///< rate control mode
			float MaxFrameRate;    ///< maximal input frame rate
			int KeyFrameInterval;
		};

		value struct EncoderExtendedParameters {
			UsageType UsageType;                 ///< application type;1.CAMERA_VIDEO_REAL_TIME:camera video signal; 2.SCREEN_CONTENT_REAL_TIME:screen content signal;
			int PicWidth;        ///< width of picture in luminance samples (the maximum of all layers if multiple spatial layers presents)
			int PicHeight;       ///< height of picture in luminance samples((the maximum of all layers if multiple spatial layers presents)
			int TargetBitrate;   ///< target bitrate desired, in unit of bps
			RateControlModes RCMode;          ///< rate control mode
			float MaxFrameRate;    ///< maximal input frame rate
			int KeyFrameInterval;
		};

#pragma endregion

	public:
		Encoder::EncodedFrame ^EncodeYUV(array<Byte>^ rawYUV);

		void SetSourcePictureParameters(int width, int height, H264Net::Encoder::VideoFormatType videoFormat);

		void Setup(H264Net::Encoder::EncoderExtendedParameters extendedParameters);

		void Setup(H264Net::Encoder::EncoderBaseParameters baseParameters);

		Encoder(String^ DllName);

	private:
		ISVCEncoder* CiscoEncoder;
		SFrameBSInfo* FrameInfo;
		SSourcePicture* SourcePicture;

		int BufferSize;
		unsigned char* FrameBuffer;
		int KeyFrameInterval;
		int FramesEncoded;

		Encoder::EncodedFrame ^Encode(unsigned char *raw, int length);

		~Encoder();
		!Encoder();

#pragma region Cisco typedefs 
		typedef int(__stdcall* WelsCreateSVCEncoderDef)(ISVCEncoder** Encoder);
		typedef void(__stdcall* WelsDestroySVCEncoderDef)(ISVCEncoder* Encoder);

		WelsCreateSVCEncoderDef CreateEncoder;
		WelsDestroySVCEncoderDef DestroyEncoder;
#pragma endregion
	};
}


