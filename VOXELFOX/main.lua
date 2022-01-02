SetVar("rotx",0)
SetVar("roty",0)
SetVar("rotz",0)
SetVar("posx",0)
SetVar("posy",0)
SetVar("posz",0)
SetVar("P1Colr",0)
SetVar("P1Colg",0)
SetVar("P1Colb",0)
SetVar("P2Colr",0)
SetVar("P2Colg",0)
SetVar("P2Colb",0)
SetVar("P3Colr",0)
SetVar("P3Colg",0)
SetVar("P3Colb",0)
SetVar("P4Colr",0)
SetVar("P4Colg",0)
SetVar("P4Colb",0)
SetVar("P5Colr",0)
SetVar("P5Colg",0)
SetVar("P5Colb",0)
SetVar("P6Colr",0)
SetVar("P6Colg",0)
SetVar("P6Colb",0)
SetVar("Speed",0.001)
SetVar("DeadZoneStick1",0.1)
SetVar("DeadZoneStick2",0.1)

--Input Here
VoxelFox.Input.NewInput("c1s1",nil,4,STICK_1,JOYSTICK_1)
VoxelFox.Input.NewInput("c1s2",nil,4,STICK_2,JOYSTICK_1)
VoxelFox.Input.NewInput("c1BA",BUTTON_A,0,nil,nil)
VoxelFox.Input.NewInput("c1BB",BUTTON_B,0,nil,nil)
VoxelFox.Input.NewInput("c1BX",BUTTON_X,0,nil,nil)
VoxelFox.Input.NewInput("c1BY",BUTTON_Y,0,nil,nil)
VoxelFox.Input.NewInput("MousePos",nil,3,nil,nil)
VoxelFox.Input.NewInput("MouseClick",0,2,nil,nil)
VoxelFox.Input.NewInput("KeyBoard_E",69,1,nil,nil)

function Data()
	
--	VoxelFox.Window.GetCam(0).SetRot(VoxelFox.Input.GetInput("c1s2").y*90,VoxelFox.Input.GetInput("c1s2").x*180,GetVar("rotz"))
--	if(VoxelFox.Input.GetInput("c1s1").x>GetVar("DeadZoneStick2") or VoxelFox.Input.GetInput("c1s1").x<-GetVar("DeadZoneStick2")) then
--		SetVar("posx",GetVar("posx")+(VoxelFox.Input.GetInput("c1s1").x*GetVar("Speed")))
--	end
--	if(VoxelFox.Input.GetInput("c1s1").y>GetVar("DeadZoneStick2") or VoxelFox.Input.GetInput("c1s1").y<-GetVar("DeadZoneStick2")) then
--		SetVar("posy",GetVar("posy")+(VoxelFox.Input.GetInput("c1s1").y*GetVar("Speed")))
--	end

--	VoxelFox.Window.GetCam(0).SetPos(-GetVar("posx"),GetVar("posy"),-GetVar("posz"))

--	if(VoxelFox.Input.GetInput("KeyBoard_E") or VoxelFox.Input.GetInput("c1BA")) then
--		print("Reloaded!")
--		VoxelFox.Window.LuaReload()
--	end
end

function UI()

  end

function Draw()
	glColor(1,0,0)
	glDrawLine(1,0,0,-1,0,0,4)
	glColor(0,1,0)
	glDrawLine(0,1,0,0,-1,0,4)
	glColor(0,0,1)
	glDrawLine(0,0,1,0,0,-1,4)
	VoxelFox.Kinect.UpdateSkeletons()
	for i = 0, 5, 1 do
		scalar = .2
		if(i==0)then glColor(GetVar("P1Colr"),GetVar("P1Colg"),GetVar("P1Colb")) end;
		if(i==1)then glColor(GetVar("P2Colr"),GetVar("P2Colg"),GetVar("P2Colb")) end;
		if(i==2)then glColor(GetVar("P3Colr"),GetVar("P3Colg"),GetVar("P3Colb")) end;
		if(i==3)then glColor(GetVar("P4Colr"),GetVar("P4Colg"),GetVar("P4Colb")) end;
		if(i==4)then glColor(GetVar("P5Colr"),GetVar("P5Colg"),GetVar("P5Colb")) end;
		if(i==5)then glColor(GetVar("P6Colr"),GetVar("P6Colg"),GetVar("P6Colb")) end;
		--print(i);
		glDrawCube(VoxelFox.Kinect.GetPerson(i).HandR().x*scalar,VoxelFox.Kinect.GetPerson(i).HandR().y*scalar,VoxelFox.Kinect.GetPerson(i).HandR().z*scalar,.01)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).HandL().x*scalar,VoxelFox.Kinect.GetPerson(i).HandL().y*scalar,VoxelFox.Kinect.GetPerson(i).HandL().z*scalar,.01)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).WristR().x*scalar,VoxelFox.Kinect.GetPerson(i).WristR().y*scalar,VoxelFox.Kinect.GetPerson(i).WristR().z*scalar,.01)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).WristL().x*scalar,VoxelFox.Kinect.GetPerson(i).WristL().y*scalar,VoxelFox.Kinect.GetPerson(i).WristL().z*scalar,.01)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).ElbowR().x*scalar,VoxelFox.Kinect.GetPerson(i).ElbowR().y*scalar,VoxelFox.Kinect.GetPerson(i).ElbowR().z*scalar,.01)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).ElbowL().x*scalar,VoxelFox.Kinect.GetPerson(i).ElbowL().y*scalar,VoxelFox.Kinect.GetPerson(i).ElbowL().z*scalar,.01)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).ShoulderR().x*scalar,VoxelFox.Kinect.GetPerson(i).ShoulderR().y*scalar,VoxelFox.Kinect.GetPerson(i).ShoulderR().z*scalar,.01)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).ShoulderL().x*scalar,VoxelFox.Kinect.GetPerson(i).ShoulderL().y*scalar,VoxelFox.Kinect.GetPerson(i).ShoulderL().z*scalar,.01)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).ShoulderC().x*scalar,VoxelFox.Kinect.GetPerson(i).ShoulderC().y*scalar,VoxelFox.Kinect.GetPerson(i).ShoulderC().z*scalar,.01)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).Head().x*scalar,VoxelFox.Kinect.GetPerson(i).Head().y*scalar,VoxelFox.Kinect.GetPerson(i).Head().z*scalar,.03)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).Spine().x*scalar,VoxelFox.Kinect.GetPerson(i).Spine().y*scalar,VoxelFox.Kinect.GetPerson(i).Spine().z*scalar,.02)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).HipC().x*scalar,VoxelFox.Kinect.GetPerson(i).HipC().y*scalar,VoxelFox.Kinect.GetPerson(i).HipC().z*scalar,.01)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).HipL().x*scalar,VoxelFox.Kinect.GetPerson(i).HipL().y*scalar,VoxelFox.Kinect.GetPerson(i).HipL().z*scalar,.01)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).HipR().x*scalar,VoxelFox.Kinect.GetPerson(i).HipR().y*scalar,VoxelFox.Kinect.GetPerson(i).HipR().z*scalar,.01)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).KneeL().x*scalar,VoxelFox.Kinect.GetPerson(i).KneeL().y*scalar,VoxelFox.Kinect.GetPerson(i).KneeL().z*scalar,.01)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).KneeR().x*scalar,VoxelFox.Kinect.GetPerson(i).KneeR().y*scalar,VoxelFox.Kinect.GetPerson(i).KneeR().z*scalar,.01)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).AnkleL().x*scalar,VoxelFox.Kinect.GetPerson(i).AnkleL().y*scalar,VoxelFox.Kinect.GetPerson(i).AnkleL().z*scalar,.01)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).AnkleR().x*scalar,VoxelFox.Kinect.GetPerson(i).AnkleR().y*scalar,VoxelFox.Kinect.GetPerson(i).AnkleR().z*scalar,.01)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).FootL().x*scalar,VoxelFox.Kinect.GetPerson(i).FootL().y*scalar,VoxelFox.Kinect.GetPerson(i).FootL().z*scalar,.01)
		glDrawCube(VoxelFox.Kinect.GetPerson(i).FootR().x*scalar,VoxelFox.Kinect.GetPerson(i).FootR().y*scalar,VoxelFox.Kinect.GetPerson(i).FootR().z*scalar,.01)

		glDrawLine(VoxelFox.Kinect.GetPerson(i).ElbowR().x*scalar,VoxelFox.Kinect.GetPerson(i).ElbowR().y*scalar,VoxelFox.Kinect.GetPerson(i).ElbowR().z*scalar,VoxelFox.Kinect.GetPerson(i).WristR().x*scalar,VoxelFox.Kinect.GetPerson(i).WristR().y*scalar,VoxelFox.Kinect.GetPerson(i).WristR().z*scalar,3)
	end

	--glColor(1,.5,1)
	--glDrawBox(0,0,1,1)
	--glColor(1,1,1)
	--glDrawCube(0,0,0,.25)

	--glDrawBezierCurve(12,4,VoxelFox.Math.Vec3(0,0,0),VoxelFox.Math.Vec3(0,1,0),VoxelFox.Math.Vec3(0,1,1),VoxelFox.Math.Vec3(0,0,1))
end