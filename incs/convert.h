#ifndef _CONVERT_
# define _CONVERT_

# include <ctype.h>

# include "ping.h"
# include "parser.h"

bool is_argu_hex (char * str);
int hex_to_int (char h);

bool timelog_enqueue (timelog_t ** list, long time);

bool convert (ping_t * ping, PARSER_INFO * parserinfo);

#endif
