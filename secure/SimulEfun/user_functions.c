int account_exists(string str) { return master()->account_exists(str); }

int player_exists(string str) { return master()->player_exists(str); }

string user_path(string name) { return ("/realms/"+name+"/"); }