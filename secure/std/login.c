//    /secure/std/login.c
//    login object, connects player accounts to users

#include <config.h>
#include <news.h>
#include <flags.h>
#include <security.h>
#include <daemons.h>
#include <objects.h>
#include "login.h"

static private int __CrackCount, __CopyExists;
static private string __Name, __CapName, __Client;
static private object __Player;
static mapping __ansiMap;


static void get_password(string str);

static private int check_password(string str);
static private int valid_site(string ip);
static private int boot_copy();
static void disconnect_copy(string str);
static private void exec_user();
static void new_user(string str);
static void choose_password(string str);
static void confirm_password(string str2, string str1);
// static void choose_gender(string str);
static void enter_email(string str);
static void enter_real_name(string str);


// -------------------------------------------------------------------------

void create() {
    __Name = "";
    __Client = 0;
    __CrackCount = 0;
    __Player = 0;
    __ansiMap = TERMINAL_D->query_term_info("ansi");
}

void remove() {
    internal_remove();
}

// -------------------------------------------------------------------------

static void logon() {
    receive(format_ansi(read_file("/news/welcome"))+"\n");
    receive("            Driver: "+version()+"    "+"Mudlib: "+mudlib()+" "+mudlib_version()+"\n\n"); // AMCP 1.1 compliant

    receive(format_ansi(PROMPT_COLOR+"Enter account name:"+PROMPT_RESET)+" ");

    call_out("idle", LOGON_TIMEOUT);
    input_to("get_name");
}

static void idle() {
    receive("\n"+format_ansi(PROMPT_COLOR+"Connection timed out."+PROMPT_RESET)+"\n");
    internal_remove();
}

static void get_name(string str) {
    if(!str || str == "") {
        receive("\n"+format_ansi(PROMPT_COLOR+"Invalid entry, connection terminated."+PROMPT_RESET)+"\n");
        internal_remove();
        return;
    }
    if(sscanf(str, "%s:%s", __Name, __Client) != 2) {
        __Client = 0;
        __Name = convert_name(__Name = str);
    } else {
        __Name = convert_name(__Name);
        receive("\n");
        if(find_player(__Name)) __CopyExists = 1;
        else __CopyExists = 0;
        if(!register_client()) return;
    }
    if(find_player(__Name)) __CopyExists = 1;
    else __CopyExists = 0;
    continue_login();
}

static void continue_login() {
    if(master()->is_locked()) {
        receive(format_ansi(read_file("/news/locked"))+"\n");
        if(locked_access()) receive("\n"+format_ansi(PROMPT_COLOR+"    >>> Access allowed <<<"+PROMPT_RESET)+"\n");
        else {
            receive("\n"+format_ansi(PROMPT_COLOR+"    >>> Access denied <<<"+PROMPT_RESET)+"\n");
            internal_remove();
            return;
        }
    }
    if(!player_exists(__Name)) {
        if(!BANISH_D->valid_name(__Name)) {
            receive(format_ansi(PROMPT_COLOR+sprintf("\nThe account name '%s' is not a valid account name.\n", capitalize(__Name))+PROMPT_RESET));
            receive(format_ansi(PROMPT_COLOR+sprintf("Names must be alphabetic characters between %d and %d letters in length.\n\n", MIN_PLAYER_NAME_LENGTH, MAX_PLAYER_NAME_LENGTH)+PROMPT_RESET));
            receive(format_ansi(PROMPT_COLOR+"Enter account name:"+PROMPT_RESET)+" ");
            input_to("get_name");
            return;
        }
        if(!BANISH_D->allow_logon(__Name, query_ip_number())) {
            receive(format_ansi(read_file("/news/registration"))+"\n");
            internal_remove();
            return;
        }
        receive("\n"+format_ansi(PROMPT_COLOR+"Do you really wish %^BOLD%^"+capitalize(__Name)+"%^BOLD_OFF%^ as an account name? (y/%^BOLD%^n%^BOLD_OFF%^) "+PROMPT_RESET));
        __Player = master()->player_object(__Name);
        input_to("new_user");
        return;
    } else {
        if(!BANISH_D->allow_logon(__Name, query_ip_number())) {
            receive(format_ansi(read_file("/news/registration"))+"\n");
            internal_remove();
            return;
        }
        __Player = master()->player_object(__Name);
        if(!__Player) {
            //debug_message("no player, dude.");
        }
        message("password", "%^ORANGE%^Password:%^RESET%^ ", this_object());
        if(__Client) input_to("get_password");
        else input_to("get_password", I_NOECHO | I_NOESC);
    }
}

static void new_user(string str) {
    if((str = lower_case(str)) == "" || str[0] != 'y') {
        receive(format_ansi(PROMPT_COLOR+"Enter account name:"+PROMPT_RESET)+" ");
        __Player->remove();
        input_to("get_name");
        return;
    }
    log_file("new_players", sprintf("%s : %s : %s\n", query_ip_number(), __Name, ctime(time())));
    receive("\n"+format_ansi(PROMPT_COLOR+"Choose a password of at least 5 characters:"+PROMPT_RESET)+" ");
    if(__Client) input_to("choose_password");
    else input_to("choose_password", I_NOECHO | I_NOESC);
}

static void get_password(string str) {
    if(!str || str == "") {
        message("system", "\n"+PROMPT_COLOR+"Invalid entry, connection terminated."+PROMPT_RESET+"\n", this_object());
        internal_remove();
        return;
    }
    if(!check_password(str)) {
        message("system", PROMPT_COLOR+"Invalid password.\n"+PROMPT_RESET, this_object());
        if(++__CrackCount > MAX_PASSWORD_TRIES) {
            message("system", PROMPT_COLOR+"No more attempts allowed.\n"+PROMPT_RESET, this_object());
            internal_remove();
            return;
    }
        log_file("watch/logon", sprintf("%s from %s\n", __Name, query_ip_number()));
        message("password", PROMPT_COLOR+"Password: "+PROMPT_RESET, this_object());
        if(__Client) input_to("get_password");
        else input_to("get_password", I_NOECHO | I_NOESC);
        return;
    }
    if(!__CopyExists) exec_user();
    else boot_copy();
}

static private int check_password(string str) {
    string pass;

    if((pass = (string)__Player->query_password()) != crypt(str, pass)) return 0;
    return valid_site(query_ip_number());
  }

static private int valid_site(string ip) {
    string a, b;
    string *miens;
    int i;

    if(!(i = sizeof(miens = (string *)__Player->query_valid_sites()))) return 1;
    while(i--) {
        if(ip == miens[i]) return 1;
        if(sscanf(miens[i], "%s.*s", a) && sscanf(ip, a+"%s", b)) return 1;
    }
    return 0;
  }

static private int boot_copy() {
    if(interactive(__Player)) {
        message("system", "\nThere currently exists an interactive copy of you.\n", this_object());
        message("prompt", "Do you wish to take over this interactive copy? (y/n) ", this_object());
        input_to("disconnect_copy", I_NORMAL);
        return 1;
    }
    log_file("enter", sprintf("%s (exec): %s\n", __Name, ctime(time())));
    if(exec(__Player, this_object())) __Player->restart_heart();
    else message("system", "Problem reconnecting.\n", this_object());
    internal_remove();
    return 1;
  }

static void disconnect_copy(string str) {
    object tmp;

    if((str = lower_case(str)) == "" || str[0] != 'y') {
        message("system", "\nThen please try again later!\n", this_object());
        internal_remove();
        return;
    }
    message("system", "You are being taken over by hostile aliens!", __Player);
    exec(tmp = new(OB_USER), __Player);
    exec(__Player, this_object());
    __Player->set_client(__Client);
    destruct(tmp);
    message("system", "\nAllowing login.\n", __Player);
    internal_remove();
  }

static private void exec_user() {
    if(MULTI_D->query_prevent_login(__Name)) {
        internal_remove();
        return;
    }
    if(!exec(__Player, this_object())) {
        message("system", PROMPT_COLOR+"\nProblem connecting.\n"+PROMPT_RESET, this_object());
        __Player->remove();
        destruct(this_object());
        return;
    }
    __Player->set_client(__Client);
    catch(__Player->setup());
    destruct(this_object());
}

static void choose_password(string str) {
    if(strlen(str) < 5) {
        message("system", PROMPT_COLOR+"Your password must be at least 5 letters long.\n"+PROMPT_RESET, this_object());
        message("password", PROMPT_COLOR+"Choose a password: "+PROMPT_RESET, this_object());
        if(__Client) input_to("choose_password");
        else input_to("choose_password", I_NOECHO | I_NOESC);
    }
    message("password", PROMPT_COLOR+"Please confirm your password choice: "+PROMPT_RESET, this_object());
    if(__Client) input_to("confirm_password", str);
    else input_to("confirm_password", I_NOECHO | I_NOESC, str);
}

static void confirm_password(string str2, string str1) {
    if(str1 == str2) {
        __Player->set_password(str2 = crypt(str2, 0));
        // message("prompt", "\nPlease choose an interesting gender (male or female): ", this_object());
        // input_to("choose_gender");

        __CapName = capitalize(__Name);
        message("system", "\n"+PROMPT_COLOR+"You may format %^BOLD%^"+__CapName+"%^BOLD_OFF%^ capitalization, spaces, \"'\", or \"-\"."+PROMPT_RESET+"\n", this_object());
        message("prompt", PROMPT_COLOR+"Please choose a display name (default: %^BOLD%^"+__CapName+"%^BOLD_OFF%^): "+PROMPT_RESET, this_object());
        input_to("choose_cap_name");

        return;
    } else {
        message("password", PROMPT_COLOR+"Password entries do not match. Choose a password: "+PROMPT_RESET, this_object());
        if(__Client) input_to("choose_password");
        else input_to("choose_password", I_NOECHO | I_NOESC);
        return;
    }
}

// static void choose_gender(string str) {
//     if(str != "male" && str != "female") {
//         message("system", "\nCute, but pretend to be either male or female instead\n", this_object());
//         message("prompt", "Gender: ", this_object());
//         input_to("choose_gender");
//         return;
//     }
//     __Player->set_gender(str);

// }

static void choose_cap_name(string str) {
    if(!str || str == "") str = capitalize(__CapName);
    if(!(BANISH_D->valid_cap_name(str, __Name))) {
        message("prompt", "Incorrect format.  Choose again: ", this_object());
        input_to("choose_cap_name");
        return;
    }
    __Player->set_cap_name(capitalize(str));
    message("system", "\n"+PROMPT_COLOR+"Please enter your email address in the form of user@host.\nOnly admins will have unless configured by you."+PROMPT_RESET+"\n", this_object());
    message("prompt", "Email: ", this_object());
    input_to("enter_email");
}

static void enter_email(string str) {
    string a, b;

    if(!str || str == "" || sscanf(str, "%s@%s", a, b) != 2) {
        message("system", "\nEmail must be in the form user@host.\n", this_object());
        message("prompt", "Email: ", this_object());
        input_to("enter_email");
        return;
    }
    __Player->set_email(str);
    message("prompt", "\nIf you do not mind, enter your real name (optional): ", this_object());
    input_to("enter_real_name");
}

static void enter_real_name(string str) {
    if(!str || str == "") str = "Unknown";
    __Player->set_rname(str);
    log_file("enter", sprintf("%s (new player): %s\n", __Name, ctime(time())));
    exec_user();
}

// -------------------------------------------------------------------------

static string format_ansi(string input) {
    return replace_strings(explode(input, "%^"), __ansiMap);
}

void receive_message(string cl, string msg) {
    if(member_array(cl, ({"system", "prompt", "password"}))==-1) return;
    if(__Client) receive("<"+cl+">"+msg+"\n");
    else receive(format_ansi(msg));
}

static private void internal_remove() {
    if(__Player && !__CopyExists) destruct(__Player);
    destruct(this_object());
}

static private int locked_access() {
    int i;

    if(BANISH_D->is_guest(__Name)) return 1;
    i = sizeof(LOCKED_ACCESS_ALLOWED);
    while(i--) if(member_group(__Name, LOCKED_ACCESS_ALLOWED[i])) return 1;
    return 0;
}

static int register_client() {
    string client, ver;

    if(sscanf(__Client, "%s/%s", client, ver) != 2) ver = 0;
    if(member_array(__Client, SUPPORTED_CLIENTS) == -1) {
        receive("<protocol>"+implode(SUPPORTED_CLIENTS, ",")+"\n");
        input_to("input_protocol");
        return 0;
    }
    receive("<connect>\n");
    return 1;
}
static void input_protocol(string str) {
    string str_class, client;

    if(sscanf(str, "<%s>%s", str_class, client) != 2  || str_class != "protocol" ||
      member_array(client, SUPPORTED_CLIENTS) == -1 || client == "none")
        __Client = 0;
    else __Client = client;
    if(__Client) receive("<connect>\n");
    continue_login();
}

string query_name() {
    if(!interactive(this_object())) return 0;
    else if(__Name) return __Name;
    else return "";
}
string query_CapName() {
    string tmp = query_name();
    return (tmp ? capitalize(tmp) : "");
}