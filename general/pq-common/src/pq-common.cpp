#include "pq-common/include/pq-common.h"

#define BUF_SIZE 1024
#define DIR_LENGTH 200


std::string get_name_by_pid(pid_t pid)
{    //should become a un inline function after move to common.
    char procPidPath[DIR_LENGTH + 1] = { '\0' };
    char buf[BUF_SIZE] = { '\0' };
    char processName[BUF_SIZE]={'\0'};
    sprintf(procPidPath, "/proc/%d/status", pid);
    FILE* fp = fopen(procPidPath, "r");
    if (nullptr != fp)
    {
        if (fgets(buf, BUF_SIZE - 1, fp) == NULL)
        {
            fclose(fp);
        }
        fclose(fp);
        sscanf(buf, "%*s %s", processName);
    }

    return std::string(processName);
}
