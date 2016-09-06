#include "crc_8.h"

 
// Global (file scope) Variables 
static unsigned char  CRC_value;       // Holds the running CRC value 
static  FILE*    fp;              // File pointer 
static  unsigned int   nb;              // Number of bytes read from file 
static  unsigned  short  table[TABLEN];   // Look-up Table 
unsigned   char   CRC8 =   0x00;			//CRC8字节初始化   




void init_table_8() 
{ 
     unsigned short generator = G_8;        // CRC generator 
     unsigned char shift_reg;            // A shift register 
     unsigned short leading_bit;          // MSB of shift_reg before shift 
     unsigned short   i;                    // Index into table 0..TABLEN 
     int     k;                    // Bit index into byte 7..0 
 
     for (i=0; i<TABLEN; i++) 
     { 
         shift_reg = (unsigned char)i; 
         for (k=0; k<8; k++) 
         { 
			if(shift_reg&0x80)
				shift_reg=(shift_reg<<1)^generator;
			else
				shift_reg=shift_reg<<1;
         } 
         table[i] = (unsigned char)shift_reg; 
		 // printf("0x%x	",table[i]);
     } 
	// printf("\n\n");
} 









unsigned  char	table_crc_8(unsigned  char 	*buff,	 unsigned  int  len)   
{	// 查表计算方法
	//	进行CRC8位校验	 
	while (len--)
    {	
		CRC8=table[CRC8^(*buff++)];
	}	 
	return(CRC8);	 //返回CRC8校验数据	
} 





unsigned char crc_8(char *filename)
{
	unsigned char  buffer[BUFFER_SIZE]={0}; // The file buffer 
	init_table_8(); 	
	CRC8 =Init_8; 
	if ((fp=fopen(filename,"rb"))== NULL) 
    { 
          printf("Error: Can't open input file\n"); 
		  return -1;
    } 
    while (!feof(fp)) 
    {
     	  int i;
          nb=fread(buffer,sizeof(char),BUFFER_SIZE,fp); 
          if (nb>0) 
          { 
          	table_crc_8(buffer,nb);
          } 
    } 
	fclose(fp); 
	return CRC8;
}




 
 
 
 
 
 unsigned char multi_crc_8(struct multi_file_crc file)
 {
	 //unsigned int nb;
	 int i,left,j;
	 unsigned char buff[BUFFER_SIZE];
	 FILE *fp;
	 
	 init_table_8();	 
	 CRC8=Init_8;
 
	 /******all data files ******/
	 for(i=0;i<file.data_file_num;i++)
	 {
		 fp=fopen(file.data_file_name[i],"r");
		 printf("*****%s\n",file.data_file_name[i]);
		 if((fp==NULL))
		 {
			 printf("all data files open error\n");
			 return -1;
		 }
		 while(!feof(fp))
		 {
			 nb=fread(buff,sizeof(char),BUFFER_SIZE,fp);		 
			 if(nb>0)
				 table_crc_8(buff,nb);
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
			 nb=fread(buff,sizeof(char),BUFFER_SIZE,fp);
			 if(nb>0)
				 table_crc_8(buff,nb);			 
		 }
		 fclose(fp);
	 }
 
		 /******luh 指定位置******/
	 fp=fopen(file.luh_file_name,"r");
	 if((fp==NULL))
	 {
		 printf("luh files	open error\n");
		 return -1;
	 }
	 left=file.luh_length;
	 if(left<BUFFER_SIZE)
	 {
		 nb=fread(buff,sizeof(char),left,fp);
		 if(nb>0)
			table_crc_8(buff,nb);		 
	 }
	 else
	 {
		 while(1)
		 {
			 if(left<BUFFER_SIZE)
			 {
				 nb=fread(buff,sizeof(char),left,fp);
				 if(nb>0)
						table_crc_8(buff,nb);		 
				 break;
			 }
			 else
			 {
				 nb=fread(buff,sizeof(char),BUFFER_SIZE,fp);
				 if(nb>0)
						 table_crc_8(buff,nb);		 
				 left=left-BUFFER_SIZE;
			 }
		 }
	 }
	 fclose(fp);
	 return CRC8;
 }





 #if 0
 void	main()	 
 {	 
		 unsigned	char   data[32]   =   {1,2,3,4,5,6,7,8,9,10};	
		 unsigned	char   crc_v   =   0;	
		 
		 init_tabl();
		 crc_v	 =	 count_crc8(   data,   31,	 0	 );   
		 printf("crc_v=0x%x\n",crc_v);
		 data[31]	=	crc_v;	 
		 crc_v	 =	 count_crc8(   data,   32,	 0	 );   
		 printf("crc_v=0x%x\n",crc_v);
		 unsigned int datalen=32;
		 crc_v=FindCRC(data,datalen);
		 printf("find  crc_v=0x%x\n",crc_v);

 }	 
#endif






#if 0
int main()
{

#if 0
	unsigned short a,b,c;
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


//	struct multi_file_crc file={(char *)data_files,4};
	struct multi_file_crc file={(char *)data_files,2,{"data_3"},1,"data_4",2};
	
	
	a=multi_crc_8(file);
	
	b=crc_8("data");	
	printf("a=%x\n",a);
	printf("b=%x\n",b);
	
#else
	char test[256];	
	unsigned short b;
	int i;
	FILE *fp;
	for(i=0;i<256;i++)
		test[i]=i;
	
	fp=fopen("data","wb");
	fwrite(test,sizeof(test),1,fp);
	fclose(fp);
	b=crc_8("data");	
	printf("b=%x\n",b);
#endif
}
#endif

