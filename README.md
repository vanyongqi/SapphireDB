# **Spphire：A Simple NoSQL.**
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

**Paper List:**
- [1] Codd, E. F. . (2001). A relational model of data for large shared data banks. Springer Berlin Heidelberg.