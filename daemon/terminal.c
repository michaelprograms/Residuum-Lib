/*    /daemon/terminal.c
 *    from Nightmare IV
 *    daemon storing terminal information
 *    created by Pinkfish@Discworld
 *    rewritten for Nightmare by Descartes of Borg 930903
 */

#include <std.h>

inherit DAEMON;

#define ANSI(p) sprintf("%c["+(p)+"m", 27)
#define ESC(p) sprintf("%c"+(p), 27)

static mapping term_info;

void create() {
    daemon::create();
    set_no_clean(1);
    term_info = ([
        "unknown": ([
            "RESET":"", "STATUS":"", "WINDOW":"", "INITTERM":"", "CLEARLINE":"", "ENDTERM":"",
            "BOLD":"", "ITALIC":"", "UNDERLINE":"", "FLASH":"", "INVERSE":"", "STRIKE":"", "BOLD_OFF":"", "ITALIC_OFF":"", "UNDERLINE_OFF":"", "FLASH_OFF": "", "INVERSE_OFF":"", "STRIKE_OFF":"",
            "BLACK":"", "RED":"", "GREEN":"", "ORANGE":"", "YELLOW":"", "BLUE":"", "MAGENTA":"", "CYAN":"", "WHITE":"",
            "B_BLACK":"", "B_RED":"", "B_GREEN":"", "B_ORANGE":"", "B_YELLOW":"", "B_BLUE":"", "B_MAGENTA":"", "B_CYAN":"", "B_WHITE":"",
        ]),
        "bold": ([
            "RESET":ANSI("0;37;40"), "STATUS":"", "WINDOW":"", "INITTERM":"", "CLEARLINE":ESC("[L")+ESC("[G"), "ENDTERM":"",
            "BOLD":ANSI(1), "ITALIC":ANSI(1), "UNDERLINE":ANSI(1), "FLASH":ANSI(1), "INVERSE":ANSI(1), "STRIKE":ANSI(1), "BOLD_OFF":ANSI("0;37;40"), "ITALIC_OFF":ANSI("0;37;40"), "UNDERLINE_OFF":ANSI("0;37;40"), "FLASH_OFF": "", "INVERSE_OFF":ANSI("0;37;40"), "STRIKE_OFF":ANSI("0;37;40"),
            "BLACK":ANSI(1), "RED":"", "GREEN":"", "ORANGE":"", "YELLOW":ANSI(1), "BLUE": "", "MAGENTA":"", "CYAN":"", "WHITE": "",
            "B_BLACK":ANSI(1), "B_RED":ANSI(1), "B_GREEN":ANSI(1), "B_ORANGE":ANSI(1), "B_YELLOW":ANSI(1), "B_BLUE":ANSI(1), "B_MAGENTA":ANSI(1), "B_CYAN":ANSI(1),"B_WHITE": ANSI(1),
        ]),
        "ansi": ([
            "RESET":ANSI("0;37;40"), "STATUS":"", "WINDOW":"", "INITTERM":ESC("[H")+ESC("[2J"), "CLEARLINE":ESC("[L")+ESC("[G"), "ENDTERM":"",
            "BOLD":ANSI(1), "ITALIC":ANSI(3), "UNDERLINE":ANSI(4), "FLASH":ANSI(5), "INVERSE":ANSI(7), "STRIKE":ANSI(9), "BOLD_OFF":ANSI(22), "ITALIC_OFF":ANSI(23), "UNDERLINE_OFF":ANSI(24), "FLASH_OFF":ANSI(25), "INVERSE_OFF":ANSI(27), "STRIKE_OFF":ANSI(29),
            "BLACK":ANSI(30), "RED":ANSI(31), "GREEN":ANSI(32), "ORANGE":ANSI(33), "YELLOW":ANSI(1)+ANSI(33), "BLUE": ANSI(34), "MAGENTA":ANSI(35), "CYAN":ANSI(36), "WHITE": ANSI(37),
            "B_BLACK":ANSI(40), "B_RED":ANSI(41), "B_GREEN":ANSI(42), "B_ORANGE":ANSI(43), "B_YELLOW":ANSI(1)+ANSI(43), "B_BLUE":ANSI(44), "B_MAGENTA":ANSI(45), "B_CYAN":ANSI(46), "B_WHITE": ANSI(47),
        ]),
        "freedom": ([
            "RESET":ESC("G0"), "STATUS":"", "WINDOW":"", "INITTERM":"", "CLEARLINE":"\r", "ENDTERM":"",
            "BOLD":ESC("G@"), "ITALIC":"", "UNDERLINE":"", "FLASH":ESC("G2"), "INVERSE":"", "STRIKE":"", "BOLD_OFF":"", "ITALIC_OFF":"", "UNDERLINE_OFF":"", "FLASH_OFF":"", "INVERSE_OFF":"", "STRIKE_OFF":"",
            "BLACK":"", "RED":"", "GREEN":"", "ORANGE":"", "YELLOW":"", "BLUE":"", "MAGENTA":"", "CYAN":"", "WHITE":"",
            "B_BLACK":ESC("GD"), "B_RED":ESC("GD"), "B_GREEN": ESC("GD"), "B_ORANGE":ESC("G4"), "B_YELLOW":ESC("G4"), "B_BLUE":ESC("G4"), "B_MAGENTA":("G4"), "B_CYAN": ESC("GD"), "B_WHITE":ESC("G4"),
        ]),
        "ansi-status": ([
            "RESET":ANSI("0;37;40"), "STATUS":ESC("[23;24r")+ESC(8), "WINDOW":ESC(7)+ESC("[0;22r")+ESC("[22H\n"), "INITTERM":ESC("[H")+ESC("[J")+ESC("[23;24r")+ESC("23H\n"), "CLEARLINE":"\r", "ENDTERM":ESC("[0r")+ESC("[H")+ESC("[J"),
            "BOLD":ANSI(1), "ITALIC":ANSI(3), "UNDERLINE":ANSI(4), "FLASH":ANSI(5), "INVERSE":ANSI(7), "STRIKE":ANSI(9), "BOLD_OFF":ANSI(22), "ITALIC_OFF":ANSI(23), "UNDERLINE_OFF":ANSI(24), "FLASH_OFF":"", "INVERSE_OFF":ANSI(27), "STRIKE_OFF":ANSI(29),
            "BLACK":ANSI(30), "RED":ANSI(31), "GREEN":ANSI(32), "ORANGE":ANSI(33), "YELLOW":ANSI(33), "BLUE":ANSI(34), "MAGENTA":ANSI(35), "CYAN":ANSI(36), "WHITE":ANSI(37),
            "B_BLACK":ANSI(40), "B_RED":ANSI(41), "B_GREEN":ANSI(42), "B_ORANGE":ANSI(43), "B_YELLOW":ANSI(1)+ANSI(43), "B_BLUE":ANSI(44), "B_MAGENTA":ANSI(45), "B_CYAN":ANSI(46), "B_WHITE":ANSI(47),
        ]),
        "xterm": ([
            "RESET":ESC(">")+ESC("[1;3;4;5;61")+ESC("[?7h")+ESC("[m"), "STATUS":"", "WINDOW":"", "INITTERM":"", "CLEARLINE":"\r", "ENDTERM":"",
            "BOLD":ESC("[7m"), "ITALIC":"", "UNDERLINE":"", "FLASH":ESC("[5m$<2>"), "INVERSE":"", "STRIKE":"", "BOLD_OFF":"", "ITALIC_OFF":"", "UNDERLINE_OFF":"", "FLASH_OFF": "", "INVERSE_OFF":"", "STRIKE_OFF":"",
            "BLACK":ANSI(30), "RED":ANSI(31), "GREEN":ANSI(32), "ORANGE":ANSI(33), "YELLOW":ANSI(33), "BLUE":ANSI(34), "MAGENTA":ANSI(35), "CYAN":ANSI(36), "WHITE":ANSI(38),
            "B_BLACK":ANSI(40), "B_RED":ANSI(41), "B_GREEN":ANSI(42), "B_ORANGE":ANSI(43), "B_YELLOW":ANSI(43), "B_BLUE":ANSI(44), "B_MAGENTA":ANSI(45), "B_CYAN":ANSI(46), "B_WHITE":ANSI(47),
        ]),
    ]);
}

mapping query_term_info(string type) {
    return (term_info[type] ? term_info[type] : term_info["unknown"]);
}
string *query_terms() { return keys(term_info); }
int query_term_support(string str) {
    return (term_info[str] ? 1 : 0);
}

string no_colours(string str) {
    string *bits;
    int i;

    if(!str) return "";
    i = sizeof(bits = explode(str, "%^"));
    while(i--)
      if(term_info["unknown"][bits[i]]) bits[i] = "";
    return implode(bits, "");
}
