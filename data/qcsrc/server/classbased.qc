void cb_changerole(entity e, string class) {
	local string cmodel;
	if(class == "medic") {
		if(e.gamerole != class) bprint("^4", e.netname, " is now a Medic\n");
		e.gamerole = "medic";
		cmodel = "models/player/pyria.zym";
	}
	else if(class == "engineer") {
		if(e.gamerole != class) bprint("^4", e.netname, " is now an Engineer\n");
		e.gamerole = "engineer";
		cmodel = "models/player/shock.zym";
	}
	else if(class == "fieldops") {
		if(e.gamerole != class) bprint("^4", e.netname, " is now a Field Ops\n");
		e.gamerole = "fieldops";
		cmodel = "models/player/insurrectionist.zym";
	}
	else if(class == "covertops") {
		if(e.gamerole != class) bprint("^4", e.netname, " is now a Covert Ops\n");
		e.gamerole = "covertops";
		cmodel = "models/player/specop.zym";
	}
	else { //soldier
		if(e.gamerole != class) bprint("^4", e.netname, " is now a Soldier\n");
		e.gamerole = "soldier";
		cmodel = "models/player/marine.zym";
	}
	precache_model(cmodel);
	setmodel(e, cmodel);
}

void cb_weapmodifiers(void) {
	self.weapons = WEP_LASER | WEP_SHOTGUN;
	if(self.gamerole == "soldier") {
		//soldiers blow stuff up
		self.weapons = self.weapons | WEP_ROCKET_LAUNCHER | WEP_HAGAR | WEP_GRENADE_LAUNCHER;
		weapon_action(WEP_ROCKET_LAUNCHER, WR_PRECACHE);
		weapon_action(WEP_HAGAR, WR_PRECACHE);
		weapon_action(WEP_GRENADE_LAUNCHER, WR_PRECACHE);
		self.ammo_rockets = cvar("g_balance_classbased_soldier_start_ammo_rockets");
	}
	if(self.gamerole == "covertops") {
		//covert ops get to snipe
		self.weapons = self.weapons | WEP_NEX;
		weapon_action(WEP_NEX, WR_PRECACHE);
		self.ammo_cells = cvar("g_balance_classbased_covert_start_ammo_cells");
	}
	if(self.gamerole == "engineer"
	|| self.gamerole == "fieldops"
	|| self.gamerole == "medic") {
		//all these guys need lightweight weapons
		self.weapons = self.weapons | WEP_UZI;
		weapon_action(WEP_UZI, WR_PRECACHE);
		self.ammo_shells = cvar("g_balance_classbased_start_ammo_shells");
		//they also get utility items, which behave differently depending on the class
		self.weapons = self.weapons | WEP_CRYLINK | WEP_ELECTRO;
		weapon_action(WEP_ELECTRO, WR_PRECACHE);
		weapon_action(WEP_CRYLINK, WR_PRECACHE);
		self.ammo_cells = cvar("g_balance_classbased_start_ammo_cells");
		self.ammo_nails = cvar("g_balance_classbased_start_ammo_nails");
	}
	if(self.gamerole == "engineer"
	|| self.gamerole == "fieldops") {
		self.weapons = self.weapons | WEP_GRENADE_LAUNCHER;
		weapon_action(WEP_GRENADE_LAUNCHER, WR_PRECACHE);
		self.ammo_rockets = cvar("g_balance_classbased_start_ammo_rockets");
	}
}

float cb_getmaxh(entity e)
{
	local string maxh = strcat("g_balance_classbased_", e.gamerole, "_health_stable");

	return cvar(maxh);
}
