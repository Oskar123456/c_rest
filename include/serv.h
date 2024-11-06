#include "incl.h"
// We use the same event handler function for HTTP and HTTPS
// connections fn_data is NULL for plain HTTP, and non-NULL for HTTPS
static void fn(struct mg_connection *c, int ev, void *ev_data);
int serv(void);

