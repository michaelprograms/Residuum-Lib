#include <std.h>
#include <move.h>
#include <rooms.h>

inherit DAEMON;

int cmd_hall(string str) {
    object tp = this_player(), room, prev;
    string dest;

    if(!creatorp(tp)) return 0;

    dest = ROOM_ADMIN;
    prev = environment(tp);

    if(!(room = find_object(dest))) room = load_object(dest);

    if(prev == room) return notify_fail("You twitch.\n");
    else if(tp->move(room) == MOVE_OK) {
        tp->describe_current_room(1);
        if(!hiddenp(tp)) {
            foreach(object ob in prev->query_living_contents()) {
                if(ob == this_player() || (tp->query_invis() && !creatorp(ob))) continue;
                message("mmout", (tp->query_invis() && creatorp(ob) ? "(invis) " : "")+strip_colours(tp->query_mhome(to_int(creatorp(ob)))), ob);
            }
            foreach(object ob in room->query_living_contents()) {
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