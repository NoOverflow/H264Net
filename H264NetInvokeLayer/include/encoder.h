
#ifndef H264NET_ENCODER_H__
#define H264NET_ENCODER_H__

#if WIN32
#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)
#elif ARM
#include <jni.h>
#define EXTERN_DLL_EXPORT extern "C" JNIEXPORT
#endif

typedef struct Source_Picture_Layer_S {
    int       iColorFormat;          ///< color space type
    int *iStride;            ///< stride for each plane pData
    unsigned char** pData;        ///< plane pData
    int       iPicWidth;             ///< luma picture width in x coordinate
    int       iPicHeight;            ///< luma picture height in y coordinate
    long long uiTimeStamp;           ///< timestamp of the source picture, unit: millisecond
} SourcePictureLayer;

EXTERN_DLL_EXPORT int InitializeEncoder(ISVCEncoder*, const SEncParamBase*);

EXTERN_DLL_EXPORT int UnInitializeEncoder(ISVCEncoder* encoder);

EXTERN_DLL_EXPORT int EncodeFrame(ISVCEncoder* encoder, SourcePictureLayer* kpSrcPic, SFrameBSInfo* pBsInfo);

EXTERN_DLL_EXPORT int EncodeParameterSets(ISVCEncoder* encoder, SFrameBSInfo* pBsInfo);

EXTERN_DLL_EXPORT int ForceIntraFrame(ISVCEncoder* encoder, bool bIDR, int iLayerId = -1);

EXTERN_DLL_EXPORT int CreateEncoder(ISVCEncoder** encoder);

#endif // !H264NET_ENCODER_H__
