#ifndef _CRC_32_H
#define _CRC_32_H
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include "common.h"


// Preprocessor Constants 
#define byte     char 
//#define BUF_SIZE  1024        // Size of file buffer in bytes 
#define G_32        0x04C11DB7  // The generator G 
#define Init_32     0xFFFFFFFF  // Initialization value for CRC_value 
#define XorOut   0xFFFFFFFF  // To be XORed to final CRC_value 




unsigned int crc_32(char *filename);
unsigned int crc_file_pos_32(char *file_name,int file_pos);
unsigned int multi_crc_32(struct multi_file_crc file);

#endif
