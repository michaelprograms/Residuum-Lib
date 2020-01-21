private mapping __Options = ([
    // New Character defaults:
    "TERM": "ansi",
    "TZONE": "EST",
]);

mapping query_options() {
    if(!__Options) __Options = ([]);
    return __Options;
}
mixed query_option(string key) {
    if(!__Options) __Options = ([]);
    return __Options[key];
}

// TODO: decide if this function should validate key/values
void set_option(string key, mixed value) {
    if(!__Options) __Options = ([]);
    if(!key || key == "") return;
    if(!__Options) __Options = ([]);
    __Options[key] = value;
}

int remove_option(string key) {
    if(!__Options) __Options = ([]);
    if(member_array(key, keys(__Options)) > -1) {
        map_delete(__Options, key);
        return 1;
    } else return 0;
}