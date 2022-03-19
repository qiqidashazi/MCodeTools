///
// implements
#include "MCodeTools.h"



//设置错误结构信息
void    SetSError(PSError ero,/*int eno, int errCode, char* FucName*/ int lineNo, char* fileName, char* fmt, ...){
    va_list mark;
    char* substring = NULL;

    if(!ero) return;
    ClearSError(ero);
    ero->ttm = time(0);
    ero->lineNum = lineNo;
    // ero->eno = eno;
    // ero->std_ErrCode = errCode;
    if(fileName){
        substring = strrchr(fileName, '/');
        snprintf(ero->fileName, sizeof(ero->fileName), "%s", (substring == NULL? fileName : ++substring));
    }
    // if(FucName){
    //     snprintf(ero->functionName, sizeof(ero->functionName), "%s", FucName);
    // }
    if(fmt){
        va_start(mark, fmt);
        vsnprintf(ero->fmtMsg, sizeof(ero->fmtMsg), fmt, mark);
        va_end(mark);
    }
    return;
}

//清除错误结构信息
void    ClearSError(PSError ero){
    if(ero){
        memset(ero, 0x00, sizeof(SError));
    }
    return;
}

//错误结构信息转换字符串
char*   SErrorToString(PSError ero, char errBuf[MAX_SERROR_LENGTH + 1]){
    if(!errBuf) return NULL;
    if(ero){
        char    timeStamp[MAX_TIME_LENGTH] = {0};
        STimeToString(&ero->ttm, timeStamp);
        snprintf(errBuf, MAX_SERROR_LENGTH + 1,"INSIDE:[--%s--[lineNO:%d]--[FileName:%s]--[ERROR-MSG: %s]--]", timeStamp, ero->lineNum, ero->fileName, /*ero->functionName,*/ ero->fmtMsg);
    }else{
        errBuf[0] = '\0';
    }
    return (char*)errBuf;
}

//时间转换字符串
char*   STimeToString(time_t* ttime, char timeStamp[MAX_TIME_LENGTH]){
    if(NULL == timeStamp)  return NULL;
    struct tm*  currentTime = NULL;
    localtime_s(currentTime, ttime);
	strftime(timeStamp, MAX_TIME_LENGTH, "%Y/%m/%d-%H:%M:%S", currentTime);
    return (char*)timeStamp;
}

//获取年月日
struct tm*   SGetYMDToString(time_t *ttime, char timeStamp[MAX_TIME_LENGTH]){
    if(NULL == timeStamp)  return NULL;
    struct tm*  currentTime = NULL;
    localtime_s(currentTime, ttime);
	strftime(timeStamp, MAX_TIME_LENGTH, "%Y/%m/%d", currentTime);
    return currentTime;
}

//获取时分秒
void    SGetHMSToString(struct tm* s_tm, char timeStamp[MAX_TIME_LENGTH]){
    if(NULL == s_tm)  return;
	strftime(timeStamp, MAX_TIME_LENGTH, "%H:%M:%S", s_tm);
    return;
}

//日志输出 --整日期 --两层信息输出
static int     _SWriteLogFile(char* filePath, int lineNo, char* localFName, char* fmt, ...){
    FILE *pFile;
    time_t ttime;
    char tbuff[32];
    va_list mark;
    char* substring = NULL;
    char filePathBck[128];
   
    if(filePath == NULL){
        //默认工作目录
        char wkBuf[32];
        snprintf(wkBuf, sizeof(wkBuf), "%s", __FILE__ );
        char* sp = strrchr(wkBuf, '/');
        if(sp) *sp = '\0';
        snprintf(filePathBck, sizeof(filePathBck), "%s/%s", wkBuf, "filename-not-specified.log");
    }else{
        snprintf(filePathBck, sizeof(filePathBck), "%s", filePath);
    }

    //初始化时间
    ttime = time(0);
    STimeToString(&ttime, tbuff);
    pFile = fopen(filePathBck, "w+");
    if(pFile){
        char fBuf[MAX_FILE_NAME_LENGTH + 1];
        char storage[MAX_SERROR_LENGTH + 1];
        storage[MAX_SERROR_LENGTH] = '\0';
        substring = strrchr(localFName, '/');
        snprintf(fBuf, sizeof(fBuf), "%s", (substring == NULL? localFName : ++substring));

        va_start(mark, fmt);
        vsnprintf(storage, sizeof(storage), fmt, mark);
        va_end(mark);

        fprintf(pFile, "LOG[----%s---%d---%s---%s]", tbuff, lineNo, fBuf, storage);
    }

    fflush(pFile);
    fclose(pFile);
    return M_SUCCESS;
}

//按天进行输出日志，必须传入文件名，filePath可指定为NULL--使用默认路径：当前文件路径
static int     _DWriteLogToFile(char* filePath, char* fName, char* fmt, ...){
    if(fName == NULL)   return M_FAIL;
    FILE *pFile = NULL;
    va_list mark;
    char date[MAX_TIME_LENGTH];
    char FullFileName[MAX_FILE_NAME_LENGTH + 1];
    char wkBuf[FULL_FILE_PATH_LENGTH];
    char LogBuff[MAX_SERROR_LENGTH + 1];
    char timestamp[MAX_TIME_LENGTH];
    time_t ttime = time(0);
    struct tm* s_tm;
    //初始化时间
    s_tm = SGetYMDToString(&ttime, date);
    
    //保留工作目录
    snprintf(wkBuf, sizeof(wkBuf), "%s", __FILE__);
    char* sp = strrchr(wkBuf, '/');
    if(sp){
        *sp = '\0';
    }
    
    snprintf(FullFileName, sizeof(FullFileName), "%s/%s_%s", (filePath==NULL?wkBuf:filePath), date, fName);
    pFile = fopen(FullFileName, "w+");
    if(pFile == NULL) return M_FAIL;
    //拼接可变参日志
    va_start(mark, fmt);
    vsnprintf(LogBuff, sizeof(LogBuff), fmt, mark);
    va_end(mark);
    //获取时间戳剩余信息
    SGetHMSToString(s_tm, timestamp);
    fprintf(pFile, "LOG[----%s---%s]\n", timestamp, LogBuff);
   
    fflush(pFile);
    fclose(pFile);
    pFile = NULL;
   
    return M_SUCCESS;
}   

//getcwd 获取当前工作目录
