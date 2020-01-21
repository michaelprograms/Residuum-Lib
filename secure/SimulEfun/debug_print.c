void debug_print(string name, string msg) {
    object ob = find_player(name);
    if(!ob || !creatorp(ob)) return;
    message("debug", msg, ob);
}