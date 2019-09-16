#include <std.h>
#include "../station.h"

inherit ROOM;

void create() {
    ::create();
    set_short("Station Arm");
    set_long("You are in a Station arm.\nThe walkway extends north and south.");
    set_exits(([
        "north": STATION_ROOMS"n_arm_2",
        "east": STATION_ROOMS"ne_inner",
        "southeast": STATION_ROOMS"e_arm_1",
        "south": STATION_ROOMS"center",
        "southwest": STATION_ROOMS"w_arm_1",
        "west": STATION_ROOMS"nw_inner",
    ]));
}