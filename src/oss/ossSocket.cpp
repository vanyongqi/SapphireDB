/*******************************************************************************

ossSocket.cpp
implement socket class in order to build connection between client and server.
供数据库客户端和数据库引擎进行通信
*******************************************************************************/
#include <stdio.h>
#include "ossSocket.hpp"
#include"pd.hpp"
// a listening socket
_ossSocket::_ossSocket ( unsigned int port, int timeout )
{
   _init = false ;
   _fd = 0 ;
   _timeout = timeout ;

   memset ( &_sockAddress, 0, sizeof(sockaddr_in) ) ;
   memset ( &_peerAddress, 0, sizeof(sockaddr_in) ) ;

   _peerAddressLen = sizeof (_peerAddress) ;
   _addressLen = sizeof ( _sockAddress ) ;

   _sockAddress.sin_family = AF_INET ;
   _sockAddress.sin_addr.s_addr = htonl ( INADDR_ANY ) ;
   _sockAddress.sin_port = htons ( port ) ;

}

//  a default socket
_ossSocket::_ossSocket()
{
   _init = false;
   _timeout = 0;
   _fd = 0;
   memset( &_sockAddress, 0, sizeof(sockaddr_in) );
   memset( &_peerAddress, 0, sizeof(sockaddr_in) );
   _peerAddressLen = sizeof(_peerAddress);
}

//  an  official socket
_ossSocket::_ossSocket ( const char *pHostname, unsigned int port, int timeout )
{
   struct hostent *hp ;
   _init = false ;
   _timeout = timeout ;
   _fd = 0 ;
   memset ( &_sockAddress, 0, sizeof(sockaddr_in) ) ;
   memset ( &_peerAddress, 0, sizeof(sockaddr_in) ) ;
   _peerAddressLen = sizeof (_peerAddress) ;
   _addressLen = sizeof ( _sockAddress ) ;

   _sockAddress.sin_family = AF_INET ;
   if ( (hp = gethostbyname ( pHostname )))// return a hostent type
      _sockAddress.sin_addr.s_addr = *((int *)hp->h_addr_list[0] ) ;
   else
      _sockAddress.sin_addr.s_addr = inet_addr ( pHostname ) ;//将一个点分十进制的IP转换成一个长整型数（u_long类型）
   _sockAddress.sin_port = htons ( port ) ;

}

// Create from a existing socket
_ossSocket::_ossSocket ( int *sock, int timeout )
{
   int rc = EDB_OK ;
   _fd = *sock ;
   _init = true ;
   _timeout = timeout ;
   _addressLen = sizeof ( _sockAddress ) ;

   memset ( &_peerAddress, 0, sizeof(sockaddr_in) ) ;
   _peerAddressLen = sizeof ( _peerAddress ) ;

   rc = getsockname ( _fd, (sockaddr*)&_sockAddress, &_addressLen ) ;
   if ( rc )
   {
      PD_LOG(PDERROR,"FAILED TO GET SOCK NAME,error = %d\n",SOCKET_GETLASTERROR);
      //printf ( "Failed to get sock name, error = %d\n",
      //        SOCKET_GETLASTERROR ) ;
      _init = false ;
   }
   else
   {
      //get peer address
      rc = getpeername ( _fd, (sockaddr*)&_peerAddress, &_peerAddressLen ) ;
      PD_RC_CHECK(rc,PDERROR,"Failed to get peer name, error = %d\n",
                 SOCKET_GETLASTERROR );
                 /*
      if ( rc )
      {
         printf ( "Failed to get peer name, error = %d\n",
                 SOCKET_GETLASTERROR ) ;
      }
      */
   }
done:
   return;
error:
   goto done;
}

int ossSocket::initSocket ()
{
   int rc = EDB_OK ;
   if ( _init )
   {
      goto done ;
   }
   memset ( &_peerAddress, 0, sizeof(sockaddr_in) ) ;
   _peerAddressLen = sizeof ( _peerAddress ) ;

   _fd = socket ( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ;//socket<domain,type,protocol>
   if ( -1 == _fd )
   {
      PD_RC_CHECK(EDB_NETWORK,PDERROR,"Failed to initialize socket, error = %d",
              SOCKET_GETLASTERROR ) ;
     // printf ( "Failed to initialize socket, error = %d",
       //       SOCKET_GETLASTERROR ) ;
     // rc = EDB_NETWORK ;
    //  goto error ;
   }
   _init = true ;
   // settimeout should always return EDB_OK
   setTimeout ( _timeout ) ;
done :
   return rc ;
error:
   goto done ;
}
// linger
/*
1、设置 l_onoff为0，则该选项关闭，l_linger的值被忽略，等于内核缺省情况，
close调用会立即返回给调用者，
如果可能将会传输任何未发送的数据；

2、设置 l_onoff为非0，l_linger为0，
则套接口关闭时TCP夭折连接，
TCP将丢弃保留在套接口发送缓冲区中的任何数据并发送一个RST给对方，
而不是通常的四分组终止序列，这避免了TIME_WAIT状态；

3、设置 l_onoff 为非0，l_linger为非0，
当套接口关闭时内核将拖延一段时间（由l_linger决定）。
如果套接口缓冲区中仍残留数据，
进程将处于睡眠状态，直 到（a）所有数据发送完且被对方确认，
之后进行正常的终止序列（描述字访问计数为0）或（b）延迟时间到。
此种情况下，应用程序检查close的返回值是非常重要的，
如果在数据发送完并被确认前时间到，
close将返回EWOULDBLOCK错误且套接口发送缓冲区中的任何数据都丢失。
close的成功返回仅告诉我们发送的数据（和FIN）已由对方TCP确认
它并不能告诉我们对方应用进程是否已读了数据。
如果套接口设为非阻塞的，它将不等待close完成.
*/
int ossSocket::setSocketLi ( int lOnOff, int linger )
{
   int rc = EDB_OK ;
   struct linger _linger ;
   _linger.l_onoff = lOnOff ;
   _linger.l_linger = linger ;
   rc = setsockopt ( _fd, SOL_SOCKET, SO_LINGER,
                     (const char*)&_linger, sizeof (_linger) ) ;

   return rc ;
}

void ossSocket::setAddress(const char* pHostname, unsigned int port )
{
    struct hostent *hp ;
    memset ( &_sockAddress, 0, sizeof(sockaddr_in) ) ;
    memset ( &_peerAddress, 0, sizeof(sockaddr_in) ) ;
    _peerAddressLen = sizeof (_peerAddress) ;

    _sockAddress.sin_family = AF_INET ;
    if ( (hp = gethostbyname ( pHostname )))
       _sockAddress.sin_addr.s_addr = *((int *)hp->h_addr_list[0] ) ;
    else
       _sockAddress.sin_addr.s_addr = inet_addr ( pHostname ) ;

    _sockAddress.sin_port = htons ( port ) ;
    _addressLen = sizeof ( _sockAddress ) ;

}

int ossSocket::bind_listen (){
   int rc = EDB_OK ;
   int temp = 1 ;
   // Allows the socket to be bind to an address that is already in use.
   // For database shutdown and restart right away, before socket close
   //为了防止socket进入timewait状态，我们需要设置SO_REUSEADDR套接字选项
   rc = setsockopt ( _fd, SOL_SOCKET,
                     SO_REUSEADDR,
                     (char*)&temp,
                     sizeof (int) );

   if ( rc ){
      PD_LOG(PDWARNING,"Failed to setsockopt SO_REUSEADDR, rc = %d",
              SOCKET_GETLASTERROR ) ;
      //printf ( "Failed to setsockopt SO_REUSEADDR, rc = %d",
         //     SOCKET_GETLASTERROR ) ;
   }
   rc = setSocketLi( 1, 30 ) ;
   if ( rc ){
      PD_LOG(PDWARNING,"Failed to setsockopt SO_LINGER, rc = %d",
              SOCKET_GETLASTERROR ) ;
      //printf ( "Failed to setsockopt SO_LINGER, rc = %d",
        //      SOCKET_GETLASTERROR ) ;
   }
   //::bind()函数不属于任何类,通常是一个全局函数
   rc = ::bind ( _fd, (struct sockaddr *)&_sockAddress, _addressLen ) ;
   if ( rc ){
      PD_RC_CHECK(EDB_NETWORK,PDERROR,"Failed to bind socket, rc = %d", SOCKET_GETLASTERROR ) ;
     // printf ( "Failed to bind socket, rc = %d", SOCKET_GETLASTERROR ) ;
     // rc = EDB_NETWORK ;
     // goto error ;
   }

   rc = listen ( _fd, SOMAXCONN ) ;
   if ( rc ){
      PD_RC_CHECK(EDB_NETWORK ,PDERROR, "Failed to listen socket, rc = %d",
                   SOCKET_GETLASTERROR ) ;
     // printf ( "Failed to listen socket, rc = %d", SOCKET_GETLASTERROR ) ;
     // rc = EDB_NETWORK ;
     // goto error ;
   }
done :
   return rc ;
error :
   close () ;
   goto done ;
}

int ossSocket::send ( const char *pMsg, int len,
                        int timeout, int flags )
{
   int rc = EDB_OK ;
   int maxFD = _fd ;
   struct timeval maxSelectTime ;
   fd_set fds ;

   maxSelectTime.tv_sec = timeout / 1000000 ;
   maxSelectTime.tv_usec = timeout % 1000000 ;
   // if we don't expect to receive anything, no need to continue
   if ( 0 == len )
      return EDB_OK ;
   // wait loop until the socket is ready
   while ( true )
   {
      FD_ZERO ( &fds ) ;
      FD_SET ( _fd, &fds ) ;
      rc = select ( maxFD + 1, NULL, &fds, NULL,
                    timeout>=0?&maxSelectTime:NULL ) ;

      // 0 means timeout
      if ( 0 == rc )
      {
         rc = EDB_TIMEOUT ;
         goto done ;
      }
      // if < 0, means something wrong
      if ( 0 > rc )
      {
         rc = SOCKET_GETLASTERROR ;
         // if we failed due to interrupt, let's continue
         if ( EINTR == rc )
         {
            continue ;
         }
         PD_RC_CHECK(EDB_NETWORK,PDERROR, "Failed to select from socket, rc = %d",
                 rc);
         //printf ( "Failed to select from socket, rc = %d",
            //     rc);
        // rc = EDB_NETWORK ;
        // goto error ;
      }

      // if the socket we interested is not receiving anything, let's continue
      if ( FD_ISSET ( _fd, &fds ) ){
         break ;
      }
   }
   while ( len > 0 )
   {
      // MSG_NOSIGNAL : Requests not to send SIGPIPE on errors on stream
      // oriented sockets when the other end breaks the connection. The EPIPE
      // error is still returned.
      //当向一个已经收到RST的套接字执行写操作时，内核会向用户进程发送一个SIGPIPE信号。
      //该信号的默认行为是终止进程。send()函数的最后一个参数可以设MSG_NOSIGNAL，来禁止内核发送SIGPIPE信号
      rc = ::send ( _fd, pMsg, len, MSG_NOSIGNAL|flags ) ;
      if ( -1 == rc )
      {
         PD_RC_CHECK(EDB_NETWORK,PDWARNING , "Failed to send, rc = %d", SOCKET_GETLASTERROR ) ;
         //printf ( "Failed to send, rc = %d", SOCKET_GETLASTERROR ) ;
         //rc = EDB_NETWORK ;
         //goto error ;
      }
      len -= rc ;
      pMsg += rc ;
   }
   rc = EDB_OK ;
done :
   return rc ;
error :
   goto done ;
}

bool ossSocket::isConnected ()
{
   int rc = EDB_OK ;
   // MSG_NOSIGNAL : Requests not to send SIGPIPE on errors on stream
   // oriented sockets when the other end breaks the connection. The EPIPE
   // error is still returned.
   rc = ::send ( _fd, "", 0, MSG_NOSIGNAL ) ;
   if ( 0 > rc )
   {
      return false ;
   }
   return true ;
}

#define MAX_RECV_RETRIES 5
int ossSocket::recv ( char *pMsg, int len,
                        int timeout, int flags )
{
   int rc = EDB_OK ;
   int retries = 0 ;
   int maxFD = _fd ;
   struct timeval maxSelectTime ;
   fd_set fds ;

   // if we don't expect to receive anything, no need to continue
   if ( 0 == len )
      return EDB_OK ;

   maxSelectTime.tv_sec = timeout / 1000000 ;
   maxSelectTime.tv_usec = timeout % 1000000 ;

   // wait loop until either we timeout or get a message
   while ( true )
   {
      FD_ZERO ( &fds ) ;
      FD_SET ( _fd, &fds ) ;
      rc = select ( maxFD + 1, &fds, NULL, NULL,
                    timeout>=0?&maxSelectTime:NULL ) ;

      // 0 means timeout
      if ( 0 == rc )
      {
         rc = EDB_TIMEOUT ;
         goto done ;
      }
      // if < 0, means something wrong
      if ( 0 > rc )
      {
         rc = SOCKET_GETLASTERROR ;
         // if we failed due to interrupt, let's continue
         if ( EINTR == rc )
         {
            continue ;
         }
         PD_RC_CHECK(EDB_NETWORK,PDERROR, "Failed to select from socket, rc = %d", rc);
        // printf ( "Failed to select from socket, rc = %d", rc);
        // rc = EDB_NETWORK ;
        // goto error ;
      }

      // if the socket we interested is not receiving anything, let's continue
      if ( FD_ISSET ( _fd, &fds ) )
      {
         break ;
      }
   }
   // Once we start receiving message, there's no chance to timeout, in order to
   // prevent partial read
   while ( len > 0 )
   {
      // MSG_NOSIGNAL : Requests not to send SIGPIPE on errors on stream
      // oriented sockets when the other end breaks the connection. The EPIPE
      // error is still returned.
      rc = ::recv ( _fd, pMsg, len, MSG_NOSIGNAL|flags ) ;

      if ( rc > 0 )
      {
         if ( flags & MSG_PEEK )
         {
            goto done ;
         }
         len -= rc ;
         pMsg += rc ;
      }
      else if ( rc == 0 )
      {
         PD_RC_CHECK(EDB_NETWORK_CLOSE,PDWARNING,"Peer unexpected shutdown" ) ;
        // printf ( "Peer unexpected shutdown" ) ;
        // rc = EDB_NETWORK_CLOSE ;
        // goto error ;
      }
      else
      {
         // if rc < 0
         rc = SOCKET_GETLASTERROR ;
         if ( (EAGAIN == rc || EWOULDBLOCK == rc) &&
              _timeout > 0 )
         {
            // if we timeout, it's partial message and we should restart
            PD_RC_CHECK(EDB_NETWORK,PDERROR,"Recv() timeout: rc = %d", rc ) ;
           // printf ( "Recv() timeout: rc = %d", rc ) ;
           // rc = EDB_NETWORK ;
           // goto error ;
         }
         if ( ( EINTR == rc ) && ( retries < MAX_RECV_RETRIES ) )
         {
            // less than max_recv_retries number, let's retry
            retries ++ ;
            continue ; 
         }
         // something bad when get here
         PD_RC_CHECK(EDB_NETWORK,PDERROR,"Recv() Failed: rc = %d", rc ) ;
        // printf ( "Recv() Failed: rc = %d", rc ) ;
        // rc = EDB_NETWORK ;
        // goto error ;
      }
   }
   // Everything is fine when get here
   rc = EDB_OK ;
done :
   return rc ;
error :
   goto done ;
}




int ossSocket::recvNF ( char *pMsg, int &len,
                          int timeout )
{
   int rc = EDB_OK ;
   int retries = 0 ;
   int maxFD = _fd ;
   struct timeval maxSelectTime ;
   fd_set fds ;

   // if we don't expect to receive anything, no need to continue
   if ( 0 == len )
      return EDB_OK ;

   maxSelectTime.tv_sec = timeout / 1000000 ;
   maxSelectTime.tv_usec = timeout % 1000000 ;
   // wait loop until either we timeout or get a message
   while ( true )
   {
      FD_ZERO ( &fds ) ;
      FD_SET ( _fd, &fds ) ;
      rc = select ( maxFD + 1, &fds, NULL, NULL,
                    timeout>=0?&maxSelectTime:NULL ) ;

      // 0 means timeout
      if ( 0 == rc )
      {
         rc = EDB_TIMEOUT ;
         goto done ;
      }
      // if < 0, means something wrong
      if ( 0 > rc )
      {
         rc = SOCKET_GETLASTERROR ;
         // if we failed due to interrupt, let's continue
         if ( EINTR == rc )
         {
            continue ;
         }
         PD_RC_CHECK(EDB_NETWORK,PDERROR, "Failed to select from socket, rc = %d",
                 rc);
        // printf ( "Failed to select from socket, rc = %d",
            //     rc);
        // rc = EDB_NETWORK ;
       //  goto error ;
      }

      // if the socket we interested is not receiving anything, let's continue
      if ( FD_ISSET ( _fd, &fds ) )
      {
         break ;
      }
   }

   // MSG_NOSIGNAL : Requests not to send SIGPIPE on errors on stream
   // oriented sockets when the other end breaks the connection. The EPIPE
   // error is still returned.
   rc = ::recv ( _fd, pMsg, len, MSG_NOSIGNAL ) ;

   if ( rc > 0 )
   {
      len = rc ;
   }
   else if ( rc == 0 )
   {
      PD_RC_CHECK(EDB_NETWORK_CLOSE,PDWARNING, "Peer unexpected shutdown" ) ;
     // printf ( "Peer unexpected shutdown" ) ;
    //  rc = EDB_NETWORK_CLOSE ;
     // goto error ;
   }
   else
   {
      // if rc < 0
      rc = SOCKET_GETLASTERROR ;
      if ( (EAGAIN == rc || EWOULDBLOCK == rc) &&
           _timeout > 0 )
      {
         // if we timeout, it's partial message and  we should restart
         PD_RC_CHECK(EDB_NETWORK,PDERROR, "Recv() timeout: rc = %d", rc ) ;
         //printf ( "Recv() timeout: rc = %d", rc ) ;
        // rc = EDB_NETWORK ;
        // goto error ;
      }
      if ( ( EINTR == rc ) && ( retries < MAX_RECV_RETRIES ) )
      {
         // less than max_recv_retries number, let's retry
         retries ++ ;
      }
      // something bad when get here
      PD_RC_CHECK(EDB_NETWORK,PDERROR,"Recv() Failed: rc = %d", rc ) ;
     // printf ( "Recv() Failed: rc = %d", rc ) ;
     // rc = EDB_NETWORK ;
     // goto error ;
   }
   // Everything is fine when get here
   rc = EDB_OK ;
done :
   return rc ;
error :
   goto done ;
}

int ossSocket::connect ()
{
   int rc = EDB_OK ;
   rc = ::connect ( _fd, (struct sockaddr *) &_sockAddress, _addressLen ) ;
   if ( rc )
   {
      PD_RC_CHECK(EDB_NETWORK,PDERROR,"Failed to connect, rc = %d", SOCKET_GETLASTERROR ) ;
     // printf ( "Failed to connect, rc = %d", SOCKET_GETLASTERROR ) ;
     // rc = EDB_NETWORK ;
     // goto error ;
   }

   //get local address
   rc = getsockname ( _fd, (sockaddr*)&_sockAddress, &_addressLen ) ;
   if ( rc )
   { 
      PD_RC_CHECK(EDB_NETWORK,PDERROR,"Failed to get local address, rc=%d", rc ) ;
     // printf ( "Failed to get local address, rc=%d", rc ) ;
     // rc = EDB_NETWORK ;
    //  goto error ;
   }
   //get peer address
   rc = getpeername ( _fd, (sockaddr*)&_peerAddress, &_peerAddressLen ) ;
   if ( rc )
   {
      PD_RC_CHECK(EDB_NETWORK,PDERROR,"Failed to get peer address, rc=%d", rc ) ;
     //printf ( "Failed to get peer address, rc=%d", rc ) ;
     // rc = EDB_NETWORK ;
     //  goto error ;
   }
done :
   return rc ;
error :
   goto done ;
}

void ossSocket::close ()
{
   if ( _init )
   {
      int i = 0 ;
      i = ::close ( _fd ) ;
      if ( i < 0 )
      {
         i = -1 ;
      }
      _init = false ;
   }
}
int ossSocket::accept ( int *sock, struct sockaddr *addr, socklen_t
                          *addrlen, int timeout )
{
   int rc = EDB_OK ;
   int maxFD = _fd ;
   struct timeval maxSelectTime ;

   fd_set fds ;
   maxSelectTime.tv_sec = timeout / 1000000 ;
   maxSelectTime.tv_usec = timeout % 1000000 ;
   while ( true )
   {
      FD_ZERO ( &fds ) ;
      FD_SET ( _fd, &fds ) ;
      rc = select ( maxFD + 1, &fds, NULL, NULL,
                    timeout>=0?&maxSelectTime:NULL ) ;

      // 0 means timeout
      if ( 0 == rc )
      {
         *sock = 0 ;
         rc = EDB_TIMEOUT ;
         goto done ;
      }
      // if < 0, means something wrong
      if ( 0 > rc )
      {
         rc = SOCKET_GETLASTERROR ;
         // if we failed due to interrupt, let's continue
         if ( EINTR == rc )
         {
            continue ;
         }
         PD_RC_CHECK(EDB_NETWORK,PDERROR,"Failed to select from socket, rc = %d", SOCKET_GETLASTERROR);
         //printf ( "Failed to select from socket, rc = %d", SOCKET_GETLASTERROR);
         //rc = EDB_NETWORK ;
         //goto error ;
      }

      // if the socket we interested is not receiving anything, let's continue
      if ( FD_ISSET ( _fd, &fds ) )
      {
         break ;
      }
   }
   // reset rc back to EDB_OK, since the rc now is the result from select()
   rc = EDB_OK ;
   *sock = ::accept ( _fd, addr, addrlen ) ;
   if ( -1 == *sock )
   {
      PD_RC_CHECK(EDB_NETWORK,PDERROR,"Failed to accept socket, rc = %d", SOCKET_GETLASTERROR ) ;
     // printf ( "Failed to accept socket, rc = %d", SOCKET_GETLASTERROR ) ;
     // rc = EDB_NETWORK ;
    //  goto error ;
   }
done :
   return rc ;
error :
   close () ;
   goto done ;
}
// nagle() have message delay 
//由于数据库需的操作需要有实时性，我们需要设置TCP_NODELAY套接字选项，来关闭TCP的Nagle算法
int ossSocket::disableNagle ()
{
   int rc = EDB_OK ;
   int temp = 1 ;
   rc = setsockopt ( _fd, IPPROTO_TCP, TCP_NODELAY, (char *) &temp,
                     sizeof ( int ) ) ;
   if ( rc )
   {
      PD_LOG(PDWARNING,"Failed to setsockopt, rc = %d", SOCKET_GETLASTERROR ) ;
     // printf ( "Failed to setsockopt, rc = %d", SOCKET_GETLASTERROR ) ;
   }

   rc = setsockopt ( _fd, SOL_SOCKET, SO_KEEPALIVE, (char *) &temp,
                     sizeof ( int ) ) ;
   if ( rc )
   {
      PD_LOG(PDWARNING,"Failed to setsockopt, rc = %d", SOCKET_GETLASTERROR ) ;
     // printf ( "Failed to setsockopt, rc = %d", SOCKET_GETLASTERROR ) ;
   }
   return rc ;
}

unsigned int ossSocket::_getPort ( sockaddr_in *addr )
{
   return ntohs ( addr->sin_port ) ;
}

int ossSocket::_getAddress ( sockaddr_in *addr, char *pAddress, unsigned int length
)
{
   int rc = EDB_OK ;
   length = length < NI_MAXHOST ? length : NI_MAXHOST ;
   rc = getnameinfo ( (struct sockaddr *)addr, sizeof(sockaddr), pAddress,
length,
                       NULL, 0, NI_NUMERICHOST ) ;
   if ( rc )
   {
      PD_RC_CHECK(EDB_NETWORK,PDERROR, "Failed to getnameinfo, rc = %d", SOCKET_GETLASTERROR ) ;
      //printf ( "Failed to getnameinfo, rc = %d", SOCKET_GETLASTERROR ) ;
     // rc = EDB_NETWORK ;
    //  goto error ;
   }
done :
   return rc ;
error :
   goto done ;
}

unsigned int ossSocket::getLocalPort ()
{
   return _getPort ( &_sockAddress ) ;
}

unsigned int ossSocket::getPeerPort ()
{
   return _getPort ( &_peerAddress ) ;
}

int ossSocket::getLocalAddress ( char * pAddress, unsigned int length )
{
   return _getAddress ( &_sockAddress, pAddress, length ) ;
}

int ossSocket::getPeerAddress ( char * pAddress, unsigned int length )
{
   return _getAddress ( &_peerAddress, pAddress, length ) ;
}

//set timeout seconds size
int ossSocket::setTimeout ( int seconds )
{
   int rc = EDB_OK ;
   struct timeval tv ;
   tv.tv_sec = seconds ;
   tv.tv_usec = 0 ;
   // windows take milliseconds as parameter
   // but linux takes timeval as input

   rc = setsockopt ( _fd, SOL_SOCKET, SO_RCVTIMEO, ( char* ) &tv,
                     sizeof ( tv ) ) ;
   if ( rc )
   {
      PD_LOG(PDWARNING,"Failed to setsockopt, rc = %d", SOCKET_GETLASTERROR ) ;
     //printf ( "Failed to setsockopt, rc = %d", SOCKET_GETLASTERROR ) ;
   }

   rc = setsockopt ( _fd, SOL_SOCKET, SO_SNDTIMEO, ( char* ) &tv,
                     sizeof ( tv ) ) ;
   if ( rc )
   {
      PD_LOG(PDWARNING,"Failed to setsockopt, rc = %d", SOCKET_GETLASTERROR ) ;
     // printf ( "Failed to setsockopt, rc = %d", SOCKET_GETLASTERROR ) ;
   }

   return rc ;
}
int _ossSocket::getHostName ( char *pName, int nameLen )
{
   return gethostname ( pName, nameLen ) ;
}

int _ossSocket::getPort ( const char *pServiceName, unsigned short &port )
{
   int rc = EDB_OK ;
   struct servent *servinfo ;
   servinfo = getservbyname ( pServiceName, "tcp" ) ;
   if ( !servinfo )
      port = atoi ( pServiceName ) ;
   else
      port = (unsigned short)ntohs(servinfo->s_port) ;
   return rc ;
}
  
 
