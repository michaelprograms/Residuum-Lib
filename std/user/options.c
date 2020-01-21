private mapping __Options;

mapping query_options() { return __Options; }
mixed query_option(string key) { return __Options && __Options[key]; }

// TODO: decide if this function should validate key/values
void set_option(string key, mixed value) {
    if(!key || key == "") return;
    if(!__Options) __Options = ([]);
    __Options[key] = value;
}

int remove_option(string key) {
    if(member_array(key, keys(__Options)) > -1) {
        map_delete(__Options, key);
        return 1;
    } else return 0;
}

void create() {
    __Options = ([]);
}