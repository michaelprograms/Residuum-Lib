#include "../ISV_Genesis.h";

inherit ROOM; //_INSTANCE;

void create() {
    ::create();
    set_property("indoors", 1);
    set_property("light", 1);
    set_short("ISV Genesis - Deck 8");
    set_long("A vaulted central corridor located at the spine of the vessel marked 'DECK 8'. The metallic walls are covered in electronic displays monitoring engine output. A staircase located in the center of the corridor allows passage between decks. The floor's non-slip plastic tiles span the available walkways which move around the four large engines, located to the north, east, south, and west. The ceiling is partially obscured by wires and piping that equally branch off into various inputs and outputs of the engines.");
    set_items(([
        "corridor": "The corridor is part of the central backbone of the vessel.",
        "stairs": "A steep stairway made of grated metal and piped handrails allow passage between decks.",
        ({"wall", "display"}): "The walls contain displays which output the rhythmic spiking status of the engine output.",
        "deck": "This is Deck 8, the bottom most deck of the vessel. Four large engines surround you.",
        "floor": "Gray colored plastic tiles with a non-slip pattern cover the available walkable space.",
        "walkways": "The walkways branch northeast, southeast, southwest, and northwest in the space between the large engines.",
        "engines": "The vessel's four engines are evenly spaced to the north, east, south, and west and look to each easily be the size of the central spine. There are walkways to the northeast, southeast, southwest, and northwest that squeeze between the engine housing.",
        "ceiling": "A plain metallic surface obscured by wires of varying thicknesses and shielding and piping of varying widths and insulation.",
    ]));
    set_smell("default", "A faint hint of burning coolant wafts through the air.");
    set_listen("default", "Loud white noise from the active engines drowns out distant sounds.");
    set_exits(([
        "northeast": ISV_GENESIS_ROOM+"1_-1_0.c",
        "southeast": ISV_GENESIS_ROOM+"1_1_0.c",
        "southwest": ISV_GENESIS_ROOM+"-1_1_0.c",
        "northwest": ISV_GENESIS_ROOM+"-1_-1_0.c",
        "up": ISV_GENESIS_ROOM+"0_0_1.c",
    ]));
}