//      /d/standard/setter.c
//      Starting room for new characters choosing races
//      from the Nightmare Mudlib
//      created by Shadowwolf@Nightmare july 1992
//      modified by Descartes of Borg for the race daemon 10 june 1993

#include <std.h>
#include <rooms.h>
#include <daemons.h>

inherit ROOM;

void do_rolls();

void create() {
  ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_long(
	"You are in a small box of both infinite and no dimensions.  "
	"Reality is black and without material.  You are being created. "
	"You intuit a list of possible races you might become, and even "
	"read it. Once you pick your race, you can begin your journey "
	"through the reality of Nightmare!  Type <read list> to see "
	"the list of races.");
    set_items(
	(["room" : "The nothingness from which you will be born "
	    "into Nightmare.",
	  "list" : "A list of races that exist in the world of Nightmare."]) );
}

void init() {
    ::init();
    add_action("read", "read");
    add_action("pick","pick");
    if (this_player()->query_exp() != 0) {
        write("\nWelcome to "+mud_name()+"!\nPlease choose a race for yourself.  Your race determines your main genetic attributes, strength, perception, endurance, charisma, intelligence, agility, and luck.");
        this_player()->set_rolls(0);
    }
}
int pick(string str) {
    string *which;
    mapping borg;
    int tmp, i;
    string Class;

    Class = "child";
    if(!str) {
        write("To pick a race, type \"pick whatever\", where whatever is the race.");
        return 1;
    }
    str = lower_case(str);
    if(str == "satyr" &&
      (string)this_player()->query_gender() != "male") {
	write("You must be a male to be a satyr!\nPick again.");
	return 1;
    }
    else if(str == "nymph" && (string)this_player()->query_gender()
      != "female") {
	write("You must be a female to be a nymph!\nPick again.");
	return 1;
    }
    if(member_array(str, (string *)RACE_D->query_races()) == -1) {
	write("You must pick a race from the list!\nType <read list>\n");
	return 1;
    }
    this_player()->set_race(str);
    this_player()->new_body();
    this_player()->set_class(Class);
    if( (string)this_player()->query_gender() == "male") this_player()->setenv("TITLE", "Newbie $N the boy");
    else this_player()->setenv("TITLE", "Newbie $N the girl");
    this_player()->init_skills(Class);
    write("You can roll your stats up to three times.");
    write("You do this in the hall of the class you will soon choose.");
    do_rolls();
    write("You are now transfered to the village square.");
    this_player()->move_player(ROOM_START);
    return 1;
}

void do_rolls() {
    string *which;
    mapping borg;
    int i, tmp;

    if((int)this_player()->query_rolls() >3) {
        write("You can roll your stats no more.");
        return;
    }
    write("You roll your stats.");
    for(i=0, tmp=sizeof(which=keys(borg=(mapping)RACE_D->do_rolls((string)this_player()->query_race()))); i<tmp; i++)
        this_player()->set_stats(which[i], borg[which[i]]);
    this_player()->set_rolls(this_player()->query_rolls()+1);
    return;
}

int read(string str) {
    string *res;
    int i, j, tmp;
    if(!str) {
	notify_fail("What do you want to read? A list?\n");
	return 0;
    }
    if(str != "list") {
	notify_fail("That is not here to be read.\n");
	return 0;
    }
    write("These are the following races available in our reality:");
    write("-----------------------------------------------------------");
    message("Ninfo", format_page((string *)RACE_D->query_races(), 4),
      this_player());
    write("----------------------------------------------------------");
    write("Only males may be satyrs, and only females may be nymphs.");
    write("pick <race> will forever make you one of these races");
    write("Type <help races> for more information.");
    return 1;
}
