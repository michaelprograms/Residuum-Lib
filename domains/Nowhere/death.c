#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_properties(([ "light" : 3, "no magic" : 1 ]));
    set_short("nowhere");
    set_long("This will become something more interesting.");
    add_exit("out", "/domains/Praxis/square");
}
