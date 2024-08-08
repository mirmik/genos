#ifndef TABLES_H
#define TABLES_H

#include <AbstractAxis.h>
#include <map>
#include <set>
#include <string>
#include <vector>

extern bool have_can;
extern bool have_mrs;
extern bool have_mrs_modbus;
extern bool have_drv;
extern bool have_mitsu_board_servos;

extern std::map<std::string,
                void (*)(const std::string &name, const nos::trent &dict)>
    device_initializers;

extern std::map<std::string,
                void (*)(const std::string &name, const nos::trent &dict)>
    axis_initializers;

extern std::map<std::string,
                void (*)(const std::string &name, const nos::trent &dict)>
    intgroups_initializers;

// extern std::vector<AbstractAxis *> axes_list;
extern std::set<std::string> axis_types;
extern std::map<std::string, Device *> devices;
extern std::vector<Device *> devices_list;
extern std::vector<std::string> devices_types;

size_t axes_total();
//size_t active_axes_total();
AbstractAxis *get_axis(size_t num);
AbstractAxis *get_axis(const std::string_view &str);

/// Find axis by string.
/// @name_or_number can by axis mnemonic or axis number in string form
AbstractAxis *find_axis(const std::string &name_or_number);

void delete_axis(size_t axno);

/// Get copy of axis list.
std::vector<AbstractAxis *> get_axis_list();
void registry_axis(AbstractAxis *axis);
void add_axis_to_list(AbstractAxis *axis);

std::vector<InterpolationGroup_v2 *> get_group_list();
void add_group_to_list(InterpolationGroup_v2 *group);
InterpolationGroup_v2 *find_group(int number);
size_t groups_total();

AbstractServo *get_servo(std::string name);

#endif