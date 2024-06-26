//      /adm/simul_efun/mud_info.c
//      from the Nightmare Mudlib
//      gives information simul_efuns about the mud
//      created by Descartes of Borg 930814

#include <config.h>

string mud_name() { return MUD_NAME; }

#ifdef MUDOS_VERSION
string version() { return MUDOS_VERSION; }
#endif // MUDOS_VERSION

#ifdef __VERSION__
string version() { return explode(__VERSION__, " (git-")[0]; }
#endif // __VERSION__

#ifdef MUDOS_ARCH
string architecture() { return MUDOS_ARCH; }
#endif // MUDOS_ARCH

#ifdef __ARCH__
string architecture() { return __ARCH__; }
#endif // __ARCH__

string mudlib() { return MUDLIB; }

string mudlib_version() { return MUDLIB_VERSION; }

int query_host_port() { return __PORT__; }
