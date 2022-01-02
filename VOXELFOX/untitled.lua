enemyhp=10
playerhp=10


while(playerhp>0) do
  print("Enemy HP: ",enemyhp)
  print("Player HP: ",playerhp)
  input=io.read()
  if(input=="attack") then
    enemyhp =enemyhp-1
  elseif(input=="rest") then
    playerhp=playerhp+2
  end
  playerhp=playerhp-1
end