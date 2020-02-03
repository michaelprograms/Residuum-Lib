#include <std.h>

inherit CMD_UTILITY;

int cmd_options(string input) {
    object tp = this_player(), target = tp;
    mapping options;
    string *names;

    if(creatorp(tp) && input && strsrch("input", " ") == -1 && !(target = find_player(input))) return notify_fail("Target not found.\n");

    options = target->query_options();
    names = sort_array(keys(options), 1);
    message("info", format_header_bar("OPTIONS", (tp != target ? "Viewing "+target->query_CapName() : 0)), tp);
    foreach(string key in names) {
        message("status", sprintf("%-16s: %s     ", key, options[key]), tp);
    }
    message("info", format_footer_bar(), tp);
    return 1;
}