math.randomseed(os.time())

formation =
{
	Vector2.new(-field.zoneLvl + 5, 0),
	Vector2.new(-field.zoneLvl + 15, -field.height * 1/6),
	Vector2.new(-field.zoneLvl + 15, field.height * 1/6),
	Vector2.new(-5, 0),
	Vector2.new(20, -field.height * 1/3),
	Vector2.new(20, field.height * 1/3),
	Vector2.new(field.zoneLvl + 3, 0)
}

function setup(startingTeam)

	local startingFormation = {}
	--{
	--	Vector2.new(-field.zoneLvl - 14, -field.height * 1/10),
	--	Vector2.new(-field.zoneLvl - 14, field.height * 1/10),
	--	Vector2.new(-field.zoneLvl - 11, -field.height * 3/10),
	--	Vector2.new(-field.zoneLvl - 11, field.height * 3/10),
	--	Vector2.new(-field.zoneLvl - 7, 0),
	--	Vector2.new(-field.zoneLvl, -field.height * 1/6),
	--	Vector2.new(-field.zoneLvl, field.height * 1/6)
	--}
	
	local offset = field.height/8

	for i = 1, 7 do
		p = myTeam.players[i]
		p.targetPos = formation[i]
		p.holdPos = false

		--local factor = -(2*((i+1)%2)-1) * i + math.ceil(i/2) * (2*((i+1)%2)-1)
		local factor = -(2*((i+1)%2)-1) * (math.ceil(i/2) - i)
		startingFormation[i] = Vector2.new(-field.zoneLvl, offset * factor)
	end

	return startingFormation
end

function start()
	handleInterception()
end

function update(elapsedSeconds)
	for i = 1, 7 do
		p = myTeam.players[i]
		--if not p.isInterceptor then
			--if disc.possessingPlayer == opponentTeam.players[i] then
				--p.targetPos = opponentTeam.players[i].position + Vector2.new(-4, 0)
			--else
				--p.targetPos = opponentTeam.players[i].position + (disc.position - opponentTeam.players[i].position):norm() * 4
			--end
		--end
	end

	handleTargetPositions()
end

function onDiscWithinReach(player)
	player:snatch()

	for i = 1, 7 do
		p = myTeam.players[i]
		p.holdPos = false
		p.isInterceptor = false
		p.targetPos = formation[i]
	end
	passTacticaly(player)
end

function onShot()
	handleInterception()
end

function handleTargetPositions()
	for i = 1, 7 do
		p = myTeam.players[i]
		dist = p.targetPos - p.position

		if (dist:length() > 0.1) --[[and not p.holdPos]] then
			p:setSpeed(dist:norm() * Player.maxSpeed)
		else
			p:setSpeed(Vector2.new())
			p.isInterceptor = false
			p.targetPos = formation[i]
		end
	end
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
	possessingPlayer:shoot((targetPlayer.position - possessingPlayer.position):norm() * Disc.maxSpeed)

	possessingPlayer.holdPos = false
	--targetPlayer.holdPos = true

	handleInterception()
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

function cosarctan(x)
	return 1/math.sqrt(1 + x*x)
end

function slopeAngle(v)
	return math.atan2(v.y,v.x)
end

function calcInterception(player)
	local dab = player.position - disc.position
	local alpha = slopeAngle(disc.speed) - slopeAngle(dab)
	local cosalpha = math.cos(alpha)
	local ab = dab:length()
	local vd = disc.speed:length()
	local vp = Player.maxSpeed
	local radicand = (vd * cosalpha) * (vd * cosalpha) - (vd * vd - vp * vp)
	local possible = radicand >= 0

	local t
	local inPos

	if possible then
		t = ab * (-math.sqrt(radicand) + vd * cosalpha)/(vd * vd - vp * vp)
		inPos = disc.position + disc.speed * t
		possible = possible and t >= 0
	end

	return possible, t, inPos
end

function handleInterception()
	local foundP = false
	local inter = {}
	local i
	for i = 1, 14 do
		local team
		local playerNumber
		if i <= 7 then
			team = myTeam
			playerNumber = i
		else
			team = opponentTeam
			playerNumber = i - 7
		end
		local p = team.players[playerNumber]
		local possible, t, inPos = calcInterception(p)
		if possible and isInField(inPos) then
			if (not foundP) or (inter.t > t) then
				inter = {player = p, t = t, inPos = inPos}
			end
			foundP = true
		end
	end

	if foundP then
	local p = inter.player
		if p.team == myTeam then
			p.isInterceptor = true
			p.targetPos = inter.inPos
		end
	end
end

function isInField(v)
	return math.abs(v.x) <= field.boundLvl and math.abs(v.y) <= field.boundExtent
end