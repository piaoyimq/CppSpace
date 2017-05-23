#ifndef _CRC_16_H
#define _CRC_16_H

#include <stdio.h>
#include "common.h"

unsigned short table_crc_16(unsigned char* ptr,unsigned int len);
unsigned short crc_16(char * filename);
unsigned short crc_file_pos_16(char *file_name,int file_pos);

#endif
