varargs string format_syntax(string text, int wrap) {
    string s = text;
    if(!s) return "";
    if (wrap && !regexp(s, "^<")) s = "<" + s;
    if (wrap && !regexp(s, ">$")) s = s + ">";
    s = replace_string(s, "<", "%^CYAN%^<%^RESET%^");
    s = replace_string(s, "[", "%^CYAN%^BOLD%^[%^RESET%^");
    s = replace_string(s, "|", "%^CYAN%^BOLD%^|%^RESET%^");
    s = replace_string(s, "]", "%^CYAN%^BOLD%^]%^RESET%^");
    s = replace_string(s, "(", "%^RESET%^BOLD%^(%^RESET%^");
    s = replace_string(s, ")", "%^RESET%^BOLD%^)%^RESET%^");
    s = replace_string(s, "{", "%^RESET%^{");
    s = replace_string(s, "}", "%^RESET%^}");
    s = replace_string(s, ">", "%^CYAN%^>%^RESET%^");
    return s;
}

string format_uptime() {
    int x = uptime(), w, d, h, m;
    string uptime = "";

    w = x / 604800;
    x = x - (w * 604800);
    d = x / 86400;
    x = x - (d * 86400);
    h = x / 3600;
    x = x - (h * 3600);
    m = x / 60;
    x = x - (m * 60);

    uptime += (w ? w + "w " : "") + (d ? d + "d " : "") + (h ? h + "h " : "") +
    (m ? m + "m " : "") + (x ? x + "s" : "");

    return uptime;
}

string format_command_help(string syntax, string description) {
    return " Syntax  : " + format_syntax(syntax, 1) + "\n\n" + description + "\n";
}