#include <std.h>

inherit DAEMON;

varargs string left_label(string label, int len) {
    if(!len) len = 21;
    return "%^BOLD%^MAGENTA%^ "+rightpad(label, len, " ") + "%^RESET%^";
}

string left_filled_bar(int percent, string leftLabel, string midLabel, string rightLabel) {
    int n = (percent+1)/2;
    int leftLabelN = 0, rightLabelN = 0, i;
    string bar;

    if (n > 50) n = 50; if (n < 0) n = 0;

    leftLabelN = sizeof(midLabel)/2;
    rightLabelN = sizeof(midLabel)/2+sizeof(midLabel)%2;

    bar = rightpad(leftLabel, 25 - leftLabelN, "-") + midLabel + leftpad(rightLabel, 25-rightLabelN, "-");
    for(i=0;i<n && i<sizeof(leftLabel) && i<(25-leftLabelN);i++) bar[i..i] = capitalize(bar[i..i]);
    for(i=sizeof(leftLabel);i<n && i<(25-leftLabelN);i++) bar[i..i] = "=";
    for(i=(25-leftLabelN);i<n && i<25+rightLabelN && i<(50-sizeof(rightLabel));i++) bar[i..i] = capitalize(bar[i..i]);
    for(i=25+rightLabelN;i<n && i<(50-sizeof(rightLabel));i++) bar[i..i] = "=";
    for(i=(50-sizeof(rightLabel));i<n && i<sizeof(bar);i++) bar[i..i] = capitalize(bar[i..i]);

    return "%^RESET%^CYAN%^BOLD%^|" + bar[0..n-1] + "%^RESET%^BLUE%^" + bar[n..49] + "%^CYAN%^BOLD%^|%^RESET%^";
}

string middle_filled_bar(int percent, string leftLabel, string midLabel, string rightLabel) {
    int n = to_int(percent > 0 ? (percent+1)/2 : percent < 0 ? (percent-1)/2 : 0);
    int leftN = 0, rightN = 0, leftLabelN = 0, rightLabelN = 0, i;
    string bar;

    if (n > 25) n = 25; if (n < -25) n = -25;
    if (n < 0) leftN = -n; if (n > 0) rightN = n;

    leftLabelN = sizeof(midLabel)/2;
    rightLabelN = sizeof(midLabel)/2+sizeof(midLabel)%2;

    bar = rightpad(leftLabel, 25 - leftLabelN, "-") + midLabel + leftpad(rightLabel, 25 - rightLabelN, "-");
    for(i=leftLabelN;i<leftN && i < 25 - sizeof(leftLabel);i++) bar[24-i..24-i] = "=";
    for(i=rightLabelN;i<rightN && i < 25 - sizeof(rightLabel);i++) bar[25+i..25+i] = "=";
    if(leftN > 0) for(i=25-leftN;i<25;i++) bar[i..i] = capitalize(bar[i..i]);
    else if(rightN > 0) for(i=25;i<25+rightN;i++) bar[i..i] = capitalize(bar[i..i]);

    return "%^RESET%^CYAN%^BOLD%^|%^RESET%^BLUE%^" + bar[0..24-leftN] + "%^CYAN%^BOLD%^" + bar[25-leftN..24+rightN] + "%^RESET%^BLUE%^" + bar[25+rightN..49] + "%^CYAN%^BOLD%^|%^RESET%^";
}

/* ---------------------------------------------------------------------- */

int verify_value(int value) { return value ? value : 0; }