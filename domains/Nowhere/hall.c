#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    room::create();
    set_short("nowhere");
    set_long("Nothing that makes up nowhere. The absense of everything can't be seen. The darkness is blinding. Shadows provide a feeling of unfelt pressure.");
    set_smell("default", "The air feels empty.");
    set_listen("default", "The silence is deafening.");
    set_exits(([
        "down": ROOM_START
    ]));
}