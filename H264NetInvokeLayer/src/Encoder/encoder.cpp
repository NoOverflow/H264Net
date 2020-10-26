#include "../include/Cisco/codec_api.h"
#include <iostream>

int InitializeEncoder(ISVCEncoder *encoder, const SEncParamBase* pParam)
{
	int result = encoder->Initialize(pParam);

	std::cout << "SUCCESS!";
	return (result);
}