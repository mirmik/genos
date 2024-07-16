/** @file */

#ifndef CROW_SELECT_H
#define CROW_SELECT_H

namespace crow
{
    extern int unselect_pipe[2];
    extern bool add_unselect_to_fds;

    void select_collect_fds();
    void add_select_fd(int fd);
    void select();
    void unselect();
    void unselect_init();
}

#endif
