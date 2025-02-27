#pragma once
#include "includes.h"
#include <cmath>
#include <Windows.h>
namespace offsets
{
	const uintptr_t localPlayer = 0x0017E0A8;
	const uintptr_t entityList = 0x0018ac04;
	const uintptr_t playerCount = 0x0018AC0C;
	const uintptr_t viewMatrix = 0x0017DFD0;
}

typedef struct Vector2
{
	float x, y;
} Vec2;

typedef struct Vector3
{
	float x, y, z;
} Vec3;

typedef struct Vector4
{
	float x, y, z, w;
} Vec4;


//player structs and stuff
class CBaseEntList
{
public:
	class EntList* entList; //0x0000
}; //Size: 0x0004

class Player
{
public:
	char pad_0000[4]; //0x0000
	Vector3 coords; //0x0004
	char pad_0010[36]; //0x0010
	Vector2 viewAngles; //0x0034
	char pad_003C[176]; //0x003C
	int32_t health; //0x00EC
	char pad_00F0[80]; //0x00F0
	int32_t arAmmo; //0x0140
}; //Size: 0x0144

class EntList
{
public:
	char pad_0000[4]; //0x0000
	Player* ents[32]; //0x0004
}; //Size: 0x0084

//some math functions
namespace math
{
	float magnitude(Vec2 v1, Vec2 v2);
	float magnitude(Vec3 v1, Vec3 v2);
	Vec3 diffVec(Vec3 v1, Vec3 v2);
}

//toolset
class Game
{
	uintptr_t gameAddr;
	Player* localPlayer;
	EntList* entList;
	int playerCount;
	float vm[16];
	Vec2 windowDims;
public:
	Game();
	~Game();
	Player* GetClosestEnemy();
	bool World2Screen(Vec3 pos, Vec2& screenPos);
	void updateViewMatrix(); 
	void GodMode();
	void esp();
	void aimbot();
};

