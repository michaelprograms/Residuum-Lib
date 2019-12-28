#include <std.h>
#include "../station.h"

inherit ROOM;

void create() {
    ::create();
    set_short("Station's Center");
    set_long("You are in the Station's Center.\nThe various arms of the station converge here.");
    set_exits(([
        // "north": STATION_ROOMS"n_arm_1",
        "northeast": STATION_ROOMS"ne_inner",
        "east": STATION_ROOMS"e_arm_1",
        "southeast": STATION_ROOMS"se_inner",
        // "south": STATION_ROOMS"s_arm_1",
        "southwest": STATION_ROOMS"sw_inner",
        "west": STATION_ROOMS"w_arm_1",
        "northwest": STATION_ROOMS"nw_inner",
    ]));
    set_safe_room();
    new(STATION_OBJ"fountain")->move(this_object());
}