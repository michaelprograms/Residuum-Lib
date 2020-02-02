#include <std.h>
#include <dirs.h>
#include <config.h>

inherit CLEAN_UP;

// -------------------------------------------------------------------------

string query_name();
void set_name(string str);
string query_password();
void set_password(string pass);
mapping query_characters();
int add_character(string name);
int query_last_on();
void update_last_on();
nomask void save_account(string nom);
nomask int restore_account(string nom);

string name, password;
int created, last_on;
mapping characters;

// -------------------------------------------------------------------------

string query_name() { return name; }
void set_name(string str) {
    if(!stringp(str)) error("Bad argument 1 to set_name().\n");
    name = str;
    if(!created) created = time();
    if(!characters || !mapp(characters)) characters = ([]);
}

// -------------------------------------------------------------------------

string query_password() { return password; }
void set_password(string pass) {
    if(!master()->valid_apply(({ query_name()}))) return 0;
    password = pass;
    save_account(query_name());
}

// -------------------------------------------------------------------------

mapping query_characters() { return characters; }
int query_characters_count() { return sizeof(keys(characters)); }

int add_character(string name) {
    if(!characters || !mapp(characters)) characters = ([]);
    if(characters[name]) {
        error("Character already exists.\n");
        return -1;
    }
    if(query_characters_count() >= MAX_CHARACTERS_PER_ACCOUNT) {
        return 0;
    }
    characters[name] = ([
        "name": name,
        "deleted": 0,
    ]);
    save_account(query_name());
    return 1;
}

// -------------------------------------------------------------------------

int query_last_on() { return last_on; }
void update_last_on() {
    last_on = time();
    save_account(query_name());
}

// -------------------------------------------------------------------------

nomask void save_account(string nom) {
    string file, dir;

    if(!nom || nom != query_name()) return;
    file = sprintf("%s/%s/%s", DIR_ACCOUNTS, nom[0..0], nom+__SAVE_EXTENSION__);
    unguarded((: save_object, file, 0 :));
}

nomask int restore_account(string nom) {
    string file;

    if(previous_object(0) != master()) return 0;
    file = sprintf("%s/%s/%s", DIR_ACCOUNTS, nom[0..0], nom+__SAVE_EXTENSION__);
    return unguarded((: restore_object, file :));
}