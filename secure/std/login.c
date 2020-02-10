//    /secure/std/login.c
//    login object, connects player accounts to users

#include <config.h>
#include <news.h>
#include <flags.h>
#include <security.h>
#include <daemons.h>
#include <objects.h>
#include "login.h"

static private int __CrackCount, __CopyExists, __AccountLastOn;
static private string __AccountName, __Name, __CapName, __Client, __IPAddress;
static private object __Account, __Player;
static mapping __ansiMap;

// -------------------------------------------------------------------------

void create() {
    __AccountName = "";
    __Name = "";
    __Client = 0;
    __CrackCount = 0;
    __Account = 0;
    __Player = 0;
    __AccountLastOn = 0;
    __ansiMap = TERMINAL_D->query_term_info("ansi");
    __IPAddress = query_ip_number();
}

void remove() { internal_remove(); }

// -------------------------------------------------------------------------

static void logon() {
    receive(format_ansi(read_file("/news/welcome"))+"\n");
    receive("            Driver: "+version()+"    "+"Mudlib: "+mudlib()+" "+mudlib_version()+"\n\n"); // AMCP 1.1 compliant

    log_file("login_connect", time()+" "+__IPAddress+"\n");
    call_out("idle", LOGON_TIMEOUT);
    prompt_account_name();
}

static void idle() {
    receive("\n"+format_ansi(PROMPT_COLOR+"Connection timed out."+PROMPT_RESET)+"\n");
    return internal_remove();
}

static void prompt_account_name() {
    receive(format_ansi(PROMPT_COLOR+"Enter account name:"+PROMPT_RESET)+" ");
    input_to("get_account_name");
}

static void get_account_name(string str) {
    if(!str || str == "") {
        receive("\n"+format_ansi(PROMPT_COLOR+"Invalid entry. Connection terminated."+PROMPT_RESET)+"\n");
        return internal_remove();
    }
    if(sscanf(str, "%s:%s", __AccountName, __Client) != 2) {
        __Client = 0;
        __AccountName = lower_case(__AccountName = str);
    } else {
        __AccountName = lower_case(__AccountName);
        receive("\n");
        if(!register_client()) return;
    }
    continue_login();
}
// if(find_player(__Name)) __CopyExists = 1;
// else __CopyExists = 0;

static void continue_login() {
    if(master()->is_locked()) {
        receive(format_ansi(read_file("/news/locked"))+"\n");
        if(locked_access()) receive("\n"+format_ansi(PROMPT_COLOR+"    >>> Access allowed <<<"+PROMPT_RESET)+"\n");
        else {
            receive("\n"+format_ansi(PROMPT_COLOR+"    >>> Access denied <<<"+PROMPT_RESET)+"\n");
            return internal_remove();
        }
    }
    if(!account_exists(__AccountName)) {
        if(!BANISH_D->valid_account_name(__AccountName)) {
            receive(format_ansi(PROMPT_COLOR+"\nThe name %^BOLD%^"+__AccountName+"%^BOLD_OFF%^ is not a valid account name.\n"+PROMPT_RESET));
            receive(format_ansi(PROMPT_COLOR+sprintf("Names must be alphanumeric characters between %d and %d letters in length.\n\n", MIN_ACCOUNT_NAME_LENGTH, MAX_ACCOUNT_NAME_LENGTH)+PROMPT_RESET));
            prompt_account_name();
            return;
        }
        prompt_banished();
        receive("\n"+format_ansi("%^BOLD%^New account.%^BOLD_OFF%^ "+PROMPT_COLOR+"Confirm %^BOLD%^"+__AccountName+"%^BOLD_OFF%^ as account name? (y/%^BOLD%^n%^BOLD_OFF%^) "+PROMPT_RESET));
        __Account = 0;
        __Account = master()->account_object(__AccountName);
        input_to("new_account");
        return;
    } else {
        prompt_banished();
        __Account = master()->account_object(__AccountName);
        if(!__Account) {
            //debug_message("no player, dude.");
        }
        prompt_password();
        return;
    }
}

static void prompt_banished() {
    string name;

    if(__Name && __Name != "") name = __Name;
    else if(__AccountName && __AccountName != "") name = __AccountName;

    if(!BANISH_D->allow_logon(name, __IPAddress)) {
        receive(format_ansi(read_file("/news/registration"))+"\n");
        return internal_remove();
    }
}

static void new_account(string str) {
    if((str = lower_case(str)) == "" || str[0] != 'y') {
        __Account->remove();
        prompt_account_name();
        return;
    }
    log_file("new_accounts", sprintf("%s : %s : %s\n", ctime(time()), __IPAddress, __AccountName));
    prompt_create_password();
    return;
}

static void prompt_create_password() {
    message("password", PROMPT_COLOR+"Create password (min length 5): "+PROMPT_RESET, this_object());
    if(__Client) input_to("choose_password");
    else input_to("choose_password", I_NOECHO | I_NOESC);
}

static void choose_password(string str) {
    if(strlen(str) < 5) {
        message("system", PROMPT_COLOR+"Password did not meet requirements.\n"+PROMPT_RESET, this_object());
        return prompt_create_password();
    }
    message("password", PROMPT_COLOR+"Confirm password: "+PROMPT_RESET, this_object());
    if(__Client) input_to("confirm_password", str);
    else input_to("confirm_password", I_NOECHO | I_NOESC, str);
}

static void confirm_password(string str2, string str1) {
    if(str1 == str2) {
        __Account->set_password(str2 = crypt(str2, 0));
        prompt_account_menu();
        return;
    } else {
        message("password", PROMPT_COLOR+"Password entries do not match."+PROMPT_RESET+"\n", this_object());
        prompt_create_password();
        return;
    }
}

static void prompt_account_menu() {
    mapping characters = __Account->query_characters();
    string *names = keys(characters), msg;
    object ob;

    remove_call_out("idle");
    call_out("idle", LOGON_ACCOUNT_TIME);

    if(!sizeof(names)) {
        message("system", PROMPT_COLOR+"No existing characters."+PROMPT_RESET+"\n", this_object());
        prompt_create_character();
        return;
    } else {
        msg = "\n"+format_header_bar("ACCOUNT MENU")+"\n%^ORANGE%^Welcome back, "+__AccountName+". Last seen "+time_ago(__AccountLastOn)+" ago.%^RESET%^\n\n";
        msg += "%^BOLD%^Account Options     :%^BOLD_OFF%^ "+format_syntax("<password>")+" "+format_syntax("<quit>")+"\n";
        msg += "%^BOLD%^Player Options      :%^BOLD_OFF%^ "+format_syntax("<new>")+" "+format_syntax("<delete>")+"\n\n";
        message("system", msg, this_object());

        msg = "%^BOLD%^Login As...       Level Race       Class            Last Seen%^BOLD_OFF%^\n";

        //sizeof(names)+"/"+MAX_CHARACTERS_PER_ACCOUNT

        names = sort_array(names, 1);
        for(int i = 0; i < sizeof(names); i ++) {
            // line = sprintf("%2s", ""+(i+1))+". ";
            // line = format_syntax("<"+names[i]+">") + pad(MAX_PLAYER_NAME_LENGTH-sizeof(names[i])) + " ";
            msg += FINGER_D->query_player_login_brief(names[i]) + "\n";
        }
        message("system", msg, this_object());

        message("system", "\n"+format_footer_bar()+"\n", this_object());
        message("prompt", "\n"+PROMPT_COLOR+"Enter choice: "+PROMPT_RESET, this_object());
        input_to("get_menu_choice");
    }

}

static void get_menu_choice(string str) {
    mapping characters = __Account->query_characters();
    string *names = keys(characters);

    if(!str || str == "") {
        prompt_account_menu();
        return;
    }
    str = lower_case(str);
    if(member_array(str, ({"p", "passwd"})) > -1) {
        prompt_create_password();
        return;
    }
    if(member_array(str, ({"n", "new"})) > -1) {
        prompt_create_character();
        return;
    }
    if(member_array(str, ({"d", "delete"})) > -1) {
        // mark character deleted
        prompt_delete_character();
        return;
    }
    if(member_array(str, ({"q", "quit"})) > -1) {
        message("system", PROMPT_COLOR+"Connection closed."+PROMPT_RESET+"\n", this_object());
        return internal_remove();
    }
    if(member_array(str, names) > -1) {
        // existing character
        __Name = str;
        __Player = master()->player_object(__Name);
        message("system", PROMPT_COLOR+"Entering as "+__Player->query_cap_name()+"..."+PROMPT_RESET+"\n", this_object());
        if(find_player(__Name)) __CopyExists = 1;
        else __CopyExists = 0;

        if(!__CopyExists) exec_user();
        else boot_copy();

        return;
    }
    prompt_account_menu();
}

static void prompt_delete_character() {
    message("system", PROMPT_COLOR+"Delete which character?"+PROMPT_RESET, this_object());
    // TODO...
}

static void prompt_create_character() {
    receive(format_ansi(PROMPT_COLOR+"Enter character name:"+PROMPT_RESET)+" ");
    input_to("get_create_character");
}

static void get_create_character(string str) {
    if(!str || str == "") {
        prompt_account_menu();
        return;
    }
    __Name = convert_name(str);
    receive("\n");

    if(player_exists(__Name)) {
        message("system", PROMPT_COLOR+"Character name already in use, pick another."+PROMPT_RESET, this_object());
        prompt_create_character();
        return;
    } else {
        if(!BANISH_D->valid_name(__Name)) {
            receive(format_ansi(PROMPT_COLOR+sprintf("\nThe name %^BOLD%^%s%^BOLD_OFF%^ is not a valid name.\n", __Name)+PROMPT_RESET));
            receive(format_ansi(PROMPT_COLOR+sprintf("Names must be alphabetic characters between %d and %d letters in length.\n\n", MIN_PLAYER_NAME_LENGTH, MAX_PLAYER_NAME_LENGTH)+PROMPT_RESET));
            prompt_create_character();
            return;
        }
        prompt_banished();
        message("system", "%^BOLD%^New character.%^BOLD_OFF%^ "+PROMPT_COLOR+"Confirm %^BOLD%^"+capitalize(__Name)+"%^BOLD_OFF%^ as name? (y/%^BOLD%^n%^BOLD_OFF%^) "+PROMPT_RESET, this_object());
        __Player = master()->player_object(__Name);
        __Player->set_account(__AccountName);
        input_to("confirm_new_character");
        return;
    }
}

static void confirm_new_character(string str) {
    if((str = lower_case(str)) == "" || str[0] != 'y') {
        __Player->remove();
        prompt_create_character();
        return;
    }
    log_file("new_players", sprintf("%s : %s : %s\n", ctime(time()), __IPAddress, __Name));
    if(__Account->add_character(__Name) != 1) {
        message("system", PROMPT_COLOR+"Problem during character creation."+PROMPT_RESET, this_object());
        return internal_remove();
    }
    __CapName = capitalize(__Name);
    prompt_cap_name();
     // log_file("enter", sprintf("%s : %s : (new player)\n", ctime(time()), __Name));

}

static void prompt_password() {
    message("password", PROMPT_COLOR+"Password: "+PROMPT_RESET, this_object());
    if(__Client) input_to("get_password");
    else input_to("get_password", I_NOECHO | I_NOESC);
}

static void get_password(string str) {
    if(!str || str == "") {
        message("system", "\n"+PROMPT_COLOR+"Invalid entry. Connection terminated."+PROMPT_RESET+"\n", this_object());
        return internal_remove();
    }
    if(!check_password(str)) {
        message("system", PROMPT_COLOR+"Invalid password."+PROMPT_RESET, this_object());
        if(++__CrackCount > MAX_PASSWORD_TRIES) {
            message("system", PROMPT_COLOR+"No more attempts allowed.\n"+PROMPT_RESET, this_object());
            return internal_remove();
        }
        log_file("watch/logon", sprintf("%s from %s\n", __Name, __IPAddress));
        return prompt_password();
    }
    __AccountLastOn = __Account->query_last_on();
    __Account->update_last_on();
    prompt_account_menu();
    // if(!__CopyExists) exec_user();
    // else boot_copy();
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
    return internal_remove();
}

static void disconnect_copy(string str) {
    object tmp;

    if((str = lower_case(str)) == "" || str[0] != 'y') {
        message("system", "\nThen please try again later!\n", this_object());
        return internal_remove();
    }
    message("system", "You are being taken over by hostile aliens!", __Player);
    exec(tmp = new(OB_USER), __Player);
    exec(__Player, this_object());
    __Player->set_client(__Client);
    destruct(tmp);
    message("system", "\nAllowing login.\n", __Player);
    return internal_remove();
}

static void prompt_cap_name() {
    message("system", "\n"+PROMPT_COLOR+"Character name may be formatted with uppercase, spaces, ', or -."+PROMPT_RESET+"\n", this_object());
    message("prompt", PROMPT_COLOR+"Choose a sensible display name (default: %^BOLD%^"+__CapName+"%^BOLD_OFF%^): "+PROMPT_RESET, this_object());
    input_to("choose_cap_name");
}

static void choose_cap_name(string str) {
    if(!str || str == "") str = capitalize(__CapName);
    if(sizeof(str) > sizeof(__Name)*3/2 || !(BANISH_D->valid_cap_name(str, __Name))) {
        message("system", PROMPT_COLOR+"Invalid entry."+PROMPT_RESET, this_object());
        prompt_cap_name();
        return;
    }
    __Player->set_cap_name(capitalize(str));
    message("system", "Character name is now formatted to "+__Player->query_cap_name()+".\n", this_object());
    prompt_gender();
    // prompt_email();
}

static void prompt_email() {
    message("system", "\n"+PROMPT_COLOR+"Enter email address in the format of user@host.\nThis information is private by default."+PROMPT_RESET+"\n", this_object());
    message("prompt", PROMPT_COLOR+"Email (user@host): "+PROMPT_RESET, this_object());
    input_to("enter_email");
}
static void enter_email(string str) {
    string a, b;

    if(!str || str == "" || sscanf(str, "%s@%s", a, b) != 2) {
        prompt_email();
        return;
    }
    // __Player->set_email(str);
    prompt_gender();
}

static void prompt_gender() {
    message("prompt", "\n"+PROMPT_COLOR+"Choose a gender (female, male, neuter): "+PROMPT_RESET, this_object());
    input_to("enter_gender");
}

static void enter_gender(string str) {
    if(member_array(str, ({"female", "male", "neuter"})) == -1) {
        message("system", PROMPT_COLOR+"Invalid entry."+PROMPT_RESET, this_object());
        prompt_gender();
        return;
    }
    __Player->set_gender(str);
    exec_user();
}

// -------------------------------------------------------------------------

static private void internal_remove() {
    if(find_player(__Name)) __CopyExists = 1;
    else __CopyExists = 0;

    if(__Player && !__CopyExists) {
        log_file("login_destruct", time()+" "+__Name+" "+identify(__Player)+" "+__IPAddress+"\n");
        destruct(__Player);
    }
    destruct(this_object());
}

static private int locked_access() {
    int i;

    if(BANISH_D->is_guest(__AccountName)) return 1;
    i = sizeof(LOCKED_ACCESS_ALLOWED);
    while(i--) if(member_group(__AccountName, LOCKED_ACCESS_ALLOWED[i])) return 1;
    return 0;
}

static private int valid_site(string ip) {
    string a, b;
    string *miens;
    int i;

    if(!(i = sizeof(miens = (string *)__Account->query_valid_sites()))) return 1;
    while(i--) {
        if(ip == miens[i]) return 1;
        if(sscanf(miens[i], "%s.*s", a) && sscanf(ip, a+"%s", b)) return 1;
    }
    return 0;
}

static private int check_password(string str) {
    string pass;
    if((pass = (string)__Account->query_password()) != crypt(str, pass)) return 0;
    return valid_site(__IPAddress);
}

static private void exec_user() {

    // Verify new account setup is complete in case of disconnect
    // if(!__Player->query_email()) {
    //     message("system", "Email not set.\n", this_object());
    //     return prompt_email();
    // }
    if(!__Player->query_gender()) {
        message("system", "Gender not set.\n", this_object());
        return prompt_gender();
    }

    if(MULTI_D->query_prevent_login(__Name)) {
        return internal_remove();
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

// -------------------------------------------------------------------------

static string format_ansi(string input) {
    return replace_strings(explode(input, "%^"), __ansiMap);
}

void receive_message(string cl, string msg) {
    if(member_array(cl, ({"system", "prompt", "password"}))==-1) return;
    if(__Client) receive("<"+cl+">"+msg+"\n");
    else receive(format_ansi(msg));
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
    if(__Name && __Name != "") return __Name;
    else if(__AccountName) return __AccountName;
    else return "";
}
string query_CapName() {
    string tmp = query_name();
    return (tmp ? capitalize(tmp) : "");
}