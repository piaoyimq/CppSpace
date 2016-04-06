/*
 * Logging.cpp
 *
 *  Created on: Mar 29, 2016
 *      Author: piaoyimq
 */
#include <string.h>
#include <sstream> //stringstream
#include <errno.h>
#include <dirent.h>
#include "Log.h"

void getNameByPid(pid_t pid, char *task_name) {
    char proc_pid_path[BUF_SIZE];
    char buf[BUF_SIZE];
    sprintf(proc_pid_path, "/proc/%d/status", pid);
    FILE* fp = fopen(proc_pid_path, "r");
    if(NULL != fp){
        if( fgets(buf, BUF_SIZE-1, fp)== NULL ){
            fclose(fp);
        }
        fclose(fp);
        sscanf(buf, "%*s %s", task_name);
    }
}

Logging::Logging() :
        counter(0), isAsync(false), currentLogAmount(0), 
        logBufSize(LOG_BUF_SIZE), splitLines(SPLIT_LINES){
    m_mutex = new pthread_mutex_t;
    pthread_mutex_init(m_mutex, NULL);
    // init("./mylog.log", 2000, 100, 10); //asynchronization model
    pid = getpid();
    printf("Process id: %d\n", pid);    
    getNameByPid(pid, pidName);
    snprintf(logName, sizeof(logName), "%s.log", pidName);
    strncpy(dirName, LOG_DIRECTORY, sizeof(dirName));
    printf("dirName=%s\n", dirName);
    snprintf(logFullName, sizeof(logFullName), "%s%s", dirName, logName);
    printf("logFullName=%s\n", logFullName);
    
     m_buf = new char[logBufSize];
    memset(m_buf, '\0', logBufSize);
    
    m_fp = fopen(logFullName, "a");
    if (NULL == m_fp) {
        fprintf(stderr, "fopen \'%s\' failed: %s\n", logFullName, strerror(errno));
        exit(EXIT_FAILURE);
    }
    char fileHeard[100];
    snprintf(fileHeard, 100, "Process ID: %d\nProcess Name: %s\nLog File Sequence ID: %d\n\n", pid, pidName, currentLogAmount);
    fputs(fileHeard, m_fp);
    
    
// 	init(100, 100, 0);//synchronization model
//	sleep(2);//piaoyimq ???
}


Logging::~Logging() {
//    sleep(15);//piaoyimq ???
    flush();
    delete m_buf;
    m_buf= NULL;
    if (m_fp != NULL) {
        fclose(m_fp);
    }
    pthread_mutex_destroy(m_mutex);
    if (m_mutex != NULL) {
        delete m_mutex;
    }
}


void Logging::moveLogs(const char* oldName, const char* newName, int alreadyCompressFileAmount){
    char oldNameTemp[150]={0};
    char newNameTemp[150]={0};
    printf("%s: oldName=%s, newName=%s\n", __FUNCTION__, oldName, newName);
    for(int i=0; i<= alreadyCompressFileAmount; i++){
        printf("i=%d\n", i);
        if(0 == i){
            if(rename(oldName, newName) < 0 ){
                printf("error: %s\n", strerror(errno));        
            }
            else{
                printf("ok!\n");        
            }
        }
        else{
            snprintf(oldNameTemp, sizeof(oldNameTemp), "%s.%d.gz", oldName, i);
            snprintf(newNameTemp, sizeof(newNameTemp), "%s.%d.gz", newName, i);
            if(rename(oldNameTemp, newNameTemp) < 0 ){
                printf("error: %s\n", strerror(errno));        
            }
            else{
                printf("ok!\n");        
            }
        }
    }
}

bool dirPathCheck(char* dirpPath){
    if(NULL == dirpPath || NULL == opendir(dirpPath)){
		return false;
	}
	
	int length = strlen(dirpPath);
	if(dirpPath[length-1] != '/'){
	    dirpPath[length] = '/';
	    dirpPath[length+1] = '\0';
	}
	return true;
}


#if 1
void Logging::init(const char* dirName, const char* fileName, int log_buf_size, int split_lines, int max_queue_size) {
    char dirNameTemp[150];
    char fileNameTemp[150];
    strncpy(dirNameTemp, dirName, sizeof(dirNameTemp));
    strncpy(fileNameTemp, fileName, sizeof(fileNameTemp));
    
    bool ret = dirPathCheck(dirNameTemp);
    if(false == ret){
        printf("dirNameTemp=%s is not valid, use default log dir: %s\n", dirNameTemp, LOG_DIRECTORY);
        strncpy(dirNameTemp, LOG_DIRECTORY, sizeof(dirNameTemp));
        strncpy(fileNameTemp, logName, sizeof(fileNameTemp));
    }
    else{
        printf("dirNameTemp=%s is valid\n", dirNameTemp);
    }
    
    char newfullName[150]={0};
    
    snprintf(newfullName, 150, "%s%s", dirNameTemp, fileNameTemp);
    printf("old logFullName =%s\n", logFullName);
    printf("new newfullName =%s\n", newfullName);
    if(0 != strcmp(logFullName, newfullName)){
        fclose(m_fp);
        
        moveLogs(logFullName, newfullName, currentLogAmount);
        printf("new logFullName=%s\n", logFullName);
        strncpy(logFullName, newfullName, sizeof(logFullName));
        m_fp = fopen(logFullName, "a");
        if (NULL == m_fp) {
            fprintf(stderr, "%s, Process ID %d ", strerror(errno), pid);
            exit(EXIT_FAILURE);
        }
    }
    
    delete m_buf;
    logBufSize = log_buf_size;
    m_buf = new char[logBufSize];
    memset(m_buf, '\0', logBufSize);
    splitLines = split_lines;
    if (max_queue_size >= 1) {
        isAsync = true;
        m_log_queue = new BlockQueue<string>(max_queue_size);
        pthread_t tid;
        pthread_create(&tid, NULL, flushLogThread, NULL);
    }
}
#endif

void Logging::logFileCompression(uint32_t alreadyCompressFileAmount){
    printf("alreadyCompressFileAmount=%u\n", alreadyCompressFileAmount);
    if(alreadyCompressFileAmount < 0){
        return;
    }

    char shellContent[1024] = {0};
    snprintf(shellContent, sizeof(shellContent),
            "echo '\
#!/bin/bash\n\
var=%s\n\
mv $var $var.1\n\
echo \"mv $var $var.1\"\n\
for((i=%u;i>0;i--));\n\
do\n\
  mv \"$var.$i.gz\" \"$var.$((i+1)).gz\"\n\
  echo \"mv $var.$i.gz\" \"$var.$((i+1)).gz\"\n\
done\n\
gzip -f $var.1\n\
echo \"gzip -f $var.1\"\n\
                    ' > %stest.sh; bash %stest.sh; rm -rf %stest.sh", 
                    logFullName , alreadyCompressFileAmount, dirName, dirName, dirName);
//    printf("length shell:%d", strlen(shellContent));
    // printf("%s\n", shellContent);
    system(shellContent);
}

void Logging::writeLog(Log::Level logLevel, Log::AppModuleID moduleId, const char* format, ...) {
    struct timeval now = { 0, 0 };
    gettimeofday(&now, NULL);
    time_t t = now.tv_sec;
    struct tm* sys_tm = localtime(&t);
    struct tm my_tm = *sys_tm;

    pthread_mutex_lock(m_mutex);

    pid_t tid = getTid();
    map<pid_t, int>::iterator where = mapThread.find(tid);  //pstree -pa [procdssid] ,  ps -Lef

    if (where == mapThread.end()) {
        mapThread[tid] = 1;
        printf("Found, tid=%d, value=%d\n", tid, mapThread[tid]);
    } else {
        mapThread[tid]++;
    }

//    printf("gettpid=%u, getTid=%u, pthread_self=%lu\n", getpid(), getTid());
    counter++;
    if (counter % splitLines == 0) //everyday Logging
            {
        char new_log[256] = { 0 };
        fflush(m_fp);
        fclose(m_fp);
        logFileCompression(currentLogAmount++);
//        sleep(1);//Maybe need to sleep, then reopen it.

        m_fp = fopen(logFullName, "a");
        if(NULL == m_fp){
            fprintf(stderr,"%s, Process ID %d ",strerror(errno), pid);
            exit(EXIT_FAILURE);
        }
#if 1//Only for test.

        char fileHeard[100];
        snprintf(fileHeard, 100, "Process ID: %d\nProcess Name: %s\nLog File Sequence ID: %d\n\n", pid, pidName, currentLogAmount);
        fputs(fileHeard, m_fp);

#endif
    }
    pthread_mutex_unlock(m_mutex);

    va_list valst;
    va_start(valst, format);

    memset(m_buf, '\0', logBufSize);
    string log_str;
    pthread_mutex_lock(m_mutex);

    int n = snprintf(m_buf, 2000, "%d-%02d-%02d %02d:%02d:%02d [%d](%d) [%s] <%s>: ",
            my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, my_tm.tm_hour, my_tm.tm_min, my_tm.tm_sec,
            tid, mapThread[tid], getLogLevelString(logLevel), getLogModuleString(moduleId));
    int m = vsnprintf(m_buf + n, 1000, format, valst);
    m_buf[n + m ] = '\n';
    log_str = m_buf;
    pthread_mutex_unlock(m_mutex);

    if (isAsync && !m_log_queue->full()) {
        m_log_queue->push(log_str);
    } else {
        pthread_mutex_lock(m_mutex);
        fputs(log_str.c_str(), m_fp);
//        fflush(m_fp);//piaoyimq, use it or not???
        pthread_mutex_unlock(m_mutex);
    }

    va_end(valst);
}


void Logging::flush(void) {
    pthread_mutex_lock(m_mutex);
    fflush(m_fp);
    pthread_mutex_unlock(m_mutex);
}

