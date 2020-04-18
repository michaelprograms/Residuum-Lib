inherit DAEMON;

static private mapping __Instances = ([
/*
    // Data Format

    "name": ([
        "path": ([
            "ob": ROOM,
            "created": timestamp
        ]),
    ]),
*/
]);

mapping query_instances() { return __Instances; }

int query_room_last_modified(string path) {
    mixed *files = get_dir(path + (!regexp(path, "\.c$") ? ".c" : ""), -1);
    if(files[0] && files[0][2]) return files[0][2];
    else return 0;
}

object query_player_instance(string name, string path) {
    object ob;

    // Verify data store setup
    if(!__Instances) __Instances = ([]);
    if(!__Instances[name]) __Instances[name] = ([]);

    // Check cache for existing instance
    if(__Instances[name][path]) {
        if(query_room_last_modified(path) > __Instances[name][path]["created"] || !__Instances[name][path]["ob"]) {
            map_delete(__Instances[name], path);
        } else return __Instances[name][path]["ob"];
    }

    // Setup new room
    ob = new(path);
    if(!ob) return 0;
    if(!ob->is_instance_room()) {
        ob->remove();
        return 0;
    }

    __Instances[name][path] = ([
        "ob": ob,
        "created": time(),
    ]);
    return ob;

}

// -------------------------------------------------------------------------

void create() {
    ::create();
    set_no_clean(1);
    set_heart_beat(60);
}

void heart_beat() {
    // Cleanup all entries
    foreach(string name in keys(__Instances)) {
        foreach(string path in keys(__Instances[name])) {
            if(!__Instances[name][path]["ob"]) map_delete(__Instances[name], path);
        }
    }
}