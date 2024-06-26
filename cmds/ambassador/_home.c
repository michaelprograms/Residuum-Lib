#include <std.h>
#include <move.h>

inherit DAEMON;

int cmd_home(string str) {
    object tp = this_player(), workroom, prev;
    string dest;

    if(!creatorp(tp)) return 0;

    dest = user_path( str ? lower_case(str) : tp->query_name() ) + "workroom";
    prev = environment(tp);

    if(!(workroom = find_object(dest))) workroom = load_object(dest);

    if(prev == workroom) return notify_fail("You twitch.\n");
    else if(tp->move(workroom) == MOVE_OK) {
        tp->describe_current_room(1);
        if(!hiddenp(tp)) {
            foreach(object ob in prev->query_living_contents()) {
                if(ob == this_player() || (tp->query_invis() && !creatorp(ob))) continue;
                message("mmout", (tp->query_invis() && creatorp(ob) ? "(invis) " : "")+strip_colours(tp->query_mhome(to_int(creatorp(ob)))), ob);
            }
            foreach(object ob in workroom->query_living_contents()) {
                if(ob == this_player() || (tp->query_invis() && !creatorp(ob))) continue;
                message("mmin", (tp->query_invis() && creatorp(ob) ? "(invis) " : "")+strip_colours(tp->query_mmin(to_int(creatorp(ob)))), ob);
            }
        }
        return 1;
    } else {
        message("system", "You remain where you are.", tp);
        return;
    }
}