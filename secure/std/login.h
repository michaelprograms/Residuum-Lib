#ifndef __LOGIN_H
#define __LOGIN_H

#define PROMPT_COLOR "%^ORANGE%^"
#define PROMPT_RESET "%^RESET%^"

static void logon();
static void idle();
static void get_name(string str);
static void continue_login();
static void new_user(string str);
static void get_password(string str);
static private int boot_copy();
static void disconnect_copy(string str);
static void choose_password(string str);
static void confirm_password(string str2, string str1);
static void choose_cap_name(string str);
static void prompt_email();
static void enter_email(string str);
static void prompt_gender();
static void enter_gender(string str);

static private void internal_remove();
static private int locked_access();
static private int valid_site(string ip);
static private int check_password(string str);
static private void exec_user();

static string format_ansi(string input);
static void receive_message(string cl, string msg);
static int register_client();
static void input_protocol(string str);
string query_name();
string query_CapName();

#endif __LOGIN_H