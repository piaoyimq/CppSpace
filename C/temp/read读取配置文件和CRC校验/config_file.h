#ifndef _CONFIG_FILE_H
#define _CONFIG_FILE_H

#include <stdint.h> 
#include <stdio.h>
#include <syslog.h>
#define    MAX_PATH_LEN         (100)
#define    MAX_FILE_NAME_LEN    (100)

#define CRC_8_TYPE 2
#define CRC_16_TYPE 2
#define CRC_32_TYPE 4



int parse_config_file(char *path_to_config_file);
char * get_config_var(char *var_name);
void make_config_to_LUH(void);
#endif
