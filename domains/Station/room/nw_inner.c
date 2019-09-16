#include <std.h>
#include "../station.h"

inherit ROOM;

void create() {
    ::create();
    set_short("Station Inner Ring");
    set_long("You are in the inner Station ring.\nThe ring continues east and south.");
    set_exits(([
        "east": STATION_ROOMS"n_arm_1",
        "south": STATION_ROOMS"w_arm_1",
    ]));
}