inherit DAEMON;

void handle_input(string input, object tp);
void input_command(string input, object tp);
void list_areas(object tp);

int cmd_forge(string input) {
    object tp = this_player();

    if(!input) {
        message("system", "Syntax: "+format_syntax("<forge list|new>"), tp);
        return 1;
    }

    handle_input(input, tp);
    return 1;
}

void handle_input(string input, object tp) {

    // update this to sscanf

    if(member_array(input, ({"quit", "q"})) > -1) {
        message("system", "Quitting.", tp);
        return;
    }

    if(member_array(input, ({"list", "ls"})) > -1) {
        message("system", "Listing:", tp);
        list_areas(tp);
        return;
    }

    message("system", "Menu:", tp);
    message("system", " - list", tp);
    message("system", " - quit", tp);
    input_to("handle_input", tp);
}

void list_areas(object tp) {
    mixed *dir;

    foreach(string domain in ({"Nowhere", "Systems"})) {
        message("system", "Domain: "+domain, tp);
        dir = filter_array(get_dir("/domains/"+domain+"/", -1), (:$1[1] == -2:));
        foreach(mixed *area in dir) message("system", "    Area: "+area[0], tp);
    }

}