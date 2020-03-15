inherit OBJECT;

void create() {
    ::create();
    set_id(({"tank", "glass tank", "large tank", "large glass tank"}));
    set_short("large glass tank");
    set_long("A cylinder shaped tank large enough to hold a human body. The tank is made of rounded glass to allow sight with displays visible within the glass to both the inside and outside of the tank. The chamber is reclined so the inhabitant would be laying down in an upright position on a firm black mat. Flexible pipes and conduits connect to underneath of the tank.");
    set_prevent_get("The tank is too large.");
}