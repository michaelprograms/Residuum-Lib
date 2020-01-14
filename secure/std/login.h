#ifndef __LOGIN_H
#define __LOGIN_H

#define PROMPT_COLOR "%^ORANGE%^"
#define PROMPT_RESET "%^RESET%^"

static void logon();
static void idle();

static string format_ansi(string input);
static void receive_message(string cl, string msg);
static private void internal_remove();

static int register_client();
static void input_protocol(string str);

#endif __LOGIN_H