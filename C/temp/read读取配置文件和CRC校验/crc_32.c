#include "crc_32.h"

 
// Global (file scope) Variables 
	static char     Buffer[BUFFER_SIZE]; // The file buffer 
    static unsigned int  CRC_value;       // Holds the running CRC value 
    static  FILE*    fp;              // File pointer 
    static  unsigned int   nb;              // Number of bytes read from file 
    static  unsigned  int  table[TABLEN];   // Look-up Table 
 

	  
void init_table_32() 
{ 
     unsigned int generator = G_32;        // CRC generator 
     unsigned int shift_reg;            // A shift register 
     unsigned int leading_bit;          // MSB of shift_reg before shift 
     int     i;                    // Index into table 0..TABLEN 
     int     k;                    // Bit index into byte 7..0 
 
     for (i=0; i<TABLEN; i++) 
     { 
          shift_reg = ((unsigned int)i << 24); 
          for (k=7; k>=0; k--) 
          { 
               leading_bit = shift_reg & 0x80000000; 
               shift_reg = shift_reg << 1; 
               if (leading_bit) 
               { 
                    shift_reg = shift_reg ^ generator; 
               } 
          } 
          table[i] = shift_reg; 
     } 
} 
 

 void table_crc_32(int nb) 
{ 
     int i;   // Byte index into buffer 
     int k;   // Index into look-up table 
 
     for (i=0; i<nb; i++) 
     { 
          k = ( (CRC_value>>24) ^ (int)Buffer[i] ) & 0xFF; 
           CRC_value = (CRC_value << 8) ^ table[k]; 
     } 
}


 
unsigned int crc_32(char *filename)
{
	
	memset(Buffer,0,BUFFER_SIZE);
	if ((fp=fopen(filename,"rb"))== NULL) 
     { 
          printf("Error: Can't open input file\n"); 
		  return -1;
     } 
     init_table_32(); 
     CRC_value =Init_32; 
     while (!feof(fp)) 
     { 
          nb=fread(Buffer,sizeof(char),BUFFER_SIZE,fp); 
          if (nb>0) 
          { 
               table_crc_32(nb); 
          } 
     } 
//	 CRC_value ^= XorOut; 
	 fclose(fp); 
	 return (CRC_value^XorOut);

} 


unsigned int crc_file_pos_32(char *file_name,int file_pos)
{
	FILE *fp;
	int left,nb;
	init_table_32() ;
	CRC_value = Init_32; 
	memset(Buffer,0,BUFFER_SIZE);
	fp=fopen(file_name,"r");	
	if((fp==NULL))
	{
		printf("luh file crc_32  open error\n");
		return -1;
	}
	left=file_pos;
	if(left<BUFFER_SIZE)
	{
		nb=fread(Buffer,sizeof(char),left,fp);
		if(nb>0)
			table_crc_32(nb); 
	}
	else
	{
		while(1)
		{
			if(left<BUFFER_SIZE)
			{
				printf("left=%d\n",left);
				nb=fread(Buffer,sizeof(char),left,fp);
				printf("nb=%d\n",nb);
				if(nb>0)
					table_crc_32(nb); 
				break;
			}
			else
			{
				nb=fread(Buffer,sizeof(char),BUFFER_SIZE,fp);
				if(nb>0)
					table_crc_32(nb); 
				left=left-BUFFER_SIZE;
			}
		}
	}
	fclose(fp);
	return (CRC_value^XorOut);
}



unsigned int multi_crc_32(struct multi_file_crc file)
{
	unsigned int nb;
	int i,left;
	FILE *fp;
	unsigned int CRC_data;
	init_table_32() ;
	CRC_value = Init_32; 
	memset(Buffer,0,BUFFER_SIZE);
	init_table_32(); 

	/******all data files ******/
	for(i=0;i<file.data_file_num;i++)
	{
		fp=fopen(file.data_file_name[i],"r");
		if((fp==NULL))
		{
			printf("all data files open error\n");
			return -1;
		}
		while(!feof(fp))
		{
			nb=fread(Buffer,sizeof(char),BUFFER_SIZE,fp);
			if(nb>0)
				table_crc_32(nb); 
		}
		fclose(fp);

	}

	/******all support files ******/	
	for(i=0;i<file.su_file_num;i++)
	{
		fp=fopen(file.su_file_name[i],"r");
		if((fp==NULL))
		{
			printf("all support files open error\n");
			return -1;
		}
		while(!feof(fp))
		{
			nb=fread(Buffer,sizeof(char),BUFFER_SIZE,fp);
			if(nb>0)
				table_crc_32(nb); 
		}
		fclose(fp);
	}

	/******luh ÷∏∂®Œª÷√******/
	fp=fopen(file.luh_file_name,"r");
	if((fp==NULL))
	{
		printf("luh files  open error\n");
		return -1;
	}
	left=file.luh_length;
	if(left<BUFFER_SIZE)
	{
		nb=fread(Buffer,sizeof(char),left,fp);
		if(nb>0)
			table_crc_32(nb); 
	}
	else
	{
		while(1)
		{
			if(left<BUFFER_SIZE)
			{
				nb=fread(Buffer,sizeof(char),left,fp);
				if(nb>0)
					table_crc_32(nb); 
				break;
			}
			else
			{
				nb=fread(Buffer,sizeof(char),BUFFER_SIZE,fp);
				if(nb>0)
					table_crc_32(nb); 
				left=left-BUFFER_SIZE;
			}
		}
	}
	fclose(fp);
	return (CRC_value^XorOut);
}


#if 0
int main()
{

#if 0
	unsigned int a,b,c;
	char data_files[10][50]={"data_1","data_2","data_3","data_4"};
	FILE *fp;
	fp=fopen("data","wb");
	fwrite(crcbuff,sizeof(crcbuff),1,fp);
	fclose(fp);

	fp=fopen("data_1","wb");
	fwrite(crcbuff_2,sizeof(crcbuff_2),1,fp);
	fclose(fp);

	fp=fopen("data_2","wb");
	fwrite(crcbuff_3,sizeof(crcbuff_3),1,fp);
	fclose(fp);

	fp=fopen("data_3","wb");
	fwrite(crcbuff_4,sizeof(crcbuff_4),1,fp);
	fclose(fp);

	fp=fopen("data_4","wb");
	fwrite(crcbuff_5,sizeof(crcbuff_5),1,fp);
	fclose(fp);
/*	int fpos;
	fp=fopen("data_4","r");
	fseek(fp,2,0);
	fpos=ftell(fp);
	printf("fpos=%d\n",fpos);
	fclose(fp);
*/

	//struct multi_file_crc file={{"data_1","data_2","data_3","data_4"},4};
	struct multi_file_crc file={(char *)data_files,2,{"data_3"},1,"data_4",4};


	

	a=multi_crc_32(file);
	b=crc_32("data");	


	printf("a=%x\n",a);
	printf("b=%x\n",b);
//	printf("c=%x\n",c);

#else
	unsigned int crc_data;
	int file_pos=1047;
	FILE *fp;
	fp=fopen("good","wb");
	char test[256];
	int i;
	for(i=0;i<256;i++)
		test[i]=(char)i;
	fwrite(test,sizeof(test),1,fp);
	fclose(fp);
	crc_data=crc_32("good");
	printf("%x\n",crc_data);

#endif
	
}
#endif
