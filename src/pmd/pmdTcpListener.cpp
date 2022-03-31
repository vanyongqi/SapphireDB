#include"core.hpp"
#include"ossSocket.hpp"

#define PMD_TCPLISTENER_RETRY 5
#define OSS_MAX_SERVICENAME NI_MAXSERV

int pmdTcpListenerEntryPoint(){
    int rc = EDB_OK;
    int port = 48127;
    ossSocket sock(port);
    rc = sock.initSocket();
    if(rc){
        printf(" failed to initailize socket,rc =%d",rc);
        goto error;
    }
    rc = sock.bind_listen();
    if(rc ){
        printf("failed to bin/listen socket,rc =%d",rc); 
        goto error;
    }
    while(true){
        int s;
        rc = sock.accept(&s , NULL,NULL);
        // if we don't get anything from period of time, let's loop
        if(EDB_TIMEOUT ==rc){
            rc = EDB_OK;
            continue;
        }
        char buffer[1024];
        int size;

        ossSocket sock1(& s);
        sock1.disableNagle();
        do{
            rc = sock1.recv((char*)&size,4);
            if(rc&&rc!= EDB_TIMEOUT){
                printf("failed to receive size rc = %d",rc);
                goto error;
            }
        }while(EDB_TIMEOUT ==rc);
        do{
            rc = sock1.recv(&buffer[0],size-sizeof(int));
            if(rc && rc!= EDB_TIMEOUT){
                printf("failed to receive buffer,rc = %d",rc);
                goto error;
            }while(EDB_TIMEOUT ==rc);
            printf("%s\n",buffer);
            sock1.close();
        }while(EDB_TIMEOUT ==rc);
        printf("%s\n",buffer);
        sock1.close();
        }
        done :
            return rc;
        error:
            switch(rc){
                case EDB_SYS:
                printf("system error occured");
                break;
                default:
                printf("internal error");
            }
            goto done;
    }
