#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    room::create();
    set_short("the void");
    set_long("An expansive yet constrained void. Nothingness is everything. Far below, a sensation of somethingness pulls on the consciousness.");
    set_exits(([
        "down": ROOM_START
    ]));
}

void init() {
    ::init();
    add_action("cmd_update", "update");
    add_action("cmd_ed", "ed");
}

int cmd_ed(string str) {
    if(str != "here") return 0;
    write("Edit the void?");
    return 1;
}

int cmd_update(string str) {
    if(str && str != "") return 0;
    write("Update the void?");
    return 1;
}