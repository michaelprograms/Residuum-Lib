//      /secure/SimulEfun/user_exists.c
//      from the Nightmare Mudlib
//      returns true if the player exists
//      created by Descartes of Borg 27 july 1993

int user_exists(string str) { return (int)master()->player_exists(str); } // TODO remove

int player_exists(string str) { return master()->player_exists2(str); } // TODO rename