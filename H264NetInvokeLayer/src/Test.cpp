#include <iostream>
#include "../include/Cisco/codec_api.h"
#include "../include/encoder.h"

void main(int argc, char** argv)
{
	ISVCEncoder* testEncoder = NULL;

	std::cout << "test";
	if (WelsCreateSVCEncoder(&testEncoder) != 0)
		std::cout << "Failed to create test encoder";

	SEncParamBase params;

	memset(&params, 0, sizeof(SEncParamBase));
	params.fMaxFrameRate = 30.0F;
	params.iPicHeight = 1080;
	params.iPicWidth = 1820;
	params.iTargetBitrate = 5000000;
	params.iRCMode = RC_BITRATE_MODE;
	params.iUsageType = CAMERA_VIDEO_REAL_TIME;
	InitializeEncoder(testEncoder, &params);
}