#include <std.h>
#include "../station.h"

inherit ROOM;

void create() {
    ::create();
    set_short("Station Arm");
    set_long("You are in a Station arm.\nThe walkway extends west and east.");
    set_exits(([
        "north": STATION_ROOMS"ne_inner",
        "east": STATION_ROOMS"e_arm_2",
        "south": STATION_ROOMS"se_inner",
        "southwest": STATION_ROOMS"s_arm_1",
        "west": STATION_ROOMS"center",
        "northwest": STATION_ROOMS"n_arm_1",
    ]));
}