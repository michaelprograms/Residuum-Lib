#ifndef __ROOMS_H
#define __ROOMS_H

#include <dirs.h>

#define ROOM_START      "/domains/Nowhere/setter"
#define ROOM_ADMIN      "/domains/Nowhere/hall"

// Used when no environment
#define ROOM_VOID 	    (DIR_STANDARD_DOMAIN "/void")
// Used by linkdead players
#define ROOM_FREEZER 	(DIR_STANDARD_DOMAIN "/freezer")
// Used by new players
#define ROOM_SETTER 	(DIR_STANDARD_DOMAIN "/setter")
// Used by dead players
#define ROOM_DEATH 	    (DIR_STANDARD_DOMAIN "/death/death_room")

// Used by hide
#define ROOM_CACHE 	    (DIR_STANDARD_DOMAIN "/cache")
// Used by lock terminal
#define ROOM_LOCKED 	(DIR_STANDARD_DOMAIN "/locked")
// Used by mudparty
#define ROOM_PARTY 	    (DIR_STANDARD_DOMAIN "/party")

// Probably Outdated
#define ROOM_HALL 	    (DIR_STANDARD_DOMAIN "/adv_main")
#define ROOM_IDLE_SHOP 	(DIR_STANDARD_DOMAIN "/idle_supply")
#define ROOM_QUEST 	    (DIR_STANDARD_DOMAIN "/quest_room")
#define ROOM_SHERIFF 	(DIR_STANDARD_DOMAIN "/sheriff")

#endif /* __ROOMS_H */
