#include <daemons.h>

inherit ROOM;

int is_instance_room() { return 1; }
int is_master_room() { return file_name(this_object()) != base_name(this_object()); }

object query_instance_room(object player) {
    return ROOM_INSTANCE_D->query_player_instance(player->query_name(), base_name(this_object()));
}