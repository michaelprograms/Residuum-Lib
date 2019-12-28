#include <std.h>

inherit OBJECT;

void create() {
    ::create();
    set_id(({"fountain", "large fountain"}));
    set_name("trickling fountain");
    set_short("a large fountain");
    set_long("A large fountain basin which shoots small arcs of water from one end to the other. It is filled with large koi floating below the surface.");
    set_value(0);
    set_prevent_get(1);
}