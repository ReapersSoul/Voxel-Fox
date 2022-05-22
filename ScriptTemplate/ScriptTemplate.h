#pragma once
#include <Windows.h>
#include "../VOXELFOX/VoxelFox.h"
#define DllExport extern "C" __declspec( dllexport ) void

	DllExport Setup(void* data, VoxelFox::Graphics::Window* wind);
	DllExport Update(void* data, VoxelFox::Graphics::Window* wind);
	DllExport Draw(void* data, VoxelFox::Graphics::Window* wind);
	DllExport UI(void* data, VoxelFox::Graphics::Window* wind);
	DllExport Final(void* data, VoxelFox::Graphics::Window* wind);