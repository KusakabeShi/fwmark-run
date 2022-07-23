#include <fcntl.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/socket.h>
#include <string.h>

#ifndef RTLD_NEXT
#define RTLD_NEXT ((void *) -1l)
#endif
#define REAL_LIBC RTLD_NEXT

unsigned int get_so_mark()
{
    const char *so_mark_str = getenv("DEFAULT_SO_MARK");
    if (!so_mark_str)
    {
        return 0;
    }

    long so_mark_long = 0;
    size_t l = strlen(so_mark_str);
    if (l > 2 && strncmp(so_mark_str, "0x", 2) == 0)
    {
        so_mark_long = strtol(so_mark_str, NULL, 16);
    }
    else
    {
        so_mark_long = atol(so_mark_str);
    }

    if (so_mark_long < 0 || so_mark_long > UINT_MAX)
    {
        return 0;
    }

    unsigned int so_mark = (unsigned int) so_mark_long;
    return so_mark;
};

int socket(int family, int type, int protocol)
{
    static int(*real_socket)(int family, int type, int protocol) = NULL;
    if (!real_socket)
    {
        real_socket = (int(*)(int family, int type, int protocol)) dlsym(REAL_LIBC, "socket");
    }

    unsigned int so_mark = get_so_mark();
    int sockid = real_socket(family, type, protocol);
    setsockopt(sockid, SOL_SOCKET, SO_MARK, &so_mark, sizeof(so_mark));
    return sockid;
};