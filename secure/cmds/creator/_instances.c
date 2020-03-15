#include <daemons.h>

int cmd_instances() {
    mapping instances = ROOM_INSTANCE_D->query_instances();

    message("system", format_header_bar("INSTANCES")+"\n", this_player());

    foreach(string name in keys(instances)) {
        message("system", "%^BOLD%^"+name+"%^RESET%^", this_player());
        foreach(string path in keys(instances[name])) {
            message("system", time_ago(instances[name][path]["created"])+" : "+file_name(instances[name][path]["ob"]), this_player());
        }
        message("system", "", this_player());
    }
    message("system", format_footer_bar(), this_player());
    return 1;
}