#include "mem.h"

//for read write permissions
void mem::Patch(char* dst, char* src, unsigned int len)
{
	DWORD temp;
	VirtualProtect(dst, len, PAGE_EXECUTE_READWRITE, &temp);
	memcpy(dst, src, len);
	VirtualProtect(dst, len, temp, &temp);
}

//uses a 5 byte jmp instruction to redirect function call to our own
//copies the original bytes to be called in allocate memory
//this is why the hook calls the og function so no bytes get mangled
bool mem::TrampHook32(char* src, char* dst, unsigned int len, void** oldFunction)
{
	if (len < 5)
	{
		error("Len of hook must at least be 5 bytes");
		return false;
	}

	char* proxy = (char*)VirtualAlloc(0, len + 10, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!proxy)
	{
		error("Failed to allocate proxy memory while hooking");
		return false;
	}
	memcpy(proxy, src, len);
	*(proxy + len) = 0xE9;
	uintptr_t relAddr = src - proxy - 5;
	*(uintptr_t*)(proxy + len + 1) = relAddr;

	DWORD temp;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &temp);
	memset(src, 0x90, len);
	*src = 0xE9;
	relAddr = dst - src - 5;
	*(uintptr_t*)(src + 1) = relAddr;

	*(void**)oldFunction = proxy;

	return true;
}