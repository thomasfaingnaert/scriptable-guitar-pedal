#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "filtereffect.h"
#include "sampledata.h"
#include "webserver.h"
#include "../include/webserver.h"
#include "../include/civetweb/civetweb.h"
#include "../include/sampledata.h"
#include "../include/distortioneffect.h"

WebServer::WebServer(unsigned int port)
{
    // create options array
    std::string portStr = std::to_string(port);
    const char *options[] =
            {
                    "listening_ports",
                    portStr.c_str(),
                    "document_root",
                    "public/",
                    0
            };

    // start server
    context = mg_start(nullptr, nullptr, options);

    if (!context)
    {
        throw std::runtime_error("Could not start HTTP server");
    }

    // register handlers
    mg_set_request_handler(context, "/exit$", handle_exit, this);
    mg_set_request_handler(context, "/conv/submit$", handle_conv_submit, this);
    mg_set_request_handler(context, "/dist/submit$", handle_dist_submit, this);
}

WebServer::~WebServer()
{
    mg_stop(context);
}

bool WebServer::isRunning() const
{
    return !exit;
}

void WebServer::render(mg_connection *connection, const std::string &data, const std::string &contentType)
{
    mg_printf(connection,
              "HTTP/1.1 200 OK\r\n"
                      "Content-Length: %u\r\n"
                      "Content-Type: %s\r\n"
                      "Connection: close\r\n\r\n",
              data.length(), contentType.c_str());
    mg_write(connection, data.c_str(), data.length());
}

void WebServer::render_text(mg_connection *connection, const std::string &text)
{
    render(connection, text, "text/plain");
}

void WebServer::render_html(mg_connection *connection, const std::string &html)
{
    render(connection, html, "text/html");
}

int WebServer::handle_exit(mg_connection *connection, void *user_data)
{
    WebServer *ws = static_cast<WebServer *>(user_data);
    render_text(connection, "Shutting down the server...");
    ws->exit = true;
    return 200;
}

int WebServer::handle_conv_submit(mg_connection *connection, void *user_data)
{
    mg_form_data_handler fdh = {0};

    // used to save paths
    struct paths_t
    {
        std::string inputPath;
        std::string impulsePath;
    } paths;

    fdh.user_data = &paths;

    fdh.field_found = [](const char *key, const char *filename, char *path, size_t pathlen, void *user_data) -> int
    {
        // check if field is a file
        if (filename && *filename)
        {
            // file, so save as tmp file
            //std::string tempPath = std::tmpnam(nullptr);
            std::string tempPath = filename;
            snprintf(path, pathlen, tempPath.c_str());

            // store path
            paths_t *paths = static_cast<paths_t *>(user_data);
            if (std::string(key) == "input")
            {
                paths->inputPath = tempPath;
            }
            else if (std::string(key) == "impulse-response")
            {
                paths->impulsePath = tempPath;
            }

            return MG_FORM_FIELD_STORAGE_STORE;
        }

        // not a file
        return MG_FORM_FIELD_STORAGE_GET;
    };

    fdh.field_store = [](const char *path, long long file_size, void *user_data) -> int
    {
        return 0;
    };

    if (mg_handle_form_request(connection, &fdh) <= 0)
    {
        throw std::runtime_error("Error handling form request.");
    }

    SampleData input(paths.inputPath);
    SampleData filter(paths.impulsePath);

    std::vector <Sample> inputSamples = input.getSamples()[0];
    std::vector <Sample> filterSamples = filter.getSamples()[0];

    std::vector <Sample> outputSamples;

    FilterEffect fe;
    fe.setImpulseResponse(filterSamples);

    const auto start = std::chrono::high_resolution_clock::now();

    unsigned int pos = 0;

    while (pos + fe.getBlockSize() < inputSamples.size())
    {
        std::vector <Sample> block(inputSamples.begin() + pos, inputSamples.begin() + pos + fe.getBlockSize());
        fe.addInputBlock(block);
        std::vector <Sample> output = fe.getOutputBlock();
        outputSamples.insert(outputSamples.end(), output.begin(), output.end());
        pos += fe.getBlockSize();
    }

    Sample max = outputSamples[0];
    for (Sample sample : outputSamples)
    {
        max = std::max(std::abs(sample), max);
    }

    for (Sample &sample : outputSamples)
    {
        sample /= max;
    }

    const auto end = std::chrono::high_resolution_clock::now();

    std::cout << "processing took " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
              << " ns (" <<
              std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms)." << std::endl;


    SampleData out({outputSamples}, input.getSampleRate());
    //std::string outputPath = std::tmpnam(nullptr);
    std::string outputPath = "output";
    outputPath += ".wav";
    out.save(outputPath);

    mg_send_file(connection, outputPath.c_str());

    return 200;
}

int WebServer::handle_dist_submit(mg_connection *connection, void *user_data)
{
    mg_form_data_handler fdh = {};

    struct vars_t
    {
        std::string inputPath;
        std::string type;
        float gain1;
        float gain2;
        float mix;
        float mix2;
    } vars;

    fdh.user_data = &vars;

    fdh.field_found = [](const char *key, const char *filename, char *path, size_t pathlen, void *user_data) -> int
    {
        // check if field is a file
        if (filename && *filename)
        {
            // file, so save as tmp file
            //std::string tempPath = std::tmpnam(nullptr);
            std::string tempPath = filename;
            snprintf(path, pathlen, tempPath.c_str());

            // store path
            vars_t *vars = static_cast<vars_t *>(user_data);
            if (std::string(key) == "input")
            {
                vars->inputPath = tempPath;
            }

            return MG_FORM_FIELD_STORAGE_STORE;
        }

        // not a file
        return MG_FORM_FIELD_STORAGE_GET;
    };

    fdh.field_store = [](const char *path, long long file_size, void *user_data) -> int
    {
        return 0;
    };

    fdh.field_get = [](const char *key, const char *value, size_t valuelen, void *user_data) -> int
    {
        std::string name = std::string(key);

        vars_t *vars = static_cast<vars_t *>(user_data);

        if (name == "type")
        {
            vars->type = std::string(value);
        }
        else if (name == "gain")
        {
            vars->gain1 = (float) atof(value);
        }
        else if (name == "gain2")
        {
            vars->gain2 = (float) atof(value);
        }
        else if (name == "mix")
        {
            vars->mix = (float) atof(value);
        }
        else if (name == "mix2")
        {
            vars->mix2 = (float) atof(value);
        }

        return MG_FORM_FIELD_STORAGE_GET;
    };

    if (mg_handle_form_request(connection, &fdh
    ) <= 0)
    {
        throw std::runtime_error("Error handling form request.");
    }

    SampleData input(vars.inputPath);

    std::vector <Sample> inputSamples = input.getSamples()[0];
    std::vector <Sample> outputSamples;

    DistortionEffect dist(vars.gain1, vars.mix);
    if (vars.type == "Asymmetric")
    {
        dist.setNegativeGain(vars.gain2);
        dist.setNegativeMix(vars.mix2);
    }

    auto in = std::make_shared < std::vector < float > > (inputSamples);
    std::vector < std::shared_ptr < std::vector < float > > > vec;
    vec.push_back(in);
    outputSamples = *dist.process(vec);

    SampleData out({outputSamples}, input.getSampleRate());
    //std::string outputPath = std::tmpnam(nullptr);
    std::string outputPath = "output";
    outputPath += ".wav";
    out.save(outputPath);

    mg_send_file(connection, outputPath.c_str());

    return 200;

}

