#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("indoors", 1);
    set_property("light", 1);
    set_short("nowhere");
    set_long(
        "\n%^BOLD%^You are naught...nowhere...nobody...nothing...%^RESET%^\n\n"+
        "...you feel an expectation to guide your destiny...\n\n"+
        //"The journey forward splits into discrete paths each blocking one another.\n\n"+
        // format_syntax("<choose [STR_race|PER_race|END_race|CHA_race|INT_race|AGI_race]>")+"\n"
        ""
    );
    skip_obvious();
}

void init() {
    ::init();
    //add_action("", "");
}