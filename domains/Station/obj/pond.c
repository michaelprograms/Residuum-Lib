#include <std.h>

inherit OBJECT;

void create() {
    ::create();
    set_id(({"pond", "garden", "garden pond"}));
    set_name("garden pond");
    set_short("a %^BOLD%^GREEN%^garden %^BLUE%^pond%^RESET%^");
    set_long("A pond filled with lily pads and small ferns. Large koi hang out below the surface.");
    set_value(0);
    set_prevent_get(1);
}