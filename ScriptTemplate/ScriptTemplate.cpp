#include "ScriptTemplate.h"
using namespace VoxelFox;
Graphics::Geometry::Cube cube;

void glDrawCube(VoxelFox::Graphics::Window* window,float x,float y,float z,float scale) {
	cube.setScl(Math::Vec3<float>(scale));
	cube.setPos(VoxelFox::Math::Vec3<float>(x, y, z));
	window->Draw(cube);
}
void glDrawLine(VoxelFox::Graphics::Window* window, float x, float y, float z, float x2, float y2, float z2, float width) {
	glLineWidth(width);
	Graphics::Geometry::Line l(Math::Vec3<float>(x, y, z), Math::Vec3<float>(x2, y2, z2));
	window->Draw(l);
}

float sizeOne = .01;
float sizeTwo = .02;
float sizeThree = .03;

DllExport Setup(void* data, VoxelFox::Graphics::Window* wind)
{
	printf("This Is An Addon!\n");
	return;
}

DllExport Update(void* data, VoxelFox::Graphics::Window* wind)
{
	//printf("This Is An Addon2!\n");
	//for (int i = 0; i < 300; i++)
	//{
	//	printf("Simulated work:%i\n", i);
	//}
	//printf("This Is The End Of An Addon!\n");
	Kinect::UpdateSkeletons();
	return;
}

DllExport UI(void* data, VoxelFox::Graphics::Window* wind)
{
	//printf("This Is An Addon2!\n");
	//for (int i = 0; i < 300; i++)
	//{
	//	printf("Simulated work:%i\n", i);
	//}
	//printf("This Is The End Of An Addon!\n");
	return;
}
DllExport Draw(void* data, VoxelFox::Graphics::Window* wind)
{
	for (int i = 0; i < 6; i++)
	{
		if (Kinect::People[i].Tracked) {
			glColor3f(1, 1, 1);
			float scalar = .2;
			Kinect::Joint* joints = Kinect::People[i].GetJointsArray();
			for (int j = 0; j < 18; j++)
			{
				glDrawCube(wind, (joints[j].x * scalar) - (sizeOne / 2), (joints[j].y * scalar) - (sizeOne / 2), (joints[j].z * scalar) - (sizeOne / 2), sizeOne);
			}
			glDrawCube(wind, (Kinect::People[i].Head.x * scalar) - (sizeThree / 2), (Kinect::People[i].Head.y * scalar) - (sizeThree / 2), (Kinect::People[i].Head.z * scalar) - (sizeThree / 2), sizeThree);
			glDrawCube(wind, (Kinect::People[i].Spine.x * scalar) - (sizeTwo / 2), (Kinect::People[i].Spine.y * scalar) - (sizeTwo / 2), (Kinect::People[i].Spine.z * scalar) - (sizeTwo / 2), sizeTwo);
			glDrawLine(wind, Kinect::People[i].ElbowR.x * scalar, Kinect::People[i].ElbowR.y * scalar, Kinect::People[i].ElbowR.z * scalar, Kinect::People[i].WristR.x * scalar, Kinect::People[i].WristR.y * scalar, Kinect::People[i].WristR.z * scalar, 3);
		}
		//printf("This Is An Addon2!\n");
		//for (int i = 0; i < 300; i++)
		//{
		//	printf("Simulated work:%i\n", i);
		//}
		//printf("This Is The End Of An Addon!\n");
	}

	
	
	return;
}
DllExport Final(void* data, VoxelFox::Graphics::Window* wind)
{
	//printf("This Is An Addon2!\n");
	//for (int i = 0; i < 300; i++)
	//{
	//	printf("Simulated work:%i\n", i);
	//}
	//printf("This Is The End Of An Addon!\n");
	return;
}