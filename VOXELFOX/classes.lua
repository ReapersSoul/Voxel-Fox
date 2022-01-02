function Vec3()
  return{x=0,y=0,z=0}
end


function Mesh()
return{points={},
  Pos=Vec3(),
  Rot=Vec3(),
  getAppliedMesh=function (mesh)
    tmp={}
    for k, v in pairs(mesh) do
      tmp[k]=v+Pos
    end

    return tmp
  end,

  }
end

m=Mesh()
m.Pos.x=2

print("Loaded Classes!")