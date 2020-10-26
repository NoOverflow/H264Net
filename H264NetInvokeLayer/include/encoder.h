
#ifndef H264NET_ENCODER_H__
#define H264NET_ENCODER_H__

int InitializeEncoder(ISVCEncoder*, const SEncParamBase*);

int UnInitializeEncoder(ISVCEncoder* encoder);

int EncodeFrame(ISVCEncoder* encoder, const SSourcePicture* kpSrcPic, SFrameBSInfo* pBsInfo);

int EncodeParameterSets(ISVCEncoder* encoder, SFrameBSInfo* pBsInfo);

int ForceIntraFrame(ISVCEncoder* encoder, bool bIDR, int iLayerId = -1);

int CreateEncoder(ISVCEncoder** encoder);

#endif // !H264NET_ENCODER_H__
