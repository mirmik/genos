/**
 * @file
 *
 * Система исполнения встраиваемых скриптов.
 */

#ifndef SCRIPT_EXEC_H
#define SCRIPT_EXEC_H

#include <string>

void set_script_directory(const std::string &dirpath);
std::string script_directory();
int script_execution(const std::string &name);
void script_terminate_all();

#endif