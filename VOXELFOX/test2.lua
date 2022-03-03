leds=4;

id={false,false,false,false}

function nextId(_id) 
  ret=_id
  if ret[1]==false then 
      ret[1]=true 
  else
    if ret[2]==false then
      ret[2]=true
    else
      if ret[3]==false then
        ret[3]=true
      else
        if ret[4]==false then
          ret[4]=true
        end
      end
  end
end
return ret
end

while true do
  id=nextId(id)
  print(id[1],id[2],id[3],id[4])
  end