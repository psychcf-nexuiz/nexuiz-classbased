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
	self.weapons = W_WeaponBit(WEP_LASER) | W_WeaponBit(WEP_SHOTGUN);
	if(self.gamerole == "soldier") {
		//soldiers blow stuff up
		self.weapons = self.weapons | W_WeaponBit(WEP_ROCKET_LAUNCHER) | W_WeaponBit(WEP_HAGAR) | W_WeaponBit(WEP_GRENADE_LAUNCHER);
		weapon_action(WEP_ROCKET_LAUNCHER, WR_PRECACHE);
		weapon_action(WEP_HAGAR, WR_PRECACHE);
		weapon_action(WEP_GRENADE_LAUNCHER, WR_PRECACHE);
		self.ammo_rockets = cvar("g_balance_classbased_soldier_start_ammo_rockets");
	}
	if(self.gamerole == "covertops") {
		//covert ops get to snipe
		self.weapons = self.weapons | W_WeaponBit(WEP_NEX);
		weapon_action(WEP_NEX, WR_PRECACHE);
		self.ammo_cells = cvar("g_balance_classbased_covert_start_ammo_cells");
	}
	if(self.gamerole == "engineer"
	|| self.gamerole == "fieldops"
	|| self.gamerole == "medic") {
		//all these guys need lightweight weapons
		self.weapons = self.weapons | W_WeaponBit(WEP_UZI);
		weapon_action(WEP_UZI, WR_PRECACHE);
		self.ammo_shells = cvar("g_balance_classbased_start_ammo_shells");
		//they also get utility items, which behave differently depending on the class
		self.weapons = self.weapons | W_WeaponBit(WEP_CRYLINK) | W_WeaponBit(WEP_ELECTRO);
		weapon_action(WEP_ELECTRO, WR_PRECACHE);
		weapon_action(WEP_CRYLINK, WR_PRECACHE);
		self.ammo_cells = cvar("g_balance_classbased_start_ammo_cells");
		self.ammo_nails = cvar("g_balance_classbased_start_ammo_nails");
	}
	if(self.gamerole == "engineer"
	|| self.gamerole == "fieldops") {
		self.weapons = self.weapons | W_WeaponBit(WEP_GRENADE_LAUNCHER);
		weapon_action(WEP_GRENADE_LAUNCHER, WR_PRECACHE);
		self.ammo_rockets = cvar("g_balance_classbased_start_ammo_rockets");
	}
}

float cb_getmaxh(entity e)
{
	local string maxh = strcat("g_balance_classbased_", e.gamerole, "_health_stable");

	return cvar(maxh);
}

void cb_autoselect_class(entity e)
{
    entity p;
    float r;
    if(!cvar("g_classbased"))
    {
        return; //don't need to do this if classbased mode is disabled
    }

    classtally[CLASS_MEDIC] = 0;
    classtally[CLASS_ENGINEER] = 0;
    classtally[CLASS_FIELDOPS] = 0;
    classtally[CLASS_COVERTOPS] = 0;
    classtally[CLASS_SOLDIER] = 0;
    // tally up how much of each class the team has
    FOR_EACH_CLIENT(p)
    {
        if(p.team == e.team)
        {
            // probably not the best way of doing this...
            if(p.gamerole == "medic")
            {
                classtally[CLASS_MEDIC] = classtally[CLASS_MEDIC]+1;
            }
            if(p.gamerole == "engineer")
            {
                classtally[CLASS_ENGINEER] = classtally[CLASS_ENGINEER]+1;
            }
            if(p.gamerole == "fieldops")
            {
                classtally[CLASS_FIELDOPS] = classtally[CLASS_FIELDOPS]+1;
            }
            if(p.gamerole == "covertops")
            {
                classtally[CLASS_COVERTOPS] = classtally[CLASS_COVERTOPS]+1;
            }
            if(p.gamerole == "soldier")
            {
                classtally[CLASS_SOLDIER] = classtally[CLASS_SOLDIER]+1;
            }
        }
    }
    // now figure out which class is needed the most
    // this probably isn't the right way to do it, but
    // I don't know quakeC well enough to do it any other way
    e.gamerole = "soldier";
    if(classtally[CLASS_SOLDIER] > classtally[CLASS_MEDIC])
    {
        e.gamerole = "medic";
    }
    else if(classtally[CLASS_MEDIC] > classtally[CLASS_ENGINEER])
    {
        e.gamerole = "engineer";
    }
    else if(classtally[CLASS_ENGINEER] > classtally[CLASS_FIELDOPS])
    {
        e.gamerole = "fieldops";
    }
    else if(classtally[CLASS_FIELDOPS] > classtally[CLASS_COVERTOPS])
    {
        e.gamerole = "covertops";
    }
    else
    {
        //pick at random
        r = random() * NUMBER_OF_CLASSES;
        if(r < CLASS_MEDIC)
        {
            e.gamerole = "medic";
        }
        else if(r < CLASS_ENGINEER)
        {
            e.gamerole = "engineer";
        }
        else if(r < CLASS_FIELDOPS)
        {
            e.gamerole = "fieldops";
        }
        else if(r < CLASS_COVERTOPS)
        {
            e.gamerole = "covertops";
        }
        else
        {
            e.gamerole = "soldier";
        }
    }
    return;
}
