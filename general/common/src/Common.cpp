/*
 * Common.cpp
 *
 *  Created on: 2016-05-21
 *      Author: piaoyimq
 */
#include "common/include/Common.h"
#include <stdio.h>
#include <string.h>
#include <cstdio>

#define BUF_SIZE 1024
#define DIR_LENGTH 200

void getPidByName(char* task_name)
{
	DIR *dir;
	struct dirent *ptr;
	FILE *fp;
	char filepath[50];
	char cur_task_name[50];
	char buf[BUF_SIZE];
	dir = opendir("/proc");
	if (NULL != dir)
	{
		while ((ptr = readdir(dir)) != NULL) //循环读取路径下的每一个文件/文件夹
		{
			//如果读取到的是"."或者".."则跳过，读取到的不是文件夹名字也跳过
			if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
			{
				continue;
			}

			if (DT_DIR != ptr->d_type)
			{
				continue;
			}

			sprintf(filepath, "/proc/%s/status", ptr->d_name);
			fp = fopen(filepath, "r");
			if (NULL != fp)
			{
				if (fgets(buf, BUF_SIZE - 1, fp) == NULL)
				{
					fclose(fp);
					continue;
				}

				sscanf(buf, "%*s %s", cur_task_name);

				//如果文件内容满足要求则打印路径的名字（即进程的PID）
				if (!strcmp(task_name, cur_task_name))
				{
					printf("PID:  %s\n", ptr->d_name);
				}
				fclose(fp);
			}
		}
		closedir(dir);
	}
}

void getNameByPid(pid_t pid, char *processName)
{    //should become a un inline function after move to common.
	char procPidPath[DIR_LENGTH + 1] = { '\0' };
	char buf[BUF_SIZE] = { '\0' };
	sprintf(procPidPath, "/proc/%d/status", pid);
	FILE* fp = fopen(procPidPath, "r");
	if (NULL != fp)
	{
		if (fgets(buf, BUF_SIZE - 1, fp) == NULL)
		{
			fclose(fp);
		}
		fclose(fp);
		sscanf(buf, "%*s %s", processName);
	}
}
