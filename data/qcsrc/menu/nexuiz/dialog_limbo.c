#ifdef INTERFACE
CLASS(NexuizLimboDialog) EXTENDS(NexuizRootDialog)
	METHOD(NexuizLimboDialog, fill, void(entity))
	ATTRIB(NexuizLimboDialog, name, string, "LimboDialog")
	ATTRIB(NexuizLimboDialog, title, string, "Limbo Menu")
	ATTRIB(NexuizLimboDialog, color, vector, SKINCOLOR_DIALOG_TEAMSELECT)
	ATTRIB(NexuizLimboDialog, intendedWidth, float, 0.4)
	ATTRIB(NexuizLimboDialog, rows, float, 10)
	ATTRIB(NexuizLimboDialog, columns, float, 6)
ENDCLASS(NexuizLimboDialog)
#endif

#ifdef IMPLEMENTATION
void fillNexuizLimboDialog(entity me)
{
	entity mc;
	mc = makeNexuizTabController(me.rows - 2);
	me.TR(me);
		me.TD(me, 1, me.columns/2, mc.makeTabButton(mc, "Red", makeNexuizLimboDialogRedTab()));
		me.TD(me, 1, me.columns/2, mc.makeTabButton(mc, "Blue", makeNexuizLimboDialogBlueTab()));
	me.TR(me);
	me.TR(me);
		me.TD(me, me.rows - 2, me.columns, mc);
}
#endif
