#pragma once
#include "includes.h"

extern HWND hwnd;
extern bool bOpen;
extern ImGuiWindowFlags flags;
extern const ImGuiViewport* viewPort;
extern ImDrawList* dl;
namespace imgui
{
	bool init();

	void startFrame();

	void endFrame();

	void drawBox(ImVec2 top, ImVec2 bot, ImU32 color);
}

