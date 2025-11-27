#ifndef EXTRA_TASKS_H
#define EXTRA_TASKS_H

#include "common.h"
#include "parse.h"   // for struct dbheader_t, struct employee_t

int rem_employee(struct dbheader_t *dbhdr,
                 struct employee_t **employees,
                 unsigned int index);

int set_employee_hours(struct dbheader_t *dbhdr,
                       struct employee_t *employees,
                       unsigned int index,
                       unsigned int new_hours);

#endif /* EXTRA_TASKS_H */
