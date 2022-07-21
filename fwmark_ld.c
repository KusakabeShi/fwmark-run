#include <fcntl.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/socket.h> 

int g_obvio=0;

#ifndef RTLD_NEXT
#define RTLD_NEXT ((void *) -1l)
#endif

#define REAL_LIBC RTLD_NEXT

unsigned int get_so_mark() {
    const char* so_mark_str = getenv("DEFAULT_SO_MARK");
    if(!so_mark_str){
        return 0;
    }
    long temp = atol(so_mark_str);
    if (temp < 0 || temp > INT_MAX){
        return 0;
    }
    unsigned int so_mark = (unsigned int)temp;
    return so_mark;
};

int socket(int family, int type, int protocol)
{
    static int (*real_socket) (int family, int type, int protocol) = NULL;
    if (!real_socket){
	    real_socket = (int (*) (int family, int type, int protocol)) dlsym (REAL_LIBC, "socket");
    }
    
    unsigned int so_mark = get_so_mark();
    int sockid = real_socket(family,type,protocol);
    setsockopt(sockid, SOL_SOCKET, SO_MARK, &so_mark, sizeof(so_mark));
    return sockid;
};
