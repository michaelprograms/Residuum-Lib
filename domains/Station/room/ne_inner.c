#include <std.h>
#include "../station.h"

inherit ROOM;

void create() {
    ::create();
    set_short("Station Inner Ring");
    set_long("You are in the inner Station ring.\nThe ring continues south and west.");
    set_exits(([
        "south": STATION_ROOMS"e_arm_1",
        "west": STATION_ROOMS"n_arm_1",
    ]));
}