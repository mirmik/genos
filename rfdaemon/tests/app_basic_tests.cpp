#include "App.h"
#include "guard/guard.h"

#include <string>
#include <vector>

TEST_CASE(
    "LinkedFile::to_trent: корректное отображение полей в трент-структуру")
{
    LinkedFile lf;
    lf.path = "/tmp/test.conf";
    lf.name = "config";
    lf.editable = true;

    nos::trent tr = lf.to_trent();

    CHECK_EQ(tr["path"].as_string(), lf.path);
    CHECK_EQ(tr["name"].as_string(), lf.name);
    CHECK_EQ(tr["editable"].as_bool(), lf.editable);
}

TEST_CASE("App::status_string зависит только от флага isStopped")
{
    std::vector<LinkedFile> linkeds;
    App app(0, "test_app", "/bin/true", RestartMode::ONCE, linkeds, "");

    CHECK_EQ(app.status_string(), std::string("stopped"));

    app.start();
    CHECK_EQ(app.status_string(), std::string("running"));

    app.stop();
}

TEST_CASE(
    "App::command и App::token_list_as_string строят строку из токенов команды")
{
    std::vector<LinkedFile> linkeds;
    App app(0, "echo_app", "echo 1 2", RestartMode::ONCE, linkeds, "");

    CHECK_EQ(app.command(), std::string("echo 1 2 "));
    CHECK_EQ(app.token_list_as_string(), std::string("[echo,1,2]"));
}
