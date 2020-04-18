#include "../ISV_Genesis.h";

inherit ROOM; //_INSTANCE;

void create() {
    ::create();
    set_property("indoors", 1);
    set_property("light", 1);
    set_short("Deck 8, Genesis, Nowhere");
    set_long("Long.");
    set_items(([

    ]));
    set_smell("default", "A faint hint of burning coolant wafts through the air.");
    set_listen("default", "Loud white noise from the active engines drowns out distant sounds.");
    set_exits(([
        "southwest": ISV_GENESIS_ROOM+"0_0_0.c",
    ]));
}