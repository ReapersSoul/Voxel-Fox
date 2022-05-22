#include "ScriptTemplate.h"
DllExport Setup(void* data, VoxelFox::Graphics::Window* wind)
{
	printf("This Is An Addon2!\n");
	for (int i = 0; i < 300; i++)
	{
		printf("Simulated work:%i\n", i);
	}
	printf("This Is The End Of An Addon!\n");
	return;
}

DllExport Update(void* data, VoxelFox::Graphics::Window* wind)
{
	printf("This Is An Addon2!\n");
	for (int i = 0; i < 300; i++)
	{
		printf("Simulated work:%i\n", i);
	}
	printf("This Is The End Of An Addon!\n");
	return;
}

DllExport UI(void* data, VoxelFox::Graphics::Window* wind)
{
	printf("This Is An Addon2!\n");
	for (int i = 0; i < 300; i++)
	{
		printf("Simulated work:%i\n", i);
	}
	printf("This Is The End Of An Addon!\n");
	return;
}
DllExport Draw(void* data, VoxelFox::Graphics::Window* wind)
{
	printf("This Is An Addon2!\n");
	for (int i = 0; i < 300; i++)
	{
		printf("Simulated work:%i\n", i);
	}
	printf("This Is The End Of An Addon!\n");
	return;
}
DllExport Final(void* data, VoxelFox::Graphics::Window* wind)
{
	printf("This Is An Addon2!\n");
	for (int i = 0; i < 300; i++)
	{
		printf("Simulated work:%i\n", i);
	}
	printf("This Is The End Of An Addon!\n");
	return;
}