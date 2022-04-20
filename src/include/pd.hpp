/*******************************************************************************
1）问题诊断模块主要是当数据库发生问题时将有用的日志信息打印到日志文件

（2）打印的日志信息必须有时间戳，并且能够精确到是哪个函数，哪一行语句出错

（3）文件操作必须加锁

（4）这里定义了6个日志级别，并且保存有一个当前日志级别，如果某次的打印日志操作要求的日志级别大于当前日志级别，则不打印日志

（5）PD_CHECK和PD_RC_CHECK这两个宏除了打印日志信息以外，还会在条件参数为假时影响程序的执行流程。
    在检测条件为假时，跳到错误处理代码段
*******************************************************************************/

#ifndef PD_HPP__
#define PD_HPP__

#include <string>

#define PD_LOG_STRINGMAX 4096
// #define LOG(format, ...) fprintf(stdout, format, __VA_ARGS__)
// 其中，...表示可变参数列表，__VA_ARGS__在预处理中，会被实际的参数集（实参列表）所替换。
// 同时gcc还支持带可以变参数名的方式（注意：VC不支持）
#define PD_LOG(level,fmt,...)                                         \
   do {                                                               \
      if ( _curPDLevel >= level ) {                                    \
      pdLog ( level, __func__, __FILE__, __LINE__, fmt, ##__VA_ARGS__ ); \
      }\
   } while ( 0 )                                                      \

#define PD_CHECK(cond,retCode,gotoLabel,level,fmt,...)                \
   do {                                                               \
      if ( !(cond) )                                                  \
      {                                                               \
         rc = (retCode) ;                                             \
         PD_LOG  ( (level), fmt, ##__VA_ARGS__) ;                     \
         goto gotoLabel ;                                             \
      }                                                               \
   } while ( 0 )                                                      \

#define PD_RC_CHECK(rc,level,fmt,...)                                 \
   do {                                                               \
      PD_CHECK ( (EDB_OK==(rc)), (rc), error, (level),                \
                 fmt, ##__VA_ARGS__) ;                                \
   } while ( 0 )                                                      \

#define EDB_VALIDATE_GOTOERROR(cond,ret,str)                          \
   {if(!(cond)) { pdLog(PDERROR, __func__, __FILE__, __LINE__, str) ; \
    rc = ret; goto error ; }}                                          \

#ifdef _DEBUG
#define EDB_ASSERT(cond,str)                                          \
   { if ( !(cond) ) {                                                 \
      pdassert ( str,__func__, __FILE__,__LINE__);}}                  \

#define EDB_CHECK(cond,str)                                           \
   { if ( !(cond) ) {                                                 \
      pdcheck ( str, __func__, __FILE__, __LINE__ ) ; }}              \

#else
#define EDB_ASSERT(cond,str)  {if(cond){}}
#define EDB_CHECK(cond,str)   {if(cond){}}
#endif

enum PDLEVEL
{
   PDSEVERE = 0,
   PDERROR,
   PDEVENT,
   PDWARNING,
   PDINFO,
   PDDEBUG
} ;
//如果全局变量不在文件的开头定义，有效的作用范围将只限于其定义处到文件结束。
//如果在定义点之前的函数想引用该全局变量，则应该在引用之前用关键字 extern 对该变量作“外部变量声明”，
//表示该变量是一个已经定义的外部变量。
extern PDLEVEL _curPDLevel ;
const char *getPDLevelDesp ( PDLEVEL level ) ;

#define PD_DFT_DIAGLEVEL PDWARNING
void pdLog ( PDLEVEL level, const char *func, const char *file,
             unsigned int line, const char *format, ... ) ;
void pdLog ( PDLEVEL level, const char *func, const char *file,
             unsigned int line, std::string message ) ;
#endif
