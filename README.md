# **Spphire：A Simple NoSQL.**
## **coding log ：**
- 2022/04/18:
      -PD问题诊断模块（日志）的编写、编译。✅

- 2022/04/15:
      - 增加对mutex的类封装✅
- 2022/04/02:
      - 增加 src/include /src/oss /src/client下的注释✅  
      - 利用PlantUML绘制ossSocket类的class diagram✅
- 2022/04/01:  
      - server与client的通信            ✅   
      - 编写README.md ✅

- 2022/03/31:  
      - 完成client发送sql的基本语句       ✅  
      - socket_demo测试                ✅

- 2022/03/21:  
      - socket类的设计封装                   ✅   
      - VIM&nbsp;&nbsp;➡️ &nbsp;&nbsp;VScode
- 2022/03/11:  
      - upload test project to Github ✅  
      - Windows-Linux虚拟机&nbsp;&nbsp; ➡️ &nbsp;&nbsp;Mac-Linux虚拟机开发  
      -XShell&nbsp;&nbsp; ➡️&nbsp;&nbsp; Royal TSX
- 2022/03/01:  
      - boost库等编译环境的配置✅  
      -  autotools编译测试✅
------------------------
### **数据库提供的功能：**

- 高可靠：提供可靠的数据服务
- 高可用：应用于程序产生的历史数据
- 高性能：提供高性能的数据存储与检索
- 高标准：提供标准的数据访问接口

**数据库的发展历史与类型：**
    
|数据库|代表|
|--|--|
|网状数据库|IDS|
|层次数据库|IMS|
|关系数据库|SQL|
|非关系数据库|MongoDB|

**非关系数据库类型：**
|||
|--|--|
|key-value|键值对|
|big-table|宽表|
|document|文档|
|graph|图数据库|

**数据访问流程：**

                      请求解析
                         |
                    **请求处理**：
                       
                       /   \
                存储访问 日志访问
                        \  /
                        磁盘

    请求解析层：接受请求-》线程调度-》解析数据-》生成数据访问流
    请求处理层：生成执行代码-》执行代码-》访问数据-》数据写入日志
    存储访问：索引数据扫描
    日志访问：日志读写

**组件：**
- CLI: client module,客户端查询服务模块
- IXM: index manager，索引管理模块
- DMS: data manage service，数据管理服务
- PMD: process mdoule detector，程序入口main(),线程调度
- MSG: message 消息组件，协议组件
- OSS:operating system service 底层服务
- MON:monitor 监控
- PD:prblem detection 问题诊断

**编译环境：**

- 编译语言：C/C++
- MAKE工具：autotools 
- 第三方库：Boost 1_78_0、JSON、BSON
- 操作系统：Ubuntu 20.04 LTS 
- 客户端驱动支持：JDK8
- 开发调试工具：g++9.4.0、gdb 9.2

**Paper List:**
- [1] Codd, E. F. . (2001). A relational model of data for large shared data banks. Springer Berlin Heidelberg.