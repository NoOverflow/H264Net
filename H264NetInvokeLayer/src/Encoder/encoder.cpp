#include "../include/Cisco/codec_api.h"
#include <iostream>

int EXTAPI InitializeEncoder(ISVCEncoder *encoder, const SEncParamBase* pParam)
{
	int result = encoder->Initialize(pParam);

	return (result);
}

int EXTAPI UnInitializeEncoder(ISVCEncoder* encoder)
{
	return (encoder->Uninitialize());
}

int EXTAPI EncodeFrame(ISVCEncoder* encoder, const SSourcePicture* kpSrcPic, SFrameBSInfo* pBsInfo)
{
	return (encoder->EncodeFrame(kpSrcPic, pBsInfo));
}

int EXTAPI EncodeParameterSets(ISVCEncoder* encoder, SFrameBSInfo* pBsInfo)
{
	return (encoder->EncodeParameterSets(pBsInfo));
}

int EXTAPI ForceIntraFrame(ISVCEncoder* encoder, bool bIDR, int iLayerId = -1)
{
	return (encoder->ForceIntraFrame(bIDR, iLayerId));
}