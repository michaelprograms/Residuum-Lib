inherit ROOM_INSTANCE;

void create() {
    ::create();
    set_property("indoors", 1);
    set_property("light", 1);
    set_short("nowhere");
    set_long("An unadorned hallway with plain metallic walls. The floor is made up of large white tiles. Various pipes run along the ceiling. An open hatchway leads into a small room.");
    add_exit("north", "/domains/Nowhere/rooms/start_chamber");
}