#include "../include/Cisco/codec_api.h"
#include <iostream>

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

EXTERN_DLL_EXPORT int CreateEncoder(ISVCEncoder** encoder)
{
	return (WelsCreateSVCEncoder(encoder));
}

EXTERN_DLL_EXPORT int InitializeEncoder(ISVCEncoder *encoder, const SEncParamBase* pParam)
{
	int result = encoder->Initialize(pParam);

	return (result);
}

EXTERN_DLL_EXPORT int EXTAPI UnInitializeEncoder(ISVCEncoder* encoder)
{
	return (encoder->Uninitialize());
}

EXTERN_DLL_EXPORT int EXTAPI EncodeFrame(ISVCEncoder* encoder, const SSourcePicture* kpSrcPic, SFrameBSInfo* pBsInfo)
{
	return (encoder->EncodeFrame(kpSrcPic, pBsInfo));
}

EXTERN_DLL_EXPORT int EXTAPI EncodeParameterSets(ISVCEncoder* encoder, SFrameBSInfo* pBsInfo)
{
	return (encoder->EncodeParameterSets(pBsInfo));
}

EXTERN_DLL_EXPORT int EXTAPI ForceIntraFrame(ISVCEncoder* encoder, bool bIDR, int iLayerId = -1)
{
	return (encoder->ForceIntraFrame(bIDR, iLayerId));
}