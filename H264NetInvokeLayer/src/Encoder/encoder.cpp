#include "../include/Cisco/codec_api.h"
#include <iostream>
#include "../../include/encoder.h"

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


EXTERN_DLL_EXPORT int EXTAPI EncodeFrame(ISVCEncoder* encoder, SourcePictureLayer* kpSrcPic, SFrameBSInfo* pBsInfo)
{
	SSourcePicture source;

	memset(&source, 0, sizeof(SSourcePicture));
	source.iColorFormat = kpSrcPic->iColorFormat;
	source.iPicHeight = kpSrcPic->iPicHeight;
	source.iPicWidth = kpSrcPic->iPicWidth;
	source.iStride[0] = kpSrcPic->iStride[0];
	source.iStride[1] = kpSrcPic->iStride[1];
	source.iStride[2] = kpSrcPic->iStride[2];
	source.iStride[3] = kpSrcPic->iStride[3];
	source.pData[0] = kpSrcPic->pData[0];
	source.pData[1] = kpSrcPic->pData[1];
	source.pData[2] = kpSrcPic->pData[2];
	source.pData[3] = kpSrcPic->pData[3];
	source.uiTimeStamp = kpSrcPic->uiTimeStamp;
	return (encoder->EncodeFrame(&source, pBsInfo));
}

EXTERN_DLL_EXPORT int EXTAPI EncodeParameterSets(ISVCEncoder* encoder, SFrameBSInfo* pBsInfo)
{
	return (encoder->EncodeParameterSets(pBsInfo));
}

EXTERN_DLL_EXPORT int EXTAPI ForceIntraFrame(ISVCEncoder* encoder, bool bIDR, int iLayerId)
{
	return (encoder->ForceIntraFrame(bIDR, iLayerId));
}