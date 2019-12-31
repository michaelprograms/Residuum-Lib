//      /bin/user/_stats.c
//      from the Nightmare mudlib
//      lists a player's stats
//      created by Descartes of Borg 25 april 1993

#include <std.h>

inherit CMD_UTILITY;

int cmd_stats(string input) {
    string *stats = ({ "strength", "perception", "endurance", "charisma", "intelligence", "agility", "luck" });

    if(input) return 0; // TODO add admin targeting
    message("info", format_header_bar("STATS"), this_player());

    foreach(string stat in stats) {
        message("status", sprintf("%s: %d     ", arrange_string(stat, 13), (int)this_player()->query_base_stats(stat)), this_player());
    }
    message("info", format_footer_bar(), this_player());
    return 1;
}

void help() {
    message("help",
      "Syntax: "+format_syntax("<stats"+(creatorp(this_player())?" ([target])":"")+">")+"\n\n"
      "Gives you a numerical representation of your genetically inherited "
      "physical traits.\n\nSee also: score, skills, status", this_player()
    );
}
