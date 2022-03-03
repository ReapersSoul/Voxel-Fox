P1Color=VoxelFox.Math.Vec3(1,1,1)
P2Color=VoxelFox.Math.Vec3(1,1,1)
P3Color=VoxelFox.Math.Vec3(1,1,1)
P4Color=VoxelFox.Math.Vec3(1,1,1)
P5Color=VoxelFox.Math.Vec3(1,1,1)
P6Color=VoxelFox.Math.Vec3(1,1,1)

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

function Update()
	
end

sizeOne=.01
sizeTwo=.02
sizeThree=.03

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
		if(i==0)then glColor(P1Color.x,P1Color.y,P1Color.z) end;
		if(i==1)then glColor(P2Color.x,P2Color.y,P2Color.z) end;
		if(i==2)then glColor(P3Color.x,P3Color.y,P3Color.z) end;
		if(i==3)then glColor(P4Color.x,P4Color.y,P4Color.z) end;
		if(i==4)then glColor(P5Color.x,P5Color.y,P5Color.z) end;
		if(i==5)then glColor(P6Color.x,P6Color.y,P6Color.z) end;
		--print(i);
		glDrawCube((VoxelFox.Kinect.GetPerson(i).HandR().x*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).HandR().y*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).HandR().z*scalar)-(sizeOne/2),sizeOne)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).HandL().x*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).HandL().y*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).HandL().z*scalar)-(sizeOne/2),sizeOne)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).WristR().x*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).WristR().y*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).WristR().z*scalar)-(sizeOne/2),sizeOne)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).WristL().x*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).WristL().y*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).WristL().z*scalar)-(sizeOne/2),sizeOne)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).ElbowR().x*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).ElbowR().y*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).ElbowR().z*scalar)-(sizeOne/2),sizeOne)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).ElbowL().x*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).ElbowL().y*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).ElbowL().z*scalar)-(sizeOne/2),sizeOne)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).ShoulderR().x*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).ShoulderR().y*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).ShoulderR().z*scalar)-(sizeOne/2),sizeOne)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).ShoulderL().x*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).ShoulderL().y*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).ShoulderL().z*scalar)-(sizeOne/2),sizeOne)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).ShoulderC().x*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).ShoulderC().y*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).ShoulderC().z*scalar)-(sizeOne/2),sizeOne)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).Head().x*scalar)-(sizeThree/2),(VoxelFox.Kinect.GetPerson(i).Head().y*scalar)-(sizeThree/2),(VoxelFox.Kinect.GetPerson(i).Head().z*scalar)-(sizeThree/2),sizeThree)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).Spine().x*scalar)-(sizeTwo/2),(VoxelFox.Kinect.GetPerson(i).Spine().y*scalar)-(sizeTwo/2),(VoxelFox.Kinect.GetPerson(i).Spine().z*scalar)-(sizeTwo/2),sizeTwo)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).HipC().x*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).HipC().y*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).HipC().z*scalar)-(sizeOne/2),sizeOne)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).HipL().x*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).HipL().y*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).HipL().z*scalar)-(sizeOne/2),sizeOne)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).HipR().x*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).HipR().y*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).HipR().z*scalar)-(sizeOne/2),sizeOne)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).KneeL().x*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).KneeL().y*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).KneeL().z*scalar)-(sizeOne/2),sizeOne)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).KneeR().x*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).KneeR().y*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).KneeR().z*scalar)-(sizeOne/2),sizeOne)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).AnkleL().x*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).AnkleL().y*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).AnkleL().z*scalar)-(sizeOne/2),sizeOne)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).AnkleR().x*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).AnkleR().y*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).AnkleR().z*scalar)-(sizeOne/2),sizeOne)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).FootL().x*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).FootL().y*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).FootL().z*scalar)-(sizeOne/2),sizeOne)
		glDrawCube((VoxelFox.Kinect.GetPerson(i).FootR().x*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).FootR().y*scalar)-(sizeOne/2),(VoxelFox.Kinect.GetPerson(i).FootR().z*scalar)-(sizeOne/2),sizeOne)

		glDrawLine(VoxelFox.Kinect.GetPerson(i).ElbowR().x*scalar,VoxelFox.Kinect.GetPerson(i).ElbowR().y*scalar,VoxelFox.Kinect.GetPerson(i).ElbowR().z*scalar,VoxelFox.Kinect.GetPerson(i).WristR().x*scalar,VoxelFox.Kinect.GetPerson(i).WristR().y*scalar,VoxelFox.Kinect.GetPerson(i).WristR().z*scalar,3)
	end

	--glColor(1,.5,1)
	--glDrawBox(0,0,1,1)
	--glColor(1,1,1)
	--glDrawCube(0,0,0,.25)

	--glDrawBezierCurve(12,4,VoxelFox.Math.Vec3(0,0,0),VoxelFox.Math.Vec3(0,1,0),VoxelFox.Math.Vec3(0,1,1),VoxelFox.Math.Vec3(0,0,1))
end

function UI()

  end