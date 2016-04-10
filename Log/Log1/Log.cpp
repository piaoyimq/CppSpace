/*
 * Log.cpp
 *
 *  Created on: Mar 29, 2016
 *      Author: piaoyimq
 */
#include "Log.h"
#include <string.h>
#include <sstream> //stringstream
#include <errno.h>
#include <sys/stat.h>





void Log::init(const char* dir, const char* fileName, uint32_t oneLineLogSize, uint32_t split_lines, uint32_t max_queue_size) {
    char dirPathTemp[DIR_LENGTH+1]={'\0'};
    char fileNameTemp[NAME_LENGTH+1]={'\0'};
    strncpy(dirPathTemp, dir, sizeof(dirPathTemp));
    strncpy(fileNameTemp, fileName, sizeof(fileNameTemp));

    bool ret = isDirPathExist(dirPathTemp);
    if(false == ret){
        fprintf(stderr, "Process \"%s\": \'%s\' is not exist, use default log directory \'%s\'\n", pidName,  dirPathTemp, LOG_DIRECTORY);
        logItself(CMethod, Error, "%s: \'%s\' is not exist, use default log directory \'%s\'", __FUNCTION__, dirPathTemp, LOG_DIRECTORY);
        strncpy(dirPathTemp, LOG_DIRECTORY, sizeof(dirPathTemp));
        strncpy(fileNameTemp, logName, sizeof(fileNameTemp));
    }

    char newfullName[DIR_LENGTH+NAME_LENGTH+2]={'\0'};
    snprintf(newfullName, sizeof(newfullName), "%s/%s", dirPathTemp, fileNameTemp);
    if(0 != strcmp(logFullName, newfullName)){
        if (NULL != m_fp) {
            fclose(m_fp);
            m_fp = NULL;
        }

        bool ret=moveLogs(logFullName, newfullName, currentLogAmount);
        if(true == ret){
            logItself(CMethod, Notice, "%s: move logs from \'%s\' to directory \'%s\'", __FUNCTION__, LOG_DIRECTORY, dirPathTemp);
            strncpy(dirPath, dirPathTemp, sizeof(dirPath));
            strncpy(logName, fileNameTemp, sizeof(logName));
            strncpy(logFullName, newfullName, sizeof(logFullName));

            m_fp = fopen(logFullName, "a");
            if (NULL == m_fp) {
                fprintf(stderr, "Process \"%s\": fopen \"%s\" failed: %s\n", pidName,  logFullName, strerror(errno));
                logItself(CmdMethod, Emergency, "%s: fopen \'%s\' failed: %s", __FUNCTION__, logFullName, strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
        else {
            logItself(CMethod, Error, "%s: move logs failed, use default log file \'%s\'", __FUNCTION__, logFullName);
            char c=getchar();
            m_fp = fopen(logFullName, "a");
            if (NULL == m_fp) {
                fprintf(stderr, "Process \"%s\": fopen \"%s\" failed: %s\n", pidName,  logFullName, strerror(errno));
                logItself(CmdMethod, Emergency, "%s: fopen \'%s\' failed: %s", __FUNCTION__, logFullName, strerror(errno));
                exit(EXIT_FAILURE);
            }
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
        logItself(CMethod, Info, "%s: create a thread \'%d\', use asynchronous type to logging", __FUNCTION__, getTid());
    }
}


void Log::writeLog(Level logLevel, AppModuleID moduleId, const char* format, ...) {
    struct timeval now = { 0, 0 };
    gettimeofday(&now, NULL);
    time_t t = now.tv_sec;
    struct tm* sys_tm = localtime(&t);
    struct tm my_tm = *sys_tm;

    pthread_mutex_lock(m_mutex);

    pid_t tid = getTid();
    map<pid_t, int>::iterator where = mapThread.find(tid);  //pstree -pa [procdssid] ,  ps -Lef

    if (where == mapThread.end()) {
        mapThread[tid] = 0; //  if use logItself() here, assign mapThread[tid]=0, otherwise assign mapThread[tid]=1
        printf("%s: Found thread %d, value=%d\n", __FUNCTION__, tid, mapThread[tid]);
        logItself(CMethod, Info, "%s: Found a new thread %d(LWP) ", __FUNCTION__, tid);
    } else {
        mapThread[tid]++;
//        printf("mpaThread[%d]=%d\n", tid, mapThread[tid]);
    }

//    printf("gettpid=%u, getTid=%u, pthread_self=%lu\n", getpid(), getTid());
    counter++;
    if (counter % splitLines == 0)
    {
        fflush(m_fp);
        if(NULL!=m_fp){
            fclose(m_fp);
            m_fp=NULL;
        }

//        logFileCompression(currentLogAmount++);
        logfilesControl(currentLogAmount);
        ++currentLogAmount;
//        sleep(1);//Maybe need to sleep, then reopen it.

        m_fp = fopen(logFullName, "a");
        if(NULL == m_fp){
            fprintf(stderr, "Process \"%s\": fopen \"%s\" failed: %s\n", pidName,  logFullName, strerror(errno));
            logItself(CmdMethod, Emergency, "%s: fopen \'%s\' failed: %s", __FUNCTION__, logFullName, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    pthread_mutex_unlock(m_mutex);

    va_list valst;
    va_start(valst, format);

    string log_str;
    pthread_mutex_lock(m_mutex);
    uint32_t k=snprintf(m_buf, oneLineLogLength-1, "%s", logItselfBuf);

    memset(logItselfBuf, '\0', sizeof(logItselfBuf));
    logItselfLength = 0;
    
    uint32_t n = snprintf(m_buf+k, oneLineLogLength-k-1, "%d-%02d-%02d %02d:%02d:%02d [%d](%d) [%s] <%s>: ",
            my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, my_tm.tm_hour, my_tm.tm_min, my_tm.tm_sec,
            tid, mapThread[tid], getLogLevelString(logLevel), getLogModuleString(moduleId));
    uint32_t m = vsnprintf(m_buf +k+ n, oneLineLogLength-k-n-1, format, valst);
    m_buf[k+n + m ] = '\n';
    m_buf[k+n + m+1] = '\0';
    log_str = m_buf;
    memset(m_buf, '\0', oneLineLogLength);
    
    pthread_mutex_unlock(m_mutex);

    if (isAsync && !m_log_queue->full()) {
        m_log_queue->push(log_str);
    } else {
        pthread_mutex_lock(m_mutex);
        fputs(log_str.c_str(), m_fp);
//        fflush(m_fp); //TODO: use it or not??? (piaoyimq).
        pthread_mutex_unlock(m_mutex);
    }

    va_end(valst);
}


Log::Log() :
        counter(0), isAsync(false), currentLogAmount(0), 
        oneLineLogLength(ONE_LINE_LOG_LENGTH), splitLines(SPLIT_LINES), logFilesTotalSize(0){
    memset(logItselfBuf, '\0', sizeof(logItselfBuf));
    m_mutex = new pthread_mutex_t;
    pthread_mutex_init(m_mutex, NULL);

    pid = getpid();
    getNameByPid(pid, pidName);
    snprintf(logName, sizeof(logName), "%s.log", pidName);
    strncpy(dirPath, LOG_DIRECTORY, sizeof(dirPath));
    snprintf(logFullName, sizeof(logFullName), "%s/%s", dirPath, logName);

    printf("Process id: %d\nlogFullName=%s\n", pid, logFullName);
    
    m_buf = new char[oneLineLogLength];
    memset(m_buf, '\0', oneLineLogLength);
    m_fp = fopen(logFullName, "a");
    if (NULL == m_fp) {
        fprintf(stderr, "Process \"%s\": fopen \"%s\" failed: %s\n", pidName,  logFullName, strerror(errno));
        logItself(CmdWithHeadMethod, Emergency, "%s: fopen \'%s\' failed: %s", __FUNCTION__, logFullName, strerror(errno));
        exit(EXIT_FAILURE);
    }

    writeLogHead();
    logItself(CMethod, Info, "%s: use synchronous type to logging", __FUNCTION__);
//	sleep(2);//piaoyimq ???
}


Log::~Log() {
//    sleep(15);//piaoyimq ???
    flush();
    delete m_buf;
    m_buf= NULL;

    if (NULL != m_fp) {
        fclose(m_fp);
        m_fp = NULL;
    }
    pthread_mutex_destroy(m_mutex);
    if (m_mutex != NULL) {
        delete m_mutex;
    }
}


void Log::writeLogHead(char *logHead) {
    char fileHead[FILE_HEAD_LENGTH] = { '\0' };
    snprintf(fileHead, sizeof(fileHead), "Process ID: %d\nProcess Name: %s\nLog File Sequence ID: %d\n\n", pid, pidName, currentLogAmount);

    if(logHead!=NULL){
        strncpy(logHead, fileHead, BUF_SIZE);
        return;
    }

    fputs(fileHead, m_fp);
}


bool Log::moveLogs(const char* oldFullName, const char* newFullName, uint32_t alreadyCompressFileAmount){
    char oldFullNameTemp[DIR_LENGTH+NAME_LENGTH+2]={'\0'};
    char newFullNameTemp[DIR_LENGTH+NAME_LENGTH+2]={'\0'};

    for(int i=0; i<= alreadyCompressFileAmount; i++){
        if(0 == i){
            if(rename(oldFullName, newFullName) < 0 ){
                return false;
            }
        }
        else{
            snprintf(oldFullNameTemp, sizeof(oldFullNameTemp), "%s.%d.gz", oldFullName, i);
            snprintf(newFullNameTemp, sizeof(newFullNameTemp), "%s.%d.gz", newFullName, i);
            if(rename(oldFullNameTemp, newFullNameTemp) < 0 ){
                return false;
            }
        }
    }

    return true;
}


void *Log::async_write_log() const {
    string single_log;
    while (m_log_queue->pop(single_log)) {
        pthread_mutex_lock(m_mutex);
        fputs(single_log.c_str(), m_fp);
        pthread_mutex_unlock(m_mutex);
    }
}


void Log::logFileCompression(uint32_t alreadyCompressFileAmount) {
    if(alreadyCompressFileAmount < 0){
        logItself(CMethod, Notice, "%s: variable \"alreadyCompressFileAmount\" is less than 0, will do not compress.", __FUNCTION__);
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

void Log::logfilesControl(int32_t alreadyCompressFileAmount){
	if(alreadyCompressFileAmount < 0){
			printf("already <0\n");
	        logItself(CmdMethod, Notice, "%s: variable \"alreadyCompressFileAmount\" is less than 0, will do not compress", __FUNCTION__);
	        return;
	 }

	char logFullNameOldTemp[DIR_LENGTH + NAME_LENGTH +2] ={'\0'};
	char logFullNameNewTemp[DIR_LENGTH + NAME_LENGTH +2] ={'\0'};
	snprintf(logFullNameNewTemp, sizeof(logFullNameNewTemp), "%s.1", logFullName);

	logItself(CmdMethod, Notice, "%s: the current file log reach the maximum file lines, will open a new file to log", __FUNCTION__);
	if (rename(logFullName, logFullNameNewTemp) < 0) {
		fprintf(stderr, "Process \"%s\": rename \"%s\" failed, %s\n", pidName, logFullName, strerror(errno));
		return;
	}

	logItself(CmdOnlyWriteHeadMethod, LastLevel, "");

	snprintf(logFullNameNewTemp, sizeof(logFullNameNewTemp), "%s.1.gz", logFullName);
//	printf("file=%s\n", logFullNameNewTemp);
	struct stat statBuff;
	if(0==stat(logFullNameNewTemp, &statBuff)){
		uint64_t fileSize=statBuff.st_size;
//		printf("one log size=%llu, _=_already: %u\ntotal:%llu\n", fileSize, alreadyCompressFileAmount+1, fileSize*(alreadyCompressFileAmount+1));
		if(logFilesTotalSize < (alreadyCompressFileAmount+1) *fileSize){
			static uint32_t sequenceId = 0;
			logItself(CmdMethod, Notice, "%s: total log size reach the maximum, will delete the file which sequence id is %d", __FUNCTION__, sequenceId);
			++sequenceId;
//			printf("al=%d, se=%d\n", alreadyCompressFileAmount, sequenceId);
			alreadyCompressFileAmount-= sequenceId;
		}
	}
	else{
		printf("no this file\n");
	}

	printf("num=%d\n", alreadyCompressFileAmount);
	for(int32_t i=alreadyCompressFileAmount;i>0;i--){
		snprintf(logFullNameOldTemp, sizeof(logFullNameOldTemp), "%s.%d.gz", logFullName, i);
		snprintf(logFullNameNewTemp, sizeof(logFullNameNewTemp), "%s.%d.gz", logFullName, i+1);
		printf("---num=%d\n", alreadyCompressFileAmount);
		if (rename(logFullNameOldTemp, logFullNameNewTemp) < 0) {
			fprintf(stderr, "Process \"%s\": rename \"%s\" failed, %s\n", pidName, logFullNameOldTemp, strerror(errno));
			logItself(CmdMethod, Error, "%s: rename \"%s\" failed, %s", __FUNCTION__, logFullNameOldTemp, strerror(errno));
		}
	}

	char command[DIR_LENGTH + NAME_LENGTH +2+20] ={'\0'};
//	if(1==alreadyCompressFileAmount){
//		snprintf(command, sizeof(command), "rm -f %s.1", logFullName);
//	}else{
		snprintf(command, sizeof(command), "gzip -f %s.1", logFullName);
//	}
	system(command);
}


void Log::flush() const{
    pthread_mutex_lock(m_mutex);
    fflush(m_fp);
    pthread_mutex_unlock(m_mutex);
}


void Log::logItself(LogMethod logMethod, Level logLevel, const char* format, ...) {
    struct timeval now = { 0, 0 };
    gettimeofday(&now, NULL);
    time_t t = now.tv_sec;
    struct tm* sys_tm = localtime(&t);
    struct tm my_tm = *sys_tm;
    pid_t tid = getTid();
    map<pid_t, int>::iterator where = mapThread.find(tid);  //pstree -pa [procdssid] ,  ps -Lef



    va_list valst;
    va_start(valst, format);
    switch(logMethod){
        case CMethod:{
        		if (where == mapThread.end()) {
        	        mapThread[tid] = 1;
        	        printf("%s: Found thread %d, value=%d\n", __FUNCTION__, tid, mapThread[tid]);
        	    } else {
        	        mapThread[tid]++;
        	//        printf("mpaThread[%d]=%d\n", tid, mapThread[tid]);
        	    }
            uint32_t n = snprintf(logItselfBuf+logItselfLength, sizeof(logItselfBuf)-logItselfLength- 1, "%d-%02d-%02d %02d:%02d:%02d [%d](%d) [%s] <Log>: ",
                    my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, my_tm.tm_hour, my_tm.tm_min,
                    my_tm.tm_sec, tid, mapThread[tid], getLogLevelString(logLevel));
            uint32_t m = vsnprintf(logItselfBuf + logItselfLength+n, sizeof(logItselfBuf) -logItselfLength- n - 1, format, valst);
            logItselfBuf[logItselfLength+n + m] = '\n';
            logItselfBuf[logItselfLength+n + m+1] = '\0';
            logItselfLength+=n+m+1;

            break;
        }
        case CmdMethod:
        case CmdWithHeadMethod:
        case CmdOnlyWriteHeadMethod:{

            char content[BUF_SIZE] = {'\0'};
            char command[BUF_SIZE+STRING_LENGTH+DIR_LENGTH+NAME_LENGTH+2] = {'\0'};

            if(CmdWithHeadMethod== logMethod || CmdOnlyWriteHeadMethod == logMethod){
                writeLogHead(content);
                snprintf(command, sizeof(command), "echo \"%s\" >> %s", content, logFullName);
                system(command);
                if(CmdOnlyWriteHeadMethod == logMethod){
                	return;
                  }
            }

            memset(content, '\0', sizeof(content));
            memset(command, '\0', sizeof(command));
            if (where == mapThread.end()) {
                    mapThread[tid] = 1;
                    printf("%s: Found thread %d, value=%d\n", __FUNCTION__, tid, mapThread[tid]);
                } else {
                    mapThread[tid]++;
            //        printf("mpaThread[%d]=%d\n", tid, mapThread[tid]);
                }

            uint32_t n = snprintf(content, sizeof(content)- 1, "%d-%02d-%02d %02d:%02d:%02d [%d](%d) [%s] <Log>: ",
                    my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, my_tm.tm_hour, my_tm.tm_min,
                    my_tm.tm_sec, tid, mapThread[tid], getLogLevelString(logLevel));

            uint32_t m = vsnprintf(content+n, sizeof(content) -n - 1, format, valst);
            content[n + m] = '\0';
            content[n +m+1] = '\0';
            snprintf(command, sizeof(command), "echo \"%s\" >> %s", content, logFullName);
            system(command);
            break;
        }    
        default:
            fprintf(stderr, "Process \"%s\": wrong log method\n", pidName);
    }

    va_end(valst);
}
