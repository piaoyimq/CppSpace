#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <errno.h> 
#include <ctype.h>
#include <syslog.h> 
#include <fcntl.h> 
#include <sys/resource.h> 
#include <signal.h>
#include "config_file.h"
#include "crc_8.h"
#include "crc_16.h"
#include "crc_32.h"
#include "common.h"

#define    MAX_VAR_NUM          (500)    //100
#define    MAX_VAR_NAME_LEN     (128)
#define    MAX_VAR_VALUE_LEN    (MAX_PATH_LEN)
#define COMMENT_CHARACTER   '#' 
#define LINE_SIZE    (768)

char ga_variables[MAX_VAR_NUM][MAX_VAR_NAME_LEN + 1];
char ga_variables_temp[MAX_VAR_NUM][MAX_VAR_NAME_LEN + 1];
char ga_values[MAX_VAR_NUM][MAX_VAR_VALUE_LEN + 1];
char ga_values_temp[MAX_VAR_NUM][MAX_VAR_VALUE_LEN + 1];
int  g_var_num = 0;
int  g_var_num_temp = 0;





void remove_trailing_chars(char *path, char c)
{ 
    size_t len;
    len = strlen(path);
	while (len > 0 && path[len-1] == c)
    path[--len] = '\0';
}
		
int get_key(char **line, char **key, char **value)
{ 	
	char *linepos;
	char *temp;
    linepos=*line;
	if (!linepos)
	{
		return FAILED;
	}
	while(isspace(linepos[0]))/* skip whitespace */  
	{
			linepos++;
	}
    if (linepos[0]=='\0')
	{
		return FAILED;
	}
	*key=linepos;	 /* get the key */ 	
	if(linepos[0]=='[')
	{	
		temp=strchr(linepos, ']');
		if (!temp)
		{ 
			return FAILED;
		}
	    temp[1]='\0';
		*value=linepos;
		return OK;
	}
	while (1) 
	{ 
		linepos++;
		if (linepos[0]=='\0')
		{
			return FAILED;
		} 
		if(isspace(linepos[0]))
		{
			break;
		}
		if(linepos[0]=='=')
		{
			break;			
		}

	}
	linepos[0]='\0';
    while (1) 
	{ 
		linepos++;
		if (linepos[0] == '\0')
		{ 
			return FAILED;
		} 
		if (isspace(linepos[0]))
		{
			continue;
		}	
		if (linepos[0] == '=')
		{
			continue;
		}
		break;
	}
	if (linepos[0] == '"')
	{ 
		linepos++;
	} 
	else 
	{ 
		return FAILED;
	} 
	*value = linepos;
    temp = strchr(linepos, '"');
	if (!temp)
	{ 
		return FAILED;
	}
	temp[0] = '\0';
	return OK;
}




int select_section(char *var_name)
{
	int i;
	for(i = 0; i < g_var_num; i++)
	{ 
		if(strcasecmp(ga_variables[i], var_name) == 0)
		{
			break;
		}	
	}
	if(i==g_var_num)
	{
		return FAILED;
	}
	memcpy(ga_variables_temp,ga_variables+i,(g_var_num-i)*(MAX_VAR_NAME_LEN + 1));
	memcpy(ga_values_temp,ga_values+i,(g_var_num-i)*(MAX_VAR_VALUE_LEN + 1));
	g_var_num_temp=g_var_num-i;
	return OK;
}



int parse_config_file(char *path_to_config_file)
{
	char line[LINE_SIZE + 2];
	char *bufline;
	char *linepos;
	char *variable;
	char *value;
	char *buf;
	size_t bufsize;
	size_t cur;
	size_t count;
	int lineno;
	int retval = 0;
    FILE *cfg_file=fopen(path_to_config_file, "r");
    if (NULL==cfg_file)
    {
    	printf("can't open '%s' as config file: %s", path_to_config_file, strerror(errno));
		goto EXIT;
	}
		 
   		 /* loop through the whole file */ 
	lineno=0;
	cur=0;
	while(NULL!=fgets(line, sizeof(line), cfg_file))
    { 	
    	lineno++;
		bufline=line;
		//printf("1_bufline:\t%s\n",bufline);
		count=strlen(line);
    	if (count>LINE_SIZE)
        {
       		printf("line too long, conf line skipped %s, line %d", path_to_config_file, lineno);
			continue;
	  	}
   
		 while ((count>0)&&isspace(bufline[0]))	 /*eat the whitespace */
   		 { 
    		bufline++;
			count--;
		 }
	 
		 if (count == 0)
		 {
			continue;
		 }
    		/* see if this is a comment */ 
	     if (bufline[0]==COMMENT_CHARACTER)
	     {
			continue;
		 }
    	 
    	 memcpy(line, bufline, count);
		 line[count]='\0';
     	 linepos=line;
		 retval=get_key(&linepos, &variable, &value);
		 if (retval!=0)
   		 { 
			printf("error parsing %s, line %d:%d\n", path_to_config_file, lineno, (int)(linepos-line));
			continue;
		 }
   		 if (g_var_num>=MAX_VAR_NUM)
   		 { 
			printf("too many vars in  %s, line %d:%d", path_to_config_file, lineno, (int)(linepos-line));
			continue;
		 }
    	if (strlen(variable) > MAX_VAR_NAME_LEN)
   		{
    		printf("var name to long %s, line %d:%d", path_to_config_file, lineno, (int)(linepos-line));
			continue;
		}
   		if (strlen(value) > MAX_VAR_VALUE_LEN)
   		{ 
			printf("value to long %s, line %d:%d", path_to_config_file, lineno, (int)(linepos-line));
			continue;
		}
		strncpy(ga_variables[g_var_num], variable, sizeof(ga_variables[g_var_num]));
		remove_trailing_chars(value, '/');
		strncpy(ga_values[g_var_num], value, sizeof(ga_values[g_var_num]));
		g_var_num++;
		continue;
   	}
	EXIT:
		 fclose(cfg_file);
	return g_var_num;
}

 
char * get_config_var(char *var_name)
{
    int i;
	for(i = 0; i < g_var_num_temp; i++)
 	{ 
		if (strcasecmp(ga_variables_temp[i], var_name) == 0)
		{
			return ga_values_temp[i];
		}
    }
 	//printf("get %s failed", var_name);
 	return NULL;
}







void print_all_vars()
{ 	
    int i;
	//printf("g_var_num == %d\n", g_var_num);
	for(i = 0; i < g_var_num; i++)
    { 
		printf("%s = %s\n", ga_variables[i], ga_values[i]);
    }
}

unsigned short chtoshort_hex(char s[])//only for 0xaabb
{
	unsigned short a;
	unsigned char m[2],s1[5];
	int i;
	strncpy(s1,s,4);
	//printf("s1=%s\n",s1);
	m[0]=htoi(s1);
	m[1]=htoi(s+4);
	//printf("s=%s\n",s+4);	
	a=(m[0]<<8)|m[1];
	return a;
}


int htoi(char s[]) //only for character 0xaa to 
{	  
	int i;
	// int n = 0; 
	unsigned char n=0x00;
	if (s[0] == '0' && (s[1]=='x' || s[1]=='X')) //0x or 0X
	{
		i = 2;
	}
	else 
	{
		i = 0;
	}
	for (; (s[i] >= '0' && s[i] <= '9')
		|| (s[i] >= 'a' && s[i] <= 'z') || (s[i] >='A' && s[i] <= 'Z');++i) 
	{	
		if (tolower(s[i]) > '9') 
		{
			n = 16 * n + (10 + tolower(s[i]) - 'a'); 
		}
		else 
		{
			n = 16 * n + (tolower(s[i]) - '0'); 
		}
	} 
	return n;
} 


int fwrite_atohex(char *ch,FILE *fp)
{
	int i=0,k=2;
	char c1[3]={0};
	unsigned char b=0x00;
	int ret;
	while(ch[k]!='\0')
	{
		for(i=0;i<2;i++,k++)
		{
			c1[i]=ch[k];
		//printf("ch=%c\t",ch[k]);
		}
		b=htoi(c1);
		ret=fwrite(&b,sizeof(char),1,fp);
		if(ret<0)
		{
			//printf("fwrite_atohex error\n");
			return FAILED;
		}
	}
	return ret;
}



int fwrite_chars(char *ch,FILE *fp)
{
	int k,ret;
	for(k=0;ch[k]!='\0';k++)
	{
		ret=fwrite(&ch[k],sizeof(char),1,fp);	
		if(ret<0)
		{
			//printf("fwrite_chars error\n");
			return FAILED;
		}
	}	
	return ret;
}

unsigned int calculate_file_length_32(char *file_name)	
{
	FILE *fp;
	unsigned int length=0;
	fp=fopen(file_name,"r");
	if(fp==NULL)
	{
		printf("can`t open file for calculate_file_length_32 \n");
	}
	fseek(fp,0,2);//移到文件尾
	length=ftell(fp);
	fclose(fp);
	return length;
}


uint64_t calculate_file_length_64(char *file_name)
{
	FILE *fp;
	uint64_t length=0;
	uint32_t va[2]={0};
	fp=fopen(file_name,"r");
	if(fp==NULL)
	{
		printf("can`t open file for calculate_file_length_64 \n");
	}
	fseek(fp,0,2);//移到文件尾
#ifdef BIG_END
	va[1]=(uint64_t)ftell(fp)&0xffffffff;
//	printf("va[0]=%x\n",va[0]);
	va[0]=(uint64_t)ftell(fp)>>32;
//	printf("va[1]=%x\n",va[1]);
	length=*(uint64_t *)va;
//	printf("length=%llx\n",length);
#else
	va[0]=(uint64_t)ftell(fp)&0xffffffff;
//	printf("va[0]=%x\n",va[0]);
	va[1]=(uint64_t)ftell(fp)>>32;
//	printf("va[1]=%x\n",va[1]);
	length=*(uint64_t *)va;
//	printf("length=%llx\n",length);
#endif
	fclose(fp);
	return length;
}

uint64_t hton64(uint64_t data)//该函数仅适用于小端
{
#ifdef BIG_END
	return data;
#else
	uint64_t da;
	uint32_t vb[2]={0},temp;
	vb[0]=data&0xffffffff;
	vb[1]=data>>32;
	vb[0]=htonl(vb[0]);
	vb[1]=htonl(vb[1]);
	temp=vb[0];
	vb[0]=vb[1];
	vb[1]=temp;
	data=*(uint64_t*)vb;
	return data;
#endif
}


int change_symbol(char *string)
{
	char *p;
	int len;
	p=string;
	len=strlen(string);
	while(len--)
	{
		if(*p=='\\')*p='/';
		{
			p++;
		}	
	}
	return OK;
}






int make_LUH()
{
    FILE *fp_luh,*f_su;
	struct multi_file_crc files;
	int k,i,m;
	int luh_file_length;
	int luh_fpos;
	int len,ret;
	uint64_t len_64;

	
	unsigned int su_len;
	unsigned short length,nu_tar,nu_with,nu_posi,nu_file;
	unsigned short file_num;
	unsigned short load_count;
	
	unsigned short check_type,check_type_temp;
	unsigned short crc_type_8,crc_type_16,crc_type_32;
	
	unsigned short data_crc_8[10],sup_crc_8[10],multi_crc8;
	
	unsigned short data_crc_16[10],sup_crc_16[10];
	unsigned short sup_total_crc_16,multi_crc16,header_file_crc;
	
	unsigned int data_crc_32[10],sup_crc_32[10];
	unsigned int multi_crc32,load_crc;

	char *ch;
	char s[15]="load_";
	char path_name[50],luh_name[70],data_name[70],sup_name[70];


	char target_ID[10][30]=
	{
	 	"luh_target_ID_001","luh_target_ID_002","luh_target_ID_003", "luh_target_ID_004","luh_target_ID_005",
	 	"luh_target_ID_006","luh_target_ID_007","luh_target_ID_008", "luh_target_ID_009","luh_target_ID_010"
	};

	char target_with_position[10][40]=
	{
	 	"luh_target_with_position_001","luh_target_with_position_002","luh_target_with_position_003", "luh_target_with_position_004","luh_target_with_position_005",
	 	"luh_target_with_position_006","luh_target_with_position_007","luh_target_with_position_008", "luh_target_with_position_009","luh_target_with_position_010"
	};

	char number_target_position[10][40]=
	{
	 	"luh_number_target_position_001","luh_number_target_position_002","luh_number_target_position_003", "luh_number_target_position_004","luh_number_target_position_005",
	 	"luh_number_target_position_006","luh_number_target_position_007","luh_number_target_position_008", "luh_number_target_position_009","luh_number_target_position_010"
	};

	char position[10][5][30]=
	{
		{"luh_position_001_001","luh_position_001_002","luh_position_001_003","luh_position_001_004","luh_position_001_005"},	
		{"luh_position_002_001","luh_position_002_002","luh_position_002_003","luh_position_002_004","luh_position_002_005"},
        {"luh_position_003_001","luh_position_003_002","luh_position_003_003","luh_position_003_004","luh_position_003_005"},
		{"luh_position_004_001","luh_position_004_002","luh_position_004_003","luh_position_004_004","luh_position_004_005"},
		{"luh_position_005_001","luh_position_005_002","luh_position_005_003","luh_position_005_004","luh_position_005_005"},
		{"luh_position_006_001","luh_position_006_002","luh_position_006_003","luh_position_006_004","luh_position_006_005"},
		{"luh_position_007_001","luh_position_007_002","luh_position_007_003","luh_position_007_004","luh_position_007_005"},
		{"luh_position_008_001","luh_position_008_002","luh_position_008_003","luh_position_008_004","luh_position_008_005"},
		{"luh_position_009_001","luh_position_009_002","luh_position_009_003","luh_position_009_004","luh_position_009_005"},
		{"luh_position_010_001","luh_position_010_002","luh_position_010_003","luh_position_010_004","luh_position_010_005"}
	};


		/***************data file*******************/
		
	char data_file_p[10][30]=
	{
	 	"0x0000","0x0000","0x0000", "0x0000","0x0000",
	 	"0x0000","0x0000","0x0000", "0x0000","0x0000"
	};

	char data_file_name[10][30]=
	{
		"luh_data_file_name_001","luh_data_file_name_002","luh_data_file_name_003", "luh_data_file_name_004","luh_data_file_name_005",
		"luh_data_file_name_006","luh_data_file_name_007","luh_data_file_name_008", "luh_data_file_name_009","luh_data_file_name_010"
	};

	char data_file_PN[10][30]=
	{
		"luh_data_file_PN_001","luh_data_file_PN_002","luh_data_file_PN_003", "luh_data_file_PN_004","luh_data_file_PN_005",
		"luh_data_file_PN_006","luh_data_file_PN_007","luh_data_file_PN_008", "luh_data_file_PN_009","luh_data_file_PN_010"
	};

	char data_check_value_type[10][40]=
	{
	 	"luh_data_file_check_value_type_001","luh_data_file_check_value_type_002","luh_data_file_check_value_type_003", "luh_data_file_check_value_type_004","luh_data_file_check_value_type_005",
		"luh_data_file_check_value_type_006","luh_data_file_check_value_type_007","luh_data_file_check_value_type_008", "luh_data_file_check_value_type_009","luh_data_file_check_value_type_010"
	};



	/***************support file*******************/

	char support_file_p[10][30]=
	{
	 	"0x0000","0x0000","0x0000", "0x0000","0x0000",
	 	"0x0000","0x0000","0x0000", "0x0000","0x0000"
	};

	char support_file_name[10][30]=
	{
	 	"luh_support_file_name_001","luh_support_file_name_002","luh_support_file_name_003", "luh_support_file_name_004","luh_support_file_name_005",
	 	"luh_support_file_name_006","luh_support_file_name_007","luh_support_file_name_008", "luh_support_file_name_009","luh_support_file_name_010"
	};

	char support_file_PN[10][30]=
	{
	 	"luh_support_file_PN_001","luh_support_file_PN_002","luh_support_file_PN_003", "luh_support_file_PN_004","luh_support_file_PN_005",
	 	"luh_support_file_PN_006","luh_support_file_PN_007","luh_support_file_PN_008", "luh_support_file_PN_009","luh_support_file_PN_010"
	};


	 char support_check_value_type[10][40]=
	{
	 	"luh_support_file_check_value_type_001","luh_support_file_check_value_type_002","luh_support_file_check_value_type_003", "luh_support_file_check_value_type_004","luh_support_file_check_value_type_005",
	 	"luh_support_file_check_value_type_006","luh_support_file_check_value_type_007","luh_support_file_check_value_type_008", "luh_support_file_check_value_type_009","luh_support_file_check_value_type_010"
	};


	 ret=select_section("[media_set]");
	 if(FAILED==ret)
	 {
	 	printf("select_section error\n");
		return FAILED;
	 }
	 ch=get_config_var("load_count");
	 if(ch==NULL)
	 {
		printf("get load_count error\n");
		return FAILED;
	 }
	 load_count=htoi(ch);
	 for(m=1;m<=load_count;m++)//make multi luh files
	 {		
		sprintf(s,"[load_%.3d]",m);
	 	//printf("s=%s\n",s);
		ret=select_section(s);
		if(FAILED==ret)
	 	{
			printf("select_section error\n");
			return FAILED;
		}
		strcpy(path_name,"./media665-3");
		ch=get_config_var("load_path");
		if(ch==NULL)
	 	{
			printf("get load_path error\n");
			return FAILED;
	 	}
		change_symbol(ch);
		strcat(path_name,ch);
		ch=get_config_var("head_file_name");
		if(ch==NULL)
	 	{
			printf("get head_file_name error\n");
			return FAILED;
	 	}
		strcpy(luh_name,path_name);
		strcat(luh_name,ch);
		fp_luh=fopen(luh_name, "wb+");
		if (NULL == fp_luh)
		{		
			printf("can't open '%s' as config file:%s\n",fp_luh,strerror(errno));
			goto EXIT;
		}

	
				/********make LUH**********/
		fseek(fp_luh,4,0);//the leading four bytes is file length
		ch=get_config_var("luh_file_version");
		if(ch==NULL)
	 	{
			printf("get luh_file_version error\n");
			return FAILED;
	 	}
		ret=fwrite_atohex(ch,fp_luh);
		if(ret<0)
		{
			printf("fwrite_atohex error\n");
			return FAILED;
		}
		ch=get_config_var("luh_part_flags");
		if(ch==NULL)
	 	{
			printf("get luh_part_flags error\n");
			return FAILED;
	 	}
		ret=fwrite_atohex(ch,fp_luh);
		if(ret<0)
		{
			printf("fwrite_atohex error\n");
			return FAILED;
		}
		ch=get_config_var("luh_p_PN_length");
		if(ch==NULL)
	 	{
			printf("get luh_p_PN_length error\n");
			return FAILED;
	 	}
		ret=fwrite_atohex(ch,fp_luh);
		if(ret<0)
		{
			printf("fwrite_atohex error\n");
			return FAILED;
		}
		ch=get_config_var("luh_p_target_ID");
		if(ch==NULL)
	 	{
			printf("get luh_p_target_ID error\n");
			return FAILED;
	 	}
		ret=fwrite_atohex(ch,fp_luh);
		if(ret<0)
		{
			printf("fwrite_atohex error\n");
			return FAILED;
		}
		ch=get_config_var("luh_p_number_data_file");
		if(ch==NULL)
	 	{
			printf("get luh_p_number_data_file error\n");
			return FAILED;
	 	}
		ret=fwrite_atohex(ch,fp_luh);
		if(ret<0)
		{
			printf("fwrite_atohex error\n");
			return FAILED;
		}
		ch=get_config_var("luh_p_number_support_file");
		if(ch==NULL)
	 	{
			printf("get luh_p_number_support_file error\n");
			return FAILED;
	 	}
		ret=fwrite_atohex(ch,fp_luh);
		if(ret<0)
		{
			printf("fwrite_atohex error\n");
			return FAILED;
		}
		ch=get_config_var("luh_p_user_defined_data");
		if(ch==NULL)
	 	{
			printf("get luh_p_user_defined_data error\n");
			return FAILED;
	 	}
		ret=fwrite_atohex(ch,fp_luh);
		if(ret<0)
		{
			printf("fwrite_atohex error\n");
			return FAILED;
		}
		ch=get_config_var("luh_p_type_description_length");
		if(ch==NULL)
	 	{
			printf("get luh_p_type_description_length error\n");
			return FAILED;
	 	}
		ret=fwrite_atohex(ch,fp_luh);
		if(ret<0)
		{
			printf("fwrite_atohex error\n");
			return FAILED;
		}
		ch=get_config_var("luh_p_number_target_positions");
		if(ch==NULL)
	 	{
			printf("get luh_p_number_target_positions error\n");
			return FAILED;
	 	}
		ret=fwrite_atohex(ch,fp_luh);
		if(ret<0)
		{
			printf("fwrite_atohex error\n");
			return FAILED;
		}
		ch=get_config_var("luh_p_check_value_length");
		if(ch==NULL)
	 	{
			printf("get luh_p_check_value_length error\n");
			return FAILED;
	 	}
		ret=fwrite_atohex(ch,fp_luh);
		if(ret<0)
		{
			printf("fwrite_atohex error\n");
			return FAILED;
		}

		ch=get_config_var("luh_load_PN");
		if(ch==NULL)
	 	{
			printf("get luh_load_PN error\n");
			return FAILED;
	 	}
		length=htons(strlen(ch));
		ret=fwrite(&length,sizeof(short),1,fp_luh);//write PN length
		if(ret<0)
		{
			printf("fwrite error\n");
			return FAILED;
		}
		ret=fwrite_chars(ch,fp_luh);//write load PN
		if(ret<0)
		{
			printf("fwrite_chars error\n");
			return FAILED;
		}
		if((strlen(ch)%2)!=0)
			{
				ret=fwrite_atohex("0x00",fp_luh);				
				if(ret<0)
				{
					printf("fwrite_atohex error\n");
					return FAILED;
				}
			}//if odd ,add 0x00
			
		ch=get_config_var("luh_type_description");
		if(ch==NULL)
	 	{
			printf("get luh_type_description error\n");
	 	}
		length=htons(strlen(ch));
		fwrite(&length,sizeof(short),1,fp_luh);//write description length
		if(ret<0)
		{
			printf("fwrite error\n");
		}
		ret=fwrite_chars(ch,fp_luh);//write description		
		if(ret<0)
		{
			printf("fwrite_chars error\n");
			return FAILED;
		}
		if(strlen(ch)%2!=0)       
		{
			ret=fwrite_atohex("0x00",fp_luh);
			if(ret<0)
			{
				printf("fwrite_atohex error\n");
				return FAILED;
			}									
		}
			
		ch=get_config_var("luh_type_ID");
		if(ch==NULL)
	 	{
			printf("get luh_type_ID error\n");
			return FAILED;
	 	}
		ret=fwrite_atohex(ch,fp_luh);
		if(ret<0)
		{
			printf("fwrite_atohex error\n");
			return FAILED;
		}
		ch=get_config_var("luh_number_target_ID");
		if(ch==NULL)
	 	{
			printf("get luh_number_target_ID error\n");
			return FAILED;
	 	}
		ret=fwrite_atohex(ch,fp_luh);
		if(ret<0)
		{
			printf("fwrite_atohex error\n");
			return FAILED;
		}
		nu_tar=htoi(ch);
		for(k=0;k<nu_tar;k++)      //number of target HW ID
		{
			ch=get_config_var(target_ID[k]);
			if(ch==NULL)
	 		{
				printf("get target_ID error\n");
				return FAILED;
	 		}
			length=htons(strlen(ch));
			fwrite(&length,sizeof(short),1,fp_luh);//write position length
			if(ret<0)
			{
				printf("fwrite error\n");
				return FAILED;
			}
			ret=fwrite_chars(ch,fp_luh);//write target id
			if(ret<0)
			{
				printf("fwrite_chars error\n");
				return FAILED;
			}
			if(strlen(ch)%2!=0)        
			{
				ret=fwrite_atohex("0x00",fp_luh);	
				if(ret<0)
				{
					printf("fwrite_atohex error\n");
					return FAILED;
				}		
			}
				
		}

		ch=get_config_var("luh_number_target_with_position");
		if(ch==NULL)
	 	{
			printf("get luh_number_target_with_position error\n");
			return FAILED;
	 	}
		ret=fwrite_atohex(ch,fp_luh);
		if(ret<0)
		{
			printf("fwrite_atohex error\n");
			return FAILED;
		}
		nu_with=htoi(ch);
		for(k=0;k<nu_with;k++)        //number of target HW ID with position循环
		{
			ch=get_config_var(target_with_position[k]);
			if(ch==NULL)
	 		{
				printf("get target_with_position error\n");
				return FAILED;
	 		}
			length=htons(strlen(ch));
			fwrite(&length,sizeof(short),1,fp_luh);//write length
			if(ret<0)
			{
				printf("fwrite error\n");
				return FAILED;
			}
			ret=fwrite_chars(ch,fp_luh);//write target with position
			if(ret<0)
			{
				printf("fwrite_chars error\n");
				return FAILED;
			}
			if(strlen(ch)%2!=0)         
			{
				ret=fwrite_atohex("0x00",fp_luh);	
				if(ret<0)
				{
					printf("fwrite_atohex error\n");
					return FAILED;
				}
				
			}


			ch=get_config_var(number_target_position[k]);
			if(ch==NULL)
	 		{
				printf("get number_target_position error\n");
				return FAILED;
	 		}
			ret=fwrite_atohex(ch,fp_luh);
			if(ret<0)
			{
				printf("fwrite_atohex error\n");
				return FAILED;
			}
			
			nu_posi=htoi(ch);
			for(i=0;i<nu_posi;i++)     //number of target HW ID position
			{
				ch=get_config_var(position[k][i]);
				if(ch==NULL)
	 			{
					printf("get position error\n");
					return FAILED;
	 			}
				length=htons(strlen(ch));
				fwrite(&length,sizeof(short),1,fp_luh);//write length
				if(ret<0)
				{
					printf("fwrite error\n");
					return FAILED;
				}
				ret=fwrite_chars(ch,fp_luh);//write position
				if(ret<0)
				{
					printf("fwrite_chars error\n");
					return FAILED;
				}
				if(strlen(ch)%2!=0)         
				{
					ret=fwrite_atohex("0x00",fp_luh);	
					if(ret<0)
					{
						printf("fwrite_atohex error\n");
						return FAILED;
					}			
				}
			}
		
		}

		crc_type_8=htons(CRC_8_TYPE);
		crc_type_16=htons(CRC_16_TYPE);
		crc_type_32=htons(CRC_32_TYPE);
		
		/***************data file*******************/
#if 1
		ch=get_config_var("luh_number_data_file");
		if(ch==NULL)
		{
			printf("get luh_number_data_file error\n");
			return FAILED;
		}

		ret=fwrite_atohex(ch,fp_luh);		
		if(ret<0)
		{
			printf("fwrite_atohex error\n");
			return FAILED;
		}
		file_num=htoi(ch);

		for(k=0;k<file_num;k++)
		{
			ret=fwrite_atohex(data_file_p[k],fp_luh);
			if(ret<0)
			{
				printf("fwrite_atohex error\n");
				return FAILED;
			}
			ch=get_config_var(data_file_name[k]);
			if(ch==NULL)
	 		{
				printf("get data_file_name error\n");
				return FAILED;
	 		}
			//printf("k=%d\n",k);			
			strcpy(data_name,path_name);
			strcat(data_name,ch);
			//printf("data_name=%s\n",data_name);
			strcpy(files.data_file_name[k],data_name);

			length=htons(strlen(ch));
			ret=fwrite(&length,sizeof(short),1,fp_luh);//write file name length
			if(ret<0)
			{
				printf("fwrite error\n");
				return FAILED;
			}
			ret=fwrite_chars(ch,fp_luh);//write name
			if(ret<0)
			{
				printf("fwrite_chars error\n");
				return FAILED;
			}
			if(strlen(ch)%2!=0)        
			{
				ret=fwrite_atohex("0x00",fp_luh);
				if(ret<0)
				{
					printf("fwrite_atohex error\n");
					return FAILED;
				}
				
			}

			ch=get_config_var(data_file_PN[k]);
			if(ch==NULL)
	 		{
				printf("get data_file_PN error\n");
				return FAILED;
	 		}
			length=htons(strlen(ch));
			ret=fwrite(&length,sizeof(short),1,fp_luh);//write data PN length
			if(ret<0)
			{
				printf("fwrite error\n");
				return FAILED;
			}
			ret=fwrite_chars(ch,fp_luh);//write data PN
			if(ret<0)
			{
				printf("fwrite_chars error\n");
				return FAILED;
			}
			if(strlen(ch)%2!=0) 		
			{
				ret=fwrite_atohex("0x00",fp_luh);	
				if(ret<0)
				{
					printf("fwrite_atohex error\n");
					return FAILED;
				}
				
			}
			
			len=calculate_file_length_32(data_name);
			if(len%2!=0)
			{
				len=(len+1)/2;
			}
			else 
			{
				len=len/2;
			}
				
			len=htonl(len);
			ret=fwrite(&len,sizeof(int),1,fp_luh);//write data file length
			if(ret<0)
			{
				printf("fwrite error\n");
				return FAILED;
			}
			
			data_crc_16[k]=crc_16(data_name);                         //CRC_16校验
			printf("data_crc_16[%d]=0x%x\n",k,data_crc_16[k]);
			data_crc_16[k]=htons(data_crc_16[k]);       
			ret=fwrite(&data_crc_16[k],sizeof(short),1,fp_luh);
			if(ret<0)
			{
				printf("fwrite error\n");
				return FAILED;
			}
								
			len_64=calculate_file_length_64(data_name);
			//printf("*****data file length_64: %x\n",len);
			len_64=hton64(len_64);//big end or small end
			//printf("*****change data file length_64: %llx\n",len);
			ret=fwrite(&len_64,sizeof(char),8,fp_luh);   //write data file length in bytes
			if(ret<0)
			{
				printf("fwrite error\n");
				return FAILED;
			}
		
			ch=get_config_var(data_check_value_type[k]);
			if(ch==NULL)
	 		{
				printf("get data_check_value_type error\n");
				return FAILED;
	 		}
			check_type=chtoshort_hex(ch);
			//printf("data file check type=0x%x\n",check_type);
			check_type_temp=htons(check_type);
		
			switch(check_type)
			{
				case 1:
					    ret=fwrite(&crc_type_8,sizeof(short),1,fp_luh);//write data check value length	
					    if(ret<0)
						{
							printf("fwrite error\n");
							return FAILED;
						}
						ret=fwrite(&check_type_temp,sizeof(short),1,fp_luh);//write data check value type			
						if(ret<0)
						{
							printf("fwrite error\n");
							return FAILED;
						}
						data_crc_8[k]=crc_8(data_name);
						//printf("data_crc_8[%d]=0x%x\n",k,data_crc_8[k]);
						data_crc_8[k]=htonl(data_crc_8[k]);
						ret=fwrite(&data_crc_8[k],sizeof(char),1,fp_luh);//write data check value									
						if(ret<0)
						{
							printf("fwrite error\n");
							return FAILED;
						}
						break;
				case 2:
						ret=fwrite(&crc_type_16,sizeof(short),1,fp_luh);//write data check value length
						if(ret<0)
						{
							printf("fwrite error\n");
							return FAILED;
						}
						ret=fwrite(&check_type_temp,sizeof(short),1,fp_luh);//write data check value type		
						if(ret<0)
						{
							printf("fwrite error\n");
							return FAILED;
						}
						ret=fwrite(&data_crc_16[k],sizeof(short),1,fp_luh);//write  data check value				
						if(ret<0)
						{
							printf("fwrite error\n");
							return FAILED;
						}
						break;
				case 3:
						ret=fwrite(&crc_type_32,sizeof(short),1,fp_luh);//write data check value length					
						if(ret<0)
						{
							printf("fwrite error\n");
							return FAILED;
						}
						ret=fwrite(&check_type_temp,sizeof(short),1,fp_luh);//write data check value type		
						if(ret<0)
						{
							printf("fwrite error\n");
							return FAILED;
						}
						data_crc_32[k]=crc_32(data_name);
						//printf("data_crc_32[%d]=0x%x\n",k,data_crc_32[k]);
						data_crc_32[k]=htonl(data_crc_32[k]);
						ret=fwrite(&data_crc_32[k],sizeof(int),1,fp_luh);//write data check value					
						if(ret<0)
						{
							printf("fwrite error\n");
							return FAILED;
						}
						break;
				default:
							;
			}	
		}
#endif

			/*******support file*********/
		ch=get_config_var("luh_number_support_file");
		if(ch==NULL)
		{
			printf("get luh_number_support_file error\n");
			return FAILED;
		}

		ret=fwrite_atohex(ch,fp_luh);
		if(ret<0)
		{
			printf("fwrite_atohex error\n");
			return FAILED;
		}
		nu_file=htoi(ch);
		for(k=0;k<nu_file;k++)              //number of support files 
		{
			ret=fwrite_atohex(support_file_p[k],fp_luh);				
			if(ret<0)
			{
				printf("fwrite_atohex error\n");
				return FAILED;
			}
			ch=get_config_var(support_file_name[k]);
			if(ch==NULL)
	 		{
				printf("get support_file_name error\n");
				return FAILED;
	 		}
			strcpy(sup_name,path_name);
			strcat(sup_name,ch);
			//printf("sup_name=%s\n",sup_name);
			strcpy(files.su_file_name[k],sup_name);
			f_su=fopen(sup_name,"r");
			if(f_su==NULL)
			{
				printf("can`t open support file\n");
				return FAILED;
			}
			length=htons(strlen(ch));
			ret=fwrite(&length,sizeof(short),1,fp_luh);//write support file name length
			if(ret<0)
			{
				printf("fwrite error\n");
				return FAILED;
			}
			ret=fwrite_chars(ch,fp_luh);//write name
			if(ret<0)
			{
				printf("fwrite_chars error\n");
				return FAILED;
			}
			if(strlen(ch)%2!=0)        
			{
				ret=fwrite_atohex("0x00",fp_luh);
				if(ret<0)
				{
					printf("fwrite_atohex error\n");
					return FAILED;
				}				
			}
			
			ch=get_config_var(support_file_PN[k]);
			if(ch==NULL)
	 		{
				printf("get support_file_PN error\n");
				return FAILED;
	 		}
			length=htons(strlen(ch));
			ret=fwrite(&length,sizeof(short),1,fp_luh);//write PN length
			if(ret<0)
			{
				printf("fwrite error\n");
				return FAILED;
			}
			ret=fwrite_chars(ch,fp_luh);//write PN
			if(ret<0)
			{
				printf("fwrite_chars error\n");
				return FAILED;
			}
			if(strlen(ch)%2!=0) 		
			{
				ret=fwrite_atohex("0x00",fp_luh);	
				if(ret<0)
				{
					printf("fwrite_atohex error\n");
					return FAILED;
				}								
			}

			fseek(f_su,0,2);//move to the file end
			su_len=ftell(f_su);
			su_len=htonl(su_len);


			ret=fwrite(&su_len,sizeof(unsigned int),1,fp_luh);//write support file length
			if(ret<0)
			{
				printf("fwrite error\n");
				return FAILED;
			}
			rewind(f_su);//move to the file start
			fclose(f_su);

			sup_crc_16[k]=crc_16(sup_name);
			sup_crc_16[k]=htons(sup_crc_16[k]);
			ret=fwrite(&sup_crc_16[k],sizeof(short),1,fp_luh);//write support file crc
			if(ret<0)
			{
				printf("fwrite error\n");
				return FAILED;
			}
			//printf("sup_crc_16[%d]=0x%x\n",k,sup_crc_16[k]);

			ch=get_config_var(support_check_value_type[k]);
			if(ch==NULL)
	 		{
				printf("get support_check_value_type error\n");
				return FAILED;
	 		}
			check_type=chtoshort_hex(ch);
			//printf("support file check type=0x%x\n",check_type);
			check_type_temp=htons(check_type);
		
			switch(check_type)
			{
				case 1:
				   	 	ret=fwrite(&crc_type_8,sizeof(short),1,fp_luh);
						if(ret<0)
						{
							printf("fwrite error\n");
							return FAILED;
						}
						ret=fwrite(&check_type_temp,sizeof(short),1,fp_luh);		
						if(ret<0)
						{
							printf("fwrite error\n");
							return FAILED;
						}
						sup_crc_8[k]=crc_8(sup_name);
						//printf("sup_crc_8[%d]=0x%x\n",k,sup_crc_8[k]);
						sup_crc_8[k]=htonl(sup_crc_8[k]);
						ret=fwrite(&sup_crc_8[k],sizeof(char),1,fp_luh);	
						if(ret<0)
						{
							printf("fwrite error\n");
							return FAILED;
						}
						break;
				case 2:
						ret=fwrite(&crc_type_16,sizeof(short),1,fp_luh);//write support check value length
						if(ret<0)
						{
							printf("fwrite error\n");
							return FAILED;
						}
						ret=fwrite(&check_type_temp,sizeof(short),1,fp_luh);//write support check value type		
						if(ret<0)
						{
							printf("fwrite error\n");
							return FAILED;
						}
						
						ret=fwrite(&sup_crc_16[k],sizeof(short),1,fp_luh);//write  support check value
						if(ret<0)
						{
							printf("fwrite error\n");
							return FAILED;
						}
						break;
				case 3:
						ret=fwrite(&crc_type_32,sizeof(short),1,fp_luh);//write support check value length					
						if(ret<0)
						{
							printf("fwrite error\n");
							return FAILED;
						}
						ret=fwrite(&check_type_temp,sizeof(short),1,fp_luh);//write support check value type		
						if(ret<0)
						{
							printf("fwrite error\n");
							return FAILED;
						}
						sup_crc_32[k]=crc_32(sup_name);
					//	printf("sup_crc_32[%d]=0x%x\n",k,sup_crc_32[k]);
						sup_crc_32[k]=htonl(sup_crc_32[k]);
						ret=fwrite(&sup_crc_32[k],sizeof(int),1,fp_luh);//write  support check value
						if(ret<0)
						{
							printf("fwrite error\n");
							return FAILED;
						}
						break;
				default:
							 ;
			}
		}

		ch=get_config_var("luh_user_defined_data");
		if(ch==NULL)
	 	{
			printf("get luh_user_defined_data error\n");
			return FAILED;
	 	}
		ret=fwrite_chars(ch,fp_luh);
		if(ret<0)
		{
			printf("fwrite_chars error\n");
			return FAILED;
		}
		luh_fpos=ftell(fp_luh);
		files.data_file_num=file_num;
		files.su_file_num=nu_file;
		strcpy(files.luh_file_name,luh_name);
		files.luh_length=luh_fpos;

		ch=get_config_var("luh_load_check_value_type");
		if(ch==NULL)
	 	{
			printf("get luh_load_check_value_type error\n");
			return FAILED;
	 	}
		check_type=chtoshort_hex(ch);
		//printf("load check type=0x%x\n",check_type);
		check_type_temp=htons(check_type);
		switch(check_type)
		{
			case 1:
			 		ret=fwrite(&crc_type_8,sizeof(short),1,fp_luh);//write load crc check value length	
			 		if(ret<0)
					{
						printf("fwrite error\n");
						return FAILED;
					}
					ret=fwrite(&check_type_temp,sizeof(short),1,fp_luh);//write load crc check value type		
					if(ret<0)
					{
						printf("fwrite error\n");
						return FAILED;
					}
					multi_crc8=multi_crc_8(files);
					//printf("multi_crc8=0x%x\n",multi_crc8);
					multi_crc8=htons(multi_crc8);
					ret=fwrite(&multi_crc8,sizeof(short),1,fp_luh);//write  load crc check value
					if(ret<0)
					{
						printf("fwrite error\n");
						return FAILED;
					}
					break;
				
			case 2:
					ret=fwrite(&crc_type_16,sizeof(short),1,fp_luh);//write load crc check value length
					if(ret<0)
					{
						printf("fwrite error\n");
						return FAILED;
					}
					ret=fwrite(&check_type_temp,sizeof(short),1,fp_luh);//write load crc check value type	
					if(ret<0)
					{
						printf("fwrite error\n");
						return FAILED;
					}
					multi_crc16=multi_crc_16(files);
					//printf("multi_crc16=0x%x\n",multi_crc16);
					multi_crc16=htons(multi_crc16);
					ret=fwrite(&multi_crc16,sizeof(short),1,fp_luh);//write  load crc check value
					if(ret<0)
					{
						printf("fwrite error\n");
						return FAILED;
					}
					break;
			case 3:
					ret=fwrite(&crc_type_32,sizeof(short),1,fp_luh);//write load crc check value length					
					if(ret<0)
					{
						printf("fwrite error\n");
						return FAILED;
					}
					ret=fwrite(&check_type_temp,sizeof(short),1,fp_luh);//write load crc check value type		
					if(ret<0)
					{
						printf("fwrite error\n");
						return FAILED;
					}
					multi_crc32=multi_crc_32(files);
					//printf("multi_crc32=0x%x\n",multi_crc32);
					multi_crc32=htonl(multi_crc32);
					ret=fwrite(&multi_crc32,sizeof(int),1,fp_luh);//write load crc check value
					if(ret<0)
					{
						printf("fwrite error\n");
						return FAILED;
					}
					break;
			default:
						 ;
		}
		luh_fpos=ftell(fp_luh);
		header_file_crc=crc_file_pos_16(luh_name,luh_fpos);
		printf("header_file_crc=0x%x\n",header_file_crc);
		header_file_crc=htons(header_file_crc);
		ret=fwrite(&header_file_crc,sizeof(short),1,fp_luh);//header file crc
		if(ret<0)
		{
			printf("fwrite error\n");
			return FAILED;
		}
					
		luh_fpos=ftell(fp_luh);
		load_crc=crc_file_pos_32(luh_name,luh_fpos);
		printf("load_crc=0x%x\n",load_crc);
		load_crc=htons(load_crc);
		ret=fwrite(&load_crc,sizeof(int),1,fp_luh);//load crc
		if(ret<0)
		{
			printf("fwrite error\n");
			return FAILED;
		}
					
		fseek(fp_luh,0,2);//move to the file end
		luh_file_length=ftell(fp_luh);
		luh_file_length=htonl(luh_file_length);
		rewind(fp_luh);//move to the file start
		ret=fwrite(&luh_file_length,sizeof(long),1,fp_luh);
		if(ret<0)
		{
			printf("fwrite error\n");
			return FAILED;
		}
		EXIT:
			  fclose(fp_luh);
			  return FAILED;
	 }
	 return OK;
}


void make_config_to_LUH(void)
{
	parse_config_file("./media665-3/665_upload_conf_b.txt");
	make_LUH();
}

	int main(int argc ,char *argv[])
{
	make_config_to_LUH();
}
