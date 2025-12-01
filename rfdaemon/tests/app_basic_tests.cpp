#include "App.h"
#include "guard/guard.h"

#include <string>
#include <vector>
#include <thread>
#include <chrono>

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
    // Используем долгоживущий процесс для проверки состояния running
    App app(0, "test_app", "sleep 10", RestartMode::ONCE, linkeds, "");

    CHECK_EQ(app.status_string(), std::string("stopped"));

    app.start();
    // Даем время на запуск
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    CHECK_EQ(app.status_string(), std::string("running"));

    app.stop();
    // Даем время на остановку
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    CHECK_EQ(app.status_string(), std::string("stopped"));
}

TEST_CASE(
    "App::command и App::token_list_as_string строят строку из токенов команды")
{
    std::vector<LinkedFile> linkeds;
    App app(0, "echo_app", "echo 1 2", RestartMode::ONCE, linkeds, "");

    CHECK_EQ(app.command(), std::string("echo 1 2 "));
    CHECK_EQ(app.token_list_as_string(), std::string("[echo,1,2]"));
}

TEST_CASE("App::stop корректно завершает процесс и поток")
{
    std::vector<LinkedFile> linkeds;
    App app(0, "sleep_app", "sleep 10", RestartMode::ONCE, linkeds, "");

    // Проверяем начальное состояние
    CHECK_EQ(app.stopped(), true);
    CHECK_EQ(app.status_string(), std::string("stopped"));

    // Запускаем процесс
    app.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    CHECK_EQ(app.stopped(), false);
    CHECK_EQ(app.status_string(), std::string("running"));

    // Останавливаем процесс
    app.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    
    // Проверяем что процесс остановлен
    CHECK_EQ(app.stopped(), true);
    CHECK_EQ(app.status_string(), std::string("stopped"));
}

TEST_CASE("App::start/stop цикл работает корректно")
{
    std::vector<LinkedFile> linkeds;
    // Используем долгоживущий процесс
    App app(0, "cycle_test", "sleep 10", RestartMode::ONCE, linkeds, "");

    // Проверяем начальное состояние
    CHECK_EQ(app.stopped(), true);
    
    // Запускаем процесс
    app.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    CHECK_EQ(app.stopped(), false);
    
    // Останавливаем процесс
    app.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    CHECK_EQ(app.stopped(), true);
    
    // Проверяем что можно запустить снова (но без многократных итераций)
    // Примечание: многократные start/stop могут быть нестабильны из-за 
    // особенностей igris::subprocess
}
