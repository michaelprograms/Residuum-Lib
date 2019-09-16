#include <std.h>
#include "../station.h"

inherit ROOM;

void create() {
    ::create();
    set_short("Station Arm");
    set_long("You are in a Station arm.\nThe walkway extends west and east.");
    set_exits(([
        "north": STATION_ROOMS"nw_inner",
        "northeast": STATION_ROOMS"n_arm_1",
        "east": STATION_ROOMS"center",
        "southeast": STATION_ROOMS"s_arm_1",
        "south": STATION_ROOMS"sw_inner",
        "west": STATION_ROOMS"w_arm_2",
    ]));
}