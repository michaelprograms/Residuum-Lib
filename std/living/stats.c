//	/std/living/stats.c

mapping stats;
static mapping stat_bonus;

void update_vitals();
void augment_body(int x);

static void init_stats() {
    stats = ([]);
}

string *query_all_stats() {
    return keys(stats);
}

int query_base_stats(string stat) {
    if(!stats || !stats[stat]) return 0;
    else return stats[stat];
}

int query_stats(string stat) {
    int x;

    if(stat_bonus) x= stat_bonus[stat];
    else x = 0;
    return stats[stat] + x;
}
void set_stats(string stat, int amount) {
    if(stats[stat] && stats[stat] != amount) {
        log_file("stats", this_object()->query_name()+" went from "+stats[stat]+" to "+amount+" in "+stat+" ("+ctime(time())+")\n");
        log_file("stats", "PRIV: "+query_privs(previous_object())+" ("+file_name(previous_object())+")\n");
    }
    stats[stat] = amount;
    update_vitals();
    if(stat == "constitution") augment_body(!!amount);
}

void add_stat_bonus(string stat, int amount) {
    if(!stat_bonus) stat_bonus = ([]);
    if(stat_bonus[stat]) stat_bonus[stat] += amount;
    else stat_bonus[stat] = amount;
    if(!stat_bonus[stat]) map_delete(stat_bonus, stat);
}

void reduce_stats() {
    string *ind;
    int i;

    if(!stats) return;
    ind = keys(stats);
    for(i=0; i<sizeof(ind); i++)
	if(random(101)>50) set_stats(ind[i], query_base_stats(ind[i])-1);
}