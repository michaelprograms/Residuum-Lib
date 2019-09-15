#include <std.h>

inherit DAEMON;

object *convert_to_objects(string *list) {
    object *result = ({});
    int x = sizeof(list);
    object ob;

    for (int i = 0; i < x; i++) {
        ob = find_object(list[i]);
        if (!ob) {
            call_other(list[i], "???");
            ob = find_object(list[i]);
        }
        if (!ob) error("Could not find object: " + list[i]);
        result += ({ob});
    }

    return result;
}

int cmd_dupdate(string ob_name) {
    string *inherits;
    string res, filename, name;
    object ob;
    object *result;
    int x;

    name = absolute_path(this_player()->get_path(), ob_name);
    ob = find_object(name);

    if (!ob) {
        // object might not be loaded, attempt to load it
        call_other(name, "???");
        ob = find_object(name);
    }
    if (!ob) return notify_fail("That is not a valid object.\n");

    message("info", "doing deep update on "+identify(ob), this_player());

    inherits = deep_inherit_list(ob);
    result = ({ob}) + convert_to_objects(inherits);

    x = sizeof(result) - 2; // 2 are taken off because /std/Object.c and /std/clean_up.c should never be updated through this command

    // Macros
    // while(x--) {
    while(x-- > 0) {
        message("info", "updating "+identify(result[x]), this_player());
        filename = base_name(result[x]);
        destruct(result[x]);
        res = catch(call_other(filename, "???"));
        if (res) {
            error("Could not update "+filename+": "+res);
        }
    }

    message("info", "Done.", this_player());

    return 1;
}

void help() {
    message("help",
      "Syntax: <dupdate [file]>\n\n"
      "Attempts to update all ancestors of the specified object, and then itself.\n"
      "See also: update", this_player()
    );
}