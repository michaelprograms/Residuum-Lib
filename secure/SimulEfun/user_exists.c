//      /secure/SimulEfun/user_exists.c
//      from the Nightmare Mudlib
//      returns true if the player exists
//      created by Descartes of Borg 27 july 1993

int account_exists(string str) { return master()->account_exists(str); }

int player_exists(string str) { return master()->player_exists(str); }