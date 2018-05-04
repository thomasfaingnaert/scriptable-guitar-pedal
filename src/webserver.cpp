#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <ios>
#include <iostream>
#include <memory>
#include <pthread.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>

#include "alsadevice.h"
#include "civetweb.h"
#include "delayeffect.h"
#include "distortioneffect.h"
#include "document.h"
#include "filesink.h"
#include "filesource.h"
#include "filtereffect.h"
#include "luaeffect.h"
#include "prettywriter.h"
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

    // Configure param
    thread_params.changed = false;
    thread_params.firstSink = nullptr;
    thread_params.lastSource = nullptr;
    thread_params.canChange = PTHREAD_COND_INITIALIZER;
    thread_params.mutex = PTHREAD_MUTEX_INITIALIZER;

    if (pthread_create(&alsaThread, nullptr, alsa_thread, &thread_params) != 0)
    {
        throw std::runtime_error("Could not create alsa_thread");
    }

    sched_param param;
    param.sched_priority = 99;
    if (pthread_setschedparam(alsaThread, SCHED_FIFO, &param) != 0)
    {
        throw std::runtime_error("Something went wrong setting the priority.");
    }

    // register handlers
    mg_set_request_handler(context, "/exit$", handle_exit, this);
    mg_set_request_handler(context, "/conv/submit$", handle_conv_submit, this);
    mg_set_request_handler(context, "/dist/submit$", handle_dist_submit, this);
    mg_set_request_handler(context, "/delay/submit$", handle_delay_submit, this);
    mg_set_request_handler(context, "/tremolo/submit$", handle_tremolo_submit, this);
    mg_set_request_handler(context, "/chain/submit$", handle_chain_submit, this);
    mg_set_request_handler(context, "/lua/submit$", handle_lua_submit, this);
    mg_set_request_handler(context, "/chain/save$", handle_chain_save, this);
    mg_set_request_handler(context, "/chain/load$", handle_chain_load, this);
    mg_set_request_handler(context, "/chain/load/active$", handle_chain_load_active, this);
    mg_set_request_handler(context, "/conv/upload$", handle_ir_upload, this);
    mg_set_request_handler(context, "/conv/list$", handle_ir_list, this);
    mg_set_request_handler(context, "/alsa/submit$", handle_alsa_submit, this);
}

WebServer::~WebServer()
{
    mg_stop(context);
}

// Static fields
std::string WebServer::jsonChain;

std::shared_ptr<AlsaDevice> WebServer::alsaDevice = std::make_shared<AlsaDevice>(0, 0, 48000, 2, 2, 1024, 1024, 2, 2);
WebServer::thread_param WebServer::thread_params;

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

void WebServer::render_json(mg_connection *connection, const std::string &json)
{
    render(connection, json, "application/json");
}

void WebServer::render_redirect(mg_connection *connection, const std::string &url)
{
    mg_printf(connection,
              "HTTP/1.1 301 Moved Permanently\r\n"
              "Location: %s\r\n\r\n",
              url.c_str());
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
#if 1
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
        std::string name(key);

        paths_t *vars = static_cast<paths_t *>(user_data);

        if (name == "ir-list")
        {
            vars->impulsePath = std::string(value);
        }
        return MG_FORM_FIELD_STORAGE_GET;
    };

    if (mg_handle_form_request(connection, &fdh) <= 0)
    {
        throw std::runtime_error("Error handling form request.");
    }

    SampleData input(paths.inputPath);

    std::string impulseResponse = "impulse-responses/" + paths.impulsePath + ".wav";
    SampleData filter(impulseResponse);

    std::vector<Sample> inputSamples = input.getSamples()[0];
    std::vector<Sample> filterSamples = filter.getSamples()[0];

    std::vector<Sample> outputSamples;

    FileSource src(paths.inputPath);

    std::string outputPath = "output.wav";
    auto fe = std::make_shared<FilterEffect>(filterSamples);
    auto sink = std::make_shared<FileSink>(outputPath, 44100);

    src.connect(fe);
    fe->connect(sink);

    while (src.generate_next());

    sink->write();

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
    fdh.field_found = [](const char *key, const char *value, char *path, size_t pathlen, void *user_data) -> int
    {
        vars_t *vars = static_cast<vars_t *>(user_data);
        // check if field is a file
        if (value && *value)
        {
            snprintf(path, pathlen, "%s", value);

            // store path
            if (std::string(key) == "input")
            {
                vars->inputPath = value;
            }
            return MG_FORM_FIELD_STORAGE_STORE;
        }
        return MG_FORM_FIELD_STORAGE_GET;
    };

    fdh.field_store = [](const char *path, long long file_size, void *user_data) -> int
    {
        return 0;
    };

    // If a field is not a file, it's processed here.
    fdh.field_get = [](const char *key, const char *value, size_t valuelen, void *user_data) -> int
    {
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

    src.connect(dist);
    dist->connect(sink);

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

    fdh.field_found = [](const char *key, const char *filename, char *path, size_t pathlen, void *user_data) -> int
    {
        vars_t *vars = static_cast<vars_t *>(user_data);
        // check if field is a file
        if (filename && *filename)
        {
            snprintf(path, pathlen, "%s", filename);

            // store path
            if (std::string(key) == "input")
            {
                vars->inputPath = filename;
            }
            return MG_FORM_FIELD_STORAGE_STORE;
        }
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

    unsigned int delaySamples = (unsigned int) (vars.delayTime * src.getSampleRate());

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
    src.connect(delay);
    delay->connect(sink);

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

    fdh.field_found = [](const char *key, const char *filename, char *path, size_t pathlen, void *user_data) -> int
    {
        vars_t *vars = static_cast<vars_t *>(user_data);
        // check if field is a file
        if (filename && *filename)
        {
            snprintf(path, pathlen, "%s", filename);

            // store path
            if (std::string(key) == "input")
            {
                vars->inputPath = filename;
            }
            return MG_FORM_FIELD_STORAGE_STORE;
        }
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
    unsigned int period = (unsigned int) (src.getSampleRate() / vars.rate);

    std::string outputFileName = "output.wav";

    // Make Effect and sink as shared_ptr
    auto tremolo = std::make_shared<TremoloEffect>(vars.depth, period);
    auto sink = std::make_shared<FileSink>(outputFileName, src.getSampleRate());

    // Connect source, effect and sink
    src.connect(tremolo);
    tremolo->connect(sink);

    while (src.generate_next());

    sink->write();

    mg_send_file(connection, outputFileName.c_str());

    return 200;
}

int WebServer::handle_chain_submit(mg_connection *connection, void *user_data)
{
    mg_form_data_handler fdh = {};

    struct vars_t
    {
        std::string inputPath;
        std::string jsonString;
    } vars;

    fdh.user_data = &vars;

    fdh.field_found = [](const char *key, const char *filename, char *path, size_t pathlen, void *user_data) -> int
    {
        vars_t *vars = static_cast<vars_t *>(user_data);
        // check if field is a file
        if (filename && *filename)
        {
            snprintf(path, pathlen, "%s", filename);

            // store path
            if (std::string(key) == "input")
            {
                vars->inputPath = filename;
            }
            return MG_FORM_FIELD_STORAGE_STORE;
        }
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

        std::string res = std::string(value);

        res = res.substr(0, res.find('\r'));

        if (name == "effect-info")
        {
            vars->jsonString = res;
        }

        return MG_FORM_FIELD_STORAGE_GET;
    };

    if (mg_handle_form_request(connection, &fdh) <= 0)
    {
        throw std::runtime_error("Error handling form request.");
    }

    // Parse
    rapidjson::Document chain;
    chain.Parse(vars.jsonString.c_str()); // Contains array of JSON objects

    jsonChain = vars.jsonString;

    std::vector < std::shared_ptr < Sink < float >> > sinks;
    std::vector < std::shared_ptr < Source < float >> > sources;

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
            sinks.push_back(dist);
            sources.push_back(dist);
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

            unsigned int delaySamples = (unsigned int) (delayTime * src.getSampleRate());

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
            sources.push_back(delay);
            sinks.push_back(delay);
        }
        else if (effect == "tremolo")
        {
            float depth = stof(std::string(obj["depth"].GetString()));
            float rate = stof(std::string(obj["rate"].GetString()));

            unsigned int period = (unsigned int) ((1 / rate) * src.getSampleRate());

            // Create effect and add to vector
            auto tremolo = std::make_shared<TremoloEffect>(depth, period);
            sources.push_back(tremolo);
            sinks.push_back(tremolo);
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

    if (sources.empty())
    {
        src.connect(sink);
    }
    else
    {
        src.connect(sinks[0]);

        for (std::size_t i = 0; i < sources.size() - 1; ++i)
        {
            sources[i]->connect(sinks[i + 1]);
        }

        sources[sources.size() - 1]->connect(sink);
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

    fdh.field_found = [](const char *key, const char *filename, char *path, size_t pathlen, void *user_data) -> int
    {
        vars_t *vars = static_cast<vars_t *>(user_data);
        // check if field is a file
        if (filename && *filename)
        {
            snprintf(path, pathlen, "%s", filename);

            // store path
            if (std::string(key) == "inputfile")
            {
                vars->inputPath = filename;
            }
            else if (std::string(key) == "scriptfile")
            {
                vars->scriptPath = filename;
            }
            return MG_FORM_FIELD_STORAGE_STORE;
        }
        // Not a file
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

    std::string outputFileName = "output.wav";
    // Make FileSource
    FileSource src(vars.inputPath);

    // Make LuaEffect
    auto script = std::make_shared<LuaEffect>(vars.scriptPath);

    // Make FileSink
    auto sink = std::make_shared<FileSink>(outputFileName, src.getSampleRate());

    // Connect
    src.connect(script);
    script->connect(sink);

    while (src.generate_next());

    sink->write();

    mg_send_file(connection, outputFileName.c_str());

    return 200;
}

int WebServer::handle_chain_save(mg_connection *connection, void *user_data)
{
    mg_form_data_handler fdh = {};

    struct vars_t
    {
        std::string chainName;
        std::string jsonString;
    } vars;

    fdh.user_data = &vars;

    fdh.field_found = [](const char *key, const char *filename, char *path, size_t pathlen, void *user_data) -> int
    {
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

        std::string res = std::string(value);

        res = res.substr(0, res.find('\r'));

        if (name == "chain-name")
        {
            vars->chainName = res;
        }
        else if (name == "save-effect-info")
        {
            vars->jsonString = res;
        }

        return MG_FORM_FIELD_STORAGE_GET;
    };

    if (mg_handle_form_request(connection, &fdh) <= 0)
    {
        throw std::runtime_error("Error handling form request.");
    }

    // Parse
    rapidjson::Document chain;

    jsonChain = vars.jsonString;

    std::string jsonObj = "{\"name\":\"" + vars.chainName + "\", \"value\":" + vars.jsonString + "}";

    chain.Parse(jsonObj.c_str()); // Contains array of JSON objects

    // open file
    std::ifstream chainReadFile("chains.json");

    std::string fileContent = "";
    if (chainReadFile)
    {
        std::stringstream buf;
        buf << chainReadFile.rdbuf();
        fileContent = buf.str();
    }

    rapidjson::Document presets;
    if (fileContent == "")
    {
        presets.Parse("[]");
    }
    else
    {
        presets.Parse(fileContent.c_str());
    }

    // Paste new value into document
    rapidjson::Document::AllocatorType &allocator = presets.GetAllocator();
    presets.PushBack(chain, allocator);

    // Write result to file
    std::ofstream chainWriteFile("chains.json");

    if (chainWriteFile)
    {
        rapidjson::StringBuffer sb;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
        presets.Accept(writer);

        chainWriteFile << sb.GetString() << "\n";
    }

    render_redirect(connection, "/chain.html");

    return 200;
}

int WebServer::handle_chain_load(mg_connection *connection, void *user_data)
{
    // Read contents from file
    std::ifstream chainFile("chains.json");
    std::stringstream buf;

    buf << chainFile.rdbuf();

    render_json(connection, (buf.str().length() == 0 ? "[]" : buf.str()));

    return 200;
}

int WebServer::handle_chain_load_active(mg_connection *connection, void *user_data)
{
    // Check if chain empty
    if (jsonChain != "")
    {
        render_json(connection, jsonChain.c_str());
    }
    else
    {
        render_json(connection, "[]");
    }
    return 200;
}

int WebServer::handle_ir_upload(mg_connection *connection, void *user_data)
{
    mg_form_data_handler fdh = {};

    struct vars_t
    {
        std::vector<std::string> inputPaths;
    } vars;

    fdh.user_data = &vars;

    // If a field is found, it is checked if it is a file. If it is, it's processed here, else it's processed in field_get.
    fdh.field_found = [](const char *key, const char *value, char *path, size_t pathlen, void *user_data) -> int
    {
        vars_t *vars = static_cast<vars_t *>(user_data);
        // check if field is a file
        if (value && *value)
        {
            snprintf(path, pathlen, "%s", value);

            // store path
            if (std::string(key) == "ir-file")
            {
                vars->inputPaths.push_back(value);
            }
            return MG_FORM_FIELD_STORAGE_STORE;
        }
        return MG_FORM_FIELD_STORAGE_GET;
    };

    fdh.field_store = [](const char *path, long long file_size, void *user_data) -> int
    {
        return 0;
    };

    // Process the input from the form
    if (mg_handle_form_request(connection, &fdh) <= 0)
    {
        throw std::runtime_error("Error handling form request.");
    }

    // Make filesink
    for (std::string file : vars.inputPaths)
    {
        std::string fileName = "impulse-responses/" + file;
        FileSink sink(fileName, 44100);
        sink.write();

        // Delete file from ~
        std::remove(file.c_str());
    }

    render_redirect(connection, "/conv.html");

    return 200;
}

int WebServer::handle_ir_list(mg_connection *connection, void *user_data)
{
    std::vector<std::string> v;

    std::string dir = "impulse-responses";
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(dir.c_str())) == NULL)
    {
        std::cout << "Error(" << errno << ") opening " << dir << std::endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        v.push_back(std::string(dirp->d_name));
    }
    closedir(dp);

    // Make json document containing all filenames
    rapidjson::Document responses;
    responses.Parse("[]");

    rapidjson::Document::AllocatorType &allocator = responses.GetAllocator();

    for (std::string file : v)
    {
        rapidjson::Value strVal;
        strVal.SetString(file.c_str(), file.length(), allocator);

        responses.PushBack(strVal, allocator);
    }

    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    responses.Accept(writer);
    std::string files(sb.GetString());

    render_json(connection, files);

    return 200;
}

int WebServer::handle_alsa_submit(mg_connection *connection, void *user_data)
{
    mg_form_data_handler fdh = {};

    struct vars_t
    {
        std::string jsonString;
    } vars;

    fdh.user_data = &vars;

    fdh.field_found = [](const char *key, const char *filename, char *path, size_t pathlen, void *user_data) -> int
    {
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

        std::string res = std::string(value);

        res = res.substr(0, res.find('\r'));

        if (name == "effect-info")
        {
            vars->jsonString = res;
        }

        return MG_FORM_FIELD_STORAGE_GET;
    };

    if (mg_handle_form_request(connection, &fdh) <= 0)
    {
        throw std::runtime_error("Error handling form request.");
    }

    // Parse
    rapidjson::Document chain;
    chain.Parse(vars.jsonString.c_str()); // Contains array of JSON objects

    jsonChain = vars.jsonString;

    std::vector < std::shared_ptr < Sink < float >> > sinks;
    std::vector < std::shared_ptr < Source < float >> > sources;

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
            sinks.push_back(dist);
            sources.push_back(dist);
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

            unsigned int delaySamples = (unsigned int) (delayTime * alsaDevice->getSampleRate());

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
            sources.push_back(delay);
            sinks.push_back(delay);
        }
        else if (effect == "tremolo")
        {
            float depth = stof(std::string(obj["depth"].GetString()));
            float rate = stof(std::string(obj["rate"].GetString()));

            unsigned int period = (unsigned int) ((1 / rate) * alsaDevice->getSampleRate());

            // Create effect and add to vector
            auto tremolo = std::make_shared<TremoloEffect>(depth, period);
            sources.push_back(tremolo);
            sinks.push_back(tremolo);
        }
        else if (effect == "convolution")
        {
            // TODO
            std::cout << "Later alligator" << std::endl;
        }
    }


    for (std::size_t i = 0; i < sources.size() - 1; ++i)
    {
        sources[i]->connect(sinks[i + 1]);
    }


    if (pthread_mutex_lock(&thread_params.mutex) != 0)
    {
        throw std::runtime_error("Error locking mutex");
    }
    thread_params.changed = true;
    if (pthread_cond_wait(&thread_params.canChange, &thread_params.mutex) != 0)
    {
        throw std::runtime_error("Error in cond_wait");
    }

    render_redirect(connection, "/chain.html");

    return 200;
}

void *WebServer::alsa_thread(void *arg)
{
    thread_param *params = static_cast<thread_param *>(arg);
    while (true)
    {

        for (unsigned int i = 0; i < alsaDevice->getSampleRate(); ++i)
        {
            alsaDevice->generate_next();
        }

        if (params->changed)
        {
            alsaDevice->connect(params->firstSink);
            params->lastSource->connect(alsaDevice);
            params->changed = false;
            pthread_cond_signal(&params->canChange);
        }

    }
}