/*    /adm/SimulEfun/strings.c
 *    from Nightmare IV
 *    SimulEfuns for string manipulation
 *    created by Descartes of Borg 940506
 */

#include <daemons.h>

varargs string center(string str, int x) {
    int y;

    if(!x) x= 80;
    if((y = strlen(strip_colours(str))) >= x) return str;
    x = x + strlen(str) - y;
    return sprintf(sprintf("%%\|%ds", x), str);
}

varargs string arrange_string(string str, int x) {
    int y;

    if(!x) x = 80;
    x += strlen(str) - strlen(strip_colours(str));
    return sprintf(sprintf("%%:-%ds", x), str);
}

varargs string wrap(string str, int x) {
    if(!str || !stringp(str)) return "";
    if(strlen(str) > 8190) str=str[0..8190];
    return terminal_colour(str, TERMINAL_D->query_term_info(previous_object()->query_option("TERM")), (x?x:0));
}
// varargs string wrap2(string str, int x) {
//     return sprintf(sprintf("%%-=%ds\n", (x ? x : 76)), str);
// }

string replace_strings(string *words, mapping info) {
    int i=sizeof(words);
    if(mapp(info)) while(i--) if(info[words[i]]) words[i]=info[words[i]];
    return implode(words,"");
}

// -------------------------------------------------------------------------

varargs string pad(int length, string c) {
    string s = "";
    if(!c || c == "") c = " ";
    for(int i = 0; i < length; i ++) {
        s += c;
    }
    return s;
}
varargs string leftpad(string text, int n, string pad) {
    if (n < 0 || n > 80) return text;
    // if(!pad || pad == "") pad = " ";
    while(sizeof(text) < n) text = pad + text;
    return text;
}
varargs string rightpad(string text, int n, string pad) {
    if (n < 0 || n > 80) return text;
    // if(!pad || pad == "") pad = " ";
    while(sizeof(text) < n) text = text + pad;
    return text;
}

// -------------------------------------------------------------------------

varargs string format_header_bar(string title, string optional) {
    string bar = "%^RESET%^CYAN%^==";
    if(title){
        bar += "%^RESET%^/ %^RESET%^BOLD%^" + title;
        if(optional) bar += ":%^RESET%^ "+optional;
        else bar += "%^RESET%^";
        bar += " \\%^CYAN%^=";
    }
    for(int i = strlen(strip_colours(bar)); i < 80; i ++) bar += "=";
    bar += "%^RESET%^";
    return bar;
}
string format_divider_bar() {
    return "%^RESET%^BLUE%^--------------------------------------------------------------------------------%^RESET%^";
}
string format_footer_bar() {
    return "%^RESET%^CYAN%^================================================================================%^RESET%^";
}

// -------------------------------------------------------------------------

string time_ago_full(int timestamp) {
    int w, d, h, m;
    int secs = time() - timestamp;
    debug_print("diavolo", time()+" - "+timestamp+" == "+secs);
    debug_print("diavolo", "time_ago_full secs = "+secs);
    w = secs / 604800;
    secs = secs - (w * 604800);
    debug_print("diavolo", "time_ago_full secs = "+secs);
    d = secs / 86400;
    secs = secs - (d * 86400);
    debug_print("diavolo", "time_ago_full secs = "+secs);
    h = secs / 3600;
    secs = secs - (h * 3600);
    debug_print("diavolo", "time_ago_full secs = "+secs);
    m = secs / 60;
    secs = secs - (m * 60);
    debug_print("diavolo", "time_ago_full secs = "+secs);
    return (w ? w + "w " : "") + (d ? d + "d " : "") + (h ? h + "h " : "")+(m ? m + "m " : "") + secs + "s";
}