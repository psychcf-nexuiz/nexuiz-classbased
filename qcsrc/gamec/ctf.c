
.entity flagcarried;

//float FLAGSCORE_PICKUP        =  1;
//float FLAGSCORE_RETURN        =  5; // returned by owner team
//float FLAGSCORE_RETURNROGUE   = 10; // returned by rogue team
//float FLAGSCORE_CAPTURE       =  5;
//float FLAGSCORE_CAPTURE_TEAM  = 20;

float FLAG_BASE = 1;
float FLAG_CARRY = 2;
float FLAG_DROPPED = 3;

void() FlagThink;
void() FlagTouch;

void() place_flag =
{
	if(!self.t_width)
		self.t_width = 0.1; // frame animation rate
	if(!self.t_length)
		self.t_length = 119; // maximum frame
		
	self.mdl = self.model;
	self.flags = FL_ITEM;
	self.solid = SOLID_TRIGGER;
	self.movetype = MOVETYPE_TOSS;
	self.velocity = '0 0 0';
	self.origin_z = self.origin_z + 6;
	self.think = FlagThink;
	self.touch = FlagTouch;
	self.nextthink = time + 0.1;
	self.cnt = FLAG_BASE;
	self.mangle = self.angles;
	//self.effects = self.effects | EF_DIMLIGHT;
	if (!droptofloor(0, 0))
	{
		dprint("Flag fell out of level at ", vtos(self.origin), "\n");
		remove(self);
		return;
	}
	self.oldorigin = self.origin;
};

void(entity e) RegenFlag =
{
	e.movetype = MOVETYPE_TOSS;
	e.solid = SOLID_TRIGGER;
	// TODO: play a sound here
	sound (e, CHAN_AUTO, self.noise3, 1, ATTN_NONE);
	setorigin(e, e.oldorigin);
	e.angles = e.mangle;
	e.cnt = FLAG_BASE;
	e.owner = world;
	e.flags = FL_ITEM; // clear FL_ONGROUND and any other junk
};

void(entity e) ReturnFlag =
{
	if (e.owner)
	if (e.owner.flagcarried == e)
		e.owner.flagcarried = world;
	e.owner = world;
	RegenFlag(e);
};

void(entity e) DropFlag =
{
	local entity p;

	if (!e.owner)
	{
		dprint("FLAG: drop - no owner?!?!\n");
		return;
	}
	p = e.owner;
	if (p.flagcarried != e)
	{
		dprint("FLAG: drop - owner is not carrying this flag??\n");
		return;
	}
	bprint(p.netname);
	if (e.team == 5)
		bprint(" lost the RED flag\n");
	else
		bprint(" lost the BLUE flag\n");
	if (p.flagcarried == e)
		p.flagcarried = world;
	e.owner = world;

	e.flags = FL_ITEM; // clear FL_ONGROUND and any other junk
	e.solid = SOLID_TRIGGER;
	e.movetype = MOVETYPE_TOSS;
	// setsize(e, '-16 -16 0', '16 16 74');
	setorigin(e, p.origin - '0 0 24');
	e.cnt = FLAG_DROPPED;
	e.velocity = '0 0 300';
	e.pain_finished = time + cvar("g_ctf_flag_returntime");//30;
};

void AnimateFlag()
{
	if(self.delay > time)
		return;
	self.delay = time + self.t_width;
	if(self.nextthink > self.delay)
		self.nextthink = self.delay;

	self.frame = self.frame + 1;
	if(self.frame > self.t_length)
		self.frame = 0;
}

void() FlagThink =
{
	local entity e;
	local vector v;
	local float f;

	self.nextthink = time + 0.1;
	
	AnimateFlag();

	if (self.cnt == FLAG_BASE)
		return;

	if (self.cnt == FLAG_DROPPED)
	{
		if (time > self.pain_finished)
		{
			if (self.team == 5)
				bprint("The RED flag has returned to base\n");
			else
				bprint("The BLUE flag has returned to base\n");
			ReturnFlag(self);
		}
		return;
	}

	e = self.owner;
	if (e.classname != "player" || (e.deadflag) || (e.flagcarried != self))
	{
		DropFlag(self);
		return;
	}

	// borrowed from threewave CTF, because it would be way too much work

	makevectors (e.angles);
	v = v_forward;
	v_z = 0 - v_z; // reverse z

	f = 14;
	/*
	if (e.frame >= 29 && e.frame <= 40)
	{
		if (e.frame >= 29 && e.frame <= 34)
		{
			//axpain
			if      (e.frame == 29) f = f + 2;
			else if (e.frame == 30) f = f + 8;
			else if (e.frame == 31) f = f + 12;
			else if (e.frame == 32) f = f + 11;
			else if (e.frame == 33) f = f + 10;
			else if (e.frame == 34) f = f + 4;
		}
		else if (e.frame >= 35 && e.frame <= 40)
		{
			// pain
			if      (e.frame == 35) f = f + 2;
			else if (e.frame == 36) f = f + 10;
			else if (e.frame == 37) f = f + 10;
			else if (e.frame == 38) f = f + 8;
			else if (e.frame == 39) f = f + 4;
			else if (e.frame == 40) f = f + 2;
		}
	}
	else if (e.frame >= 103 && e.frame <= 118)
	{
		if      (e.frame >= 103 && e.frame <= 104) f = f + 6;  //nailattack
		else if (e.frame >= 105 && e.frame <= 106) f = f + 6;  //light
		else if (e.frame >= 107 && e.frame <= 112) f = f + 7;  //rocketattack
		else if (e.frame >= 112 && e.frame <= 118) f = f + 7;  //shotattack
	}
	*/
	self.angles = e.angles + '0 0 -45';
	setorigin (self, e.origin + '0 0 0' - f*v + v_right * 22);
	self.nextthink = time + 0.01;
};

float   flagcaptimerecord;
.float  flagpickuptime;

void() FlagTouch =
{
	local float t;
	local entity head;
	if (other.classname != "player")
		return;
	if (other.health < 1) // ignore dead players
		return;

	if (self.cnt == FLAG_CARRY)
		return;

	if (self.cnt == FLAG_BASE)
	if (other.team == self.team)
	if (other.flagcarried) // he's got a flag
	if (other.flagcarried.team != self.team) // capture
	{
		t = time - other.flagpickuptime;
		if (flagcaptimerecord == 0)
		{
			if (other.flagcarried.team == 5)
				bprint(other.netname, " captured the RED flag in ", ftos(t), " seconds\n");
			else
				bprint(other.netname, " captured the BLUE flag in ", ftos(t), " seconds\n");
			flagcaptimerecord = t;
		}
		else if (t < flagcaptimerecord)
		{
			if (other.flagcarried.team == 5)
				bprint(other.netname, " captured the RED flag in ", ftos(t), ", breaking the previous record of", ftos(flagcaptimerecord), " seconds\n");
			else
				bprint(other.netname, " captured the BLUE flag in ", ftos(t), ", breaking the previous record of", ftos(flagcaptimerecord), " seconds\n");
			flagcaptimerecord = t;
		}
		else
		{
			if (other.flagcarried.team == 5)
				bprint(other.netname, " captured the RED flag in ", ftos(t), ", failing to break the previous record of", ftos(flagcaptimerecord), " seconds\n");
			else
				bprint(other.netname, " captured the BLUE flag in ", ftos(t), ", failing to break the previous record of", ftos(flagcaptimerecord), " seconds\n");
		}
		other.frags = other.frags + cvar("g_ctf_flagscore_capture");//FLAGSCORE_CAPTURE;
		head = find(head, classname, "player");
		while (head)
		{
			if (head.team == self.team)
				head.frags = head.frags + cvar("g_ctf_flagscore_capture_team");//FLAGSCORE_CAPTURE_TEAM;
			head = find(head, classname, "player");
		}
		sound (self, CHAN_AUTO, self.noise2, 1, ATTN_NONE);
		ReturnFlag(other.flagcarried);
	}
	if (self.cnt == FLAG_BASE)
	if (other.team == 5 || other.team == 14) // only red and blue team can steal flags
	if (other.team != self.team)
	if (!other.flagcarried)
	{
		// pick up
		other.flagpickuptime = time; // used for timing runs
		self.solid = SOLID_NOT;
		setorigin(self, self.origin); // relink
		self.owner = other;
		other.flagcarried = self;
		self.cnt = FLAG_CARRY;
		if (other.flagcarried.team == 5)
			bprint(other.netname, " got the RED flag\n");
		else
			bprint(other.netname, " got the BLUE flag\n");
		other.frags = other.frags + cvar("g_ctf_flagscore_pickup");//FLAGSCORE_PICKUP;
		sound (self, CHAN_AUTO, self.noise, 1, ATTN_NONE);
		return;
	}

	if (self.cnt == FLAG_DROPPED)
	{
		self.flags = FL_ITEM; // clear FL_ONGROUND and any other junk
		if (other.team == self.team || (other.team != 5 && other.team != 14))
		{
			// return flag
			if (self.team == 5)
				bprint(other.netname, " returned the RED flag\n");
			else
				bprint(other.netname, " returned the BLUE flag\n");
			if (other.team == 5 || other.team == 14)
				other.frags = other.frags + cvar("g_ctf_flagscore_return");//FLAGSCORE_RETURN;
			else
				other.frags = other.frags + cvar("g_ctf_flagscore_return_rogue");//FLAGSCORE_RETURNROGUE;
			sound (self, CHAN_AUTO, self.noise1, 1, ATTN_NONE);
			ReturnFlag(self);
		}
		else if (!other.flagcarried)
		{
			// pick up
			other.flagpickuptime = time; // used for timing runs
			self.solid = SOLID_NOT;
			setorigin(self, self.origin); // relink
			self.owner = other;
			other.flagcarried = self;
			self.cnt = FLAG_CARRY;
			if (self.team == 5)
				bprint(other.netname, " picked up the RED flag\n");
			else
				bprint(other.netname, " picked up the BLUE flag\n");
			other.frags = other.frags + cvar("g_ctf_flagscore_pickup");//FLAGSCORE_PICKUP;
			sound (self, CHAN_AUTO, self.noise, 1, ATTN_NONE);
		}
	}
};

/*QUAKED info_player_team1 (1 0 0) (-16 -16 -24) (16 16 24)
CTF Starting point for a player
in team one (Red).

Keys:
"angle"
 viewing angle when spawning
*/
void() info_player_team1 = 
{
	if(!cvar("g_ctf"))
		self.classname = "info_player_deathmatch";
};
//self.team = 4;self.classname = "info_player_start";info_player_start();};

/*QUAKED info_player_team2 (1 0 0) (-16 -16 -24) (16 16 24)
CTF Starting point for a player in
team two (Blue).

Keys:
"angle"
 viewing angle when spawning
*/
void() info_player_team2 =
{
	if(!cvar("g_ctf"))
		self.classname = "info_player_deathmatch";
};
//self.team = 13;self.classname = "info_player_start";info_player_start();};

/*QUAKED info_player_team3 (1 0 0) (-16 -16 -24) (16 16 24)
CTF Starting point for a player in
team three (Green).

Keys:
"angle"
 viewing angle when spawning
*/
void() info_player_team3 =
{
	if(!cvar("g_ctf"))
		self.classname = "info_player_deathmatch";
};


/*QUAKED info_player_team4 (1 0 0) (-16 -16 -24) (16 16 24)
CTF Starting point for a player in
team four (Magenta).

Keys:
"angle"
 viewing angle when spawning
*/
void() info_player_team4 =
{
	if(!cvar("g_ctf"))
		self.classname = "info_player_deathmatch";
};




/*QUAKED item_flag_team1 (0 0.5 0.8) (-48 -48 -37) (48 48 37)
CTF flag for team one (Red).
Multiple are allowed.

Keys:
"angle"
 Angle the flag will point
(minus 90 degrees)
"model"
 model to use, note this needs red and blue as skins 0 and 1
 (default models/ctf/flag.md3)
"noise"
 sound played when flag is picked up
 (default ctf/take.wav)
"noise1"
 sound played when flag is returned by a teammate
 (default ctf/return.wav)
"noise2"
 sound played when flag is captured
 (default ctf/capture.wav)
"noise3"
 sound played when flag is lost in the field and respawns itself
 (default ctf/respawn.wav)
*/

void() item_flag_team1 =
{
	if (!cvar("g_ctf"))
		return;
	//if(!cvar("teamplay"))
	//	cvar_set("teamplay", "3");

	self.team = 5; // color 4 team (red)
	self.items = IT_KEY2; // gold key (redish enough)
	self.skin = 0;
	if (!self.model)
		self.model = "models/ctf/flag_red.md3";
	if (!self.noise)
		self.noise = "ctf/take.wav";
	if (!self.noise1)
		self.noise1 = "ctf/return.wav";
	if (!self.noise2)
		self.noise2 = "ctf/capture.wav";
	if (!self.noise3)
		self.noise3 = "ctf/respawn.wav";
	precache_model (self.model);
	setmodel (self, self.model);
	precache_sound (self.noise);
	precache_sound (self.noise1);
	precache_sound (self.noise2);
	precache_sound (self.noise3);
	setsize(self, '-16 -16 -37', '16 16 37');
	setorigin(self, self.origin + '0 0 37');
	self.nextthink = time + 0.2; // start after doors etc
	self.think = place_flag;

	if(!self.scale)
		self.scale = 0.6;
	//if(!self.glow_size)
	//	self.glow_size = 50;
	
	self.effects = self.effects | EF_FULLBRIGHT | EF_LOWPRECISION;
};

/*QUAKED item_flag_team2 (0 0.5 0.8) (-48 -48 -24) (48 48 64)
CTF flag for team two (Blue).
Multiple are allowed.

Keys:
"angle"
 Angle the flag will point
(minus 90 degrees)

*/

void() item_flag_team2 =
{
	if (!cvar("g_ctf"))
		return;
	//if(!cvar("teamplay"))
	//	cvar_set("teamplay", "3");

	self.team = 14; // color 13 team (blue)
	self.items = IT_KEY1; // silver key (bluish enough)
	self.skin = 1;
	if (!self.model)
		self.model = "models/ctf/flag_blue.md3";
	if (!self.noise)
		self.noise = "ctf/take.wav";
	if (!self.noise1)
		self.noise1 = "ctf/return.wav";
	if (!self.noise2)
		self.noise2 = "ctf/capture.wav";
	if (!self.noise3)
		self.noise3 = "ctf/respawn.wav";
	precache_model (self.model);
	setmodel (self, self.model);
	precache_sound (self.noise);
	precache_sound (self.noise1);
	precache_sound (self.noise2);
	precache_sound (self.noise3);
	setsize(self, '-16 -16 -37', '16 16 37');
	setorigin(self, self.origin + '0 0 37');
	self.nextthink = time + 0.2; // start after doors etc
	self.think = place_flag;

	if(!self.scale)
		self.scale = 0.6;
	//if(!self.glow_size)
	//	self.glow_size = 50;
	
	self.effects = self.effects | EF_FULLBRIGHT | EF_LOWPRECISION;
};


/*QUAKED ctf_team (0 .5 .8) (-16 -16 -24) (16 16 32)
Team declaration for CTF gameplay, this allows you to decide what team
names and control point models are used in your map.

Note: If you use ctf_team entities you must define at least 2!  However, unlike
domination, you don't need to make a blank one too.

Keys:
"netname"
 Name of the team (for example Red, Blue, Green, Yellow, Life, Death, Offense, Defense, etc)
"cnt"
 Scoreboard color of the team (for example 4 is red and 13 is blue)

*/

void() ctf_team =
{
	self.classname = "ctf_team";
	self.team = self.cnt + 1;
};

// code from here on is just to support maps that don't have control point and team entities
void ctf_spawnteam (string teamname, float teamcolor)
{
	local entity oldself;
	oldself = self;
	self = spawn();
	self.classname = "ctf_team";
	self.netname = teamname;
	self.cnt = teamcolor;

	ctf_team();

	self = oldself;
};

// spawn some default teams if the map is not set up for ctf
void() ctf_spawnteams =
{
	float numteams;

	numteams = 2;//cvar("g_ctf_default_teams");

	ctf_spawnteam("Red", 4);
	ctf_spawnteam("Blue", 13);
};

void() ctf_delayedinit =
{
	self.think = SUB_Remove;
	self.nextthink = time;
	// if no teams are found, spawn defaults
	if (find(world, classname, "ctf_team") == world)
		ctf_spawnteams();
};

void() ctf_init =
{
	local entity e;
	e = spawn();
	e.think = ctf_delayedinit;
	e.nextthink = time + 0.1;
};

