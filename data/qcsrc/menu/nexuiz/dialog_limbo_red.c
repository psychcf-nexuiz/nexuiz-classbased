#ifdef INTERFACE
CLASS(NexuizLimboDialogRedTab) EXTENDS(NexuizTab)
	METHOD(NexuizLimboDialogRedTab, fill, void(entity))
	ATTRIB(NexuizLimboDialogRedTab, title, string, "Red")
	ATTRIB(NexuizLimboDialogRedTab, intendedWidth, float, 0.9)
	ATTRIB(NexuizLimboDialogRedTab, rows, float, 8)
	ATTRIB(NexuizLimboDialogRedTab, columns, float, 6.5)
ENDCLASS(NexuizLimboDialogRedTab)
entity makeNexuizLimboDialogRedTab();
#endif

#ifdef IMPLEMENTATION
entity makeNexuizLimboDialogRedTab()
{
	entity me;
	me = spawnNexuizLimboDialogRedTab();
	me.configureDialog(me);
	return me;
}


void fillNexuizLimboDialogRedTab(entity me)
{
	entity e;
	float n;
	n = 6 + !!cvar("developer");
	me.TR(me);
		me.TD(me, 1, 1, e = makeNexuizTextLabel(0, "Class:"));
	me.TR(me);
		me.TD(me, 1, 5.5 / n, e = makeNexuizRadioButton(0.1, "player_class", "soldier", "Soldier"));
	me.TR(me);
		me.TD(me, 1, 5.5 / n, e = makeNexuizRadioButton(0.1, "player_class", "medic", "Medic"));
	me.TR(me);
		me.TD(me, 1, 5.5 / n, e = makeNexuizRadioButton(0.1, "player_class", "engineer", "Engineer"));
	me.TR(me);
		me.TD(me, 1, 5.5 / n, e = makeNexuizRadioButton(0.1, "player_class", "fieldops", "Field Ops"));
	me.TR(me);
		me.TD(me, 1, 5.5 / n, e = makeNexuizRadioButton(0.1, "player_class", "covertops", "Covert Ops"));
	me.TR(me);

	me.gotoRC(me, me.rows - 1, 0);
		me.TD(me, 1, me.columns/2, makeNexuizCommandButton("Deploy", '1 0.5 0.5', "cmd selectclass $player_class\ncmd selectteam red\ncmd join", 1));
		me.TD(me, 1, me.columns/2, makeNexuizCommandButton("Respawn", '1 0.5 0.5', "kill\n", 1));
}

#endif
