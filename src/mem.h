#pragma once
#include "includes.h"

namespace mem
{
	void Patch(char* dst, char* src, unsigned int len);

	bool TrampHook32(char* src, char* dst, unsigned int len, void** oldFunction);
}