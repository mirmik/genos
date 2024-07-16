#include <ralgo/global_protection.h>
#include <ralgo/oldheimer/command_center.h>

heimer::command_center_cls<float, float> heimer::command_center;

int heimer::axcmd(int argc, char **argv)
{
    return command_center.axcmd(argc, argv);
}

int heimer::igcmd(int argc, char **argv)
{
    return command_center.igcmd(argc, argv);
}

int heimer::ctrcmd(int argc, char **argv)
{
    return command_center.ctrcmd(argc, argv);
}

igris::console_command heimer::command_center_cmdtable[] = {
    {"ax", axcmd},
    {"ig", igcmd},
    {"ctr", ctrcmd},
    {"setprotect", ralgo::set_global_protection_command},
    {nullptr, (void *)nullptr}};
