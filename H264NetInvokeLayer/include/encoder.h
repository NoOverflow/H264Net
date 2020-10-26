
#ifndef H264NET_ENCODER_H__
#define H264NET_ENCODER_H__

int EXTAPI InitializeEncoder(ISVCEncoder*, const SEncParamBase*);

int EXTAPI UnInitializeEncoder(ISVCEncoder* encoder);

int EXTAPI EncodeFrame(ISVCEncoder* encoder, const SSourcePicture* kpSrcPic, SFrameBSInfo* pBsInfo);

int EXTAPI EncodeParameterSets(ISVCEncoder* encoder, SFrameBSInfo* pBsInfo);

int EXTAPI ForceIntraFrame(ISVCEncoder* encoder, bool bIDR, int iLayerId = -1);

#endif // !H264NET_ENCODER_H__
