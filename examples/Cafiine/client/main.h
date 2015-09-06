/* string.h */
#define NULL ((void *)0)

void *memcpy(void *dst, const void *src, int bytes);
void *memset(void *dst, int val, int bytes);

/* malloc.h */
extern void *(* const MEMAllocFromDefaultHeapEx)(int size, int align);
#define memalign (*MEMAllocFromDefaultHeapEx)
/* socket.h */
#define AF_INET 2
#define SOCK_STREAM 1
#define IPPROTO_TCP 6

extern void socket_lib_init();
extern int socket(int domain, int type, int protocol);
extern int socketclose(int socket);
extern int connect(int socket, void *addr, int addrlen);
extern int send(int socket, const void *buffer, int size, int flags);
extern int recv(int socket, void *buffer, int size, int flags);

struct in_addr {
	unsigned int s_addr;
};
struct sockaddr_in {
    short sin_family;
    unsigned short sin_port;
    struct in_addr sin_addr;
    char sin_zero[8];
};

/* OS stuff */
extern const long long title_id;

/* Forward declarations */
#define MAX_CLIENT 32
#define MASK_FD 0x0fff00ff

struct bss_t {
    int socket_fsa[MAX_CLIENT];
    void *pClient_fs[MAX_CLIENT];
    int socket_fs[MAX_CLIENT];
    volatile int lock;
};

#define bss_ptr (*(struct bss_t **)0x100000e4)
#define bss (*bss_ptr)

void cafiine_connect(int *socket);
void cafiine_disconnect(int socket);
int cafiine_fopen(int socket, int *result, const char *path, const char *mode, int *handle);
void cafiine_send_handle(int sock, int client, const char *path, int handle);
void cafiine_send_file(int sock, char *file, int size, int fd);
int cafiine_fread(int socket, int *result, void *buffer, int size, int count, int fd);
int cafiine_fclose(int socket, int *result, int fd);
int cafiine_fsetpos(int socket, int *result, int fd, int set);
int cafiine_fgetpos(int socket, int *result, int fd, int *pos);
int cafiine_fstat(int sock, int *result, int fd, void *ptr);
int cafiine_feof(int sock, int *result, int fd);
void cafiine_send_ping(int sock, int val1, int val2);