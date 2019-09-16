#include <std.h>
#include "../station.h"

inherit ROOM;

void create() {
    ::create();
    set_short("Station Inner Ring");
    set_long("You are in the inner Station ring.\nThe ring continues north and west.");
    set_exits(([
        "north": STATION_ROOMS"e_arm_1",
        "west": STATION_ROOMS"s_arm_1",
    ]));
}