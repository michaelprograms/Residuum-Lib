#include <std.h>

inherit DAEMON;

int myfun(mixed a, mixed b) {
    if(a[2]==b[2]) return 0;
    if (a[2]>b[2]) return 1;
    return -1;
}

mixed cmd_callouts(string filter) {
    mixed *coi;
    string tmp;
    int i, maxi;

    if(filter && filter != "") {
        coi = ({});
        foreach(mixed m in call_out_info()) {
            if(strsrch(identify(m[0]), filter) > -1) coi += ({m});
        }
    } else coi = call_out_info();

    coi = sort_array(coi, "myfun");
    if(!sizeof(coi)) {
        message("system", "No pending callouts.", this_player());
        return 1;
    }
    tmp = sprintf("%:-40s %:-25s %:-6s\n", "Object", "Function", "Delay");

    tmp += "---------------------------------------------------------------------------\n";
    for(i=0, maxi = sizeof(coi); i<maxi; i++) {
        if( sizeof(coi[i]) != 3 ) {
            tmp += sprintf("Error in element %O\n", coi[i]);
            continue;
        }
        tmp += sprintf("%:-40s %:-25s %:-5s \n",
                       identify(coi[i][0]), identify(coi[i][1]),
                       identify(coi[i][2]));
    }
    this_player()->more(explode(tmp, "\n"), "system");
    return 1;
}

void help() {
    message("help", "Syntax: <callouts (filter)>\n\n"
            "Lists all pending callouts. Optional filter can be used display specific information.\n\n"
            "See also: events, mstatus, netstat", this_player());
}
