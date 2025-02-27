#pragma once
#define print(...) std::cout << "[*] " << __VA_ARGS__ << std::endl;
#define error(...) std::cout << "[!] " << __VA_ARGS__ << std::endl;
#define M_PI 3.14159265358979323846
#include <Windows.h>
#include <iostream>
#include <cstdint>
#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_win32.h"
#include "../include/imgui/imgui_impl_opengl2.h"
#include "draw.h"
#include "mem.h"
#include "game.h"