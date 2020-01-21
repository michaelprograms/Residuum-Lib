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
varargs string wrap2(string str, int x) {
    return sprintf(sprintf("%%-=%ds\n", (x ? x : 76)), str);
}

string replace_strings(string *words, mapping info)
{
    int i=sizeof(words);
    if(mapp(info)) while(i--) if(info[words[i]]) words[i]=info[words[i]];
    return implode(words,"");
}