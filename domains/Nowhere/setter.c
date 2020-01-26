#include <std.h>
#include <rooms.h>

#define BECOME_SYNTAX "<become [female|male|neuter]>"

inherit ROOM;

string get_long() {
    string msg = "\nYou are naught...nowhere...nobody...nothing...but a feeling of an expectation to guide destiny compels you...\n\n";

    if(this_player()) {
        msg += sprintf("%-20s %s", "You are %^ORANGE%^"+this_player()->query_gender()+"%^RESET%^.", format_syntax(BECOME_SYNTAX))+"\n";

        msg += "\n...or roll the dice on your fate... " + format_syntax("<randomize>") + ".";
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
    add_action("cmd_randomize", "randomize");
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
int cmd_randomize(string str) {

    int x;
    write("About to error.");
    x = 1 / to_int(str);
    return 1;
}