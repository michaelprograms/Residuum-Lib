int cmd_lc(string str) {
    if(!str) return notify_fail("Syntax: lc <filename>\n");
    if(str[0..0] != "/") str = this_player()->get_path()+"/"+str;
    if(file_size(str) == -1) return notify_fail("File: "+str+" not found.\n");
    message("info", str+": "+sizeof(explode(read_file(str), "\n")), this_player());
    return 1;
}

void help() {
    message("info", "Syntax: lc <filename>\n\nReturns the line count of specified file.\n", this_player());
}