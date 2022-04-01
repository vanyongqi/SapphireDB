#ifndef OSSSOCKET_HPP_
#define OSSSOCKET_HPP_
/*******************************************************************************

ossSocket.hpp
envelop the socket by implement a ossSocket class.

*******************************************************************************/
#include"core.hpp"
#define SOCKET_GETLASTERROR errno
#define OSS_SOCKET_DFT_TIMEOUT 10000 //default timeout :10 ms
#define OSS_MAX_HOSTNAME NI_MAXHOST
#define OSS_MAX_SERVICENAME NI_MAXSERV

class _ossSocket{
private:
    int _fd;//file description
    socklen_t _addressLen;// server len
    socklen_t _peerAddressLen;// client len
    struct sockaddr_in _sockAddress;//server sockaddr
    struct sockaddr_in _peerAddress;//client sockaddr
    bool _init;
    int _timeout;
protected:
    unsigned int _getPort (sockaddr_in *addr);
    int _getAddress(sockaddr_in *addr,char *_pAddress,unsigned int length);
public:
    int setSocketLi(int lOnOff, int linger);
    void setAddress(const char* pHostName, unsigned int port);
    _ossSocket();
    _ossSocket(unsigned int port, int timeout = 0);
    _ossSocket(const char* pHostName,unsigned int port,int timeout =0);
    _ossSocket(int *sock ,int timeout =0);
    
    ~_ossSocket(){
        printf("socket is closed\n");
        close();
    }

    int initSocket();
    int bind_listen();
    bool isConnected();
    int send(const char *pMsg, int len,
                int timeout= OSS_SOCKET_DFT_TIMEOUT,
                    int flags = 0);
    int recv(char *pMsg, int len,
                int timeout =OSS_SOCKET_DFT_TIMEOUT,
                    int flags =0);
    int recvNF(char* pMsg,int& len, int timeout= OSS_SOCKET_DFT_TIMEOUT);
    int connect();
    void close();
    int accept(int *sock,struct sockaddr *addr,socklen_t *addrlen,
                int timeout = OSS_SOCKET_DFT_TIMEOUT);
    int disableNagle();
    unsigned int getPeerPort();
    int getPeerAddress(char* pAddress,unsigned int length);
    unsigned int getLocalPort();
    int getLocalAddress(char *pAddress, unsigned int length);
    int setTimeout(int seconds);
    static int getHostName(char *pName,int nameLen);
    static int getPort(const char* pServiceName,unsigned short &port);
};

typedef class _ossSocket ossSocket;

#endif