/*    /daemon/postal/localpost.c
 *    from Nightmare IV
 *    filter for mails coming in from outside and inside
 *    created by Descartes of Borg 940503
 */

#include <std.h>
#include <cfg.h>
#include <security.h>
#include <objects.h>
#include <daemons.h>
#include "localpost.h"

inherit DAEMON;

static private mapping __MudGroups;

void create() {
    daemon::create();
    set_no_clean(1);
    load_groups();
}

void load_groups() {
    mapping tmp;
    string *lines, *parts;
    string a, b;
    int i, maxi, x, j;

    maxi = sizeof(lines = read_database(CFG_ALIASES));
    for(i=0, tmp = ([]); i<maxi; i++) {
        if(sscanf(lines[i], "%s: %s", a, b) != 2) continue;
        if(b[0..0] == "|") {
            x = sizeof(parts = explode(b[1..strlen(b)-1], ";"));
            if(a == "include") {
                switch(sizeof(parts)) {
                    case 2:
                        tmp += (mapping)call_other(parts[0],parts[1]);
                        break;
                    case 3:
                        tmp+=(mapping)call_other(parts[0],parts[1],
                          parts[2]);
                        break;
                    default:
                        tmp += (mapping)call_other(parts[0], parts[1],
                          parts[2], parts[3]);
                        break;
                }
            }
            else switch(sizeof(parts)) {
                case 2:
                    tmp[a] = (string *)call_other(parts[0],parts[1]);
                    break;
                case 3:
                     tmp[a] = (string *)call_other(parts[0],parts[1],
                       parts[2]);
                     break;
                default:
                     tmp[a] = (string *)call_other(parts[0],parts[1],
                       parts[2], parts[3]);
            }
	  }
        else {
            x = sizeof(parts = explode(b, " "));
            if(a != "exclude") tmp[a] = ({});
            for(j=0; j<x; j++) {
                if(a == "exclude" && tmp[parts[j]])
                  map_delete(tmp, parts[j]);
                else if(a == "exclude") continue;
                else if(tmp[parts[j]]) tmp[a]+=tmp[parts[j]];
                else tmp[a] += ({ parts[j] });
            }
        }
    }
    i = sizeof(parts = keys(tmp));
    __MudGroups = ([]);
    while(i--) __MudGroups[lower_case(parts[i])] = tmp[parts[i]];
}

varargs string *send_post(mapping borg, mixed who) {
    mapping remote_mail, grps;
    string *rejects, *tmp, *tmpwho, *cles;
    string msg, a, b, tmpstr;
    int i, j, x;

    if(!who) who = distinct_array(borg["to"] + borg["cc"]);
    tmpstr = base_name(previous_object(0));
    if(tmpstr != OB_POSTAL && tmpstr != REMOTEPOST_D && tmpstr != FOLDERS_D)
      return (pointerp(who) ? who : ({ who }));
    rejects = ({});
    grps = (mapping)OPTIONS_D->query_groups(borg["from"]);
    borg["id"] = (string)LETTERS_D->create_letter(borg["message"]);
    msg = borg["message"];
    map_delete(borg, "message");
    remote_mail = ([]);
    if(pointerp(who)) {
        i = sizeof(who);
        tmpwho = ({});
        x = (int)OPTIONS_D->query_option(borg["from"], "metoo");
        while(i--) {
            if(pointerp(who[i] = map_groups(who[i], grps, borg["from"], x)))
              tmpwho += who[i];
            else tmpwho += ({ who[i] });
        }
        i = sizeof(who = tmpwho);
        while(i--) {
            if(sscanf(who[i], "%s@%s", a, b) == 2) {
                if(!remote_mail[b]) remote_mail[b] = ({});
                remote_mail[b] += ({});
	    }
            else if(!player_exists(who[i])) rejects += ({ who[i] });
            else FOLDERS_D->add_post(who[i], "new", borg);
        }
        j = sizeof(cles = keys(remote_mail));
        while(j--)
          if(!((int)REMOTEPOST_D->send_post(borg+(["message":msg]), cles[j])))
            rejects += remote_mail[cles[j]];
        return rejects;
    }
    else if(pointerp(tmp = __MudGroups[who]) && (i = sizeof(tmp))) {
        while(i--) {
            if(sscanf(tmp[i], "%s@%s", a, b) == 2) {
                if(!remote_mail[b]) remote_mail[b] = ({});
                remote_mail[b] += ({ tmp[i] });
	      }
            else if(!player_exists(tmp[i])) rejects += ({ tmp[i] });
            else FOLDERS_D->add_post(tmp[i], "new", borg);
	  }
        i = sizeof(cles = keys(remote_mail));
        while(i--)
          if(!((int)REMOTEPOST_D->send_post(borg+(["message":msg]), cles[i])))
            rejects += remote_mail[cles[i]];
        return rejects;
    }
    else if(pointerp(tmp = (string *)OPTIONS_D->query_group(who)) &&
      (i = sizeof(tmp))) {
        while(i--) {
            if(sscanf(tmp[i], "%s@%s", a, b) == 2) {
                if(!remote_mail[b]) remote_mail[b] = ({});
                remote_mail[b] += ({ tmp[i] });
	      }
            else if(!player_exists(tmp[i])) rejects += ({ tmp[i] });
            else FOLDERS_D->add_post(tmp[i], "new", borg);
	  }
        i = sizeof(cles = keys(remote_mail));
        while(i--)
          if(!((int)REMOTEPOST_D->send_post(borg+(["message":msg]),cles[i])))
            rejects += remote_mail[cles[i]];
        return rejects;
    }
    else {
        if(sscanf(who, "%s@%s", a, b) == 2) {
            if(!((int)REMOTEPOST_D->send_post(borg+(["message":msg]),b)))
              return ({ who });
	  }
        else if(!player_exists(who)) return ({ who });
        else FOLDERS_D->add_post(who, "new", borg);
        return ({});
    }
}

static private mixed map_groups(string str, mapping grps, string who, int x) {
    if(__MudGroups[str]) return (string *)__MudGroups[str] - ({ who });
    else if(grps[str] && sizeof(grps[str]))
      return (string *)grps[str] - ({ who });
    else return str;
}

string *query_mud_group(string str) { return copy(__MudGroups[str]); }

mapping query_mud_groups() { return copy(__MudGroups); }
