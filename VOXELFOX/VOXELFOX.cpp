#include "VoxelFox.h"
#include <chrono>
#include <map>
#include <Windows.h>

#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>
#include <imgui_impl_glfw.h>




using namespace VoxelFox;
int width = 0;
int height = 0;

Graphics::Geometry::Mesh GlobalMesh;


Lua luaVM;

Graphics::Window window;

//Lua::Table vars;

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

	window->GetCamera(window->GetSelectedCam())->SetEyePos(Math::Vec3<float>(0, 0, 1));

	VoxelFox::Graphics::Geometry::LoadMeshesFromDir("Models/");
	for(std::map<std::string,Graphics::Geometry::Mesh>::iterator it=Graphics::Geometry::Meshes.begin(); it != Graphics::Geometry::Meshes.end(); it++)
	{
		it->second.setScl(.05);
	}
}

float color[4]={1,1,1,1};
float P1color[4] = { 1,0,0,1 };;
float P2color[4] = { 1,1,0,1 };;
float P3color[4] = { 1,1,1,1 };;
float P4color[4] = { 0,0,1,1 };;
float P5color[4] = { 0,1,1,1 };;
float P6color[4] = { 1,0,1,1 };;

float CamRotation[3] = { 0 ,0,0};
float LastCamRotation[3] = { 0 ,0,0};
float CamAutoRotation[3] = { 0 ,0,0};
bool autoRotate = false;

void Update(Graphics::Window* window) {
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glLoadIdentity();
	glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (autoRotate) {
		if (CamRotation[0]!=LastCamRotation[0]||CamRotation[1]!=LastCamRotation[1]||CamRotation[2]!=LastCamRotation[2]) {
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
}

float tiltang = 0;

int SelectedCam = 0;

void Draw(Graphics::Window* window) {
	glColor3f(1, 1, 1);
	try {
		luaVM.CallLuaFunct("Draw");
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

void UI(Graphics::Window* window) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//ImGui::BeginMainMenuBar();
	//if (ImGui::MenuItem("Exit", "", new bool())) {
	//	std::terminate();
	//}
	//ImGui::EndMainMenuBar();

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
					rot[0] = it->second.getRot().x;
					rot[1] = it->second.getRot().y;
					rot[2] = it->second.getRot().z;
					scl[0] = it->second.getScl().x;
					scl[1] = it->second.getScl().y;
					scl[2] = it->second.getScl().z;
					ImGui::InputFloat3("Position", pos);
					ImGui::TableNextColumn();
					ImGui::InputFloat3("Rotation", rot);
					ImGui::TableNextColumn();
					ImGui::InputFloat3("Scale", scl);
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
					float texcolor[4] = { element->second.Color.x, element->second.Color.y, element->second.Color.z , element->second.Color.w};
					ImGui::ColorEdit4("Color", texcolor);
					element->second.Color.x = texcolor[0];
					element->second.Color.y = texcolor[1];
					element->second.Color.z = texcolor[2];
					element->second.Color.w = texcolor[3];
					ImGui::TableNextColumn();
					ImGui::Text(std::to_string(element->second.ColorMap.TexID).c_str());
					ImGui::TableNextColumn();
					ImGui::Text(element->second.ColorMap.Path.c_str());
					ImGui::TableNextColumn();
					ImGui::Text(std::to_string(element->second.ColorMap.WrapingMode).c_str());
					ImGui::TableNextColumn();
					ImGui::Text(std::to_string(element->second.ColorMap.Interpolation).c_str());
					ImGui::TableNextColumn();
				}
				ImGui::EndTable();
			}
		}
	}
	ImGui::End();


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
		ImGui::InputFloat3("Camera Rotation", CamRotation);
		ImGui::InputFloat3("Camera Auto Rotation", CamAutoRotation);
		ImGui::Checkbox("Auto Rotate", &autoRotate);
		ImGui::InputFloat3("Cam Eye Pos", *CamEyePos);
		ImGui::InputFloat3("Cam Pos", *Campos);
	}
	ImGui::End();

	if (ImGui::Begin("Kinect")) {
		if (ImGui::InputFloat("Tile Angle", &tiltang, 1, 5)) {
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
	try {
		luaVM.CallLuaFunct("UI");
	}
	catch (Exception e) {
		std::cout << e.what() << std::endl;
	}
	// Render dear imgui into screen
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void FinalF(Graphics::Window* window) {
}


void LuaReloadMain() {
	luaVM.RunScript("main.lua");
}

void LuaVMSetup() {
	//shared lua functions setup
	std::vector < std::pair<std::string, int (*)(lua_State* L)>> LuaFuncts = {
		//exit
		std::pair<std::string,int(*)(lua_State* L)>("exit", [](lua_State* L) {
		std::exit(0);
		return 0;
	})
	};

	//end shared lua functions setup



	//lua Draw functions setup

	for (int i = 0; i < LuaFuncts.size(); i++) {
		luaVM.RegisterFunction(LuaFuncts[i].first, LuaFuncts[i].second);
	}

	//change color
	luaVM.RegisterFunction("glColor", [](lua_State* L) {
		double r = lua_tonumber(L, 1);
		double g = lua_tonumber(L, 2);
		double b = lua_tonumber(L, 3);

		glColor3f(r, g, b);

		return 0;
		});


	//rotate world
	luaVM.RegisterFunction("glRotate", [](lua_State* L) {
		double val = lua_tonumber(L, 1);
		double x = lua_tonumber(L, 2);
		double y = lua_tonumber(L, 3);
		double z = lua_tonumber(L, 4);

		glRotatef(val, x, y, z);

		return 0;
		});
	//translate world
	luaVM.RegisterFunction("glTranslate", [](lua_State* L) {
		double x = lua_tonumber(L, 1);
		double y = lua_tonumber(L, 2);
		double z = lua_tonumber(L, 3);

		glTranslatef(x, y, z);

		return 0;
		});

	//scale world
	luaVM.RegisterFunction("glScale", [](lua_State* L) {
		double x = lua_tonumber(L, 1);
		double y = lua_tonumber(L, 2);
		double z = lua_tonumber(L, 3);

		glScalef(.5, .5, .5);

		return 0;
		});

	//draw cube
	luaVM.RegisterFunction("glDrawCube", [](lua_State* L) {
		double x = lua_tonumber(L, 1);
		double y = lua_tonumber(L, 2);
		double z = lua_tonumber(L, 3);
		double scale = lua_tonumber(L, 4);
		Graphics::Geometry::Cube cube;
		cube.setScl(Math::Vec3<float>(scale));
		cube.setPos(VoxelFox::Math::Vec3<float>(x, y, z));
		window.Draw(cube);
		return 0;
		});

	//draw line
	luaVM.RegisterFunction("glDrawLine", [](lua_State* L) {
		double x = lua_tonumber(L, 1);
		double y = lua_tonumber(L, 2);
		double z = lua_tonumber(L, 3);
		double x2 = lua_tonumber(L, 4);
		double y2 = lua_tonumber(L, 5);
		double z2 = lua_tonumber(L, 6);
		double width = lua_tonumber(L, 7);
		glLineWidth(width);
		Graphics::Geometry::Line l(Math::Vec3<float>(x, y, z), Math::Vec3<float>(x2, y2, z2));
		window.Draw(l);
		return 0;
		});
	//draw BezierCurve
	luaVM.RegisterFunction("glDrawBezierCurve", [](lua_State* L) {
		int nargs=lua_gettop(L);
		std::vector<Math::Vec3<float>> points;
		for (int i = nargs; i > 2; i--) {
			Math::Vec3<float> v1;
			v1.FromTable(Lua::lua_gettable(L, i));
			points.push_back(v1);
		}
		glLineWidth(lua_tonumber(L, 2));
		window.Draw(VoxelFox::Graphics::Geometry::BezierCurve(points, lua_tonumber(L, 1)));
		return 0;
		});

	//draw box
	luaVM.RegisterFunction("glDrawBox", [](lua_State* L) {
		double x = lua_tonumber(L, 1);
		double y = lua_tonumber(L, 2);
		double width = lua_tonumber(L, 3);
		double height = lua_tonumber(L, 4);

		Graphics::Geometry::Quad box;

		box.setPos(Math::Vec3<float>(x, y, 0));
		box.setScl(Math::Vec3<float>(width, height, 1));

		window.Draw(box);
		return 0;
		});
	//draw circle
	luaVM.RegisterFunction("glDrawCircle", [](lua_State* L) {
		double x = lua_tonumber(L, 1);
		double y = lua_tonumber(L, 2);
		double radius = lua_tonumber(L, 3);
		double resolution = lua_tonumber(L, 4);

		Graphics::Geometry::Circle circle(radius, resolution);

		circle.setPos(Math::Vec3<float>(x, y, 0));

		window.Draw(circle);
		return 0;
		});
	luaVM.RegisterFunction("CheckCollision", [](lua_State* L) {
		double x = lua_tonumber(L, 1);
		double y = lua_tonumber(L, 2);
		double radius = lua_tonumber(L, 3);
		double resolution = lua_tonumber(L, 4);

		Graphics::Geometry::Circle circle(radius, resolution);

		circle.setPos(Math::Vec3<float>(x, y, 0));

		window.Draw(circle);
		return 0;
		});
	//end lua draw function setup

	//start lua data function setup

	for (int i = 0; i < LuaFuncts.size(); i++) {
		luaVM.RegisterFunction(LuaFuncts[i].first, LuaFuncts[i].second);
	}

	//end Lua data function setup

	//lua help function
	luaVM.RegisterFunction("help", [](lua_State* L) {
		std::cout << "Functions:" << std::endl;
		for (std::map<std::string, int(*)(lua_State* L)>::iterator it = luaVM.functs.begin(); it != luaVM.functs.end(); it++) {
			std::cout << it->first << "()" << std::endl;
		}
		return 0;
		});



	//window class
	static Lua::LuaTableBuilder lwindow;
	lwindow.AddFunction(
		"isFocused",
		[](lua_State* L) {
			lua_pushboolean(L, window.IsFocused());
			return 1; }
	);
	lwindow.AddFunction(
		"GetCam",
		[](lua_State* L) {
			static int cam = lua_tointeger(L, 1);
			Lua::LuaTableBuilder Cam;
			Cam.AddFunction("GetPos", [](lua_State* L) {
				Lua::lua_pushtable(L, window.GetCamera(cam)->GetPos().ToTable());
				return 1;
				});
			Cam.AddFunction("SetPos", [](lua_State* L) {
				double x = lua_tonumber(L, 1);
				double y = lua_tonumber(L, 2);
				double z = lua_tonumber(L, 3);
				window.GetCamera(cam)->SetPos(VoxelFox::Math::Vec3<float>(x, y, z));
				return 0;
				});
			Cam.AddFunction("GetRot", [](lua_State* L) {
				Lua::lua_pushtable(L, window.GetCamera(cam)->GetEyePos().ToTable());
				return 1;
				});
			Cam.AddFunction("SetRot", [](lua_State* L) {
				double x = lua_tonumber(L, 1);
				double y = lua_tonumber(L, 2);
				double z = lua_tonumber(L, 3);
				window.GetCamera(cam)->SetEyePos(VoxelFox::Math::Vec3<float>(x, y, z));
				return 0;
				});
			Lua::lua_pushtable(L, Cam.GetTable());
			return 1; });

	lwindow.AddFunction(
		"SetSelectedCam",
		[](lua_State* L) {
			int cam = lua_tonumber(L, 1);
			window.SelectCam(cam);
			return 0;
		}
	);
	lwindow.AddFunction(
		"Camera",
		[](lua_State* L) {
			Lua::lua_pushtable(L, VoxelFox::Graphics::Camera().ToTable());
			return 1;
		}
	);


	lwindow.AddFunction(
		"AddCam",
		[](lua_State* L) {
			int nargs = lua_gettop(L);
			if (nargs > 1) {
				lua_pushstring(L, "ERROR: Only 1 argument in the form of table must be supplied.");
				lua_error(L);
			};
			int type = lua_type(L, 1);
			if (type != LUA_TTABLE) {
				lua_pushstring(L, "ERROR: Argument must be a table");
				lua_error(L);
			}
			VoxelFox::Graphics::Camera Cam;
			Cam.FromTable(Lua::lua_gettable(L,1));
			window.AddCam(Cam);

			return 0;
		}
	);

	lwindow.AddFunction(
		"LuaReload",
		[](lua_State* L) {
			LuaReloadMain();
			return 0;
		}
	);
	//window class end

	static Lua::LuaTableBuilder VoxelFox;
	static Lua::LuaTableBuilder Math;
	Math.AddFunction("MapRange", [](lua_State* L) {
		double input = lua_tonumber(L, 1);
		double input_start = lua_tonumber(L, 2);
		double input_end = lua_tonumber(L, 3);
		double output_start = lua_tonumber(L, 4);
		double output_end = lua_tonumber(L, 5);
		lua_pushnumber(L, Math::MapRange<double>(input, input_start, input_end, output_start, output_end));
		return 1;
		});
	Math.AddFunction("DegreeToRadian", [](lua_State* L) {
		double input = lua_tonumber(L, 1);
		lua_pushnumber(L, Math::DegreeToRadians(input));
		return 1;
		});

	//vec 2
	Math.AddFunction("Vec2", [](lua_State* L) {
		Lua::lua_pushtable(L, Math::Vec2<float>().ToTable());
		return 1;
		});
	Math.AddFunction("AddVec2", [](lua_State* L) {
		Math::Vec2<float> v1;
		v1.FromTable(Lua::lua_gettable(L,1));
		Math::Vec2<float> v2;
		v2.FromTable(Lua::lua_gettable(L,2));
		Lua::lua_pushtable(L, (v1 + v2).ToTable());
		return 1;
		});
	Math.AddFunction("SubVec2", [](lua_State* L) {
		Math::Vec2<float> v1;
		v1.FromTable(Lua::lua_gettable(L,1));
		Math::Vec2<float> v2;
		v2.FromTable(Lua::lua_gettable(L,2));
		Lua::lua_pushtable(L, (v1 - v2).ToTable());
		return 1;
		});
	Math.AddFunction("DivVec2", [](lua_State* L) {
		Math::Vec2<float> v1;
		v1.FromTable(Lua::lua_gettable(L,1));
		Math::Vec2<float> v2;
		v2.FromTable(Lua::lua_gettable(L,2));
		Lua::lua_pushtable(L, (v1 / v2).ToTable());
		return 1;
		});
	Math.AddFunction("MultVec2", [](lua_State* L) {
		Math::Vec2<float> v1;
		v1.FromTable(Lua::lua_gettable(L,1));
		Math::Vec2<float> v2;
		v2.FromTable(Lua::lua_gettable(L,2));
		Lua::lua_pushtable(L, (v1 * v2).ToTable());
		return 1;
		});
	//end vec2

	//vec3
	Math.AddFunction("Vec3", [](lua_State* L) {
		double x = lua_tonumber(L, 1);
		double y = lua_tonumber(L, 2);
		double z = lua_tonumber(L, 3);
		Lua::lua_pushtable(L, Math::Vec3<float>(x,y,z).ToTable());
		return 1;
		});
	Math.AddFunction("AddVec3", [](lua_State* L) {
		Math::Vec3<float> v1;
		v1.FromTable(Lua::lua_gettable(L,1));
		Math::Vec3<float> v2;
		v2.FromTable(Lua::lua_gettable(L,2));
		Lua::lua_pushtable(L, (v1 + v2).ToTable());
		return 1;
		});
	Math.AddFunction("SubVec3", [](lua_State* L) {
		Math::Vec3<float> v1;
		v1.FromTable(Lua::lua_gettable(L,1));
		Math::Vec3<float> v2;
		v2.FromTable(Lua::lua_gettable(L,2));
		Lua::lua_pushtable(L, (v1 - v2).ToTable());
		return 1;
		});
	Math.AddFunction("DivVec3", [](lua_State* L) {
		Math::Vec3<float> v1;
		v1.FromTable(Lua::lua_gettable(L,1));
		Math::Vec3<float> v2;
		v2.FromTable(Lua::lua_gettable(L,2));
		Lua::lua_pushtable(L, (v1 / v2).ToTable());
		return 1;
		});
	Math.AddFunction("MultVec3", [](lua_State* L) {
		Math::Vec3<float> v1;
		v1.FromTable(Lua::lua_gettable(L,1));
		Math::Vec3<float> v2;
		v2.FromTable(Lua::lua_gettable(L,2));
		Lua::lua_pushtable(L, (v1 * v2).ToTable());
		return 1;
		});
	Math.AddFunction("CrossVec3", [](lua_State* L) {
		Math::Vec3<float> v1;
		v1.FromTable(Lua::lua_gettable(L,1));
		Math::Vec3<float> v2;
		v2.FromTable(Lua::lua_gettable(L,2));
		Lua::lua_pushtable(L, (v1.Cross(v2)).ToTable());
		return 1;
		});
	Math.AddFunction("RotateVec3", [](lua_State* L) {
		Math::Vec3<float> v1;
		v1.FromTable(Lua::lua_gettable(L,1));
		Math::Vec3<float> v2;
		v2.FromTable(Lua::lua_gettable(L,2));
		v1.Rotate(v2);
		Lua::lua_pushtable(L, v1.ToTable());
		return 1;
		});
	Math.AddFunction("DotVec3", [](lua_State* L) {
		Math::Vec3<float> v1;
		v1.FromTable(Lua::lua_gettable(L,1));
		Math::Vec3<float> v2;
		v2.FromTable(Lua::lua_gettable(L,2));
		lua_pushnumber(L, v1.Dot(v2));
		return 1;
		});
	Math.AddFunction("NegateVec3", [](lua_State* L) {
		Math::Vec3<float> v1;
		v1.FromTable(Lua::lua_gettable(L,1));
		Lua::lua_pushtable(L, v1.negate().ToTable());
		return 1;
		});
	Math.AddFunction("NormalizeVec3", [](lua_State* L) {
		Math::Vec3<float> v1;
		v1.FromTable(Lua::lua_gettable(L,1));
		Lua::lua_pushtable(L, v1.Normalize().ToTable());
		return 1;
		});
	Math.AddFunction("DifferenceVec3", [](lua_State* L) {
		Math::Vec3<float> v1;
		v1.FromTable(Lua::lua_gettable(L,1));
		Math::Vec3<float> v2;
		v2.FromTable(Lua::lua_gettable(L,2));
		Lua::lua_pushtable(L, Math::Difference(v1, v2).ToTable());
		return 1;
		});
	Math.AddFunction("AveragePointVec3", [](lua_State* L) {
		Lua::Table t = Lua::lua_gettable(L,1);
		std::vector<Math::Vec3<float>> points;
		for (Lua::Table::iterator i = t.begin(); i != t.end(); i++)
		{
			Math::Vec3<float> tmp;
			tmp.FromTable(i->second.table);
			points.push_back(tmp);
		}
		Lua::lua_pushtable(L, Math::averagePoint(points).ToTable());
		return 1;
		});
	Math.AddFunction("IndexOfFurthestPointVec3", [](lua_State* L) {
		Math::Vec3<float> v1;
		v1.FromTable(Lua::lua_gettable(L,1));
		Lua::Table t = Lua::lua_gettable(L,2);
		std::vector<Math::Vec3<float>> points;
		for (Lua::Table::iterator i = t.begin(); i != t.end(); i++)
		{
			Math::Vec3<float> tmp;
			tmp.FromTable(i->second.table);
			points.push_back(tmp);
		}
		lua_pushnumber(L, Math::indexOfFurthestPoint(points, v1));
		return 1;
		});
	Math.AddFunction("SupportVec3", [](lua_State* L) {
		Math::Vec3<float> v1;
		v1.FromTable(Lua::lua_gettable(L,1));
		Lua::Table t = Lua::lua_gettable(L,2);
		std::vector<Math::Vec3<float>> points2;
		for (Lua::Table::iterator i = t.begin(); i != t.end(); i++)
		{
			Math::Vec3<float> tmp;
			tmp.FromTable(i->second.table);
			points2.push_back(tmp);
		}
		Lua::Table t2 = Lua::lua_gettable(L,1);
		std::vector<Math::Vec3<float>> points;
		for (Lua::Table::iterator i = t2.begin(); i != t2.end(); i++)
		{
			Math::Vec3<float> tmp;
			tmp.FromTable(i->second.table);
			points.push_back(tmp);
		}
		Lua::lua_pushtable(L, Math::Support(points, points2, v1).ToTable());
		return 1;
		});
	Math.AddFunction("TripleProductVec3", [](lua_State* L) {
		Math::Vec3<float> v1;
		v1.FromTable(Lua::lua_gettable(L,1));
		lua_settop(L, 2);
		Math::Vec3<float> v2;
		v2.FromTable(Lua::lua_gettable(L,1));
		lua_settop(L, 1);
		Math::Vec3<float> v3;
		v2.FromTable(Lua::lua_gettable(L,1));
		Lua::lua_pushtable(L, Math::tripleProduct(v1, v2, v3).ToTable());
		return 1;
		});

	static Lua::LuaTableBuilder Inputtbl;

	//TODO:: implement lua side of creating, accesing and managing input devices

	Inputtbl.AddFunction("NewInput", [](lua_State* L) {
		Graphics::InputHandler::Binding tmp;
		tmp.keyButton= lua_tointeger(L, 2);
		tmp.bt = (Graphics::InputHandler::Binding::BindingType)lua_tointeger(L, 3);
		tmp.stick = lua_tointeger(L, 4);
		tmp.controller = lua_tointeger(L, 5);
		Graphics::inputHandler.AddUpdateInput(lua_tostring(L, 1), tmp);
		return 0;
		});

	Inputtbl.AddFunction(
		"GetInput",
		[](lua_State* L) {
			std::string name = lua_tostring(L, 1);

			VoxelFox::Graphics::InputHandler::Binding input = VoxelFox::Graphics::inputHandler.GetInput(name);
			switch (input.bt)
			{
			case VoxelFox::Graphics::InputHandler::Binding::ControllerAxis:
				Lua::lua_pushtable(L, input.AxisPos.ToTable());
				break;
			case VoxelFox::Graphics::InputHandler::Binding::ControllerButton:
				lua_pushboolean(L, input.ButtonPressed);
				break;
			case VoxelFox::Graphics::InputHandler::Binding::KeyboardButton:
				lua_pushboolean(L, input.ButtonPressed);
				break;
			case VoxelFox::Graphics::InputHandler::Binding::MouseButton:
				lua_pushboolean(L, input.ButtonPressed);
				break;
			case VoxelFox::Graphics::InputHandler::Binding::MousePos:
				Lua::lua_pushtable(L, input.AxisPos.ToTable());
				break;

			default:
				break;
			}
			return 1;
		}
	);

	static Lua::LuaTableBuilder Kinecttbl;

	Kinecttbl.AddFunction(
		"GetPerson",
		[](lua_State* L) {
			static int P;
			P = lua_tonumber(L, 1);
			Lua::LuaTableBuilder person;
			person.AddBool("Tracked",Kinect::People[P].Tracked);

			person.AddFunction("HandL", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].HandL.ToTable());
				return 1;
				});
			person.AddFunction("HandR", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].HandR.ToTable());
				
				return 1;
				});
			person.AddFunction("WristL", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].WristL.ToTable());
				return 1;
				});
			person.AddFunction("WristR", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].WristR.ToTable());
				return 1;
				});
			person.AddFunction("ElbowL", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].ElbowL.ToTable());
				return 1;
				});
			person.AddFunction("ElbowR", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].ElbowR.ToTable());
				return 1;
				});
			person.AddFunction("ShoulderL", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].ShoulderL.ToTable());
				return 1;
				});
			person.AddFunction("ShoulderR", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].ShoulderR.ToTable());
				return 1;
				});
			person.AddFunction("ShoulderC", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].ShoulderC.ToTable());
				return 1;
				});
			person.AddFunction("Spine", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].Spine.ToTable());
				return 1;
				});
			person.AddFunction("HipL", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].HipL.ToTable());
				return 1;
				});
			person.AddFunction("HipR", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].HipR.ToTable());
				return 1;
				});
			person.AddFunction("HipC", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].HipC.ToTable());
				return 1;
				});
			person.AddFunction("KneeL", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].KneeL.ToTable());
				return 1;
				});
			person.AddFunction("KneeR", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].KneeR.ToTable());
				return 1;
				});
			person.AddFunction("AnkleL", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].AnkleL.ToTable());
				return 1;
				});
			person.AddFunction("AnkleR", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].AnkleR.ToTable());
				return 1;
				});
			person.AddFunction("FootL", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].FootL.ToTable());
				return 1;
				});
			person.AddFunction("FootR", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].FootR.ToTable());
				return 1;
				});
			person.AddFunction("Head", [](lua_State* L) {
				Lua::lua_pushtable(L, Kinect::People[P].Head.ToTable());
				return 1;
				});
			person.AddFunction("WristHandL", [](lua_State* L) {
				Lua::LuaTableBuilder WristHandL;
				WristHandL.AddFunction("Start", [](lua_State* L) {
					Lua::lua_pushtable(L, Kinect::People[P].WristHandL.Start.ToTable());
					return 1;
					});
				WristHandL.AddFunction("End", [](lua_State* L) {
					Lua::lua_pushtable(L, Kinect::People[P].WristHandL.End.ToTable());
					return 1;
					});
				WristHandL.AddFunction("Normalized", [](lua_State* L) {
					Lua::lua_pushtable(L, Kinect::People[P].WristHandL.Normalize().ToTable());
					return 1;
					});
				Lua::lua_pushtable(L, WristHandL.GetTable());
				return 1;
				});
			person.AddFunction("WristHandR", [](lua_State* L) {
				Lua::LuaTableBuilder WristHandR;
				WristHandR.AddTable("Start", Kinect::People[P].WristHandR.Start.ToTable());
				WristHandR.AddTable("End", Kinect::People[P].WristHandR.End.ToTable());
				WristHandR.AddTable("Normalized", Kinect::People[P].WristHandR.Normalize().ToTable());
				Lua::lua_pushtable(L, WristHandR.GetTable());
				return 1;
				});
			person.AddFunction("ElbowWristL", [](lua_State* L) {
				Lua::LuaTableBuilder ElbowWristL;
				ElbowWristL.AddTable("Start", Kinect::People[P].ElbowWristL.Start.ToTable());
				ElbowWristL.AddTable("End", Kinect::People[P].ElbowWristL.End.ToTable());
				ElbowWristL.AddTable("Normalized", Kinect::People[P].ElbowWristL.Normalize().ToTable());
				Lua::lua_pushtable(L, ElbowWristL.GetTable());
				return 1;
				});
			person.AddFunction("ElbowWristR", [](lua_State* L) {
				Lua::LuaTableBuilder ElbowWristR;
				ElbowWristR.AddTable("Start", Kinect::People[P].ElbowWristR.Start.ToTable());
				ElbowWristR.AddTable("End", Kinect::People[P].ElbowWristR.End.ToTable());
				ElbowWristR.AddTable("Normalized", Kinect::People[P].ElbowWristR.Normalize().ToTable());
				Lua::lua_pushtable(L, ElbowWristR.GetTable());
				return 1;
				});
			person.AddFunction("ShoulderElbowL", [](lua_State* L) {
				Lua::LuaTableBuilder ShoulderElbowL;
				ShoulderElbowL.AddTable("Start", Kinect::People[P].ShoulderElbowL.Start.ToTable());
				ShoulderElbowL.AddTable("End", Kinect::People[P].ShoulderElbowL.End.ToTable());
				ShoulderElbowL.AddTable("Normalized", Kinect::People[P].ShoulderElbowL.Normalize().ToTable());
				Lua::lua_pushtable(L, ShoulderElbowL.GetTable());
				return 1;
				});
			person.AddFunction("ShoulderElbowR", [](lua_State* L) {
				Lua::LuaTableBuilder ShoulderElbowR;
				ShoulderElbowR.AddTable("Start", Kinect::People[P].ShoulderElbowR.Start.ToTable());
				ShoulderElbowR.AddTable("End", Kinect::People[P].ShoulderElbowR.End.ToTable());
				ShoulderElbowR.AddTable("Normalized", Kinect::People[P].ShoulderElbowR.Normalize().ToTable());
				Lua::lua_pushtable(L, ShoulderElbowR.GetTable());
				return 1;
				});
			person.AddFunction("ShoulderCShoulderL", [](lua_State* L) {
				Lua::LuaTableBuilder ShoulderCShoulderL;
				ShoulderCShoulderL.AddTable("Start", Kinect::People[P].ShoulderCShoulderL.Start.ToTable());
				ShoulderCShoulderL.AddTable("End", Kinect::People[P].ShoulderCShoulderL.End.ToTable());
				ShoulderCShoulderL.AddTable("Normalized", Kinect::People[P].ShoulderCShoulderL.Normalize().ToTable());
				Lua::lua_pushtable(L, ShoulderCShoulderL.GetTable());
				return 1;
				});
			person.AddFunction("ShoulderCShoulderR", [](lua_State* L) {
				Lua::LuaTableBuilder ShoulderCShoulderR;
				ShoulderCShoulderR.AddTable("Start", Kinect::People[P].ShoulderCShoulderR.Start.ToTable());
				ShoulderCShoulderR.AddTable("End", Kinect::People[P].ShoulderCShoulderR.End.ToTable());
				ShoulderCShoulderR.AddTable("Normalized", Kinect::People[P].ShoulderCShoulderR.Normalize().ToTable());
				Lua::lua_pushtable(L, ShoulderCShoulderR.GetTable());
				return 1;
				});
			person.AddFunction("ShoulderCHead", [](lua_State* L) {
				Lua::LuaTableBuilder ShoulderCHead;
				ShoulderCHead.AddTable("Start", Kinect::People[P].ShoulderCHead.Start.ToTable());
				ShoulderCHead.AddTable("End", Kinect::People[P].ShoulderCHead.End.ToTable());
				ShoulderCHead.AddTable("Normalized", Kinect::People[P].ShoulderCHead.Normalize().ToTable());
				Lua::lua_pushtable(L, ShoulderCHead.GetTable());
				return 1;
				});
			person.AddFunction("SpineShoulderC", [](lua_State* L) {
				Lua::LuaTableBuilder SpineShoulderC;
				SpineShoulderC.AddTable("Start", Kinect::People[P].SpineShoulderC.Start.ToTable());
				SpineShoulderC.AddTable("End", Kinect::People[P].SpineShoulderC.End.ToTable());
				SpineShoulderC.AddTable("Normalized", Kinect::People[P].SpineShoulderC.Normalize().ToTable());
				Lua::lua_pushtable(L, SpineShoulderC.GetTable());
				return 1;
				});
			person.AddFunction("SpineHipC", [](lua_State* L) {
				Lua::LuaTableBuilder SpineHipC;
				SpineHipC.AddTable("Start", Kinect::People[P].SpineHipC.Start.ToTable());
				SpineHipC.AddTable("End", Kinect::People[P].SpineHipC.End.ToTable());
				SpineHipC.AddTable("Normalized", Kinect::People[P].SpineHipC.Normalize().ToTable());
				Lua::lua_pushtable(L, SpineHipC.GetTable());
				return 1;
				});
			person.AddFunction("HipCHipL", [](lua_State* L) {
				Lua::LuaTableBuilder HipCHipL;
				HipCHipL.AddTable("Start", Kinect::People[P].HipCHipL.Start.ToTable());
				HipCHipL.AddTable("End", Kinect::People[P].HipCHipL.End.ToTable());
				HipCHipL.AddTable("Normalized", Kinect::People[P].HipCHipL.Normalize().ToTable());
				Lua::lua_pushtable(L, HipCHipL.GetTable());
				return 1;
				});
			person.AddFunction("HipCHipR", [](lua_State* L) {
				Lua::LuaTableBuilder HipCHipR;
				HipCHipR.AddTable("Start", Kinect::People[P].HipCHipR.Start.ToTable());
				HipCHipR.AddTable("End", Kinect::People[P].HipCHipR.End.ToTable());
				HipCHipR.AddTable("Normalized", Kinect::People[P].HipCHipR.Normalize().ToTable());
				Lua::lua_pushtable(L, HipCHipR.GetTable());
				return 1;
				});
			person.AddFunction("HipKneeL", [](lua_State* L) {
				Lua::LuaTableBuilder HipCHipL;
				HipCHipL.AddTable("Start", Kinect::People[P].HipCHipL.Start.ToTable());
				HipCHipL.AddTable("End", Kinect::People[P].HipCHipL.End.ToTable());
				HipCHipL.AddTable("Normalized", Kinect::People[P].HipCHipL.Normalize().ToTable());
				Lua::lua_pushtable(L, HipCHipL.GetTable());
				return 1;
				});
			person.AddFunction("HipKneeR", [](lua_State* L) {
				Lua::LuaTableBuilder HipCHipR;
				HipCHipR.AddTable("Start", Kinect::People[P].HipCHipR.Start.ToTable());
				HipCHipR.AddTable("End", Kinect::People[P].HipCHipR.End.ToTable());
				HipCHipR.AddTable("Normalized", Kinect::People[P].HipCHipR.Normalize().ToTable());
				Lua::lua_pushtable(L, HipCHipR.GetTable());
				return 1;
				});
			person.AddFunction("KneeAnkleL", [](lua_State* L) {
				Lua::LuaTableBuilder KneeAnkleL;
				KneeAnkleL.AddTable("Start", Kinect::People[P].KneeAnkleL.Start.ToTable());
				KneeAnkleL.AddTable("End", Kinect::People[P].KneeAnkleL.End.ToTable());
				KneeAnkleL.AddTable("Normalized", Kinect::People[P].KneeAnkleL.Normalize().ToTable());
				Lua::lua_pushtable(L, KneeAnkleL.GetTable());
				return 1;
				});
			person.AddFunction("KneeAnkleR", [](lua_State* L) {
				Lua::LuaTableBuilder KneeAnkleR;
				KneeAnkleR.AddTable("Start", Kinect::People[P].KneeAnkleR.Start.ToTable());
				KneeAnkleR.AddTable("End", Kinect::People[P].KneeAnkleR.End.ToTable());
				KneeAnkleR.AddTable("Normalized", Kinect::People[P].KneeAnkleR.Normalize().ToTable());
				Lua::lua_pushtable(L, KneeAnkleR.GetTable());
				return 1;
				});
			person.AddFunction("AnkleFootL", [](lua_State* L) {
				Lua::LuaTableBuilder AnkleFootL;
				AnkleFootL.AddTable("Start", Kinect::People[P].AnkleFootL.Start.ToTable());
				AnkleFootL.AddTable("End", Kinect::People[P].AnkleFootL.End.ToTable());
				AnkleFootL.AddTable("Normalized", Kinect::People[P].AnkleFootL.Normalize().ToTable());
				Lua::lua_pushtable(L, AnkleFootL.GetTable());
				return 1;
				});
			person.AddFunction("AnkleFootR", [](lua_State* L) {
				Lua::LuaTableBuilder AnkleFootR;
				AnkleFootR.AddTable("Start", Kinect::People[P].AnkleFootR.Start.ToTable());
				AnkleFootR.AddTable("End", Kinect::People[P].AnkleFootR.End.ToTable());
				AnkleFootR.AddTable("Normalized", Kinect::People[P].AnkleFootR.Normalize().ToTable());
				Lua::lua_pushtable(L, AnkleFootR.GetTable());
				return 1;
				});
			
			
			Lua::lua_pushtable(L, person.GetTable());
			return 1;
		}
	);

	Kinecttbl.AddFunction("UpdateSkeletons", [](lua_State* L) {
		VoxelFox::Kinect::UpdateSkeletons();
		return 0;
		});


	VoxelFox.AddTable("Input", Inputtbl.GetTable());
	VoxelFox.AddTable("Math", Math.GetTable());
	VoxelFox.AddTable("Window", lwindow.GetTable());
	VoxelFox.AddTable("Kinect", Kinecttbl.GetTable());

	//controller predefines
	std::vector<std::pair<std::string, int>> predefines = {
		std::pair<std::string,int>("BUTTON_A",BUTTON_A),
		std::pair<std::string,int>("BUTTON_B",BUTTON_B),
		std::pair<std::string,int>("BUTTON_X",BUTTON_X),
		std::pair<std::string,int>("BUTTON_Y",BUTTON_Y),
		std::pair<std::string,int>("BUTTON_LB",BUTTON_LB),
		std::pair<std::string,int>("BUTTON_RB",BUTTON_RB),
		std::pair<std::string,int>("BUTTON_SELECT",BUTTON_SELECT),
		std::pair<std::string,int>("BUTTON_START",BUTTON_START),
		std::pair<std::string,int>("BUTTON_LSTICK",BUTTON_LSTICK),
		std::pair<std::string,int>("BUTTON_RSTICK",BUTTON_RSTICK),
		std::pair<std::string,int>("BUTTON_DPADU",BUTTON_DPADU),
		std::pair<std::string,int>("BUTTON_DPADR",BUTTON_DPADR),
		std::pair<std::string,int>("BUTTON_DPADD",BUTTON_DPADD),
		std::pair<std::string,int>("BUTTON_DPADL",BUTTON_DPADL),
		std::pair<std::string,int>("JOYSTICK_1",GLFW_JOYSTICK_1),
		std::pair<std::string,int>("JOYSTICK_2",GLFW_JOYSTICK_2),
		std::pair<std::string,int>("JOYSTICK_3",GLFW_JOYSTICK_3),
		std::pair<std::string,int>("JOYSTICK_4",GLFW_JOYSTICK_4),
		std::pair<std::string,int>("JOYSTICK_5",GLFW_JOYSTICK_5),
		std::pair<std::string,int>("JOYSTICK_6",GLFW_JOYSTICK_6),
		std::pair<std::string,int>("JOYSTICK_7",GLFW_JOYSTICK_7),
		std::pair<std::string,int>("JOYSTICK_8",GLFW_JOYSTICK_8),
		std::pair<std::string,int>("JOYSTICK_9",GLFW_JOYSTICK_9),
		std::pair<std::string,int>("JOYSTICK_10",GLFW_JOYSTICK_10),
		std::pair<std::string,int>("JOYSTICK_11",GLFW_JOYSTICK_11),
		std::pair<std::string,int>("JOYSTICK_12",GLFW_JOYSTICK_12),
		std::pair<std::string,int>("JOYSTICK_13",GLFW_JOYSTICK_13),
		std::pair<std::string,int>("JOYSTICK_14",GLFW_JOYSTICK_14),
		std::pair<std::string,int>("JOYSTICK_15",GLFW_JOYSTICK_15),
		std::pair<std::string,int>("JOYSTICK_16",GLFW_JOYSTICK_16),
		std::pair<std::string,int>("STICK_1",STICK_1),
		std::pair<std::string, int>("STICK_2", STICK_2),
		std::pair<std::string,int>("TRIGGERS",TRIGGERS)
	};

	for (int i = 0; i < predefines.size(); i++)
	{
		luaVM.SetGlobal(predefines[i].first, predefines[i].second);
	}

	luaVM.SetGlobal("VoxelFox", VoxelFox.GetTable());
	luaVM.RunScript("main.lua");
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