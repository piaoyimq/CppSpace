/*
 * Log.cpp
 *
 *  Created on: Mar 29, 2016
 *      Author: piaoyimq
 */
#include <string.h>
#include <sstream> //stringstream
#include "Log.h"




Log::Log() :
        counter(0), isAsync(false), currentLogAmount(0) {
    m_mutex = new pthread_mutex_t;
    pid = getpid();
    printf("Process id: %d\n", pid);
    pthread_mutex_init(m_mutex, NULL);
    init("./mylog.log", 100, 100, 10); //asynchronization model
//	init("./mylog.log", 100, 1000000, 0);//synchronization model
//	sleep(2);//piaoyimq ???
}


Log::~Log() {
//    sleep(15);//piaoyimq ???
    flush();
    if (m_fp != NULL) {
        fclose(m_fp);
    }
    pthread_mutex_destroy(m_mutex);
    if (m_mutex != NULL) {
        delete m_mutex;
    }
}


bool Log::init(const char* file_name, int log_buf_size, int split_lines, int max_queue_size) {
    if (max_queue_size >= 1) {
        isAsync = true;
        m_log_queue = new BlockQueue<string>(max_queue_size);
        pthread_t tid;
        pthread_create(&tid, NULL, flushLogThread, NULL);
    }

    logBufSize = log_buf_size;
    m_buf = new char[logBufSize];
    memset(m_buf, '\0', sizeof(m_buf));
    splitLines = split_lines;

    time_t t = time(NULL);
    struct tm* sys_tm = localtime(&t);
    struct tm my_tm = *sys_tm;
    const char *p = strrchr(file_name, '/');
    char log_full_name[256] = { 0 };
    if (p == NULL) {
        snprintf(log_full_name, 255, "%d_%02d_%02d_%s", my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, file_name);
    } else {
        strcpy(logName, p + 1);
        strncpy(dirName, file_name, p - file_name + 1);
        snprintf(log_full_name, 255, "%s%d_%02d_%02d_%s", dirName, my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, logName);
    }

    m_today = my_tm.tm_mday;

    m_fp = fopen(logName, "a");
    if (m_fp == NULL) {
        return false;
    }

    return true;
}

void Log::logFileCompression(const char* fileName, uint32_t alreadyCompressFileAmount){
    printf("alreadyCompressFileAmount=%u\n", alreadyCompressFileAmount);
    if(alreadyCompressFileAmount < 0){
        return;
    }

    char shellContent[300] = {0};
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
gzip $var.1\n\
echo \"gzip -f $var.1\"\n\
                    ' > test.sh; bash test.sh; rm -rf test.sh", fileName , alreadyCompressFileAmount);
//    printf("length shell:%d", strlen(shellContent));
    system(shellContent);
}

void Log::writeLog(const Log::Level logLevel, int moduleId, const char* format, ...) {
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
    if (m_today != my_tm.tm_mday || counter % splitLines == 0) //everyday Log
            {
        char new_log[256] = { 0 };
        fflush(m_fp);
        fclose(m_fp);
        logFileCompression(logName, currentLogAmount++);
//        sleep(1);//Maybe need to sleep, then reopen it.
#if 0
        char tail[16] = { 0 };
        snprintf(tail, 16, "%d_%02d_%02d_", my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday);

        if (m_today != my_tm.tm_mday) {
            snprintf(new_log, 255, "%s%s%s", dirName, tail, logName);
            m_today = my_tm.tm_mday;
            counter = 0;
        } else {
            snprintf(new_log, 255, "%s%s%s.%d", dirName, tail, logName, counter / splitLines);
        }
#endif
        m_fp = fopen(logName, "a");
#if 1//Only for test.

        std::stringstream ss;
        ss << "\n**************************  ";
        ss << currentLogAmount;
        ss << "  **************************\n\n";
        fputs(ss.str().c_str(), m_fp);

#endif
    }
    pthread_mutex_unlock(m_mutex);

    va_list valst;
    va_start(valst, format);

    string log_str;
    pthread_mutex_lock(m_mutex);

    int n = snprintf(m_buf, 100, "%d-%02d-%02d %02d:%02d:%02d.%06d [%u][%u](%d)%s %d ",
            my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, my_tm.tm_hour, my_tm.tm_min, my_tm.tm_sec, now.tv_usec,
            pid, tid, mapThread[tid], getLogLevelString(logLevel), moduleId);

    int m = vsnprintf(m_buf + n, logBufSize - 1, format, valst);
    m_buf[n + m - 1] = '\n';
    log_str = m_buf;
    pthread_mutex_unlock(m_mutex);

    if (isAsync && !m_log_queue->full()) {
        m_log_queue->push(log_str);
    } else {
        pthread_mutex_lock(m_mutex);
        fputs(log_str.c_str(), m_fp);
        pthread_mutex_unlock(m_mutex);
    }

    va_end(valst);
}


void Log::flush(void) {
    pthread_mutex_lock(m_mutex);
    fflush(m_fp);
    pthread_mutex_unlock(m_mutex);
}

