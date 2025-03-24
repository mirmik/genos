#include <AbstractAxis.h>
#include <Axis.h>
#include <doctest/doctest.h>
#include <plugins/py.h>

TEST_CASE("py")
{
    std::string script = R"(
import my_module
print("Initial state:", my_module.get_testvariable())
my_module.set_testvariable(102)
print("Updated state:", my_module.get_testvariable())

result = {}
result["test"] = my_module.get_testvariable()
result["b"] = indata["a"] + 1
)";

    python_script_hook hook(script);
    nos::trent indata;
    nos::trent outdata;
    indata["a"] = 321;
    hook.execute(indata, outdata);
    CHECK(outdata.contains("result"));
    CHECK(outdata["result"].is_dict());
    CHECK_EQ(outdata["result"]["test"].as_integer(), 102);
    CHECK_EQ(outdata["result"]["b"].as_integer(), 322);
}