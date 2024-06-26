#include <std.h>

inherit DAEMON;

int cmd_cache(string arg) {
    if(!archp(previous_object())) return 0;
    write(cache_stats());
    return 1;
}

int help() {
  write( @EndText
Syntax: cache
Effect: Gets the cache stats from the driver, including the hit rate
See man: cache_stats
See also: callouts, mstatus, netstat, opcprof, fdinfo, dumpallobj
EndText
  );
  return 1;
}
