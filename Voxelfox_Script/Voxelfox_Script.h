#pragma once
#pragma once
#define DllExport   __declspec(dllexport) __stdcall
#include <Windows.h>
#include "../VOXELFOX/VoxelFox.h"

extern "C" {
	int DllExport Setup(VoxelFox::Graphics::Window* wind);
	int DllExport Update(VoxelFox::Graphics::Window* wind);
	int DllExport UI(VoxelFox::Graphics::Window* wind);
	int DllExport Draw(VoxelFox::Graphics::Window* wind);
	int DllExport Final(VoxelFox::Graphics::Window* wind);
}