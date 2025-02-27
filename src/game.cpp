#include "game.h"

Game::Game()
{
	gameAddr = (uintptr_t)GetModuleHandleA(nullptr);
	localPlayer = *(Player**)(gameAddr + offsets::localPlayer);
	
	
	entList = *(EntList**)(gameAddr + offsets::entityList);
	playerCount = *(int*)(gameAddr + offsets::playerCount);
	RECT rect;
	GetWindowRect(FindWindowA(0, "AssaultCube"), &rect);
	windowDims.x = rect.right - rect.left;
	windowDims.y = rect.bottom - rect.top;

	
}

Game::~Game()
{
	delete localPlayer;
	delete entList;
	delete this;
}

float math::magnitude(Vec2 v1, Vec2 v2)
{
	return sqrt(pow(v2.x - v1.x,2) + pow(v2.y - v1.y,2));
}

float math::magnitude(Vec3 v1, Vec3 v2)
{
	return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z,2));
}
Vec3 math::diffVec(Vec3 v1, Vec3 v2)
{
	Vec3 res;
	res.x =	v2.x - v1.x;
	res.y = v2.y - v1.y;
	res.z = v2.z - v1.z;
	return res;
}

//find closest enemy by cross hairs
Player* Game::GetClosestEnemy()
{
	float temp = FLT_MAX;
	float dist;
	int index = -1;
	Player* curEnt = nullptr;
	Vec2 pos2D;
	for (int i = 0; i < playerCount - 1; i++)
	{
		curEnt = entList->ents[i];

		if (curEnt == nullptr)
			continue;
		if (curEnt->health <= 0 || curEnt->health > 100)
			continue;

		if (World2Screen(curEnt->coords, pos2D))
		{
			dist = math::magnitude(pos2D, windowDims);
			if (dist < temp)
			{
				temp = dist;
				index = i;
			}
		}
	}
	curEnt = entList->ents[index];
	if (!curEnt)
		return nullptr;

	return curEnt;

}

//3D to 2D coordinates, so player coords to screen position
bool Game::World2Screen(Vec3 pos, Vec2& screenPos)
{
	Vec4 cc;
	cc.x = pos.x * vm[0] + pos.y * vm[4] + pos.z * vm[8] + vm[12];
	cc.y = pos.x * vm[1] + pos.y * vm[5] + pos.z * vm[9] + vm[13];
	cc.z = pos.x * vm[2] + pos.y * vm[6] + pos.z * vm[10] + vm[14];
	cc.w = pos.x * vm[3] + pos.y * vm[7] + pos.z * vm[11] + vm[15];
	
	if (cc.w < 0.1f) 
		return false;

	Vec2 ndc;
	ndc.x = cc.x / cc.w;
	ndc.y = cc.y / cc.w;

	screenPos.x = (windowDims.x / 2 * ndc.x) + (ndc.x + windowDims.x / 2);
	screenPos.y = -(windowDims.y / 2 * ndc.y) + (ndc.y + windowDims.y / 2);
	
	return true;
}

void Game::updateViewMatrix()
{
	memcpy(vm, (char*)(gameAddr + offsets::viewMatrix), sizeof(vm));
}

//highlight enemies
void Game::esp()
{
	Player* curEnt = nullptr;
	Vec2 screen;
	Vec3 head3D;
	Vec2 head2D;
	for (int i = 0; i < playerCount - 1; i++)
	{
		curEnt = entList->ents[i];
		if (curEnt == nullptr)
			continue;

		if (curEnt == localPlayer)
			continue;

		if (curEnt->health <= 0 || curEnt->health > 100)
			continue;

		//for one hit
		curEnt->health = 1;

		if (World2Screen(curEnt->coords, screen))
		{
			dl->AddLine(
				ImVec2(screen.x,screen.y),
				ImVec2(windowDims.x / 2, windowDims.y),
				ImGui::ColorConvertFloat4ToU32(ImVec4(255.0 / 255.0, 0.0 / 255.0, 0.0 / 255.0, 255.0 / 255.0))
			);
		}
		head3D = curEnt->coords;
		head3D.z -= 4;
		
		if (World2Screen(head3D, head2D))
		{
			imgui::drawBox(
				ImVec2(screen.x, screen.y),
				ImVec2(head2D.x, head2D.y),
				ImGui::ColorConvertFloat4ToU32(ImVec4(255.0 / 255.0, 0.0 / 255.0, 0.0 / 255.0, 255.0 / 255.0))
			);
		}
		
	}
}

//uses some trig to have perfect aim
void Game::aimbot()
{
	if (GetAsyncKeyState(VK_MENU))
	{
		Vec2 screen;
		Player* enemy = GetClosestEnemy();
		if (!enemy)
			return;

		Vec3 dist = math::diffVec(localPlayer->coords, enemy->coords);
		float yaw = atan2(dist.y, dist.x) * (180 / M_PI);
		yaw += 90;
		float hypot = math::magnitude(dist, Vec3(0));
		float pitch = atan2(dist.z, hypot) * (180 / M_PI);
		localPlayer->viewAngles.x = yaw;
		localPlayer->viewAngles.y = pitch;
		
	}
}

void Game::GodMode()
{
	localPlayer->health = 777;
	localPlayer->arAmmo = 777;
}