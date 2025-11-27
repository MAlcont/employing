#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>   // for getopt on POSIX systems
#include <stdlib.h>

#include <common.h>
#include <file.h>
#include <parse.h>
#include "extra_tasks.h"

void print_usage(char *argv[]) {
	printf("Usage: %s -n -f <database file>\n", argv[0]);
	printf("or: %s -f <database file> -flag for action\n", argv[0]);
	printf("\t -n - create a new db file\n");
	printf("\t -f - (required) path to db file\n");
	printf("\t -l - list the employees\n");
	printf("\t -a - add employee via CSV name, adddress, salry\n");
	printf("\t -r - remove employee via index in the list\n");
	printf("\t -h - update work time via ''index,hours''\n");

	return;
}


int main(int argc, char *argv[]) {

	char *filepath = NULL;
	bool newfile = false;
	int c;
	struct employee_t *employees= NULL;
	char *addstring = NULL;
	bool list = false;

	int dbfd = -1;
	struct dbheader_t *dbhdr = NULL;

	char *rem_index_str = NULL;
	unsigned int rem_index = 0;
	bool do_remove = false;

	char *hours_arg = NULL;
	bool do_set_hours = false;
	unsigned int hours_index = 0;
	unsigned int new_hours = 0;

	while ((c = getopt(argc, argv, "nf:a:lr:h:")) != -1){

		switch (c) {
			case 'n':
				newfile = true;
				break;
			case 'f':
				filepath = optarg;
				break;
			case 'a':
		        addstring = optarg;
		        break;
	        case 'r':
		        do_remove = true;
		        rem_index = (unsigned int)atoi(optarg);
		        if (rem_index > 0) {
		            rem_index--;     // CLI 1-based -> internal 0-based
		        }
		        break;
		    case 'h': {
		        unsigned int idx = 0;
		        unsigned int hrs = 0;

		        if (sscanf(optarg, "%u,%u", &idx, &hrs) != 2) {
		            fprintf(stderr, "Invalid -h argument, expected index,hours (e.g. \"2,50\")\n");
		            return STATUS_ERROR;
		        }

		        if (idx > 0) {
		            idx--;           // 1-based -> 0-based
		        }

		        hours_index = idx;
		        new_hours   = hrs;
		        do_set_hours = true;
		        break;
		    }
	        case 'l':
			    list = true;
			    break;
			case '?':
				printf("Unknown option -%c\n", c);
				break;
			default:
				return -1;
		}
	}

		if (filepath == NULL){
			printf("filepath is a required argument\n");
			print_usage(argv);
			return 0;
		}
		if (newfile){
			printf("\n\n");
		    dbfd = create_db_file(filepath);
		    printf("\n");
			print_usage(argv);
			printf("\n");

		    if (dbfd == STATUS_ERROR) {
		        printf("Unable to create database file\n");
		        return -1;
		    }

		    if (create_db_header(&dbhdr) == STATUS_ERROR) {
		        printf("Failed to create db header\n");
		        return -1;
		    }
		}
		else{
			dbfd = open_db_file(filepath);

			if (dbfd == STATUS_ERROR){
				printf("Unable to open database file\n");
				return -1;
			}
			if (validate_db_header(dbfd, &dbhdr) == STATUS_ERROR){
				printf("Failed to vaildate db header\n");
				return -1;
			}
		}
			if (read_employees(dbfd, dbhdr, &employees) != STATUS_SUCCESS){
				printf("Failed to read employees");
				return 0;
			}
			if (addstring){
				add_employee(dbhdr, &employees, addstring);
				printf("\n\n");
			}
			if (do_remove) {
			    rem_employee(dbhdr, &employees, rem_index);
			}

			if (do_set_hours) {
			    set_employee_hours(dbhdr, employees, hours_index, new_hours);
			}
			if (list){
				list_employees(dbhdr, employees);
			}

	output_file(dbfd, dbhdr, employees);

return 0;
}
