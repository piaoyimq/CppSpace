/*
 * Logging.cpp
 *
 *  Created on: Mar 29, 2016
 *      Author: piaoyimq
 */
#include <string.h>
#include <sstream> //stringstream
#include <errno.h>
#include "Log.h"




void Logging::init(const char* dirPath, const char* fileName, uint32_t oneLineLogSize, uint32_t split_lines, uint32_t max_queue_size) {
    char dirPathTemp[DIR_LENGTH+1]={'\0'};
    char fileNameTemp[NAME_LENGTH+1]={'\0'};
    strncpy(dirPathTemp, dirPath, sizeof(dirPathTemp));
    strncpy(fileNameTemp, fileName, sizeof(fileNameTemp));

    bool ret = isDirPathExist(dirPathTemp);
    if(false == ret){
        printf("dirPathTemp=%s is not valid, use default log dir: %s\n", dirPathTemp, LOG_DIRECTORY);
        strncpy(dirPathTemp, LOG_DIRECTORY, sizeof(dirPathTemp));
        strncpy(fileNameTemp, logName, sizeof(fileNameTemp));
    }
    else{
        printf("dirPathTemp=%s is valid\n", dirPathTemp);
    }

    char newfullName[DIR_LENGTH+NAME_LENGTH+2]={'\0'};

    snprintf(newfullName, sizeof(newfullName), "%s/%s", dirPathTemp, fileNameTemp);
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
    oneLineLogLength = oneLineLogSize;
    m_buf = new char[oneLineLogLength];
    memset(m_buf, '\0', oneLineLogLength);
    splitLines = split_lines;
    if (max_queue_size >= 1) {
        isAsync = true;
        m_log_queue = new BlockQueue<string>(max_queue_size);
        pthread_t tid;
        pthread_create(&tid, NULL, flushLogThread, NULL);
    }
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
        fflush(m_fp);
        fclose(m_fp);
        logFileCompression(currentLogAmount++);
//        sleep(1);//Maybe need to sleep, then reopen it.

        m_fp = fopen(logFullName, "a");
        if(NULL == m_fp){
            fprintf(stderr,"%s, Process ID %d ",strerror(errno), pid);
            exit(EXIT_FAILURE);
        }

        char fileHeard[FILE_HEAD_LENGTH]={'\0'};
        snprintf(fileHeard, sizeof(fileHeard), "Process ID: %d\nProcess Name: %s\nLog File Sequence ID: %d\n\n", pid, pidName, currentLogAmount);
        fputs(fileHeard, m_fp);

    }
    pthread_mutex_unlock(m_mutex);

    va_list valst;
    va_start(valst, format);

    memset(m_buf, '\0', oneLineLogLength);
    string log_str;
    pthread_mutex_lock(m_mutex);

    uint32_t n = snprintf(m_buf, oneLineLogLength-1, "%d-%02d-%02d %02d:%02d:%02d [%d](%d) [%s] <%s>: ",
            my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, my_tm.tm_hour, my_tm.tm_min, my_tm.tm_sec,
            tid, mapThread[tid], getLogLevelString(logLevel), getLogModuleString(moduleId));
    uint32_t m = vsnprintf(m_buf + n, oneLineLogLength-n-1, format, valst);
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


Logging::Logging() :
        counter(0), isAsync(false), currentLogAmount(0), 
        oneLineLogLength(ONE_LINE_LOG_LENGTH), splitLines(SPLIT_LINES){
    m_mutex = new pthread_mutex_t;
    pthread_mutex_init(m_mutex, NULL);
    pid = getpid();
    printf("Process id: %d\n", pid);    
    getNameByPid(pid, pidName);
    snprintf(logName, sizeof(logName), "%s.log", pidName);
    strncpy(dirPath, LOG_DIRECTORY, sizeof(dirPath));
    printf("dirPath=%s\n", dirPath);
    snprintf(logFullName, sizeof(logFullName), "%s/%s", dirPath, logName);
    printf("logFullName=%s\n", logFullName);
    
    m_buf = new char[oneLineLogLength];
    memset(m_buf, '\0', oneLineLogLength);
    
    m_fp = fopen(logFullName, "a");
    if (NULL == m_fp) {
        fprintf(stderr, "fopen \'%s\' failed: %s\n", logFullName, strerror(errno));
        exit(EXIT_FAILURE);
    }
    char fileHead[FILE_HEAD_LENGTH]={'\0'};
    snprintf(fileHead, sizeof(fileHead), "Process ID: %d\nProcess Name: %s\nLog File Sequence ID: %d\n\n", pid, pidName, currentLogAmount);
    fputs(fileHead, m_fp);
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


void Logging::moveLogs(const char* oldFullName, const char* newFullName, uint32_t alreadyCompressFileAmount) const{
    char oldFullNameTemp[DIR_LENGTH+NAME_LENGTH+2]={'\0'};
    char newFullNameTemp[DIR_LENGTH+NAME_LENGTH+2]={'\0'};
    printf("%s: oldFullName=%s, newFullName=%s\n", __FUNCTION__, oldFullName, newFullName);
    for(int i=0; i<= alreadyCompressFileAmount; i++){
        printf("i=%d\n", i);
        if(0 == i){
            if(rename(oldFullName, newFullName) < 0 ){
                printf("error: %s\n", strerror(errno));        
            }
            else{
                printf("ok!\n");        
            }
        }
        else{
            snprintf(oldFullNameTemp, sizeof(oldFullNameTemp), "%s.%d.gz", oldFullName, i);
            snprintf(newFullNameTemp, sizeof(newFullNameTemp), "%s.%d.gz", newFullName, i);
            if(rename(oldFullNameTemp, newFullNameTemp) < 0 ){
                printf("error: %s\n", strerror(errno));        
            }
            else{
                printf("ok!\n");        
            }
        }
    }
}


void *Logging::async_write_log() const {
    string single_log;
    while (m_log_queue->pop(single_log)) {
        pthread_mutex_lock(m_mutex);
        fputs(single_log.c_str(), m_fp);
        pthread_mutex_unlock(m_mutex);
    }
}


void Logging::logFileCompression(uint32_t alreadyCompressFileAmount) const{
    printf("alreadyCompressFileAmount=%u\n", alreadyCompressFileAmount);
    if(alreadyCompressFileAmount < 0){
        return;
    }
 
    char shellContent[SHELL_CONTENT_LENGTH] = {'\0'};
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
                    logFullName , alreadyCompressFileAmount, dirPath, dirPath, dirPath);
//    printf("length shell:%d", strlen(shellContent));
    // printf("%s\n", shellContent);
    system(shellContent);
}


void Logging::flush() const{
    pthread_mutex_lock(m_mutex);
    fflush(m_fp);
    pthread_mutex_unlock(m_mutex);
}

