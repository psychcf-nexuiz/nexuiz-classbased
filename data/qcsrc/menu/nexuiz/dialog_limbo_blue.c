#ifdef INTERFACE
CLASS(NexuizLimboDialogBlueTab) EXTENDS(NexuizTab)
	METHOD(NexuizLimboDialogBlueTab, fill, void(entity))
	ATTRIB(NexuizLimboDialogBlueTab, title, string, "Blue")
	ATTRIB(NexuizLimboDialogBlueTab, intendedWidth, float, 0.9)
	ATTRIB(NexuizLimboDialogBlueTab, rows, float, 8)
	ATTRIB(NexuizLimboDialogBlueTab, columns, float, 6.5)
ENDCLASS(NexuizLimboDialogBlueTab)
entity makeNexuizLimboDialogBlueTab();
#endif

#ifdef IMPLEMENTATION
entity makeNexuizLimboDialogBlueTab()
{
	entity me;
	me = spawnNexuizLimboDialogBlueTab();
	me.configureDialog(me);
	return me;
}
void fillNexuizLimboDialogBlueTab(entity me)
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
		me.TD(me, 1, me.columns/2, makeNexuizCommandButton("Deploy", '1 0.5 0.5', "cmd selectclass $player_class\ncmd selectteam blue\ncmd join", 1));
		me.TD(me, 1, me.columns/2, makeNexuizCommandButton("Respawn", '1 0.5 0.5', "kill\n", 1));
}

#endif
