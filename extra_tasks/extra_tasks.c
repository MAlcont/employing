#include <string.h>
#include "extra_tasks.h"

int rem_employee(struct dbheader_t *dbhdr,
                 struct employee_t **employees,
                 unsigned int index) {
    if (dbhdr == NULL) return STATUS_ERROR;
    if (employees == NULL) return STATUS_ERROR;
    if (*employees == NULL) return STATUS_ERROR;

    if (dbhdr->count == 0) return STATUS_ERROR;
    if (index >= dbhdr->count) return STATUS_ERROR;

    struct employee_t *e = *employees;

    if (index < dbhdr->count - 1) {
        memmove(&e[index],
                &e[index + 1],
                (dbhdr->count - index - 1) * sizeof(struct employee_t));
    }

    dbhdr->count--;
    return STATUS_SUCCESS;
}

int set_employee_hours(struct dbheader_t *dbhdr,
                       struct employee_t *employees,
                       unsigned int index,
                       unsigned int new_hours) {
    if (dbhdr == NULL) return STATUS_ERROR;
    if (employees == NULL) return STATUS_ERROR;
    if (index >= dbhdr->count) return STATUS_ERROR;

    employees[index].hours = new_hours;
    return STATUS_SUCCESS;
}
