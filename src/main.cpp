#include "includes.h" 

//Setting up hook and globals
typedef bool(__stdcall* _wglSwapBuffers)(HDC hdc);
_wglSwapBuffers oSwap;
bool init = false;
Game* game = nullptr;
Player* p = nullptr;

bool __stdcall hook(HDC hdc)
{	
	//initialize hook
	if (!init)
	{
		//initialize graphics (ImGUI)
		if (!imgui::init())
		{
			error("ImGui initialization failed");
			system("pause");
			exit(-4);
		}
		init = true;
	}
	//start of frame
	imgui::startFrame();
	
	//hacks
	game->aimbot();
	game->esp();
	game->GodMode();
	//end the frame
	imgui::endFrame();


	//return to original swapbuffers
	return oSwap(hdc);
}


void __stdcall MainThread(HMODULE hMod)
{
	//for debugging
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	//find and hook SwapBuffers function in the OpenGL lib
	void* tarFunc = nullptr;
	HMODULE hOpenGL = GetModuleHandleA("opengl32.dll");
	if (!hOpenGL)
	{
		error("Failed to get handle to opengl32.dll");
		system("pause");
		exit(-1);
	}
	tarFunc = GetProcAddress(hOpenGL, "wglSwapBuffers");
	if (!tarFunc)
	{
		error("Failed to get address of wglSwapBuffers");
		system("pause");
		exit(-2);
	}
	//Save the old bytes for exit
	char oldBytes[5];
	memcpy(oldBytes, (void*)tarFunc, 5);

	//initialize tools
	game = new Game();

	//trampoline hook, not viable for x64
	if (!mem::TrampHook32((char*)tarFunc, (char*)hook, 5, (void**)&oSwap))
	{
		error("Failed to hook swap buffers");
		system("pause");
		exit(-3);
	}

	while (!GetAsyncKeyState(VK_END))
	{
		//Graphics need to be update constantly
		game->updateViewMatrix();
	}
	//patching memory
	mem::Patch((char*)tarFunc, oldBytes, 5);
	delete game;
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hMod, 0);
}

//dll entry
bool __stdcall DllMain(HMODULE hMod, DWORD reason, void* lpr)
{
	if (reason == 1)
	{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hMod, 0, 0);
	}

	return true;
}
