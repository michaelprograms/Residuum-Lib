/*    /std/room/exits.c
 *    from Nightmare IV
 *    handles players exiting from rooms
 *    created by Descartes of Borg 940711
 */

#include <std.h>
#include <rooms.h>
#include "exits.h"

#define MAX_TRACKS           10
#define MAX_SCENTS           10
#define TRACK_FADE_TIME      120
#define NUMBER ({"two","three","four","five","six","seven","eight","nine","ten","many",})
#define TMPLONG ({"north","northeast","east","southeast","south","southwest","west","northwest","up","down","in","out","enter","exit",})
#define TMPSHORT ({"n","ne","e","se","s","sw","w","nw","u","d","in","out","enter","exit",})

static private mapping __Exits, __Enters;
static private mapping __Tracks, __TrackFaded, __Scents, __ScentFaded;

static void create() {
    __Exits = ([]);
    __Enters = ([]);
    __Tracks = ([]);
    __TrackFaded = ([]);
    __Scents = ([]);
    __ScentFaded = ([]);
}

void init() {
    add_action("cmd_go", "go");
    add_action("cmd_enter", "enter");
    foreach(string dir in keys(__Exits)) add_action("cmd_use_exit", dir);
}

int skip_obvious=0;
int skip_obvious() { skip_obvious=1; }
int query_skip_obvious() { return skip_obvious; }

int cmd_go(string str) {
    if((int)this_player()->query_paralyzed()) return notify_fail("You are unable to move.\n");
    if(!__Exits[str]) return notify_fail("You go nowhere at all.\n");
    if(__Exits[str]["pre"] && !((int)(*__Exits[str]["pre"])(str))) return 1;
    add_track(this_player(), str);
    add_scent(this_player(), str);
    this_player()->move_player(__Exits[str]["room"], str);
    if(__Exits[str]["post"]) (*__Exits[str]["post"])(str);
    return 1;
}

int cmd_use_exit() {
    string verb = query_verb();
    if(!verb) return 0;
    return cmd_go(verb);
}

int cmd_enter(string str) {
    if((int)this_player()->query_paralyzed()) return notify_fail("You are unable to move.\n");
    if(!__Enters[str]) return notify_fail("You cannot enter that.\n");
    if(__Enters[str]["pre"] && !((int)(*__Enters[str]["pre"])(str))) return 1;
    add_track(this_player(), str);
    add_scent(this_player(), str);
    this_player()->move_player(__Enters[str]["room"], str);
    if(__Enters[str]["post"]) (*__Enters[str]["post"])( str);
    return 1;
}

void reinitiate() {
    object *obs;

    if(!sizeof(obs = all_inventory(this_object()))) return;
    obs->move(ROOM_VOID);
    obs->move(this_object());
}

void set_exits(mapping mp) {
    mixed it;
    string *cles;
    int i, j;

    __Exits = ([]);
    i = sizeof(cles = keys(mp));
    while(i--) {
        it = mp[cles[i]];
        if(stringp(it))
            __Exits[cles[i]] = ([ "room" : it ]);
        else if( pointerp(it) && (j = sizeof(it)) ) {
            if(j && stringp(it[0]) ) {
                __Exits[cles[i]] = ([ "room" : it[0] ]);
            if(j > 1 && functionp(it[1])) {
                __Exits[cles[i]]["pre"]=it[1];
                if(j > 2 && functionp(it[2]))
                    __Exits[cles[i]]["post"]=it[2];
            }
        }
    }
}
}

varargs void add_exit(string dir, string dest, function pre, function post) {
    if(!stringp(dir)) error("Bad argument 1 to add_exit().\n");
    if(!stringp(dest)) error("Bad argument 2 to add_exit().\n");
    __Exits[dir] = ([ "room" : dest ]);
    if(pre) __Exits[dir]["pre"] = pre;
    if(post) __Exits[dir]["post"] = post;
    reinitiate();
}

void remove_exit(string dir) {
    if(__Exits[dir]) map_delete(__Exits, dir);
    reinitiate();
}

string query_exit(string str) {
    if(!__Exits[str]) return 0;
    else return __Exits[str]["room"];
}

string *query_exits() { return keys(__Exits); }

void set_enters(mapping mp) {
    string *cles;
    int i;

    __Enters = ([]);
    i = sizeof(cles = keys(mp));
    while(i--) __Enters[cles[i]] = ([ "room" : mp[cles[i]] ]);
}

varargs void add_enter(string dir, string dest, function pre, function post) {
    if(!stringp(dir)) error("Bad argument 1 to add_enter().\n");
    if(!stringp(dest)) error("Bad argument 2 to add_enter().\n");
    __Enters[dir] = ([ "room" : dest ]);
    if(pre) __Enters[dir]["pre"] = pre;
    if(post) __Enters[dir]["post"] = post;
    reinitiate();
}

void remove_enter(string dir) {
    if(__Enters[dir]) map_delete(__Enters, dir);
    reinitiate();
}

string query_enter(string dir) {
    if(!__Enters[dir]) return 0;
    else return __Enters[dir]["room"];
}

string *query_enters() { return keys(__Enters); }

string query_direction(string dest) {
    string *cles;
    int i;

    i = sizeof(cles = keys(__Exits));
    while(i--) if(__Exits[cles[i]]["room"] == dest) return "go "+cles[i];
    i = sizeof(cles = keys(__Enters));
    while(i--) if(__Enters[cles[i]]["room"] == dest) return "enter "+cles[i];
    return 0;
}

void add_track(object ob, string str) {
    string res;

    if(sizeof(__Tracks) > MAX_TRACKS) {
        __Tracks = ([]);
        __TrackFaded = ([]);
    }
    if(!(res = (string)ob->query_race())) return;
    if(!__TrackFaded[res]) __TrackFaded[res] = time();
    if(!__Tracks[res]) __Tracks[res] = 0;
    __Tracks[res]++;
}

int query_track(string res) {
    if(!__Tracks[res]) return 0;
    else __Tracks[res] -= (time() - __TrackFaded[res])/TRACK_FADE_TIME;
    __TrackFaded[res] = time();
    if(__Tracks[res] < 1) {
        map_delete(__Tracks, res);
        map_delete(__TrackFaded, res);
    }
    return __Tracks[res];
}

mapping query_tracks() {
    mapping tmp;
    string *cles;
    int i;

    tmp = allocate_mapping(i = sizeof(cles = keys(__Tracks)));
    while(i--) tmp[cles[i]] = query_track(cles[i]);
    return tmp;
}

void add_scent(object ob, string str) {
    string nom;

    if(sizeof(__Scents) > MAX_SCENTS) {
        __Scents = ([]);
        __ScentFaded = ([]);
    }
    if(!(nom = (string)ob->query_name())) return;
    if(!__ScentFaded[nom]) __ScentFaded[nom] = time();
    if(!__Scents[nom]) __Scents[nom] = 0;
    __Scents[nom] += (int)ob->query_scent();
}

int query_scent(string str) {
    if(!__Scents[str]) return 0;
    else __Scents[str] -= (time() - __ScentFaded[str]);
    __ScentFaded[str] = time();
    if(__Scents[str] < 1) map_delete(__Scents, str);
    return __Scents[str];
}

mapping query_scents() {
    mapping tmp;
    string *cles;
    int i;

    tmp = allocate_mapping( i = sizeof( cles = keys(__Scents)));
    while(i--) tmp[cles[i]] = query_scent(cles[i]);
    return tmp;
}

string *query_obvious_exits() {
    string *exits,*sorties;
    int i;

    if(!sizeof(__Exits)) return ({});
    else i = sizeof(sorties = keys(__Exits));
    exits = ({});
    while(i--)
        /*if(!invis_exit(sorties[i]))*/ exits+=({sorties[i]});
    return (exits);
}
string query_short_exits() {
    int i,x,max;
    string s;
    string *lx,*sx;

    if(skip_obvious) return "";
    s="";
    sx=allocate(max=sizeof(lx=query_obvious_exits()));
    for(x=0;x<max;x++) {
        if((i=member_array(lx[x],TMPLONG))!=-1)
            sx[x]=TMPSHORT[i];
        else sx[x]=lx[x];
    }
    if(!sizeof(sx)) return "no exits";
    if(max==1) return "[%^GREEN%^1%^RESET%^: %^GREEN%^"+sx[0]+"%^RESET%^]";
    s=("%^GREEN%^"+max+"%^RESET%^: ");
    for(x=0;x<max;x++) {
        if(x) s+=", ";
        s+="%^GREEN%^"+sx[x]+"%^RESET%^";
    }
    return s;
}
string query_long_exits() {
    int i,max;
    string str;
    string *sorties;

    if(skip_obvious) return "";
    if(!(i=sizeof(sorties=query_obvious_exits()))) return "There are no obvious exits.";
    if(i==1) return "The only obvious exit is "+sorties[0]+".";
    if((max=i) > sizeof(NUMBER) + 1) max=sizeof(NUMBER)+1;
    str=("There are "+NUMBER[max-2]+" obvious exits:  ");
    for(max=i,i=0;i<max;i++) {
        if(i==max-1) str+="and ";
        str+=sorties[i];
        if(i==max-1) str+=".";
        else if(max>2) str+=", ";
        else str+=" ";
    }
    return str;
}

// -----------------------------------------------------------------------------

int is_visible_exit(string dir) { return __Exits[dir] && !__Exits[dir]["invis"]; }
int is_blocked_exit(string dir) {
    return (__Exits[dir] && __Exits[dir]["no peer"]) || (this_object()->query_door(dir) && !this_object()->query_open(this_object()->query_door(dir))) || (__Exits[dir] && !to_object(replace_string(__Exits[dir]["room"], ".c", "")));
}
string format_square(string dir) {
    return is_blocked_exit(dir) ? "%^BLACK%^BOLD%^[ ]%^RESET%^" : "[ ]";
}
string format_vertical(string text, string dir) {
    return is_blocked_exit(dir) ? "%^BLACK%^BOLD%^"+(dir == "up" ? "+" : (dir == "down" ? "-" : " "))+"%^RESET%^" : text;
}
string *get_exit_map() {
    return ({
        (is_visible_exit("northwest")?format_square("northwest")+" ":"    ")+
        (is_visible_exit("north")?format_square("north"):"   ")+
        (is_visible_exit("northeast")?" "+format_square("northeast"):"    "),

        (is_visible_exit("west")?format_square("west")+" ":"    ")+
        (is_visible_exit("down")?format_vertical("-","down"):" ")+
        ("%^CYAN%^BOLD%^o%^RESET%^")+
        (is_visible_exit("up")?format_vertical("+","up"):" ")+
        (is_visible_exit("east")?" "+format_square("east"):"    "),

        (is_visible_exit("southwest")?format_square("southwest")+" ":"    ")+
        (is_visible_exit("south")?format_square("south"):"   ")+
        (is_visible_exit("southeast")?" "+format_square("southeast"):"    "),
    });
}

string format_room_properties() {
    mapping props = this_object()->query_properties();
    string *names = keys(props), *display = ({});

    if(member_array("indoors", names) != -1 && props["indoors"]) display += ({"indoors"});
    else display += ({"outdoors"});

    if(member_array("light", names) != -1) {
        if(props["light"] >= 9) display += ({ "blinding" });
        else if(props["light"] >= 6) display += ({ "bright" });
        else if(props["light"] >= 4) display += ({ "lit" });
        else if(props["light"] >= 1) display += ({ "dim" });
        else if(props["light"] == 0) display += ({ "unlit" });
        else if(props["light"] >= -4) display += ({ "shaded" });
        else if(props["light"] >= -6) display += ({ "dark" });
        else if(props["light"] >= -9) display += ({ "unseen" });
    }

    return implode(display, ", ");
}
string format_room_map_details() {
    string *exits = get_exit_map();
    string properties;
    string ret = "";

    ret += "  " + exits[0] + "  %^ORANGE%^|%^RESET%^  " + this_object()->query_short() + "\n";
    ret += "  " + exits[1] + "  %^ORANGE%^|%^RESET%^  " + format_room_properties() + "\n";
    ret += "  " + exits[2] + "  %^ORANGE%^|%^RESET%^  " + this_object()->query_short_exits();

    return ret;
}