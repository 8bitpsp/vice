/*
 * mon_breakpoint.c - The VICE built-in monitor breakpoint functions.
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
 *  Daniel Sladic <sladic@eecg.toronto.edu>
 *  Ettore Perazzoli <ettore@comm2000.it>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#include "vice.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interrupt.h"
#include "lib.h"
#include "log.h"
#include "mon_breakpoint.h"
#include "mon_disassemble.h"
#include "mon_util.h"
#include "montypes.h"
#include "uimon.h"


#define any_breakpoints(mem) (breakpoints[(mem)] != NULL)

struct breakpoint_s {
   int brknum;
   MON_ADDR start_addr;
   MON_ADDR end_addr;
   int hit_count;
   int ignore_count;
   cond_node_t *condition;
   char *command;
   bool trace;
   bool enabled;
   bool watch_load;
   bool watch_store;
   bool temporary;
};
typedef struct breakpoint_s breakpoint_t;

struct break_list_s {
   breakpoint_t *brkpt;
   struct break_list_s *next;
};
typedef struct break_list_s break_list_t;

static int breakpoint_count;
break_list_t *breakpoints[NUM_MEMSPACES];

extern void parse_and_execute_line(char *input);

void mon_breakpoint_init(void)
{
    breakpoint_count = 1;
}

static void remove_checkpoint_from_list(break_list_t **head, breakpoint_t *bp)
{
    break_list_t *cur_entry, *prev_entry;

    cur_entry = *head;
    prev_entry = NULL;

    while (cur_entry) {
        if (cur_entry->brkpt == bp)
            break;

        prev_entry = cur_entry;
        cur_entry = cur_entry->next;
    }

    if (!cur_entry) {
        log_error(LOG_ERR, "Invalid checkpoint entry!");
        return;
    } else {
        if (!prev_entry) {
            *head = cur_entry->next;
        } else {
             prev_entry->next = cur_entry->next;
        }
        lib_free(cur_entry);
    }
}

static breakpoint_t *find_checkpoint(int brknum)
{
    break_list_t *ptr;
    int i;

    for (i = e_comp_space; i < LAST_SPACE; i++) {
        ptr = breakpoints[i];
        while (ptr) {
            if (ptr->brkpt->brknum == brknum)
                return ptr->brkpt;
            ptr = ptr->next;
        }

        ptr = watchpoints_load[i];
        while (ptr) {
            if (ptr->brkpt->brknum == brknum)
                return ptr->brkpt;
            ptr = ptr->next;
        }

        ptr = watchpoints_store[i];
        while (ptr) {
            if (ptr->brkpt->brknum == brknum)
                return ptr->brkpt;
            ptr = ptr->next;
        }
    }

    return NULL;
}

void mon_breakpoint_switch_checkpoint(int op, int breakpt_num)
{
    breakpoint_t *bp;
    bp = find_checkpoint(breakpt_num);

    if (!bp) {
        mon_out("#%d not a valid breakpoint\n", breakpt_num);
    } else {
        bp->enabled = op;
        mon_out("Set breakpoint #%d to state: %s\n",
                  breakpt_num, (op == e_ON) ? "enabled" : "disabled");
    }
}

void mon_breakpoint_set_ignore_count(int breakpt_num, int count)
{
    breakpoint_t *bp;
    bp = find_checkpoint(breakpt_num);

    if (!bp) {
        mon_out("#%d not a valid breakpoint\n", breakpt_num);
    } else {
        bp->ignore_count = count;
        mon_out("Ignoring the next %d crossings of breakpoint #%d\n",
                  count, breakpt_num);
    }
}

static void print_checkpoint_info(breakpoint_t *bp)
{
    if (bp->trace) {
        mon_out("TRACE: ");
    } else if (bp->watch_load || bp->watch_store) {
        mon_out("WATCH: ");
    } else {
        if (bp->temporary)
            mon_out("UNTIL: ");
        else
            mon_out("BREAK: ");
    }
    mon_out("%d %s:$%04x",bp->brknum,
        mon_memspace_string[addr_memspace(bp->start_addr)],addr_location(bp->start_addr));
    if (mon_is_valid_addr(bp->end_addr) && (bp->start_addr != bp->end_addr))
        mon_out("-$%04x",addr_location(bp->end_addr));

    if (bp->watch_load)
        mon_out(" load");
    if (bp->watch_store)
        mon_out(" store");

    mon_out("   %s\n", (bp->enabled==e_ON) ? "enabled" : "disabled");

    if (bp->condition) {
        mon_out("\tCondition: ");
        mon_print_conditional(bp->condition);
        mon_out("\n");
    }
    if (bp->command)
        mon_out("\tCommand: %s\n", bp->command);
}

void mon_breakpoint_print_checkpoints(void)
{
    int i, any_set = 0;
    breakpoint_t *bp;

    for (i = 1; i < breakpoint_count; i++) {
        if ((bp = find_checkpoint(i))) {
            print_checkpoint_info(bp);
            any_set = 1;
        }
    }

    if (!any_set)
        mon_out("No breakpoints are set\n");
}

void mon_breakpoint_delete_checkpoint(int brknum)
{
    int i;
    breakpoint_t *bp = NULL;
    MEMSPACE mem;

    if (brknum == -1) {
        /* Add user confirmation here. */
        mon_out("Deleting all breakpoints\n");
        for (i = 1; i < breakpoint_count; i++) {
            bp = find_checkpoint(i);
            if (bp)
                mon_breakpoint_delete_checkpoint(i);
        }
    }
    else if (!(bp = find_checkpoint(brknum))) {
        mon_out("#%d not a valid breakpoint\n", brknum);
        return;
    } else {
        mem = addr_memspace(bp->start_addr);

        if (!(bp->watch_load) && !(bp->watch_store)) {
            remove_checkpoint_from_list(&(breakpoints[mem]), bp);

            if (!any_breakpoints(mem)) {
                monitor_mask[mem] &= ~MI_BREAK;
                if (!monitor_mask[mem])
                    interrupt_monitor_trap_off(mon_interfaces[mem]->int_status);            }
        } else {
            if (bp->watch_load)
                remove_checkpoint_from_list(&(watchpoints_load[mem]), bp);
            if (bp->watch_store)
                remove_checkpoint_from_list(&(watchpoints_store[mem]), bp);

            if (!any_watchpoints(mem)) {
                monitor_mask[mem] &= ~MI_WATCH;
                mon_interfaces[mem]->toggle_watchpoints_func(0,
                    mon_interfaces[mem]->context);

                if (!monitor_mask[mem])
                    interrupt_monitor_trap_off(mon_interfaces[mem]->int_status);            }
        }
    }
    if (bp != NULL) {
        mon_delete_conditional(bp->condition);
        if (bp->command)
            lib_free(bp->command);
    }
}

void mon_breakpoint_set_checkpoint_condition(int brk_num,
                                             cond_node_t *cnode)
{
    breakpoint_t *bp;
    bp = find_checkpoint(brk_num);

    if (!bp) {
        mon_out("#%d not a valid breakpoint\n", brk_num);
    } else {
        bp->condition = cnode;

        mon_out("Setting breakpoint %d condition to: ", brk_num);
        mon_print_conditional(cnode);
        mon_out("\n");
    }
}


void mon_breakpoint_set_checkpoint_command(int brk_num, char *cmd)
{
    breakpoint_t *bp;
    bp = find_checkpoint(brk_num);

    if (!bp) {
        mon_out("#%d not a valid breakpoint\n", brk_num);
    } else {
        bp->command = cmd;
        mon_out("Setting breakpoint %d command to: %s\n",
                  brk_num, cmd);
    }
}

static break_list_t *search_checkpoint_list(break_list_t *head, unsigned loc)
{
    break_list_t *cur_entry;

    cur_entry = head;

    /* The list should be sorted in increasing order. If the current entry
       is > than the search item, we can drop out early.
    */
    while (cur_entry) {
        if (mon_is_in_range(cur_entry->brkpt->start_addr,
            cur_entry->brkpt->end_addr, loc))
            return cur_entry;

        cur_entry = cur_entry->next;
    }

    return NULL;
}

static int compare_checkpoints(breakpoint_t *bp1, breakpoint_t *bp2)
{
    unsigned addr1, addr2;
    /* Returns < 0 if bp1 < bp2
               = 0 if bp1 = bp2
               > 0 if bp1 > bp2
    */

    addr1 = addr_location(bp1->start_addr);
    addr2 = addr_location(bp2->end_addr);

    if (addr1 < addr2)
        return -1;

    if (addr1 > addr2)
        return 1;

    return 0;
}

bool monitor_breakpoint_check_checkpoint(MEMSPACE mem, WORD addr,
                                         break_list_t *list)
{
    break_list_t *ptr;
    breakpoint_t *bp;
    bool result = FALSE;
    MON_ADDR temp;
    const char *type;

    ptr = search_checkpoint_list(list, addr);

    while (ptr && mon_is_in_range(ptr->brkpt->start_addr,
           ptr->brkpt->end_addr, addr)) {
        bp = ptr->brkpt;
        ptr = ptr->next;
        if (bp && bp->enabled==e_ON) {
            /* If condition test fails, skip this checkpoint */
            if (bp->condition) {
                if (!mon_evaluate_conditional(bp->condition)) {
                    continue;
                }
            }

            /* Check if the user specified some ignores */
            if (bp->ignore_count) {
                bp->ignore_count--;
                continue;
            }

            bp->hit_count++;

            result = TRUE;

            temp = new_addr(mem,
                            (monitor_cpu_type.mon_register_get_val)(mem, e_PC));            if (bp->trace) {
                type = "Trace";
                result = FALSE;
            }
            else if (bp->watch_load)
                type = "Watch-load";
            else if (bp->watch_store)
                type = "Watch-store";
            else
                type = "Break";

            /*archdep_open_monitor_console(&mon_input, &mon_output);*/
            mon_out("#%d (%s) ", bp->brknum, type);
            mon_disassemble_instr(temp);

            if (bp->command) {
                mon_out("Executing: %s\n", bp->command);
                parse_and_execute_line(bp->command);
            }

            if (bp->temporary)
                mon_breakpoint_delete_checkpoint(bp->brknum);
        }
    }
    return result;
}

static void add_to_checkpoint_list(break_list_t **head, breakpoint_t *bp)
{
    break_list_t *new_entry, *cur_entry, *prev_entry;

    new_entry = (break_list_t *)lib_malloc(sizeof(break_list_t));
    new_entry->brkpt = bp;

    cur_entry = *head;
    prev_entry = NULL;

    /* Make sure the list is in increasing order. (Ranges are entered
       based on the lower bound) This way if the searched for address is
       less than the current ptr, we can skip the rest of the list. Note
       that ranges that wrap around 0xffff aren't handled in this scheme.
       Suggestion: Split the range and create two entries.
    */
    while (cur_entry && (compare_checkpoints(cur_entry->brkpt, bp) <= 0) ) {
        prev_entry = cur_entry;
        cur_entry = cur_entry->next;
    }

    if (!prev_entry) {
        *head = new_entry;
        new_entry->next = cur_entry;
        return;
    }

    prev_entry->next = new_entry;
    new_entry->next = cur_entry;
}

static 
int breakpoint_add_checkpoint(MON_ADDR start_addr, MON_ADDR end_addr,
                                  bool is_trace, bool is_load, bool is_store,
                                  bool is_temp, bool do_print)
{
    breakpoint_t *new_bp;
    MEMSPACE mem;
    long len;

    len = mon_evaluate_address_range(&start_addr, &end_addr, FALSE, 0);
    new_bp = (breakpoint_t *)lib_malloc(sizeof(breakpoint_t));

    new_bp->brknum = breakpoint_count++;
    new_bp->start_addr = start_addr;
    new_bp->end_addr = end_addr;
    new_bp->trace = is_trace;
    new_bp->enabled = e_ON;
    new_bp->hit_count = 0;
    new_bp->ignore_count = 0;
    new_bp->condition = NULL;
    new_bp->command = NULL;
    new_bp->watch_load = is_load;
    new_bp->watch_store = is_store;
    new_bp->temporary = is_temp;

    mem = addr_memspace(start_addr);
    if (!is_load && !is_store) {
        if (!any_breakpoints(mem)) {
            monitor_mask[mem] |= MI_BREAK;
            interrupt_monitor_trap_on(mon_interfaces[mem]->int_status);
        }

        add_to_checkpoint_list(&(breakpoints[mem]), new_bp);
    } else {
        if (!any_watchpoints(mem)) {
            monitor_mask[mem] |= MI_WATCH;
            mon_interfaces[mem]->toggle_watchpoints_func(1,
                mon_interfaces[mem]->context);
            interrupt_monitor_trap_on(mon_interfaces[mem]->int_status);
        }

        if (is_load)
            add_to_checkpoint_list(&(watchpoints_load[mem]), new_bp);
        if (is_store)
            add_to_checkpoint_list(&(watchpoints_store[mem]), new_bp);
    }

    if (is_temp)
        exit_mon = 1;

    if (do_print)
        print_checkpoint_info(new_bp);

    return new_bp->brknum;
}

int mon_breakpoint_add_checkpoint(MON_ADDR start_addr, MON_ADDR end_addr,
                                  bool is_trace, bool is_load, bool is_store,
                                  bool is_temp)
{
    return breakpoint_add_checkpoint(start_addr, end_addr,
                                  is_trace, is_load, is_store,
                                  is_temp, TRUE );
}

mon_breakpoint_type_t mon_breakpoint_is(MON_ADDR address)
{
    MEMSPACE mem = addr_memspace(address);
    WORD addr = addr_location(address);
    break_list_t *ptr;

    ptr = search_checkpoint_list(breakpoints[mem], addr);
    
    if (!ptr)
        return BP_NONE;

    return (ptr->brkpt->enabled == e_ON) ? BP_ACTIVE : BP_INACTIVE;
}

void mon_breakpoint_set(MON_ADDR address)
{
    MEMSPACE mem = addr_memspace(address);
    WORD addr = addr_location(address);
    break_list_t *ptr;

    ptr = search_checkpoint_list(breakpoints[mem], addr);
    
    if (ptr) {
        /* there's a breakpoint, so enable it */
        ptr->brkpt->enabled = e_ON;
    } else {
        /* there's no breakpoint, so set a new one */
        breakpoint_add_checkpoint(address, address,
                                  FALSE, FALSE, FALSE, FALSE, FALSE );
    }
}

void mon_breakpoint_unset(MON_ADDR address)
{
    MEMSPACE mem = addr_memspace(address);
    WORD addr = addr_location(address);
    break_list_t *ptr;

    ptr = search_checkpoint_list(breakpoints[mem], addr);
    
    if (ptr) {
        /* there's a breakpoint, so remove it */
        remove_checkpoint_from_list( &breakpoints[mem], ptr->brkpt );
    }
}

void mon_breakpoint_enable(MON_ADDR address)
{
    MEMSPACE mem = addr_memspace(address);
    WORD addr = addr_location(address);
    break_list_t *ptr;

    ptr = search_checkpoint_list(breakpoints[mem], addr);
    
    if (ptr) {
        /* there's a breakpoint, so enable it */
        ptr->brkpt->enabled = e_ON;
    }
}

void mon_breakpoint_disable(MON_ADDR address)
{
    MEMSPACE mem = addr_memspace(address);
    WORD addr = addr_location(address);
    break_list_t *ptr;

    ptr = search_checkpoint_list(breakpoints[mem], addr);
    
    if (ptr) {
        /* there's a breakpoint, so disable it */
        ptr->brkpt->enabled = e_OFF;
    }
}

