#include "pch.h"
#include "Encoder.h"
#include <vcclr.h>

using namespace System;

namespace H264Net
{
	Encoder::Encoder(String ^DllName)
	{
		ISVCEncoder* enc = nullptr;
		pin_ptr<const wchar_t> dllname = PtrToStringChars(DllName);
		HMODULE hDll = LoadLibrary(dllname);

		if (!hDll) 
			throw gcnew System::DllNotFoundException(String::Format("Unable to load '{0}'", DllName));
		dllname = nullptr;
		CreateEncoder = (WelsCreateSVCEncoderDef)GetProcAddress(hDll, "WelsCreateSVCEncoder");
		if (!CreateEncoder)
			throw gcnew System::DllNotFoundException(String::Format("Unable to load WelsCreateSVCEncoder func in '{0}'", DllName));
		DestroyEncoder = (WelsDestroySVCEncoderDef)GetProcAddress(hDll, "WelsDestroySVCEncoder");
		if (!DestroyEncoder)
			throw gcnew System::DllNotFoundException(String::Format("Unable to load WelsDestroySVCEncoder func in '{0}'"));
		if (CreateEncoder(&enc) != 0)
			throw gcnew System::Exception("Unable to create the encoder");
		FrameInfo = new SFrameBSInfo();
		CiscoEncoder = enc;
	}

	Encoder::EncodedFrame ^Encoder::EncodeYUV(array<Byte> ^rawYUV)
	{
		if (!CiscoEncoder)
			throw gcnew System::Exception("The encoder has not been initialized, or the DLL provided is invalid.");
		pin_ptr<Byte> ptr = &rawYUV[0];
		EncodedFrame ^ret = Encode(ptr, rawYUV->Length);
		ptr = nullptr;
		return (ret);
	}

	void Encoder::Setup(H264Net::Encoder::EncoderBaseParameters baseParameters)
	{
		SEncParamBase param;

		if (CreateEncoder == NULL || DestroyEncoder == NULL)
			throw gcnew System::Exception("The DLL provided is either invalid or failed to load.");
		if (!CiscoEncoder)
			throw gcnew System::Exception("The encoder has not been created.");

		memset(&param, 0, sizeof(SEncParamBase));
		param.iUsageType = (EUsageType)baseParameters.UsageType;
		param.fMaxFrameRate = baseParameters.MaxFrameRate;
		param.iPicWidth = baseParameters.PicWidth;
		param.iPicHeight = baseParameters.PicHeight;
		param.iTargetBitrate = baseParameters.TargetBitrate;
		KeyFrameInterval = baseParameters.KeyFrameInterval * param.fMaxFrameRate;
		CiscoEncoder->Initialize(&param);
	}

	void Encoder::Setup(H264Net::Encoder::EncoderExtendedParameters extendedParameters)
	{
		SEncParamExt param;

		if (CreateEncoder == NULL || DestroyEncoder == NULL)
			throw gcnew System::Exception("The DLL provided is either invalid or failed to load.");
		if (!CiscoEncoder)
			throw gcnew System::Exception("The encoder has not been created.");

		memset(&param, 0, sizeof(SEncParamBase));
		
		param.iUsageType = (EUsageType)extendedParameters.UsageType;
		param.fMaxFrameRate = extendedParameters.MaxFrameRate;
		param.iPicWidth = extendedParameters.PicWidth;
		param.iPicHeight = extendedParameters.PicHeight;
		param.iTargetBitrate = extendedParameters.TargetBitrate;
		param.eSpsPpsIdStrategy = INCREASING_ID;
		param.uiIntraPeriod = 2;
		KeyFrameInterval = extendedParameters.KeyFrameInterval * param.fMaxFrameRate;
		CiscoEncoder->InitializeExt(&param);
	}

	void Encoder::SetSourcePictureParameters(int width, int height, H264Net::Encoder::VideoFormatType videoFormat)
	{
		BufferSize = width * height * 3 / 2;
		FrameBuffer = new unsigned char[BufferSize];
		SourcePicture = new SSourcePicture();
		SourcePicture->iPicWidth = width;
		SourcePicture->iPicHeight = height;
		SourcePicture->iColorFormat = (int)videoFormat;
		SourcePicture->iStride[0] = SourcePicture->iPicWidth;
		SourcePicture->iStride[1] = SourcePicture->iStride[2] = SourcePicture->iPicWidth >> 1;
		SourcePicture->pData[0] = FrameBuffer;
		SourcePicture->pData[1] = SourcePicture->pData[0] + width * height;
		SourcePicture->pData[2] = SourcePicture->pData[1] + (width * height >> 2);
		FrameInfo = new SFrameBSInfo();
	}
	 
	Encoder::EncodedFrame ^Encoder::Encode(unsigned char *raw, int length)
	{
		EncodedFrame ^returnFrame = gcnew EncodedFrame();

		if (!CiscoEncoder)
			throw gcnew System::Exception("The encoder has not been initialized, or the DLL provided is invalid.");
		if (length != BufferSize)
			throw gcnew System::Exception(System::String::Format("Image provided is of incorrect size, expected {0}. Did you forget to call SetSourcePictureParameters() ?", BufferSize));
		
		if ( false && FramesEncoded++ % KeyFrameInterval == 0)
			CiscoEncoder->ForceIntraFrame(true);
		CiscoEncoder->ForceIntraFrame(true);
		memcpy(FrameBuffer, raw, length);
		if (CiscoEncoder->EncodeFrame(SourcePicture, FrameInfo) != 0)
			throw gcnew System::Exception("Failed to encode frame.");

		returnFrame->FrameType = (VideoFrameType)FrameInfo->eFrameType;
		returnFrame->TimeStamp = FrameInfo->uiTimeStamp;
		returnFrame->FrameSizeInBytes = FrameInfo->iFrameSizeInBytes;
		returnFrame->FrameData = gcnew array<Byte>(FrameInfo->iFrameSizeInBytes);
		
		int currentArrayIndex = 0;
		for (int iLayer = 0; iLayer < FrameInfo->iLayerNum; iLayer++)
		{
			SLayerBSInfo* pLayerBsInfo = &FrameInfo->sLayerInfo[iLayer];
			int iLayerSize = 0;
			int iNalIdx = pLayerBsInfo->iNalCount - 1;

			do {
				iLayerSize += pLayerBsInfo->pNalLengthInByte[iNalIdx];
				--iNalIdx;
			} while (iNalIdx >= 0);

			System::Runtime::InteropServices::Marshal::Copy((IntPtr)pLayerBsInfo->pBsBuf, returnFrame->FrameData, currentArrayIndex, iLayerSize);
			currentArrayIndex += iLayerSize;
		}
		return (returnFrame);
	}

	Encoder::~Encoder()
	{
		this->!Encoder();
	}

	Encoder::!Encoder()
	{
		if (!CiscoEncoder)
			return;
		CiscoEncoder->Uninitialize();
		DestroyEncoder(CiscoEncoder);
		delete FrameBuffer;
		delete FrameInfo;
		delete SourcePicture;
	}
}