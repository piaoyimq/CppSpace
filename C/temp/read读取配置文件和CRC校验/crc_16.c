#include "crc_16.h"

unsigned short crc=0xffff;



unsigned short const crc_ta[256]={ //X16+X12+X5+1 余式表
0x0000,0x1021, 0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
0x6e1,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};




unsigned short table_crc_16(unsigned char* ptr,unsigned int len)// 字节查表法求CRC
{
	unsigned int da;
	while(len--!=0)
	{
		da=(unsigned int)(crc/256); // 以8位二进制数暂存CRC的高8位
		crc<<=8; // 左移8位
		crc^=crc_ta[da^*ptr]; // 高字节和当前数据XOR再查表
		ptr++;
	}
	return(crc);
}




unsigned short crc_file_pos_16(char *file_name,int file_pos)
{
	FILE *fp;
	unsigned short CRC=0;
	int left,nb;
	char buffer[BUFFER_SIZE];
	crc=0xffff;
	fp=fopen(file_name,"r");	
	if((fp==NULL))
	{
		printf("luh file crc_16  open error\n");
		return -1;
	}
	left=file_pos;
	if(left<BUFFER_SIZE)
	{
		nb=fread(buffer,sizeof(char),left,fp);
		if(nb>0)
				CRC=table_crc_16(buffer,nb);		
	}
	else
	{
		while(1)
		{
			if(left<BUFFER_SIZE)
			{
				printf("left=%d\n",left);
				nb=fread(buffer,sizeof(char),left,fp);
				printf("nb=%d\n",nb);
				if(nb>0)
						CRC=table_crc_16(buffer,nb);	
				break;
			}
			else
			{
				nb=fread(buffer,sizeof(char),BUFFER_SIZE,fp);
				if(nb>0)
						CRC=table_crc_16(buffer,nb);		
				left=left-BUFFER_SIZE;
			}
		}
	}
	fclose(fp);
	return CRC;
}




unsigned short crc_16(char *filename)//对单个文件进行校验
{
	unsigned short CRC=0;
	unsigned int nb;
	char buffer[BUFFER_SIZE];
	FILE *fp;
	crc=0xffff;
	if((fp=fopen(filename,"rb"))==NULL)
	{
		printf("crc open error\n");
		return -1;
	}
	while(!feof(fp))
	{
		nb=fread(buffer,sizeof(char),BUFFER_SIZE,fp);
		if(nb>0)
			CRC=table_crc_16(buffer,nb);			
	}
	fclose(fp);
	//printf("CRC=0x%x\n",CRC);
	return CRC;
}





unsigned short multi_crc_16(struct multi_file_crc file)
{
	/*   对所有的数据文件和 所有的支持文件以
	  *	及luh指定位置的文件内容进行校验
	  */

	unsigned short CRC=0;
	unsigned int nb;
	int i,left;
	char buffer[BUFFER_SIZE];
	FILE *fp;
	crc=0xffff;
	//printf("file.data_file_num=%d\n",file.data_file_num);
	
	/******all data files ******/
	for(i=0;i<file.data_file_num;i++)
	{
		fp=fopen(file.data_file_name[i],"r");
		//printf("*****%s\n",file.data_file_name[i]);
		if((fp==NULL))
		{
			printf("all data files open error\n");
			return -1;
		}
		while(!feof(fp))
		{
			nb=fread(buffer,sizeof(char),BUFFER_SIZE,fp);
			if(nb>0)
				CRC=table_crc_16(buffer,nb);			
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
			nb=fread(buffer,sizeof(char),BUFFER_SIZE,fp);
			if(nb>0)
				CRC=table_crc_16(buffer,nb);			
		}
		fclose(fp);
	}

		/******luh 指定位置******/
	fp=fopen(file.luh_file_name,"r");
	printf("luh file name :%s\n",file.luh_file_name);
	if((fp==NULL))
	{
		printf("luh files  open error\n");
		return -1;
	}
	left=file.luh_length;
	if(left<BUFFER_SIZE)
	{
		nb=fread(buffer,sizeof(char),left,fp);
		if(nb>0)
				CRC=table_crc_16(buffer,nb);		
	}
	else
	{
		while(1)
		{
			if(left<BUFFER_SIZE)
			{
				nb=fread(buffer,sizeof(char),left,fp);
				if(nb>0)
						CRC=table_crc_16(buffer,nb);		
				break;
			}
			else
			{
				nb=fread(buffer,sizeof(char),BUFFER_SIZE,fp);
				if(nb>0)
						CRC=table_crc_16(buffer,nb);		
				left=left-BUFFER_SIZE;
			}
		}
	}
	fclose(fp);
	return CRC;
}




	



#if 0
int main(int argc,char *argv[])
{
#if 0
	unsigned short a,b,c;
	char data_files[10][50]={"data_1","data_2"};
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

	int fpos;
	fp=fopen("data_4","r");
	fseek(fp,2,0);
	fpos=ftell(fp);
	printf("fpos=%d\n",fpos);
	fclose(fp);
	//struct multi_file_crc file={{"data_1","data_2","data_3","data_4"},4};
	struct multi_file_crc file={(char *)data_files,2,{"data_3"},1,"data_4",2};
	/*
	memcpy(file.data_file_name,data_files,sizeof(data_files));
	printf("###########%s\n",file.data_file_name[0]);
	printf("###########%s\n",file.data_file_name[1]);
	file.data_file_num=2;
	memcpy(file.su_file_name,"data_3",strlen("data_3"));
	file.su_file_num=1;
	strcpy(file.luh_file_name,"data_4");
	file.luh_length=2;
	*/

	
	crc=0;
	a=multi_crc_16(file);
	crc=0;
	b=crc_16("data");	
	crc=0;
	c=table_crc_16(crcbuff,8);
	printf("a=%x\n",a);
	printf("b=%x\n",b);
	printf("c=%x\n",c);
#else
	

	FILE *fp;
	unsigned short crc_data;
	fp=fopen("good","wb");
	char test[256];
	int i;
	for(i=0;i<256;i++)
		test[i]=(char)i;
	fwrite(test,sizeof(test),1,fp);
	fclose(fp);
	crc_data=crc_16("good");
	printf("%x\n",crc_data);

#endif

}
#endif








