//	/adm/daemon/finger_d.c
//	from the Nightmare Mudlib
//	by Kalinash 18 Feb 94
//      Based on the old finger daemon by Descartes.

#include <objects.h>
#include <dirs.h>
#include <daemons.h>
#include <config.h>

inherit OB_USER;

string query_hah(object ob, int verbose);
string query_ttl(string name);

void create() {
    ::create();
}

string query_player_login_brief(string who) {
    string nom, line = "";
    object ob;

    if(!who) return "";
    nom = lower_case(who);
    if(!player_exists(nom)) return "Player does not exist.";

    who = sprintf("%s/%s/%s", DIR_PLAYERS, nom[0..0], nom);
    unguarded((: restore_object, who :));

    // 3 + MAX_PLAYER_CAP_NAME_LENGTH
    line = format_syntax(query_cap_name(), 1) + pad(MAX_PLAYER_CAP_NAME_LENGTH - sizeof(query_cap_name())) + " ";
    // 4 + 1
    line += leftpad(""+query_level(), 4, " ") + " ";
    // 10 + 1
    line += rightpad(capitalize(query_race()), 10, " ") + " ";
    // 16 + 1
    line += rightpad(capitalize(""+query_class()), 16, " ") + " ";

    // if(query_position() != "player") line += " ("+query_position()+")";
    if(ob = find_player(nom)) {
        if(interactive(ob)) line += "connected";
        else line += "linkdead";
    } else line += time_ago(query_last_on());

    return line;
}

string user_finger_display(string who) {
    object ob;
    mapping mail_stat;
    mixed birth;
    string drow, name, level, tmp, where;
    string race, Class, guild, gender;

    if(!who) return "You must specify whom you wish to finger.";
    name = lower_case(who);
    if(!player_exists(name)) return "No such member of "+mud_name()+".";
    who = sprintf("%s/%s/%s", DIR_PLAYERS, name[0..0], name);
    unguarded((: restore_object, who :));
    mail_stat = (mapping)FOLDERS_D->mail_status(name);
    drow = sprintf("%s\n", query_ttl(name));
    if(!race=query_race())
        race = "unborn";
    if(!Class=query_class())
        Class = "child";
    guild = query_guild();
    if(!gender=query_gender())
        gender = "neuter";
    if(query_description()) {
        tmp = sprintf("%s%s %s%%^RESET%%^\n", drow, capitalize(name), query_description());
        drow = tmp;
    }
    if(query_guild())
    tmp = sprintf("%s %s %s %s",capitalize(gender),
      capitalize(race), capitalize(Class), guild
      == "assassin" ? "" : "of the " + capitalize(pluralize(guild)));
    else
        tmp = sprintf("%s %s %s",capitalize(gender),
          capitalize(race), capitalize(Class));
    tmp = arrange_string(tmp, 40);
        level = capitalize(query_position());
    tmp = sprintf("%s%sLevel: %s (%d)\n", drow, tmp, level, query_level());
    drow = tmp;
    tmp = sprintf("In real life: %s", query_rname());
    tmp = arrange_string(tmp, 40);
    tmp = sprintf("%sMarried to : %s\n", tmp, !query_married() ? "No one" :
          capitalize(query_married()));
    drow = sprintf("%s%s", drow, tmp);
    birth = query_birthday();
    tmp = sprintf("%sBirthday: %s %d, %d BN\n", drow, capitalize(month(birth)),
      date(birth), (-(year(birth)-18)));
    drow = tmp;
    if(this_player() && archp(this_player())) {
        tmp = sprintf("%sAccount : %s\n", drow, query_account_name());
        drow = tmp;
        if(query_email())
            tmp = sprintf("%sEmail : %s\n", drow, query_email());
        else
            tmp = sprintf("%sNo email address.\n", drow);
    }
    drow = tmp;
    if(!(ob = find_player(name)) || (int)ob->query_invis() ||
      hiddenp(ob)) {
        tmp = sprintf("%sLast on: ", drow);
        drow = tmp;
    }
    else {
        tmp = query_hah(find_player(name), 1);
        if(tmp != "") {
            tmp = sprintf("%s%s\n", drow, tmp);
            drow = tmp;
        }
        tmp = sprintf("%sOn since : ", drow);
        drow = tmp;
    }
    if(query_where_block() && !archp(this_player()))
        where = "Anonymous";
    else
        where = query_ip();
    tmp = sprintf("%s%s from %s.\n", drow, ctime(query_last_on()), where);
    drow = tmp;
    if(!mail_stat["unread"])
        tmp = sprintf("%sNo unread mail.\n", drow);
    else if(mail_stat["unread"] == 1)
        tmp = sprintf("%s1 unread piece of mail.\n", drow);
    else
        tmp = sprintf("%s%d unread pieces of mail.\n", drow, mail_stat["unread"]);
    drow = tmp;
    who = user_path(name);
    if(unguarded((: file_exists, who+".project" :)))
      tmp = sprintf("%sProject: %s", drow,
        unguarded((: read_file, who+".project" :)));
    drow = tmp;
    if(unguarded((: file_exists, who+".plan" :)))
      tmp = drow +"Plan:\n"+unguarded((: read_file, who+".plan" :));
    else
        tmp = sprintf("%sNo Plan.\n", drow);
    drow = tmp;
    return drow;
}

string query_hah(object ob, int verbose) {
    int time, tmp;
    string line;

    if(!interactive(ob)) {
	if(verbose) return "Disconnected.";
	else return "disconnected";
    }
    line = "";
    time = query_idle(ob);
    if(time < 60) return line;
    tmp = time / 86400;
    if(tmp) {
	if(!verbose) return tmp + " d";
	else line += tmp + " day" + (tmp>1 ? "s " : " ");
    }
    tmp = (time%86400)/3600;
    if(tmp) {
      if(!verbose) {
	if(tmp>9) return tmp+" h";
	else line += tmp+":";
      }
	else line += tmp + " hour"+(tmp>1 ? "s " : " ");
   }
     tmp = (time%3600)/60;
   if(tmp) {
 	if(!verbose) {
	    if(strlen(line)>0 && tmp < 10) line += "0";
	    line += tmp;
	}
	else line += tmp+ " minute"+(tmp>1 ? "s " : " ");
    }
    if(verbose) line += "idle time";
    else line += "idle";
    return line;
}


string query_ttl(string name) {
    string pre, post;
    if(!getenv("TITLE")) return capitalize(name);
    return replace_string(getenv("TITLE"), "$N", capitalize(name));
}

int remove() {
    destruct(this_object());
    return !(this_object());
}

int clean_up() {
    this_object()->remove();
    return 0;
}
