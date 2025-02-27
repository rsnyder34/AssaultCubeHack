#include "draw.h"

#include "draw.h"

HWND hwnd;
bool bOpen = true;
ImGuiWindowFlags flags = 0;
const ImGuiViewport* viewPort;
ImDrawList* dl;


bool imgui::init()
{
	hwnd = FindWindowA(0, "AssaultCube");
	
	if (!hwnd)
	{
		error("Failed to find game window");
		return false;
	}
	ImGui::CreateContext();
	ImGui_ImplWin32_InitForOpenGL(hwnd);
	ImGui_ImplOpenGL2_Init();
	viewPort = ImGui::GetMainViewport();
	flags |= ImGuiWindowFlags_NoBackground;
	
	return true;
}

void imgui::startFrame()
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowPos(ImVec2(viewPort->WorkPos.x, viewPort->WorkPos.y));
	ImGui::SetNextWindowSize(ImVec2(800,600));
	ImGui::Begin("Ryan's ESP", &bOpen, flags);
	dl = ImGui::GetBackgroundDrawList();
}

void imgui::endFrame()
{
	ImGui::End();
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void imgui::drawBox(ImVec2 top, ImVec2 bot, ImU32 color)
{
	int height = top.y - bot.y;
	if (height < 0)
		height *= -1;

	ImVec2 tl, tr;
	ImVec2 bl, br;
	tl.x = top.x - height / 4;
	tr.x = top.x + height / 4;
	tl.y = tr.y = top.y;
	bl.x = bot.x - height / 4;
	br.x = bot.x + height / 4;
	bl.y = br.y = bot.y;

	dl->AddLine(tl, tr, color);
	dl->AddLine(bl, br, color);
	dl->AddLine(tr, br, color);
	dl->AddLine(tl, bl, color);
}