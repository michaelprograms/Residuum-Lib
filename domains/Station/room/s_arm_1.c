#include <std.h>
#include "../station.h"

inherit ROOM;

void create() {
    ::create();
    set_short("Station Arm");
    set_long("You are in a Station arm.\nThe walkway extends north and south.");
    set_exits(([
        "north": STATION_ROOMS"center",
        "northeast": STATION_ROOMS"e_arm_1",
        "east": STATION_ROOMS"se_inner",
        "south": STATION_ROOMS"s_arm_2",
        "west": STATION_ROOMS"sw_inner",
        "northwest": STATION_ROOMS"w_arm_1",
    ]));
}