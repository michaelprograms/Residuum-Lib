inherit ROOM_INSTANCE;

void create() {
    ::create();
    set_property("indoors", 1);
    set_property("light", 1);
    set_short("nowhere");
    set_long("A simple room with plain metallic walls. The floor is made up of large white tiles. Tubes and wires descend from the center of the ceiling towards a person sized tank. An open hatchway leads towards the hallway.");
    add_exit("south", "/domains/Nowhere/rooms/start_hallway");
    new("/domains/Nowhere/obj/tank")->move(this_object());
    add_enter("tank", "/domains/Nowhere/rooms/start");
}