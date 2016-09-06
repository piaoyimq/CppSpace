#ifndef _COMMON_H
#define _COMMON_H


typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;


#define FAILED    -1
#define OK 		   0

#define VAR_ERROR  -1



#define BUFFER_SIZE 1024
#define TABLEN   256         // Length of look-up table 

struct multi_file_crc
{
	char data_file_name[10][50];
	//char *data_file_name;
	char data_file_num; 
	char su_file_name[10][50];
	char su_file_num;
	char luh_file_name[50];
	int luh_length;
	
};


#endif
