math.randomseed(os.time())

function setup(startingTeam)

	local formation =
	{
		Vector2.new(-field.zoneLvl - 14, -field.height * 1/10),
		Vector2.new(-field.zoneLvl - 14, field.height * 1/10),
		Vector2.new(-field.zoneLvl - 11, -field.height * 3/10),
		Vector2.new(-field.zoneLvl - 11, field.height * 3/10),
		Vector2.new(-field.zoneLvl - 7, 0),
		Vector2.new(-field.zoneLvl, -field.height * 1/6),
		Vector2.new(-field.zoneLvl, field.height * 1/6)
	}

	local targetFormation =
	{
		Vector2.new(-field.zoneLvl + 5, 0),
		Vector2.new(-field.zoneLvl + 15, -field.height * 1/6),
		Vector2.new(-field.zoneLvl + 15, field.height * 1/6),
		Vector2.new(-5, 0),
		Vector2.new(20, -field.height * 1/3),
		Vector2.new(20, field.height * 1/3),
		Vector2.new(field.zoneLvl + 3, 0)
	}

	for i = 1, 7 do
		p = myTeam.players[i]
		p.targetPos = targetFormation[i]
		p.holdPos = false
	end

	return formation
end

function start()
	if(disc.possessingPlayer ~= nil and disc.possessingPlayer.team == myTeam) then
		passTacticaly(disc.possessingPlayer)
	end
end

function update(elapsedSeconds)
	for i = 1, 7 do
		p = myTeam.players[i]
		dist = p.targetPos - p.position

		if (dist:length() > 1) and not p.holdPos then
			p:setSpeed(dist:norm() * 8)
		else
			p:setSpeed(Vector2.new())
		end
	end
end

function onDiscWithinReach(player)
	player:snatch()

	for i = 1, 7 do
		p = myTeam.players[i]
		p.holdPos = false
	end

	passTacticaly(player)
end

function getTacticleTargets(player)
	local targets = {}

	n = player.number
	ps = myTeam.players

	if n == 1 then
		targets = {ps[2], ps[3]}
	elseif n == 2 then
		targets = {ps[1], ps[4]}
	elseif n == 3 then
		targets = {ps[1], ps[4]}
	elseif n == 4 then
		targets = {ps[2], ps[3], ps[5], ps[6]}
	elseif n == 5 then
		targets = {ps[4], ps[7]}
	elseif n == 6 then
		targets = {ps[4], ps[7]}
	elseif n == 7 then
		targets = {ps[5], ps[6]}
	end
	return targets
end

function playTo(possessingPlayer, targetPlayer)
	possessingPlayer:shoot((targetPlayer.position - possessingPlayer.position):norm() * 10)

	possessingPlayer.holdPos = false
	targetPlayer.holdPos = true
end

function passTacticaly(player)
	playTo(player, chooseRand(getTacticleTargets(player)))
end

function chooseRand(arr)
	local len = #arr
	if len > 0 then
		return arr[math.random(#arr)]
	else
		return nil
	end
end