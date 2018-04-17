#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

#include "civetweb.h"
#include "delayeffect.h"
#include "distortioneffect.h"
#include "document.h"
#include "filesink.h"
#include "filesource.h"
#include "luaeffect.h"
#include "sampledata.h"
#include "source.h"
#include "tremoloeffect.h"
#include "webserver.h"

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
    mg_set_request_handler(context, "/delay/submit$", handle_delay_submit, this);
    mg_set_request_handler(context, "/tremolo/submit$", handle_tremolo_submit, this);
    mg_set_request_handler(context, "/chain/submit$", handle_chain_submit, this);
    mg_set_request_handler(context, "/lua/submit$", handle_lua_submit, this);
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
#if 0
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

#endif
    return 200;
}

int WebServer::handle_dist_submit(mg_connection *connection, void *user_data)
{
    mg_form_data_handler fdh = {};

    struct vars_t
    {
        std::string inputPath;
        std::string type;
        float gain;
        float gain2;
        float mix;
        float mix2;
        float threshold;
    } vars;

    fdh.user_data = &vars;

    // If a field is found, it is checked if it is a file. If it is, it's processed here, else it's processed in field_get.
    fdh.field_found = [](const char *key, const char *value, char *path, size_t pathlen, void *user_data) -> int {
        vars_t *vars = static_cast<vars_t *>(user_data);
        // check if field is a file
        if (value && *value)
        {
            // file, so save as tmp file
            //std::string tempPath = std::tmpnam(nullptr);
            std::string tempPath = value;
            snprintf(path, pathlen, "%s", tempPath.c_str());

            // store path
            if (std::string(key) == "input")
            {
                vars->inputPath = tempPath;
            }
            return MG_FORM_FIELD_STORAGE_STORE;
        }
        return MG_FORM_FIELD_STORAGE_GET;
    };

    fdh.field_store = [](const char *path, long long file_size, void *user_data) -> int {
        return 0;
    };

    // If a field is not a file, it's processed here.
    fdh.field_get = [](const char *key, const char *value, size_t valuelen, void *user_data) -> int {
        std::string name = std::string(key);
        vars_t *vars = static_cast<vars_t *>(user_data);

        std::string res = std::string(value);

        // Extract relevant information from 'value'
        res = res.substr(0, res.find('\r'));

        // Fill in the right fields of the vars.
        if (name == "type")
        {
            vars->type = res;
        }
        else if (name == "gain")
        {
            vars->gain = std::stof(res, nullptr) * 2;
        }
        else if (name == "gain2")
        {
            vars->gain2 = std::stof(res, nullptr) * 2;
        }
        else if (name == "mix")
        {
            vars->mix = std::stof(res, nullptr);
        }
        else if (name == "mix2")
        {
            vars->mix2 = std::stof(res, nullptr);
        }
        else if (name == "threshold")
        {
            vars->threshold = std::stof(res, nullptr);
        }

        return MG_FORM_FIELD_STORAGE_GET;
    };

    // Process the input from the form
    if (mg_handle_form_request(connection, &fdh) <= 0)
    {
        throw std::runtime_error("Error handling form request.");
    }

    // Adjust input if type is symmetric.
    if (vars.type == "symmetric")
    {
        vars.gain2 = vars.gain;
        vars.mix2 = vars.mix;
    }

    const std::string outputFileName = "output.wav";

    // Make source, effect and sink and connect them together to process the output.
    FileSource src(vars.inputPath);

    auto dist = std::make_shared<DistortionEffect>(vars.gain, vars.gain2, vars.mix, vars.mix2, vars.threshold);
    auto sink = std::make_shared<FileSink>(outputFileName, src.getSampleRate());

    src.connect(dist, 0);
    dist->connect(sink, 0);

    // process output
    while (src.generate_next());

    // Write output to appropriate file
    sink->write();

    // Send HTTP response containing the file to the user
    mg_send_file(connection, outputFileName.c_str());

    return 200;
}

int WebServer::handle_delay_submit(mg_connection *connection, void *user_data)
{
    mg_form_data_handler fdh = {};

    struct vars_t
    {
        std::string inputPath;
        std::string type;
        float delayTime;
        float decay;
    } vars;

    fdh.user_data = &vars;

    fdh.field_found = [](const char *key, const char *filename, char *path, size_t pathlen, void *user_data) -> int {
        vars_t *vars = static_cast<vars_t *>(user_data);
        // check if field is a file
        if (filename && *filename)
        {
            // file, so save as tmp file
            //std::string tempPath = std::tmpnam(nullptr);
            std::string tempPath = filename;
            snprintf(path, pathlen, "%s", tempPath.c_str());

            // store path
            if (std::string(key) == "input")
            {
                vars->inputPath = tempPath;
            }
            return MG_FORM_FIELD_STORAGE_STORE;
        }
        return MG_FORM_FIELD_STORAGE_GET;
    };

    fdh.field_store = [](const char *path, long long file_size, void *user_data) -> int {
        return 0;
    };

    fdh.field_get = [](const char *key, const char *value, size_t valuelen, void *user_data) -> int {
        std::string name = std::string(key);
        vars_t *vars = static_cast<vars_t *>(user_data);

        std::string res = std::string(value);

        res = res.substr(0, res.find('\r'));

        if (name == "type")
        {
            vars->type = res;
        }
        else if (name == "delay")
        {
            vars->delayTime = std::stof(res, nullptr);
        }
        else if (name == "decay")
        {
            vars->decay = std::stof(res, nullptr);
        }
        return MG_FORM_FIELD_STORAGE_GET;
    };

    if (mg_handle_form_request(connection, &fdh) <= 0)
    {
        throw std::runtime_error("Error handling form request.");
    }

    // Process vars from form
    float mainCoeff = 1.0;
    std::vector<unsigned int> delays;
    std::vector<float> coeffs;
    float decay = 1 - vars.decay;

    // Make FileSource
    FileSource src(vars.inputPath);

    unsigned int delaySamples = (unsigned int) (vars.delayTime * src.getSampleRate() / Source<float>::BLOCK_SIZE);

    if (vars.type == "fir")
    {
        delays = {delaySamples};
        coeffs = {decay};
    }
    else // iir
    {
        int i = 1;
        while (decay >= 0.05)
        {
            delays.push_back(delaySamples * i++);
            coeffs.push_back(decay);
            decay *= decay;
        }
    }

    std::string outputFileName = "output.wav";

    auto delay = std::make_shared<DelayEffect>(mainCoeff, delays, coeffs);
    auto sink = std::make_shared<FileSink>(outputFileName, src.getSampleRate());

    // connect
    src.connect(delay, 0);
    delay->connect(sink, 0);

    while (src.generate_next());

    sink->write();

    // Send HTTP response containing the file to the user
    mg_send_file(connection, outputFileName.c_str());

    return 200;
}

int WebServer::handle_tremolo_submit(mg_connection *connection, void *user_data)
{
    mg_form_data_handler fdh = {};

    struct vars_t
    {
        std::string inputPath;
        float depth;
        float rate;
    } vars;

    fdh.user_data = &vars;

    fdh.field_found = [](const char *key, const char *filename, char *path, size_t pathlen, void *user_data) -> int {
        vars_t *vars = static_cast<vars_t *>(user_data);
        // check if field is a file
        if (filename && *filename)
        {
            // file, so save as tmp file
            //std::string tempPath = std::tmpnam(nullptr);
            std::string tempPath = filename;
            snprintf(path, pathlen, "%s", tempPath.c_str());

            // store path
            if (std::string(key) == "input")
            {
                vars->inputPath = tempPath;
            }
            return MG_FORM_FIELD_STORAGE_STORE;
        }
        return MG_FORM_FIELD_STORAGE_GET;
    };

    fdh.field_store = [](const char *path, long long file_size, void *user_data) -> int {
        return 0;
    };

    fdh.field_get = [](const char *key, const char *value, size_t valuelen, void *user_data) -> int {
        std::string name = std::string(key);
        vars_t *vars = static_cast<vars_t *>(user_data);

        std::string res = std::string(value);

        res = res.substr(0, res.find('\r'));

        if (name == "depth")
        {
            vars->depth = std::stof(res, nullptr);
        }
        else if (name == "rate")
        {
            vars->rate = std::stof(res, nullptr);
        }
        return MG_FORM_FIELD_STORAGE_GET;
    };

    if (mg_handle_form_request(connection, &fdh) <= 0)
    {
        throw std::runtime_error("Error handling form request.");
    }

    // Make FileSource
    FileSource src(vars.inputPath);

    // Process vars from form
    unsigned int period = (unsigned int) ((1 / vars.rate) * src.getSampleRate());

    std::string outputFileName = "output.wav";

    // Make Effect and sink as shared_ptr
    auto tremolo = std::make_shared<TremoloEffect>(vars.depth, period);
    auto sink = std::make_shared<FileSink>(outputFileName, src.getSampleRate());

    // Connect source, effect and sink
    src.connect(tremolo, 0);
    tremolo->connect(sink, 0);

    while (src.generate_next());

    sink->write();

    mg_send_file(connection, outputFileName.c_str());

    return 200;
}

int WebServer::handle_chain_submit(mg_connection *connection, void *user_data)
{
    mg_form_data_handler fdh = {0};

    struct vars_t
    {
        std::string inputPath;
        const char *jsonString;
    } vars;

    fdh.user_data = &vars;

    fdh.field_found = [](const char *key, const char *filename, char *path, size_t pathlen, void *user_data) -> int {
        vars_t *vars = static_cast<vars_t *>(user_data);
        // check if field is a file
        if (filename && *filename)
        {
            // file, so save as tmp file
            //std::string tempPath = std::tmpnam(nullptr);
            std::string tempPath = filename;
            snprintf(path, pathlen, tempPath.c_str());

            // store path
            if (std::string(key) == "input")
            {
                vars->inputPath = tempPath;
            }
            return MG_FORM_FIELD_STORAGE_STORE;
        }
        return MG_FORM_FIELD_STORAGE_GET;
    };

    fdh.field_store = [](const char *path, long long file_size, void *user_data) -> int {
        return 0;
    };

    fdh.field_get = [](const char *key, const char *value, size_t valuelen, void *user_data) -> int {
        std::string name = std::string(key);
        vars_t *vars = static_cast<vars_t *>(user_data);

        std::string res = std::string(value);

        res = res.substr(0, res.find('\r'));

        if (name == "effect-info")
        {
            vars->jsonString = res.c_str();
        }

        return MG_FORM_FIELD_STORAGE_GET;
    };

    if (mg_handle_form_request(connection, &fdh) <= 0)
    {
        throw std::runtime_error("Error handling form request.");
    }

    // Parse
    rapidjson::Document chain;
    chain.Parse(vars.jsonString); // Contains array of JSON objects

    std::vector < std::shared_ptr < Processor < float, float >> > effects;

    // Make file source
    FileSource src(vars.inputPath);

    // Iterate through array
    for (auto &obj : chain.GetArray())
    {
        std::string effect(obj["effect"].GetString());
        if (effect == "distortion")
        {
            std::string type = "symmetric";
            if (obj.HasMember("type"))
            {
                type = std::string(obj["type"].GetString());
            }

            float gain1 = stof(std::string(obj["gain1"].GetString()));
            float gain2 = (type == "asymmetric") ? stof(std::string(obj["gain2"].GetString())) : gain1;
            float mix1 = stof(std::string(obj["mix1"].GetString()));
            float mix2 = (type == "asymmetric") ? stof(std::string(obj["mix2"].GetString())) : mix1;
            float threshold = stof(std::string(obj["threshold"].GetString()));

            // Make effect and add to vector
            auto dist = std::make_shared<DistortionEffect>(gain1, gain2, mix1, mix2, threshold);
            effects.push_back(dist);
        }
        else if (effect == "delay")
        {
            std::string type = "fir";
            if (obj.HasMember("type"))
            {
                std::string tmp(obj["type"].GetString());
                type = tmp;
            }
            float delayTime = stof(std::string(obj["delay"].GetString()));
            float decayCoeff = stof(std::string(obj["decay"].GetString()));

            // Process vars
            float mainCoeff = 1.0;
            std::vector<unsigned int> delays;
            std::vector<float> coeffs;
            float decay = 1 - decayCoeff;

            unsigned int delaySamples = (unsigned int) (delayTime * src.getSampleRate() / Source<float>::BLOCK_SIZE);

            if (type == "fir")
            {
                delays = {delaySamples};
                coeffs = {decay};
            }
            else // iir
            {
                int i = 1;
                while (decay >= 0.05)
                {
                    delays.push_back(delaySamples * i++);
                    coeffs.push_back(decay);
                    decay *= decay;
                }
            }

            // Create effect and add to vector
            auto delay = std::make_shared<DelayEffect>(mainCoeff, delays, coeffs);
            effects.push_back(delay);
        }
        else if (effect == "tremolo")
        {
            float depth = stof(std::string(obj["depth"].GetString()));
            float rate = stof(std::string(obj["rate"].GetString()));

            unsigned int period = (unsigned int) ((1 / rate) * src.getSampleRate());

            // Create effect and add to vector
            auto tremolo = std::make_shared<TremoloEffect>(depth, period);
            effects.push_back(tremolo);
        }
        else if (effect == "convolution")
        {
            // TODO
            std::cout << "Later alligator" << std::endl;
        }
    }

    std::string outputFileName = "output.wav";

    // Make file sink
    auto sink = std::make_shared<FileSink>(outputFileName, src.getSampleRate());

    if (effects.empty())
    {
        src.connect(sink, 0);
    }
    else
    {
        std::shared_ptr <Processor<float, float>> lastEffect;
        for (auto it = effects.begin(); it != effects.end(); ++it)
        {
            if (it == effects.begin())
            {
                lastEffect = *it;
                src.connect(lastEffect, 0);
            }
            else
            {
                lastEffect->connect(*it, 0);
                lastEffect = *it;
            }
        }
        lastEffect->connect(sink, 0);
    }

    while (src.generate_next());

    sink->write();

    mg_send_file(connection, outputFileName.c_str());

    return 200;
}

int WebServer::handle_lua_submit(mg_connection *connection, void *user_data)
{
    mg_form_data_handler fdh = {};

    struct vars_t
    {
        std::string inputPath;
        std::string scriptPath;
    } vars;

    fdh.user_data = &vars;

    fdh.field_found = [](const char *key, const char *filename, char *path, size_t pathlen, void *user_data) -> int {
        vars_t *vars = static_cast<vars_t *>(user_data);
        // check if field is a file
        if (filename && *filename)
        {
            // file, so save as tmp file
            //std::string tempPath = std::tmpnam(nullptr);
            std::string tempPath = filename;
            snprintf(path, pathlen, "%s", tempPath.c_str());

            // store path
            if (std::string(key) == "inputfile")
            {
                vars->inputPath = tempPath;
            }
            else if (std::string(key) == "scriptfile")
            {
                vars->scriptPath = tempPath;
            }
            return MG_FORM_FIELD_STORAGE_STORE;
        }
        // Not a file
        return MG_FORM_FIELD_STORAGE_GET;
    };

    fdh.field_store = [](const char *path, long long file_size, void *user_data) -> int {
        return 0;
    };

    if (mg_handle_form_request(connection, &fdh) <= 0)
    {
        throw std::runtime_error("Error handling form request.");
    }

    std::string outputFileName = "output.wav";
    // Make FileSource
    FileSource src(vars.inputPath);

    // Make LuaEffect
    auto script = std::make_shared<LuaEffect>(vars.scriptPath);

    // Make FileSink
    auto sink = std::make_shared<FileSink>(outputFileName, src.getSampleRate());

    // Connect
    src.connect(script, 0);
    script->connect(sink, 0);

    while (src.generate_next());

    sink->write();

    mg_send_file(connection, outputFileName.c_str());



    return 200;
}
