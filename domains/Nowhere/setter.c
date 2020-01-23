#include <std.h>
#include <rooms.h>

#define BECOME_SYNTAX "<become [female|male|neuter]>"

inherit ROOM;

string get_long() {
    string msg = "\nYou are naught...nowhere...nobody...nothing...\n\n"+
        "...you feel an expectation to guide your destiny...\n\n";

    if(this_player()) {
        msg += "You are "+this_player()->query_gender()+".\n";

        msg += "\n"+format_syntax(BECOME_SYNTAX)+"\n";
    }

    return msg;
}

void create() {
    ::create();
    set_property("indoors", 1);
    set_property("light", 1);
    set_short("nowhere");
    set_long((: get_long :));
    skip_obvious();
}

void init() {
    ::init();
    add_action("cmd_become", "become");
}

int cmd_become(string input) {
    object tp = this_player();

    if(member_array(input, ({"female", "male", "neuter"})) > -1) {
        if(input == tp->query_gender()) return notify_fail("You are already "+input+".\n");
        else {
            tp->set_gender(input);
            write("You are now "+input+".");
            return 1;
        }
    } else {
        write("Become what? "+format_syntax(BECOME_SYNTAX));
    }
    return 1;
}