#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "civetweb.h"

volatile bool exit_server = false;

void render_html(mg_connection *connection, const std::string& html)
{
    mg_printf(connection,
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: %u\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n\r\n",
            html.length());
    mg_write(connection, html.c_str(), html.length());
}

int handler(mg_connection *connection, void *user_data)
{
    std::string html =
        "<!DOCTYPE html>"
        "<html><head><meta charset=\"utf-8\"><title>Upload Samples</title></head>"
        "<body><form action=\"/samples\" method=\"post\" enctype=\"multipart/form-data\">"
        "<input type=\"file\" name=\"samples\"><br>"
        "<input type=\"submit\" value=\"Process Samples\">"
        "</form></body></html>";

    render_html(connection, html);

    return 200;
}

int exit_handler(mg_connection *connection, void *user_data)
{
    std::string message = "Shutting down server...";

    render_html(connection, message);
    exit_server = true;

    return 200;
}

int sample_handler(mg_connection *connection, void *user_data)
{
    mg_form_data_handler fdh = { 0 };

    fdh.user_data = connection;

    fdh.field_found = [] (const char *key, const char *filename, char *path, size_t pathlen, void *user_data) -> int
    {
        // check if field is a file
        if (filename && *filename)
        {
            // yes, so save file in /tmp
            snprintf(path, pathlen, "/tmp/%s", filename);
            return MG_FORM_FIELD_STORAGE_STORE;
        }

        // not a file
        return MG_FORM_FIELD_STORAGE_GET;
    };

    fdh.field_store = [] (const char *path, long long file_size, void *user_data) -> int
    {
        mg_connection *connection = (mg_connection*) user_data;
        std::ifstream file(path);
        std::vector<double> samples;

        std::copy(std::istream_iterator<double>(file), std::istream_iterator<double>(), std::back_inserter(samples));

        std::string msg = "Loaded " + std::to_string(samples.size()) + " samples.";
        render_html(connection, msg);

        return 0;
    };


    if (mg_handle_form_request(connection, &fdh) <= 0)
    {
        std::cout << "Error handling form request." << std::endl;
        return -1;
    }

    return 200;
}

int main(int argc, char *argv[])
{
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

    mg_set_request_handler(context, "/$", handler, nullptr);
    mg_set_request_handler(context, "/exit$", exit_handler, nullptr);
    mg_set_request_handler(context, "/samples$", sample_handler, nullptr);

    std::cout << "Server running on port 8888." << std::endl;
    while (!exit_server) ;

    mg_stop(context);
    mg_exit_library();

    return EXIT_SUCCESS;
}

