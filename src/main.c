#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "server.h"

int main() {

    struct Server* server = create_server("0.0.0.0", 8080, 10);
    start_server(server);


    return EXIT_SUCCESS;
}