#include "../include/mongoose.h"
#include "../include/obhnothing_includes.h"

/*
 * -----------------------
 * RESTfulness in C.......
 * ***********************
 * Oskar Bahner Hansen....
 * cph-oh82@cphbusiness.dk
 * 2024-10-31.............
 * -----------------------
 */

int main(int argc, char *argv[])
{

    const enum Event events[] = {APP_PASSIVE_OPEN, APP_SEND, RCV_SYN_ACK}; // =>  "ESTABLISHED";

    enum State res = get_TCP_state(3, events);

    printf("res: %d\n", res);

    return 0;
}







































