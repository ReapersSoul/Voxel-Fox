#include "VoxelFox.h"
#include <chrono>
#include <map>
#include <Windows.h>

#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>
#include <imgui_impl_glfw.h>

using namespace VoxelFox;

VoxelFox::ScriptLoader sl;
float* CamEyePos[3];
float* Campos[3];

void Setup(Graphics::Window* window) {
	glEnable(GL_DEPTH_TEST); // Depth Testing
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(), true);
	//ImGui_ImplOpenGL3_Init("#version 130");
	ImGui_ImplOpenGL3_Init();
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	sl.Init();
	sl.LoadAllScripts();
	sl.Setup(window);

	window->GetCamera(window->GetSelectedCam())->SetEyePos(Math::Vec3<float>(0, 0, 1));

	VoxelFox::Graphics::Geometry::LoadMeshesFromDir("Models/");
	for (std::map<std::string, Graphics::Geometry::Mesh>::iterator it = Graphics::Geometry::Meshes.begin(); it != Graphics::Geometry::Meshes.end(); it++)
	{
		it->second.setScl(.05);
	}
}

float color[4] = { 1,1,1,1 };
float P1color[4] = { 1,0,0,1 };;
float P2color[4] = { 1,1,0,1 };;
float P3color[4] = { 1,1,1,1 };;
float P4color[4] = { 0,0,1,1 };;
float P5color[4] = { 0,1,1,1 };;
float P6color[4] = { 1,0,1,1 };;

float CamRotation[3] = { 0 ,0,0 };
float LastCamRotation[3] = { 0 ,0,0 };
float CamAutoRotation[3] = { 0 ,0,0 };
bool autoRotate = false;

void Update(Graphics::Window* window) {
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glLoadIdentity();
	glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (autoRotate) {
		if (CamRotation[0] != LastCamRotation[0] || CamRotation[1] != LastCamRotation[1] || CamRotation[2] != LastCamRotation[2]) {
			window->GetCamera(window->GetSelectedCam())->SetRot(Math::Vec3<float>(Math::DegreeToRadians(CamRotation[0]), Math::DegreeToRadians(CamRotation[1]), Math::DegreeToRadians(CamRotation[2])));
		}
		window->GetCamera(window->GetSelectedCam())->SetRot(window->GetCamera(window->GetSelectedCam())->GetRot() + Math::Vec3<float>(Math::DegreeToRadians(CamAutoRotation[0]), Math::DegreeToRadians(CamAutoRotation[1]), Math::DegreeToRadians(CamAutoRotation[2])));
	}
	else {
		window->GetCamera(window->GetSelectedCam())->SetRot(Math::Vec3<float>(Math::DegreeToRadians(CamRotation[0]), Math::DegreeToRadians(CamRotation[1]), Math::DegreeToRadians(CamRotation[2])));
	}
	LastCamRotation[0] = CamRotation[0];
	LastCamRotation[1] = CamRotation[1];
	LastCamRotation[2] = CamRotation[2];

	try {
		//luaVM.CallLuaFunct("Update");
		sl.Update(window);
	}
	catch (Exception e) {
		std::cout << e.what() << std::endl;
	}
}

float tiltang = 0;

int SelectedCam = 0;

void Draw(Graphics::Window* window) {
	glColor3f(1, 1, 1);
	try {
		luaVM.CallLuaFunct("Update");
		luaVM.CallLuaFunct("Draw");
		sl.Draw(window);
	}
	catch (Exception e) {
		std::cout << e.what() << std::endl;
	}
	// Draw stuff
	int i = 0;
	for (std::map<std::string, Graphics::Geometry::Mesh>::iterator it = Graphics::Geometry::Meshes.begin(); it != Graphics::Geometry::Meshes.end(); it++)
	{
		window->Draw(it->second);
		i++;
	}
}

bool ShowMeshes = false;
bool ShowTextures = false;
bool ShowKinect = false;
bool ShowCamera = false;
bool ShowScripts = false;

bool ShouldExit = false;

void UI(Graphics::Window* window) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::BeginMainMenuBar();
	if(ImGui::MenuItem("Show Meshes")){
		ShowMeshes = !ShowMeshes;
	}
	if(ImGui::MenuItem("Show Textures")){
		ShowTextures = !ShowTextures;
	}
	if(ImGui::MenuItem("Show Kinect")){
		ShowKinect = !ShowKinect;
	}
	if(ImGui::MenuItem("Show Camera")){
		ShowCamera = !ShowCamera;
	}
	if(ImGui::MenuItem("Show Scripts")){
		ShowScripts = !ShowScripts;
	}
	if (ImGui::MenuItem("Exit")) {
		ShouldExit = true;
	}
	ImGui::EndMainMenuBar();
	if (ShouldExit) {
		std::exit(0);
	}

	if (ShowMeshes) {
		if (ImGui::Begin("Meshes")) {
			for (std::map<std::string, Graphics::Geometry::Mesh>::iterator it = Graphics::Geometry::Meshes.begin(); it != Graphics::Geometry::Meshes.end(); it++)
			{
				if (ImGui::CollapsingHeader(it->first.c_str())) {
					if (ImGui::BeginTable(it->first.c_str(), 8)) {
						ImGui::TableSetupColumn("Mode");
						ImGui::TableSetupColumn("Position");
						ImGui::TableSetupColumn("Rotation");
						ImGui::TableSetupColumn("Scale");
						ImGui::TableSetupColumn("Num Verts");
						ImGui::TableSetupColumn("Num Vert Normals");
						ImGui::TableSetupColumn("Num TexCoords");
						ImGui::TableSetupColumn("Textures");
						ImGui::TableHeadersRow();
						ImGui::TableNextColumn();
						switch (it->second.getMode())
						{
						case Graphics::Geometry::Mesh::loaded:
							ImGui::Text("Loaded");
							break;
						default:
							ImGui::Text("unknown");
							break;
						}
						ImGui::TableNextColumn();
						float pos[3];
						float rot[3];
						float scl[3];
						pos[0] = it->second.getPos().x;
						pos[1] = it->second.getPos().y;
						pos[2] = it->second.getPos().z;
						rot[0] = degrees(it->second.getRot().x);
						rot[1] = degrees(it->second.getRot().y);
						rot[2] = degrees(it->second.getRot().z);
						scl[0] = it->second.getScl().x;
						scl[1] = it->second.getScl().y;
						scl[2] = it->second.getScl().z;
						ImGui::DragFloat3("Position", pos,.01);
						ImGui::TableNextColumn();
						ImGui::DragFloat3("Rotation", rot,.01);
						ImGui::TableNextColumn();
						ImGui::DragFloat3("Scale", scl,.01);
						it->second.setPos(Math::Vec3<float>(pos[0], pos[1], pos[2]));
						it->second.setRot(Math::Vec3<float>(Math::DegreeToRadians(rot[0]), Math::DegreeToRadians(rot[1]), Math::DegreeToRadians(rot[2])));
						it->second.setScl(Math::Vec3<float>(scl[0], scl[1], scl[2]));
						ImGui::TableNextColumn();
						ImGui::Text(std::to_string(it->second.getData()->size()).c_str());
						ImGui::TableNextColumn();
						ImGui::Text(std::to_string(it->second.getData()->size()).c_str());
						ImGui::TableNextColumn();
						ImGui::Text(std::to_string(it->second.getData()->size()).c_str());
						ImGui::TableNextColumn();

						for (int i = 0; i < it->second.getMatIDS()->size(); i++) {
							ImGui::Text((*it->second.getMatIDS())[i].c_str());
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::TableNextColumn();
							ImGui::TableNextColumn();
							ImGui::TableNextColumn();
							ImGui::TableNextColumn();
							ImGui::TableNextColumn();
							ImGui::TableNextColumn();
							ImGui::TableNextColumn();

						}
					}
					ImGui::EndTable();
				}
			}
		}
		ImGui::End();
	}

	if (ShowTextures) {
		if (ImGui::Begin("Textures and Materials")) {
			for (std::map<std::string, VoxelFox::Graphics::Material>::iterator element = VoxelFox::Graphics::Materials.begin(); element != VoxelFox::Graphics::Materials.end(); element++) {
				if (ImGui::CollapsingHeader(element->first.c_str())) {
					if (ImGui::BeginTable("Materials", 6)) {
						ImGui::TableSetupColumn("Material Name");
						ImGui::TableSetupColumn("Color");
						ImGui::TableSetupColumn("Texture ID");
						ImGui::TableSetupColumn("Texture Path");
						ImGui::TableSetupColumn("wraping type");
						ImGui::TableSetupColumn("interpolation");
						ImGui::TableHeadersRow();
						ImGui::TableNextColumn();
						ImGui::Text(element->first.c_str());
						ImGui::TableNextColumn();
						float texcolor[4] = { element->second.Color.x, element->second.Color.y, element->second.Color.z , element->second.Color.w };
						ImGui::ColorEdit4("Color", texcolor);
						element->second.Color.x = texcolor[0];
						element->second.Color.y = texcolor[1];
						element->second.Color.z = texcolor[2];
						element->second.Color.w = texcolor[3];
						ImGui::TableNextColumn();
						ImGui::Text(std::to_string(VoxelFox::Graphics::Textures.find(element->second.ColorMap)->second.TexID).c_str());
						ImGui::TableNextColumn();
						ImGui::Text(VoxelFox::Graphics::Textures.find(element->second.ColorMap)->second.Path.c_str());
						ImGui::TableNextColumn();
						ImGui::Text(std::to_string(VoxelFox::Graphics::Textures.find(element->second.ColorMap)->second.WrapingMode).c_str());
						ImGui::TableNextColumn();
						ImGui::Text(std::to_string(VoxelFox::Graphics::Textures.find(element->second.ColorMap)->second.Interpolation).c_str());
						ImGui::TableNextColumn();
					}
					ImGui::EndTable();
				}
			}
		}
		ImGui::End();
	}

	if (ShowCamera) {
		if (ImGui::Begin("Camera")) {
			if (ImGui::InputInt("Selected Camera", &SelectedCam, 1)) {
				window->SelectCam(SelectedCam);
			}
			CamEyePos[0] = &window->GetCamera(window->GetSelectedCam())->GetEyePosPoint()->x;
			CamEyePos[1] = &window->GetCamera(window->GetSelectedCam())->GetEyePosPoint()->y;
			CamEyePos[2] = &window->GetCamera(window->GetSelectedCam())->GetEyePosPoint()->z;
			Campos[0] = &window->GetCamera(window->GetSelectedCam())->GetPosPoint()->x;
			Campos[1] = &window->GetCamera(window->GetSelectedCam())->GetPosPoint()->y;
			Campos[2] = &window->GetCamera(window->GetSelectedCam())->GetPosPoint()->z;
			ImGui::DragFloat3("Camera Rotation", CamRotation,.5, -360, 360);
			//ImGui::InputFloat3("Camera Rotation", CamRotation);
			ImGui::DragFloat3("Camera Auto Rotation", CamAutoRotation,.5, -360, 360);
			//ImGui::InputFloat3("Camera Auto Rotation", CamAutoRotation);
			ImGui::Checkbox("Auto Rotate", &autoRotate);
			ImGui::DragFloat3("Cam Eye Pos", *CamEyePos,.01);
			ImGui::DragFloat3("Cam Pos", *Campos,.01);
		}
		ImGui::End();
	}
	if (ShowScripts) {
		if (ImGui::Begin("Scripts")) {
			if (ImGui::Button("Refresh")) {
				sl.LoadAllScripts();
			}
			for (int i = 0; i < sl.GetScriptNames().size(); i++) {
				ImGui::Text(sl.GetScriptNames()[i].c_str());
			}
		}
		ImGui::End();
	}

	if (ShowKinect) {
		if (ImGui::Begin("Kinect")) {
			if (ImGui::InputFloat("Tilt Angle", &tiltang, 1, 5)) {
				NuiCameraElevationSetAngle((LONG)tiltang);
			}
			if (ImGui::CollapsingHeader("Colors")) {
				ImGui::ColorPicker4("Player 1 Color", P1color);
				ImGui::ColorPicker4("Player 2 Color", P2color);
				ImGui::ColorPicker4("Player 3 Color", P3color);
				ImGui::ColorPicker4("Player 4 Color", P4color);
				ImGui::ColorPicker4("Player 5 Color", P5color);
				ImGui::ColorPicker4("Player 6 Color", P6color);
			}
		}
		ImGui::End();
		luaVM.SetGlobal("P1Color", Math::Vec3<float>(P1color[0], P1color[1], P1color[2]).ToTable());
		luaVM.SetGlobal("P2Color", Math::Vec3<float>(P2color[0], P2color[1], P2color[2]).ToTable());
		luaVM.SetGlobal("P3Color", Math::Vec3<float>(P3color[0], P3color[1], P3color[2]).ToTable());
		luaVM.SetGlobal("P4Color", Math::Vec3<float>(P4color[0], P4color[1], P4color[2]).ToTable());
		luaVM.SetGlobal("P5Color", Math::Vec3<float>(P5color[0], P5color[1], P5color[2]).ToTable());
		luaVM.SetGlobal("P6Color", Math::Vec3<float>(P6color[0], P6color[1], P6color[2]).ToTable());
	}


	try {
		luaVM.CallLuaFunct("UI");
		sl.UI(window);
	}
	catch (Exception e) {
		std::cout << e.what() << std::endl;
	}
	// Render dear imgui into screen
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void FinalF(Graphics::Window* window) {
	try {
		luaVM.CallLuaFunct("FinalF");
		sl.Final(window);
	}
	catch (Exception e) {
		std::cout << e.what() << std::endl;
	}
}

void main()
{
	// Initialise all DevIL functionality
	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);    // Tell DevIL that we're using OpenGL for our rendering
	VoxelFox::Kinect::Init();
	srand(time(0));
	//try {
		LuaVMSetup();
		luaVM.RunScript("main.lua");

		window.Init("Voxel Fox", 1920 / 2, 1920 / 2,&Update, &Draw, &Setup,&UI,&FinalF);

		while (window.IsOpen()) {
			try {
				std::string s;
				std::getline(std::cin, s);
				luaVM.RunString(s);
			}
			catch (Exception e) {
				std::cout << e.what() << std::endl;
			}
		}
	//}
	//catch (Exception e) {
	//	std::cout << e.what() << std::endl;
	//}
	return;
}