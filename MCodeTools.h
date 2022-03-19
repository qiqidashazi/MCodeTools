////
//错误定位工具


#ifndef _MCODETOOLS_H_
#define _MCODETOOLS_H_

//头文件概括
#include<stdio.h>   //C输入输出流等
#include<string.h>  //C字符串
#include<time.h>    //C时间库
#include<stdlib.h>  //C通用工具库
#include<stdarg.h>  //可变参
#include<unistd.h>

//MCodeTools所用宏
#define M_SUCCESS   0
#define M_FAIL      -1

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#define FMT_MESSAGE_LENGTH     255
#define FULL_FILE_PATH_LENGTH  256
#define MAX_FILE_NAME_LENGTH       63
#define FUNCTION_NAME_LENGTH   127
#define MAX_SERROR_LENGTH      (32 + 224 + FMT_MESSAGE_LENGTH) 
#define MAX_TIME_LENGTH        32

#define SWriteLogFile(fn, fmt, ...) \
      if((fn) != NULL) _SWriteLogFile((fn), __LINE__, __FILE__, (fmt), __VA_ARGS__ )
#define DWriteLogFile(fp,fn, fmt, ...)   \
      if((fn) != NULL) _DWriteLogToFile((fp), (fn), (fmt), __VA_ARGS__)

//错误结构
typedef struct{
    time_t  ttm;               //时间信息
    //Linux编译器调试内置宏
    int     lineNum;           //行号
    char    fileName[MAX_FILE_NAME_LENGTH + 1];             //错误代码所在文件
  // char    functionName[FUNCTION_NAME_LENGTH + 1];    //问题函数名
    char    fmtMsg[FMT_MESSAGE_LENGTH + 1];             //错误信息
    //扩展
   // int     eno;            //引用 errno.h的 errno
   // int     std_ErrCode;    //补充为 自定义错误 --为整形数值 
}SError, *PSError;

//定义
char MErrMsg[MAX_SERROR_LENGTH + 1];  //转换字符串用，最好还是在函数内定义

//接口声明
void    SetSError(IN PSError ero,/*int eno, int errCode, char* FucName*/ int lineNo, char* fileName, char* fmt, ...);

void    ClearSError(IN PSError ero);

char*   SErrorToString( IN PSError ero, 
                        OUT char errBuf[MAX_SERROR_LENGTH + 1]);

char*   STimeToString(  IN time_t* time, 
                        OUT char timeStamp[MAX_TIME_LENGTH]);

struct tm*   SGetYMDToString( IN time_t *ttime, 
                              OUT char timeStamp[MAX_TIME_LENGTH]);

void    SGetHMSToString( IN struct tm* s_tm, 
                         OUT char timeStamp[MAX_TIME_LENGTH]);

static int     _DWriteLogToFile(IN char* filePath, 
                                IN char* fName, 
                                IN char* fmt, ...);

static int     _SWriteLogFile(IN char* filePath, 
                              IN int lineNo, 
                              IN char* localFName, 
                              IN char* fmt, ...);



#endif