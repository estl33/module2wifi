#ifndef LOG_H_
#define LOG_H_

#include <iostream>
#include <string>

using namespace std;

#define LOG_LEVEL 3

#define ERROR 0
#define WARN  1
#define INFO 2
#define LOW 3

#if LOG_LEVEL > -1
#   define Log_Error(Header, Msg) \
    __Log_Message(Header, Msg, ERROR);
#else
#   define Log_Error(Header, Msg) ;
#endif

#if LOG_LEVEL > 0
#   define Log_Warn(Header, Msg) \
    __Log_Message(Header, Msg, WARN);
#else
#   define Log_Warn(Header, Msg) ;
#endif

#if LOG_LEVEL > 1
#   define Log_Info(Header, Msg) \
    __Log_Message(Header, Msg, INFO);
#else
#   define Log_Info(Header, Msg) ;
#endif

#if LOG_LEVEL > 2
#   define Log_Low(Header, Msg) \
    __Log_Message(Header, Msg, LOW);
#else
#   define Log_Low(Header, Msg) ;
#endif

void __Log_Message(string header, string msg, int level);

#endif
