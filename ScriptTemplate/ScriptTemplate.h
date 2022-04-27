#pragma once
#define DllExport extern "C" __declspec(dllexport) int __stdcall
#include <Windows.h>
#include "../VOXELFOX/VoxelFox.h"
DllExport Setup(void* data, VoxelFox::Graphics::Window* wind)
{
	printf("This Is An Addon2!\n");
	for (int i = 0; i < 300; i++)
	{
		printf("Simulated work:%i\n", i);
	}
	printf("This Is The End Of An Addon!\n");
	return 0;
}

DllExport Update(void* data, VoxelFox::Graphics::Window* wind)
{
	printf("This Is An Addon2!\n");
	for (int i = 0; i < 300; i++)
	{
		printf("Simulated work:%i\n", i);
	}
	printf("This Is The End Of An Addon!\n");
	return 0;
}

DllExport UI(void* data, VoxelFox::Graphics::Window* wind)
{
	printf("This Is An Addon2!\n");
	for (int i = 0; i < 300; i++)
	{
		printf("Simulated work:%i\n", i);
	}
	printf("This Is The End Of An Addon!\n");
	return 0;
}
DllExport Draw(void* data, VoxelFox::Graphics::Window* wind)
{
	printf("This Is An Addon2!\n");
	for (int i = 0; i < 300; i++)
	{
		printf("Simulated work:%i\n", i);
	}
	printf("This Is The End Of An Addon!\n");
	return 0;
}
DllExport Final(void* data, VoxelFox::Graphics::Window* wind)
{
	printf("This Is An Addon2!\n");
	for (int i = 0; i < 300; i++)
	{
		printf("Simulated work:%i\n", i);
	}
	printf("This Is The End Of An Addon!\n");
	return 0;
}