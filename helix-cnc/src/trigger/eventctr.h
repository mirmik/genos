#ifndef EVENTCTR_H
#define EVENTCTR_H

#include <igris/dprint.h>
#include <igris/event/delegate.h>
#include <igris/util/numconvert.h>

#include <zillot/common/uartring.h>
#include <trigger/global.h>

#include <string.h>

extern bool eventctr_info_mode;

#define EVENTCTR_ARRAY_SIZE 128

extern bool eventctr_mode;

class EventCtr
{
    struct table_save
    {
        uint8_t *cycle = nullptr;
        uint8_t *prefix = nullptr;
        uint8_t *postfix = nullptr;

        size_t cycle_total = 0;
        size_t cycle_size = 0;
        size_t prefix_size = 0;
        size_t postfix_size = 0;
    };

public:
    table_save saved_tables[10];

    uint8_t *cmdtable_prefix = nullptr;
    uint8_t *cmdtable_postfix = nullptr;
    uint8_t *cmdtable_cycle = nullptr;
    size_t cmdtable_size_prefix = 0;
    size_t cmdtable_size_postfix = 0;
    size_t cmdtable_size_cycle = 0;

    size_t counter = 0;
    size_t cycle_count = 0;
    size_t cycle_total = 0;
    int stage = 0;

    size_t cmd_wcursor = 0;
    size_t cmd_wsection = 0;

    int16_t maxev = -1;
    igris::delegate<void> handlers[EVENTCTR_ARRAY_SIZE];
    const char *description[EVENTCTR_ARRAY_SIZE];

    bool debug_mode = false;

public:
    EventCtr()
    {
        memset(description, 0, EVENTCTR_ARRAY_SIZE * sizeof(const char *));
        memset(saved_tables, 0, sizeof(saved_tables));
    }

    // Use handler with index
    void do_event(uint32_t no)
    {
        if (no >= EVENTCTR_ARRAY_SIZE)
        {
            dprln("WARNING: In EventCtr::do_event no >= EVENTCTR_ARRAY_SIZE",
                  no);
            return;
        }

        if (handlers[no] == igris::delegate<void>())
        {
            dprln("WARNING: uninitialized event was invoked code:", no);
            return;
        }

        handlers[no]();
    }

    void do_next_event()
    {
        uint16_t cmd;
        switch (stage)
        {
        case 0:
            cmd = cmdtable_prefix[counter++];
            do_event(cmd);

            if (counter == cmdtable_size_prefix)
            {
                counter = 0;
                stage = 1;
            }

            break;

        case 1:
            cmd = cmdtable_cycle[counter++];
            do_event(cmd);

            if (counter == cmdtable_size_cycle)
            {
                cycle_count++;
                counter = 0;

                if (cycle_count == cycle_total)
                {
                    stage = 2;
                }
            }
            break;

        case 2:
            cmd = cmdtable_postfix[counter++];
            do_event(cmd);

            if (counter == cmdtable_size_postfix)
            {
                // eventctr_mode = false;
            }

            break;
        }
    }

    // Attach event handler to event
    void set_handler(int16_t no, igris::delegate<void> dlg, const char *help)
    {
        if (no > maxev)
            maxev = no + 1;
        handlers[no] = dlg;
        description[no] = help;
    }

    // Print all handlers
    void print_eventlist()
    {
        for (int i = 0; i < maxev; ++i)
        {
            dprln(i, ":", description[i]);
        }
    }

    // Clear prefix table and set it as pushed data receiver
    void reset_table_prefix(uint32_t size)
    {
        delete[] cmdtable_prefix;

        cmdtable_prefix = new uint8_t[size];
        cmdtable_size_prefix = size;

        cmd_wsection = 0;
        cmd_wcursor = 0;
        counter = 0;
    }

    // Clear postfix table and set it as pushed data receiver
    void reset_table_postfix(uint32_t size)
    {
        delete[] cmdtable_postfix;

        cmdtable_postfix = new uint8_t[size];
        cmdtable_size_postfix = size;

        cmd_wsection = 2;
        cmd_wcursor = 0;
        counter = 0;
    }

    // Clear cycle table and set it as pushed data receiver
    void reset_table_cycle(uint32_t size)
    {
        delete[] cmdtable_cycle;

        cmdtable_cycle = new uint8_t[size];
        cmdtable_size_cycle = size;

        cmd_wcursor = 0;
        cmd_wsection = 1;
        counter = 0;
        cycle_count = 0;
    }

    // Set count of cycle invocations.
    void set_loop_total(int total)
    {
        cycle_total = total;
        cycle_count = 0;
        counter = 0;
        stage = 0;
    }

    // Save current table to saved_tables with @saveno index
    void save_table(int saveno)
    {
        delete[] saved_tables[saveno].prefix;
        delete[] saved_tables[saveno].postfix;
        delete[] saved_tables[saveno].cycle;

        saved_tables[saveno].prefix = new uint8_t[cmdtable_size_prefix];
        saved_tables[saveno].prefix_size = cmdtable_size_prefix;

        saved_tables[saveno].postfix = new uint8_t[cmdtable_size_postfix];
        saved_tables[saveno].postfix_size = cmdtable_size_postfix;

        saved_tables[saveno].cycle = new uint8_t[cmdtable_size_cycle];
        saved_tables[saveno].cycle_size = cmdtable_size_cycle;

        memcpy(saved_tables[saveno].prefix, cmdtable_prefix,
               saved_tables[saveno].prefix_size);
        memcpy(saved_tables[saveno].postfix, cmdtable_postfix,
               saved_tables[saveno].postfix_size);
        memcpy(saved_tables[saveno].cycle, cmdtable_cycle,
               saved_tables[saveno].cycle_size);

        saved_tables[saveno].cycle_total = cycle_total;
    }

    // Restore saved table with @saveno index
    void load_table(int saveno)
    {
        delete[] cmdtable_prefix;
        delete[] cmdtable_postfix;
        delete[] cmdtable_cycle;

        cmdtable_prefix = new uint8_t[saved_tables[saveno].prefix_size];
        cmdtable_size_prefix = saved_tables[saveno].prefix_size;

        cmdtable_cycle = new uint8_t[saved_tables[saveno].cycle_size];
        cmdtable_size_cycle = saved_tables[saveno].cycle_size;

        cmdtable_postfix = new uint8_t[saved_tables[saveno].postfix_size];
        cmdtable_size_postfix = saved_tables[saveno].postfix_size;

        cmd_wsection = 0;
        cmd_wcursor = 0;
        counter = 0;
        stage = 0;
        cycle_count = 0;
        cycle_total = saved_tables[saveno].cycle_total;

        memcpy(cmdtable_prefix, saved_tables[saveno].prefix,
               saved_tables[saveno].prefix_size);
        memcpy(cmdtable_cycle, saved_tables[saveno].cycle,
               saved_tables[saveno].cycle_size);
        memcpy(cmdtable_postfix, saved_tables[saveno].postfix,
               saved_tables[saveno].postfix_size);

        cycle_total = saved_tables[saveno].cycle_total;

        if (cmdtable_size_prefix == 0)
            stage = 1;

        eventctr_mode = true;
    }

    // Push byte to last ressetted table (cycle or prefix or postfix)
    void push_back(uint32_t arg)
    {
        uint16_t no = (arg & 0xFFFFFF00) >> 8;
        uint16_t cmd = (arg & 0x000000FF);

        switch (cmd_wsection)
        {
        case 0:
            if (no >= cmdtable_size_prefix)
            {
                return;
            }

            cmdtable_prefix[no] = cmd;
            break;

        case 1:
            if (no >= cmdtable_size_cycle)
            {
                return;
            }

            cmdtable_cycle[no] = cmd;
            break;

        case 2:
            if (no >= cmdtable_size_postfix)
            {
                return;
            }

            cmdtable_postfix[no] = cmd;
            break;
        }
    }
};

extern EventCtr eventctr;

static inline int print_eventlist(int, char **)
{
    eventctr.print_eventlist();
    return 0;
}

#endif