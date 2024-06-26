#ifndef __SIMULEFUN_H
#define __SIMULEFUN_H

// debug_print
void debug_print(string name, string msg);

// files
int file_exists(string str);
int create_dir(string str);

// format
varargs string format_syntax(string text, int wrap);
string format_uptime();
string format_command_help(string syntax, string description);

// user_functions
int account_exists(string str);
int player_exists(string str);
string user_path(string name);

// strings
varargs string center(string str, int x);
string arrange_string(string str, int x);
varargs string wrap(string str, int width);
string replace_strings(string *words, mapping info);
varargs string pad(int length, string c);
varargs string leftpad(string text, int n, string pad);
varargs string rightpad(string text, int n, string pad);
varargs string format_header_bar(string title, string optional);
string format_divider_bar();
string format_footer_bar();
string time_ago(int timestamp);
string time_ago_full(int timestamp);

// unsorted
int absolute_value(int x);
void add_sky_event(function f);
int alignment_ok(object ob);
string alignment_string(mixed val);
int ambassadorp(object ob);
string architecture();
int archp(object ob);
string base_name(mixed val);
string cardinal(int x);
string consolidate(int x, string str);
string convert_name(string str);
mixed copy(mixed val);
string creator_file(string str);
varargs int creatorp(object ob);
float currency_inflation(string type);
float currency_rate(string type);
int currency_mass(string type, int x);
int currency_value(int x, string str);
string date(int x);
string day(int x);
int destruct(object ob);
mixed *distinct_array(mixed *arr);
string domain(mixed val);
int domain_exists(string dmn);
object domain_master(mixed val);
int effective_light(object ob);
varargs void event(string fun, int when, mixed *args, int reg);
int event_pending(object ob);
varargs mixed *exclude_array(mixed *array, int from, int to);
int exec(object target, object src);
int export_uid(object ob);
string file_privs(string file);
string format_page(string *items, int columns);
int geteuid(object ob);
int getuid(object ob);
varargs object get_object(string str, object player);
varargs mixed get_objects(string str, object player, int no_arr);
int hiddenp(object ob);
int high_mortalp(object ob);
int hour(int x);
string identify(mixed a);
int interact(string substr, string str);
int leaderp(object ob);
object *livings();
object load_object(string str);
void log_file(string fl, string msg);
int member_group(mixed who, string grp);
int minutes(int x);
string month(int x);
int moon_light();
string *mud_currencies();
string mud_name();
string mudlib();
string mudlib_version();
string nominative(mixed val);
string objective(mixed val);
string ordinal(int x);
object parse_objects(object where, string str);
mixed *path_file(mixed full_path);
float percent(mixed numerator, mixed denominator);
void personal_log(string str);
string pluralize(mixed single);
string possessive(mixed val);
string possessive_noun(mixed val);
int query_host_port();
string query_idle_string(object ob);
int query_night();
object query_snoop(object snoopee);
object query_snooping(object ob);
string *read_database(string file);
string reflexive(mixed val);
void remove_sky_event(function f);
string absolute_path(string curr, string newstr);
varargs void say(mixed str, object ob);
string season(int x);
void set_eval_limit(int x);
void set_privs(object ob, string str);
int seteuid(string str);
varargs void shout(mixed str, mixed exclude);
varargs void shutdown(int code);
varargs object snoop(object who, object target);
string strip_colours(string str);
string substr(string str, string match, string replace);
void tell_object(object ob, mixed str);
varargs void tell_room(object ob, mixed str, mixed exclude);
object to_object(mixed target);
int total_light(object ob);
string translate(string str, int prof);
mixed unguarded(function f);
string version();
varargs int visible(object detectee_obj, object detector_obj);
void write(string str);
int year(int x);

#endif /* __SIMULEFUN_H */
