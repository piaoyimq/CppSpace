#ifndef _CRC_8_H
#define _CRC_8_H
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include "common.h"


#define G_8      0x01  // The generator G 
#define Init_8   0x00  // Initialization value for CRC_value 


void init_table_8() ;
unsigned  char	table_crc_8(unsigned  char 	*buff,	 unsigned  int  len);   
unsigned char crc_8(char *filename);
unsigned char multi_crc_8(struct multi_file_crc file);


#endif

