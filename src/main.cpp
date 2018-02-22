#include <cstdlib>
#include <iostream>

#include "NE10.h"
#include "civetweb.h"
#include "i2c.h"

int handler(mg_connection *connection, void *ignored)
{
    std::string message = "Test";

    mg_printf(connection,
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: %u\r\n"
            "Content-Type: text/plain\r\n"
            "Connection: close\r\n\r\n",
            message.length());
    mg_write(connection, message.c_str(), message.length());

    return 200;
}

int main(int argc, char *argv[])
{
    /* Test Ne10 */
    if (ne10_init() != NE10_OK)
    {
        std::cout << "Could not initialise Ne10.\n";
        return EXIT_FAILURE;
    }

    /* Test I2C */
    I2C i2c(2);

    i2c.set_device(0x47);


    /* Test CivetWeb */
    const char *options[] =
    {
        "listening_ports",
        "8888",
        0
    };

    mg_init_library(0);
    mg_context *context = mg_start(nullptr, nullptr, options);

    if (!context)
    {
        std::cout << "Could not start the HTTP server.\n";
        return EXIT_FAILURE;
    }

    mg_set_request_handler(context, "/", handler, nullptr);

    /* temporary infinite loop */
    std::cout << "Server running on port 8888.\n";
    // while ( true ) ;

    mg_stop(context);
    mg_exit_library();

    return EXIT_SUCCESS;
}
