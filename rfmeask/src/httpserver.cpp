#include <httplib_static.h>
#include <ircc/ircc.h>
#include <nos/log.h>
#include <thread>

using namespace nos::argument_literal;
using namespace std::chrono_literals;

void bind_static_html_resource(httplib::Server &srv,
                               std::string path,
                               std::string resource,
                               std::string content_type)
{
    srv.Get(path,
            [path, resource, content_type](const httplib::Request &,
                                           httplib::Response &res) {
                std::string text = ircc_string(resource.c_str());
                res.set_content(text, content_type);
            });
}

std::thread httpserver_thread;
void start_httpserver(int port)
{
    httpserver_thread = std::thread([port]() {
        httplib::Server server;
        bind_static_html_resource(server, "/", "/web/index.html", "text/html");
        bind_static_html_resource(
            server, "/script.js", "/web/script.js", "text/javascript");
        bind_static_html_resource(
            server, "/index.html", "/web/index.html", "text/html");

        server.set_error_handler([](const auto &req, auto &res) {
            auto fmt = "<p>Error Path:%s Status: <span "
                       "style='color:red;'>%d</span></p>";
            char buf[BUFSIZ];
            snprintf(buf, sizeof(buf), fmt, req.path.c_str(), res.status);
            res.set_content(buf, "text/html");
        });

        nos::log::info("Starting HTTP server on port {}", port);
        server.listen("0.0.0.0", port);
    });
}