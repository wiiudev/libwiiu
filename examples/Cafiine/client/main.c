#include "main.h"

#define DECL(res, name, ...) \
	extern res name(__VA_ARGS__); \
	res (* real_ ## name)(__VA_ARGS__)  __attribute__((section(".magicptr"))); \
	res my_ ## name(__VA_ARGS__)

DECL(int, FSAInit, void) {
    if ((int)bss_ptr == 0x0a000000) {
        bss_ptr = memalign(sizeof(struct bss_t), 0x40);
        memset(bss_ptr, 0, sizeof(struct bss_t));
    }
    return real_FSAInit();
}
DECL(int, FSAShutdown, void) {
    return real_FSAShutdown();
}
DECL(int, FSAAddClient, void *r3) {
    int res = real_FSAAddClient(r3);

    if ((int)bss_ptr != 0x0a000000 && res < MAX_CLIENT && res >= 0) {
        cafiine_connect(&bss.socket_fsa[res]);
    }

    return res;
}
DECL(int, FSADelClient, int client) {
    if ((int)bss_ptr != 0x0a000000 && client < MAX_CLIENT && client >= 0) {
        cafiine_disconnect(bss.socket_fsa[client]);
    }

    return real_FSADelClient(client);
}
DECL(int, FSAOpenFile, int client, const char *path, const char *mode, int *handle) {
    if ((int)bss_ptr != 0x0a000000 && client < MAX_CLIENT && client >= 0) {
        int ret;
        if (cafiine_fopen(bss.socket_fsa[client], &ret, path, mode, handle) == 0)
            return ret;
    }

    return real_FSAOpenFile(client, path, mode, handle);
}

static int client_num_alloc(void *pClient) {
    int i;

    for (i = 0; i < MAX_CLIENT; i++)
        if (bss.pClient_fs[i] == 0) {
            bss.pClient_fs[i] = pClient;
            return i;
        }
    return -1;
}
static void clietn_num_free(int client) {
    bss.pClient_fs[client] = 0;
}
static int client_num(void *pClient) {
    int i;

    for (i = 0; i < MAX_CLIENT; i++)
        if (bss.pClient_fs[i] == pClient)
            return i;
    return -1;
}

struct fs_async_t {
    void (*callback)(int status, int command, void *request, void *response, void *context);
    void *context;
    void *queue;
};

DECL(int, FSInit, void) {
    if ((int)bss_ptr == 0x0a000000) {
        bss_ptr = memalign(sizeof(struct bss_t), 0x40);
        memset(bss_ptr, 0, sizeof(struct bss_t));
    }
    return real_FSInit();
}
DECL(int, FSShutdown, void) {
    return real_FSShutdown();
}
DECL(int, FSAddClientEx, void *r3, void *r4, void *r5) {
    int res = real_FSAddClientEx(r3, r4, r5);

    if ((int)bss_ptr != 0x0a000000 && res >= 0) {
        int client = client_num_alloc(r3);
        if (client < MAX_CLIENT && client >= 0) {
            cafiine_connect(&bss.socket_fs[client]);
        }
    }

    return res;
}
DECL(int, FSDelClient, void *pClient) {
    if ((int)bss_ptr != 0x0a000000) {
        int client = client_num(pClient);
        if (client < MAX_CLIENT && client >= 0) {
            cafiine_disconnect(bss.socket_fs[client]);
            clietn_num_free(client);
        }
    }

    return real_FSDelClient(pClient);
}
DECL(int, FSCloseFile, void *pClient, void *pCmd, int fd, int error) {
    if ((int)bss_ptr != 0x0a000000 && ((fd & MASK_FD) == MASK_FD)) {
        int client = client_num(pClient);
        if (client < MAX_CLIENT && client >= 0) {
            int ret;
            if (cafiine_fclose(bss.socket_fsa[client], &ret, fd) == 0) {
                return ret;
            }
        }
    }

    return real_FSCloseFile(pClient, pCmd, fd, error);
}

DECL(int, FSSetPosFile, void *pClient, void *pCmd, int fd, int pos, int error) {
    if ((int)bss_ptr != 0x0a000000 && ((fd & MASK_FD) == MASK_FD)) {
        int client = client_num(pClient);
        if (client < MAX_CLIENT && client >= 0) {
            int ret;
            if (cafiine_fsetpos(bss.socket_fsa[client], &ret, fd, pos) == 0) {
                    return ret;
            }
        }
    }

    return real_FSSetPosFile(pClient, pCmd, fd, pos, error);
}
DECL(int, FSGetPosFile, void *pClient, void *pCmd, int fd, int *pos, int error) {
    if ((int)bss_ptr != 0x0a000000 && ((fd & MASK_FD) == MASK_FD)) {
        int client = client_num(pClient);
        if (client < MAX_CLIENT && client >= 0) {
            int ret;
            if (cafiine_fgetpos(bss.socket_fsa[client], &ret, fd, pos) == 0) {
                return ret;
            }
        }
    }

    return real_FSGetPosFile(pClient, pCmd, fd, pos, error);
}
DECL(int, FSGetStatFile, void *pClient, void *pCmd, int fd, void *buffer, int error) {
    if ((int)bss_ptr != 0x0a000000 && ((fd & MASK_FD) == MASK_FD)) {
        int client = client_num(pClient);
        if (client < MAX_CLIENT && client >= 0) {
            int ret;
            if (cafiine_fstat(bss.socket_fsa[client], &ret, fd, buffer) == 0) {
                return ret;
            }
        }
    }

    return real_FSGetStatFile(pClient, pCmd, fd, buffer, error);
}
DECL(int, FSIsEof, void *pClient, void *pCmd, int fd, int error) {
    if ((int)bss_ptr != 0x0a000000 && ((fd & MASK_FD) == MASK_FD)) {
        int client = client_num(pClient);
        if (client < MAX_CLIENT && client >= 0) {
            int ret;
            if (cafiine_feof(bss.socket_fsa[client], &ret, fd) == 0) {
                return ret;
            }
        }
    }

    return real_FSIsEof(pClient, pCmd, fd, error);
}

DECL(int, FSReadFile, void *pClient, void *pCmd, void *buffer, int size, int count, int fd, int flag, int error) {
    if ((int)bss_ptr != 0x0a000000 && ((fd & MASK_FD) == MASK_FD)) {
        int client = client_num(pClient);
        if (client < MAX_CLIENT && client >= 0) {
            int ret;
            if (cafiine_fread(bss.socket_fsa[client], &ret, buffer, size, count, fd) == 0) {
                return ret;
            }
        }
    }

    return real_FSReadFile(pClient, pCmd, buffer, size, count, fd, flag, error);
}
DECL(int, FSReadFileWithPos, void *pClient, void *pCmd, void *buffer, int size, int count, int pos, int fd, int flag, int error) {
    if ((int)bss_ptr != 0x0a000000 && ((fd & MASK_FD) == MASK_FD)) {
        int client = client_num(pClient);
        if (client < MAX_CLIENT && client >= 0) {
            int ret;
            if (cafiine_fsetpos(bss.socket_fsa[client], &ret, fd, pos) == 0) {
                if (cafiine_fread(bss.socket_fsa[client], &ret, buffer, size, count, fd) == 0) {
                    return ret;
                }
            }
        }
    }

    return real_FSReadFileWithPos(pClient, pCmd, buffer, size, count, pos, fd, flag, error);
}

#define DUMP_BLOCK_SIZE (0x200 * 100)
#define DUMP_BLOCK_SIZE_SLOW (0x20 * 100)
DECL(int, FSOpenFile, void *pClient, void *pCmd, const char *path, const char *mode, int *handle, int error) {
    int my_ret = -1;
    error = 0xffffffff;
    if ((int)bss_ptr != 0x0a000000) {
        int client = client_num(pClient);
        if (client < MAX_CLIENT && client >= 0) {
            int ret;
            my_ret = cafiine_fopen(bss.socket_fsa[client], &ret, path, mode, handle);
            if (my_ret == 0) {
                // File exists in cafiine server, a new handle has been created
                return ret;
            }
            else if (my_ret >= 1) {
                // File has been requested from cafiine server,
                ret = real_FSOpenFile(pClient, pCmd, path, mode, handle, error);
                if (ret >= 0) {
                    int size = (my_ret == 1 ? DUMP_BLOCK_SIZE : DUMP_BLOCK_SIZE_SLOW);
                    cafiine_send_handle(bss.socket_fsa[client], client, path, *handle);
                    void* buffer = memalign(sizeof(char) * size, 0x40);
                    int ret2;
                    while ((ret2 = real_FSReadFile(pClient, pCmd, buffer, 1, size, *handle, 0, error)) > 0)
                        cafiine_send_file(bss.socket_fsa[client], buffer, ret2, *handle);
                    cafiine_fclose(bss.socket_fsa[client], &ret2, *handle);
                    real_FSSetPosFile(pClient, pCmd, *handle, 0, error);
                }
                return ret;
            }
        }
    }

    return real_FSOpenFile(pClient, pCmd, path, mode, handle, error);
}

#define MAKE_MAGIC(x) { x, my_ ## x, &real_ ## x }

struct magic_t {
    const void *real;
    const void *replacement;
    const void *call;
} methods[] __attribute__((section(".magic"))) = {
    MAKE_MAGIC(FSAInit),
    MAKE_MAGIC(FSAShutdown),
    MAKE_MAGIC(FSAAddClient),
    MAKE_MAGIC(FSADelClient),
    MAKE_MAGIC(FSAOpenFile),
    MAKE_MAGIC(FSInit),
    MAKE_MAGIC(FSShutdown),
    MAKE_MAGIC(FSAddClientEx),
    MAKE_MAGIC(FSDelClient),
    MAKE_MAGIC(FSOpenFile),
    MAKE_MAGIC(FSCloseFile),
    MAKE_MAGIC(FSReadFile),
    MAKE_MAGIC(FSReadFileWithPos),
    MAKE_MAGIC(FSGetPosFile),
    MAKE_MAGIC(FSSetPosFile),
    MAKE_MAGIC(FSGetStatFile),
    MAKE_MAGIC(FSIsEof),
};
