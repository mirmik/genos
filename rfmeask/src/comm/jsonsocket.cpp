#include <AbstractAxis.h>
#include <comm/jsonsocket.h>
#include <fstream>
#include <groups/InterpolationGroup.h>
#include <iostream>
#include <ndmath/util.h>
#include <nos/inet/tcp_client.h>
#include <nos/inet/tcp_server.h>
#include <nos/input.h>
#include <nos/shell/weaked_function.h>
#include <nos/trent/json.h>
#include <nos/trent/json_print.h>
#include <tables.h>
#include <thread>
#include <unordered_map>

void json_reply_error(nos::inet::tcp_client &sock, std::string text)
{
    nos::trent tr;
    tr["text"] = text;
    tr["success"] = false;
    nos::println_to(sock, nos::json::to_string(tr));
}

void set_oneaxis_correction_for_device(nos::inet::tcp_client &client,
                                       const nos::trent &tr)
{
    std::string name = tr["servo_name"].as_string();
    AbstractServo *servo = get_servo(name);
    if (servo == nullptr)
    {
        json_reply_error(client, "axis not found");
        return;
    }

    std::vector<double> points;
    std::vector<double> deltas;

    auto enabled = tr["enabled"].as_bool_default(true);
    auto &tr_points_list = tr["points"].as_list();
    auto &tr_deltas_list = tr["deltas"].as_list();

    if (tr_points_list.size() == 0)
    {
        servo->clear_one_axis_correction();
        return;
    }

    for (size_t i = 0; i < tr_points_list.size(); i++)
    {
        points.push_back(tr_points_list[i].as_numer());
        deltas.push_back(tr_deltas_list[i].as_numer());
    }

    auto correction =
        one_axis_cartesian_correction::arrays_to_correction(points, deltas);

    servo->set_one_axis_correction(correction, enabled);
}

void get_oneaxis_correction_for_device(nos::inet::tcp_client &client,
                                       const nos::trent &tr)
{
    std::string name = tr["servo_name"].as_string();
    AbstractServo *servo = get_servo(name);
    if (servo == nullptr)
    {
        json_reply_error(client, "axis not found");
        return;
    }

    const auto &cc = servo->get_one_axis_correction();
    auto [points, deltas] =
        one_axis_cartesian_correction::correction_to_arrays(cc);

    nos::trent reply;

    reply["success"] = true;
    reply["servo_name"] = name;
    reply["enabled"] = servo->is_correction_enabled();
    reply["points"].as_list();
    reply["deltas"].as_list();
    for (size_t i = 0; i < points.size(); ++i)
    {
        reply["points"].as_list().push_back(points[i]);
        reply["deltas"].as_list().push_back(deltas[i]);
    }

    nos::println_to(client, nos::json::to_string(reply));
    nos::println(nos::json::to_string(reply));
}

void set_oneaxis_correction(nos::inet::tcp_client &client, const nos::trent &tr)
{
    std::string axis_name = tr["axis_name"].as_string();
    AbstractAxis *axis = get_axis(axis_name);
    if (axis == nullptr)
    {
        json_reply_error(client, "axis not found");
        return;
    }

    std::vector<double> points;
    std::vector<double> deltas;

    auto enabled = tr["enabled"].as_bool_default(true);
    auto &tr_points_list = tr["points"].as_list();
    auto &tr_deltas_list = tr["deltas"].as_list();

    if (tr_points_list.size() == 0)
    {
        axis->clear_one_axis_correction();
        return;
    }

    for (size_t i = 0; i < tr_points_list.size(); i++)
    {
        points.push_back(tr_points_list[i].as_numer());
        deltas.push_back(tr_deltas_list[i].as_numer());
    }

    auto correction =
        one_axis_cartesian_correction::arrays_to_correction(points, deltas);

    axis->set_one_axis_correction(correction, enabled);
}

void set_oneaxis_correction_2(const nos::trent &tr)
{
    std::string axis_name = tr["axis_name"].as_string();
    AbstractAxis *axis = get_axis(axis_name);
    if (axis == nullptr)
    {
        throw std::runtime_error("axis not found");
    }

    std::vector<double> points;
    std::vector<double> deltas;

    auto enabled = tr["enabled"].as_bool_default(true);
    auto &tr_points_list = tr["points"].as_list();
    auto &tr_deltas_list = tr["deltas"].as_list();

    if (tr_points_list.size() == 0)
    {
        axis->clear_one_axis_correction();
        return;
    }

    for (size_t i = 0; i < tr_points_list.size(); i++)
    {
        points.push_back(tr_points_list[i].as_numer());
        deltas.push_back(tr_deltas_list[i].as_numer());
        nos::println("point: ", points[i], " delta: ", deltas[i]);
    }

    auto correction =
        one_axis_cartesian_correction::arrays_to_correction(points, deltas);

    axis->set_one_axis_correction(correction, enabled);
}

void get_oneaxis_correction(nos::inet::tcp_client &client, const nos::trent &tr)
{
    std::string axis_name = tr["axis_name"].as_string();
    AbstractAxis *axis = get_axis(axis_name);
    if (axis == nullptr)
    {
        json_reply_error(client, "axis not found");
        return;
    }

    if (!axis->has_correction())
    {
        json_reply_error(client, "no correction");
        return;
    }

    const auto &cc = axis->get_one_axis_correction();
    auto [points, deltas] =
        one_axis_cartesian_correction::correction_to_arrays(cc);

    nos::trent reply;

    reply["success"] = true;
    reply["axis_name"] = axis_name;
    reply["enabled"] = axis->is_correction_enabled();
    reply["points"].as_list();
    reply["deltas"].as_list();
    for (size_t i = 0; i < points.size(); ++i)
    {
        reply["points"].as_list().push_back(points[i]);
        reply["deltas"].as_list().push_back(deltas[i]);
    }

    nos::println_to(client, nos::json::to_string(reply));
    nos::println(nos::json::to_string(reply));
}

void get_multiaxes_correction_ss(nos::inet::tcp_client &client,
                                 const nos::trent &tr)
{
    std::string group_name = tr["group_name"].as_string();
    InterpolationGroup_v2 *group = find_group(group_name);
    auto correction_trent = group->correction_as_trent();

    nos::println("correction_trent: ", correction_trent);

    nos::trent reply;
    reply["success"] = true;
    reply["result"] = correction_trent;

    nos::println_to(client, nos::json::to_string(reply));
}

void set_multiaxes_correction_ss(nos::inet::tcp_client &client,
                                 const nos::trent &tr)
{
    nos::println("SET_MULTIAXES_CORRECTION_SS");
    std::string group_name = tr["group_name"].as_string();
    InterpolationGroup_v2 *group = find_group(group_name);
    group->set_correction_by_trent(tr);
}

void group_trajectory_move(nos::inet::tcp_client &client, const nos::trent &tr)
{
    std::string group_name = tr["group_name"].as_string();
    InterpolationGroup_v2 *group = find_group(group_name);
    group->trajectory_move_by_trent(tr);
}

nos::trent get_oneaxis_correction_2(const std::string &axis_name)
{
    AbstractAxis *axis = get_axis(axis_name);
    if (axis == nullptr)
    {
        throw std::runtime_error("axis not found");
    }

    if (!axis->has_correction())
    {
        throw std::runtime_error("no correction");
    }

    const auto &cc = axis->get_one_axis_correction();
    auto [points, deltas] =
        one_axis_cartesian_correction::correction_to_arrays(cc);

    nos::trent reply;

    reply["axis_name"] = axis_name;
    reply["enabled"] = axis->is_correction_enabled();
    reply["points"].as_list();
    reply["deltas"].as_list();
    for (size_t i = 0; i < points.size(); ++i)
    {
        reply["points"].as_list().push_back(points[i]);
        reply["deltas"].as_list().push_back(deltas[i]);
    }

    return reply;
}

void enable_status_notification(JsonApiClient &client, const nos::trent &tr)
{
    bool enable = tr["enable"].as_bool();
    client.set_status_notification_enabled(enable);
}

void JsonApiClient::json_handler(const std::string &msg)
{
    nos::trent tr;
    try
    {
        nos::println("JSON:", msg);
        tr = nos::json::parse(msg);
    }
    catch (const std::exception &e)
    {
        nos::log::warn(nos::format("Invalid json. Drop message. {}", e.what()));
        return;
    }

    auto cmd = tr["cmd"].as_string();
    {
        if (cmd == "set_oneaxis_correction")
        {
            set_oneaxis_correction(socket(), tr);
            return;
        }

        else if (cmd == "get_oneaxis_correction")
        {
            get_oneaxis_correction(socket(), tr);
            return;
        }

        else if (cmd == "set_oneaxis_correction_for_device")
        {
            set_oneaxis_correction_for_device(socket(), tr);
            return;
        }

        else if (cmd == "get_oneaxis_correction_for_device")
        {
            get_oneaxis_correction_for_device(socket(), tr);
            return;
        }

        else if (cmd == "get_multiaxes_correction")
        {
            get_multiaxes_correction_ss(socket(), tr);
            return;
        }

        else if (cmd == "set_multiaxes_correction")
        {
            set_multiaxes_correction_ss(socket(), tr);
            return;
        }

        else if (cmd == "group_trajectory")
        {
            group_trajectory_move(socket(), tr);
            return;
        }

        else if (cmd == "enable_status_notification")
        {
            enable_status_notification(*this, tr);
            return;
        }

        else if (cmd == "idn")
        {
            auto idn = system_idn();
            nos::trent reply;
            reply["success"] = true;
            reply["result"] = idn;
            nos::println_to(socket(), nos::json::to_string(reply));
            return;
        }
    }

    nos::trent reply;
    try
    {
        nos::trent ret = *interpreter.execute_json_protocol(tr);
        reply["success"] = true;
        reply["result"] = ret;
        nos::println_to(socket(), nos::json::to_string(reply));
        return;
    }
    catch (std::exception &e)
    {
        nos::println("exception: ", e.what());
        json_reply_error(socket(), e.what());
        return;
    }
}

void JsonApiClient::console_handler(const std::string &msg)
{
    auto ret = interpreter.execute_console_command_protocol(msg);
    if (ret.is_error())
        nos::println_to(socket(), ret.error());
    else
        nos::println_to(socket(), nos::json::to_string(*ret));
}

void JsonApiClient::run()
{
    nos::log::debug("JsonApiClient::run()");

    try
    {
        while (true)
        {
            auto emsg = nos::readline_from(socket(), 1024, false);

            if (emsg.is_error())
            {
                nos::log::debug("Json api connection refused");
                break;
            }

            auto msg = *emsg;
            if (msg.size() == 0)
            {
                nos::log::debug("Json api socket connection closed.");
                break;
            }

            if (msg[0] == '{')
            {
                json_handler(msg);
                continue;
            }
            else
            {
                console_handler(msg);
                continue;
            }
        }
    }
    catch (std::exception &e)
    {
        nos::log::error("Json api client exception: {}", e.what());
    }

    nos::log::debug("JsonApiClient::run() exit");

    if (socket().good())
        socket().disconnect();
}

void execute_script(const nos::trent &tr)
{
    std::string script = tr["script"].as_string();
    nos::println("execute_script: ", script);
}

void JsonApiServer::init_wf_collection()
{
    interpreter.add("system_idn", std::function<std::string()>(system_idn));

    interpreter.add("get_oneaxis_correction_2",
                    std::function<nos::trent(const std::string &)>(
                        get_oneaxis_correction_2),
                    {"axis_name"});

    interpreter.add(
        "set_oneaxis_correction_2",
        std::function<void(const nos::trent &)>(set_oneaxis_correction_2),
        {"args"});

    interpreter.add("execute_script",
                    std::function<void(const nos::trent &)>(execute_script),
                    {"args"});

    // interpreter.add("get_multiaxes_correction",
    //                 std::function<nos::trent(const std::string &)>(
    //                     get_multiaxes_correction),
    //                 {"group_name"});

    // interpreter.add("set_multiaxes_correction",
    //                 std::function<void(std::string, const nos::trent &)>(
    //                     set_multiaxes_correction),
    //                 {"args"});

    interpreter.add(
        "sum",
        std::function<int(int, int)>(+[](int a, int b) { return a + b; }),
        {"a", "b"});
}

void JsonApiServer::start(int port)
{
    init_wf_collection();

    this->port = port;
    server_thread = std::thread([this, port]() {
        int init_sts = server.init();
        if (init_sts <= 0)
        {
            nos::log::error("JsonApiServer::start() failed to init server: {}",
                            strerror(errno));
            return;
        }
        int bind_sts = server.bind("0.0.0.0", port);
        if (bind_sts != 0)
        {
            nos::log::error("JsonApiServer::start() failed to bind server: {}",
                            strerror(errno));
            server.close();
            return;
        }
        int reusing_sts = server.reusing(true);
        if (reusing_sts != 0)
        {
            nos::log::error("JsonApiServer::start() failed to set reusing: {}",
                            strerror(errno));
            server.close();
            return;
        }
        int listen_sts = server.listen();
        if (listen_sts != 0)
        {
            nos::log::error("JsonApiServer::start() failed to listen: {}",
                            strerror(errno));
            server.close();
            return;
        }

        nos::log::info("JsonApi server started on port {}", port);

        while (true)
        {
            auto client = server.accept();

            if (cancel_thread)
                break;

            if (!client.good())
            {
                nos::log::warn("JsonApi server accept failed: {}",
                               strerror(errno));
                return;
            }

            nos::log::info("JsonApi client connected");
            clients.emplace_back(std::move(client), interpreter);
        }
        nos::log::info("JsonApi server thread stopped");
        server.close_socket();
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void JsonApiServer::stop()
{
    cancel_thread = true;
    nos::inet::tcp_client::dial(
        "127.0.0.1", port, 20ms); // connect for server wakeup from accept
    server_thread.join();
}
