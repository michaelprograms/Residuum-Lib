#include <std.h>
#include "../station.h"

inherit ROOM;

void create() {
    ::create();
    set_short("Station's Center");
    set_long("You are in the Station's Center.\nThe various arms of the station converge here.");
    set_exits(([
        "north": STATION_ROOMS"n_arm_1",
        "east": STATION_ROOMS"e_arm_1",
        "south": STATION_ROOMS"s_arm_1",
        "west": STATION_ROOMS"w_arm_1",
    ]));
    set_safe_room();
    new(STATION_OBJ"pond")->move(this_object());
}