inherit DAEMON;

#define LR "\n"

// --- Prototypes --------------------------------------------------------------

void forge_goto(string areaName, object tp);
void forge_list(string areaDirName, object tp);
void forge_map(string areaName, object tp);
void forge_new(string forgeName, object tp);
void forge_help(object tp);

int is_valid_area_name(string areaName);
string convert_to_filepath(string arenaName);
varargs string format_forge_path(string playerName, string areaName);
void write_header_file(string dirPath, string areaDirName, string areaName);
void write_room_file(string roomPath);

// -----------------------------------------------------------------------------

int cmd_forge(string input) {
    object tp = this_player();
    string *inputs, cmd, arg;

    inputs = explode("" + input, " ");
    cmd = inputs[0];
    arg = sizeof(inputs) > 1 ? implode(inputs[1..sizeof(inputs)-1], " ") : "";

    message("system", format_header_bar("FORGE", cmd) + LR, tp);
    switch(cmd) {
        case "goto": forge_goto(arg, tp); break;
        case "list": forge_list(arg, tp); break;
        case "map": forge_map(arg, tp); break;
        case "new": forge_new(arg, tp); break;
        case "push": /*forge_push(arg, tp);*/ break;
        case "tag": /*forge_tag(arg, tp);*/ break;
        default: forge_help(tp); break;
    }
    message("system", LR + format_header_bar(), tp);
    return 1;
}

// -----------------------------------------------------------------------------

void forge_goto(string areaName, object tp) {
    string dirPath = format_forge_path(tp->query_name(), areaName), roomPath;

    if(file_size(dirPath) != -2) {
        message("system", "Forge directory not found: " + dirPath, tp);
        return;
    }

    // @TODO: check primary tagged room
    // else try 0_0_0

    roomPath = dirPath + "room/0_0_0.c";

    tp->move_player(roomPath);
}

void forge_list(string forgeName, object tp) {
    mixed *dirContents;
    string dirPath;

    if(!forgeName || forgeName == "" || forgeName == tp->query_name()) {
        dirPath = format_forge_path(tp->query_name());
        if(create_dir(dirPath) != 0) {
            message("system", "Forge directory created:" + LR + "    " + dirPath + LR, tp);
        } else {
            message("system", "Forge directory:" + LR + "    " + dirPath + LR, tp);
        }
    } else {
        dirPath = format_forge_path(forgeName);
        if(file_size(dirPath) != -2) {
            message("system", "Forge directory not found: " + dirPath, tp);
            return;
        }
    }
    message("system", "Forge directory contents:", tp);
    dirContents = filter_array(get_dir(dirPath, -1), (:$1[1] == -2:));
    if(sizeof(dirContents) == 0) message("system", "    No areas.", tp);
    foreach(mixed *area in dirContents) {
        message("system", "    Area: "+area[0], tp);
    }
}

void forge_map(string areaName, object tp) {

}

void forge_new(string areaName, object tp) {
    string dirPath, areaDirName, roomPath;

    if(!areaName || areaName == "" || !is_valid_area_name(areaName)) {
        message("system", "Invalid characters or formating encountered.", tp);
        return;
    } else {
        areaDirName = convert_to_filepath(areaName);
        if(!areaDirName || areaDirName == "") {
            message("system", "Forge area invalid name: " + areaDirName, tp);
            input_to("forge_new", tp);
            return;
        }

        dirPath = format_forge_path(tp->query_name(), areaDirName);
        if(file_size(dirPath) == -2) {
            message("system", "Forge area already exists: " + dirPath, tp);
            return;
        }

        message("system", "Forge area created: " + dirPath, tp);

        create_dir(dirPath);
        mkdir(dirPath + "armor");
        mkdir(dirPath + "room");
        mkdir(dirPath + "weapon");
        mkdir(dirPath + "npc");
        mkdir(dirPath + "obj");

        // create dirPath/.forge ?
        write_header_file(dirPath, areaDirName, areaName);

        roomPath = dirPath + "room/0_0_0.c";
        write_room_file(roomPath);

        tp->move_player(roomPath);
    }
}

void forge_help(object tp) {
    string msg;

    // msg = format_syntax("<forge [command] ([arguments])>") + LR;
    // message("system",  msg, tp);

    msg = format_syntax("<forge help>");
    message("system",  msg, tp);

    msg = LR + format_syntax("<forge list>") + LR + "    Displays your forge areas.";
    msg += LR + format_syntax("<forge list [name]>") + LR + "    Displays another person's forge areas.";
    message("system", msg, tp);

    msg = LR + format_syntax("<forge new [area]>") + LR + "    Setup a new area directory";
    message("system", msg, tp);
}

// -----------------------------------------------------------------------------

int is_valid_area_name(string arenaName) {
    string n = arenaName;
    int last = 0;

    // Verify first character
    if(n[0] == '\'' || n[0] == '-' || n[0] == ' ' || n[0] < 'A' || n[0] > 'Z') return 0;

    // Verifyy rest of name
    for(int i = 1; i < sizeof(n); i ++) {
        // Only one special character in a row
        if(n[i] == '\'' || n[i] == '-' || n[i] == ' ') {
            if(last == 1) return 0;
            last = 1;
        } else if(n[i] >= 'a' && n[i] <= 'z' || n[i] >= 'A' && n[i] <= 'Z') {
            last = 0;
        } else return 0;
    }
    return 1;
}

string convert_to_filepath(string arenaName) {
    string path = arenaName;

    if(!path) return 0;
    if(strsrch(path, "'") != -1) path = replace_string(path, "'", "");
    if(strsrch(path, "-") != -1) path = replace_string(path, "-", "");
    if(strsrch(path, " ") != -1) path = replace_string(path, "_", "");
    return path;
}

varargs string format_forge_path(string playerName, string areaName) {
    return "/realms/" + playerName + "/forge/" + (areaName && areaName != "" ? areaName + "/" : "");
}

void write_header_file(string dirPath, string areaDirName, string areaName) {
    string content, capName = upper_case(areaDirName);

    content = "#ifndef __" + capName + "_H__" + LR;
    content += "#define __" + capName + "_H__" + LR;
    content += LR;
    content += rightpad("#define " + capName + "_NAME ", 32) + "\"" + areaName + "\"" + LR;
    content += rightpad("#define " + capName + "_LEVEL ", 32) + "1" + LR;
    content += rightpad("#define " + capName + "_ROOT ", 32) + "\"" + dirPath + "\"" + LR;
    content += LR;
    content += rightpad("#define " + capName + "_ARMOR ", 32) + capName + "_ROOT \"armor/\"" + LR;
    content += rightpad("#define " + capName + "_NPC ", 32) + capName + "_ROOT \"npc/\"" + LR;
    content += rightpad("#define " + capName + "_OBJ ", 32) + capName + "_ROOT \"obj/\"" + LR;
    content += rightpad("#define " + capName + "_ROOM ", 32) + capName + "_ROOT \"room/\"" + LR;
    content += rightpad("#define " + capName + "_WEAPON ", 32) + capName + "_ROOT \"weapon/\"" + LR;
    content += LR;
    content += "#endif";

    write_file(dirPath + areaDirName + ".h", content, 1);
}

void write_room_file(string roomPath) {
    string content;

    content = "inherit ROOM;" + LR + LR;
    content += "void create() {" + LR;
    content += "    ::create();" + LR;
    content += "}" + LR;

    write_file(roomPath, content, 1);
}