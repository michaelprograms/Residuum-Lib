#include <std.h>

#define SYNTAX_GENDER "<become [female|male|neuter]>"

inherit ROOM_INSTANCE;

string get_long() {
    string msg = "\nThe warm embrace of conditioned air in a closely confined tank around your body.\n";

    if(this_player()) {
        msg += "\nA diagnostic display is projected onto the glass.\n\n";
        msg += "You are "+this_player()->query_gender()+". "+format_syntax(SYNTAX_GENDER)+"\n";
        msg += "You have "+this_player()->query_complexion()+" skin complexion. \n";
        msg += "You have "+this_player()->query_eye()+" eye color. \n";
        msg += "Your primary attribute is "+this_player()->query_primary_attribute()+". \n";
        msg += "Your secondary attribute is "+this_player()->query_secondary_attribute()+". \n";
    };

    return msg;
}

void create() {
    ::create();
    set_property("indoors", 1);
    set_property("light", 1);
    set_short("nowhere");
    set_long((: get_long :));
    skip_obvious();
    add_exit("out", "/domains/Nowhere/rooms/start_chamber.c"); // debug
}

void init() {
    ::init();
    // add_action("cmd_become", "become");
    // add_action("cmd_randomize", "randomize");
}

// int cmd_become(string input) {
//     object tp = this_player();

//     if(member_array(input, ({"female", "male", "neuter"})) > -1) {
//         if(input == tp->query_gender()) return notify_fail("You are already "+input+".\n");
//         else {
//             tp->set_gender(input);
//             write("You are now "+input+".");
//             return 1;
//         }
//     } else {
//         write("Become what? "+format_syntax(BECOME_SYNTAX));
//     }
//     return 1;
// }