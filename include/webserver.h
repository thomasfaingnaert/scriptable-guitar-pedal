#include <atomic>
#include <memory>
#include <pthread.h>
#include <string>

#include "alsadevice.h"
#include "civetweb.h"
#include "document.h"

#ifndef WEBSERVER_H_LMKVHNTB
#define WEBSERVER_H_LMKVHNTB

class WebServer
{
    public:
        WebServer(unsigned int port);
        ~WebServer();
        bool isRunning() const;
        static std::shared_ptr<AlsaDevice> getAlsaDevice() { return alsaDevice; };

        static void alsa_thread();
    private:
        struct thread_param
        {
            std::atomic<bool> changed; // Has chain changed?
            std::shared_ptr<Sink<float>> firstSink; // The first sink (to connect with alsa device)
            std::shared_ptr<Source<float>> lastSource; // The last source (to connect with alsa device)
            pthread_cond_t canChange; // Webserver needs to wait for changes until changed is set to false
            pthread_mutex_t mutex; // mutex for condition variable
        };

        mg_context *context;
        bool exit = false;
        static std::string jsonChain;
        static std::shared_ptr<AlsaDevice> alsaDevice;
        pthread_t alsaThread;
        static thread_param thread_params;

        static void render(mg_connection *connection, const std::string &data, const std::string &contentType);
        static void render_text(mg_connection *connection, const std::string &text);
        static void render_html(mg_connection *connection, const std::string &html);
        static void render_json(mg_connection *connection, const std::string &json);
        static void render_redirect(mg_connection *connection, const std::string &url);

        static int handle_exit(mg_connection *connection, void *user_data);
        static int handle_conv_submit(mg_connection *connection, void *user_data);
        static int handle_dist_submit(mg_connection *connection, void *user_data);
        static int handle_delay_submit(mg_connection *connection, void *user_data);
        static int handle_tremolo_submit(mg_connection *connection, void *user_data);
        static int handle_chain_submit(mg_connection *connection, void *user_data);
        static int handle_lua_submit(mg_connection *connection, void *user_data);
        static int handle_chain_save(mg_connection *connection, void *user_data);
        static int handle_chain_load(mg_connection *connection, void *user_data);
        static int handle_chain_load_active(mg_connection *connection, void *user_data);
        static int handle_ir_upload(mg_connection *connection, void *user_data);
        static int handle_ir_list(mg_connection *connection, void *user_data);
        static int handle_alsa_submit(mg_connection *connection, void *user_data);
};

#endif /* end of include guard: WEBSERVER_H_LMKVHNTB */
