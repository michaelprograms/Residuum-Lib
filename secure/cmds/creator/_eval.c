#define TMP_FILE "/tmp/"+this_player()->query_name()+".eval"
#define ED_BASIC_COMMANDS "\"%^CYAN%^BOLD%^i%^RESET%^\"nsert code, \"%^CYAN%^BOLD%^.%^RESET%^\" to save, e\"%^CYAN%^BOLD%^x%^RESET%^\"ecute, \"%^CYAN%^BOLD%^q%^RESET%^\"uit to abort"

inherit DAEMON;

// -----------------------------------------------------------------------------

void execute_file(string file, string input);
void clear_file(string file);
void create_tmp_file(string file, string input);
void end_edit(mixed *args);
void abort();

// -----------------------------------------------------------------------------

int cmd_eval(string input) {
    string file = user_path(this_player()->query_name());
    string editor, tmp;

    if(file_size(file) != -2) return notify_fail("You must have a valid home directory.\n");
    file += "CMD_EVAL_TMP_FILE.c";
    if(!write_file(file, "")) return notify_fail("You must have write access.\n");

    if(input) {
        if(!regexp(input, ";$")) input = input + ";";
        if(regexp(input, ";")) input = replace_string(input, "; ", ";\n");
        clear_file(file);
        execute_file(file, input);
    }
    else {
        editor = this_player()->getenv("EDITOR");
        if(editor != "ed") this_player()->setenv("EDITOR", "ed");
        if(this_player()->getenv("EDITOR") == "ed") {
            message("system", "Entering eval ed mode, standard ed commands apply:", this_player());
            message("system", ED_BASIC_COMMANDS, this_player());
            message("system", "__________________________________________________________________________", this_player());
            if(tmp = read_file(TMP_FILE)) this_player()->catch_tell(tmp);
        }
        this_player()->edit(TMP_FILE, (:end_edit:), (:abort:), ({file}));
        this_player()->setenv("EDITOR", editor);
    }
    return 1;
}

void help() {
  write(format_command_help(
      "eval ([LPC commands])",
      "Creates a temporary file containing "+format_syntax("[LPC commands]")+"%^RESET%^ which is executed with call_other and the results are returned. If "+format_syntax("[LPC commands]")+"%^RESET%^ are not input, places the user in edit mode.\n\n"
      "Examples:\n"
      "> eval return 1 + cos( 0.0 )\n"
      "Result = 2.000000\n"
      "> eval return explode(\"banana\", \"a\")\n"
      "Result = ({ \"b\", \"n\", \"n\" })\n\n"
      "> %^YELLOW%^eval%^RESET%^\n"
      "Entering eval ed mode, standard ed commands apply:\n"
      ED_BASIC_COMMANDS+"\n"
      "__________________________________________________________________________\n"
      ": %^YELLOW%^i%^RESET%^\n"
      "%^YELLOW%^object ob = environment(this_player());%^RESET%^\n"
      "%^YELLOW%^return ob->query_long();%^RESET%^\n"
      "%^YELLOW%^%^.%^RESET%^\n"
      ": %^YELLOW%^x%^RESET%^\n"
      "\"tmp/CREATOR_NAME.eval\" 2 lines 65 bytes\n"
      "Result = \"A room's description.\"\n"
      "Exit from ed.",
      1
  ));
}

// -----------------------------------------------------------------------------

void execute_file(string file, string input) {
    mixed ret;

    log_file("adm/eval", time()+" "+this_player()->query_name()+" <"+replace_string(input, "\n", " ")+">\n");
    create_tmp_file(file, input);
    ret = (mixed)call_other(file, "eval");
    if(regexp(input, "return")) message("system", "Result = " + identify(ret), this_player());
}

void clear_file(string file) {
    mixed ret;
    rm(file);
    if(ret = find_object(file)) destruct(ret);
}

void create_tmp_file(string file, string input) {
    string lines = @EndCode
#include <std.h>
#include <daemons.h>

#define TP this_player()
#define TO this_object()
#define ENVTP environment(this_player())
#define ENVTO environment(this_object())

inherit OBJECT;

EndCode;
    lines += "mixed eval() {\n" + input + "\n}";

    write_file(file, lines);
}

void end_edit(mixed *args) {
  string file = args[0];
  string tmpFile = TMP_FILE;
  string input = read_file(tmpFile);
  clear_file(file);
  execute_file(file, input);
}
void abort() { }
