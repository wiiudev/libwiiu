#include "loader.h"

/* Log hook */
int _start(int arg0, int arg1, unsigned int arg2, void *addr, int len, int arg5)
{
	/* Kernel functions */
#if VER == 532
	void (*spinlock)(void *lock) = (void (*)(void*))0xFFF075BC;
	void (*spunlock)(void *lock) = (void (*)(void*))0xFFF075E0;
	int (*fd_to_handle)(unsigned int *ipcbuf, int rampid) = (int (*)(unsigned int*,int))0xFFF0C7F8;
	int (*IOS_IoctlvAsync)(int arg0, int fd, int cmd, int cnt_in, int cnt_out, struct iovec *vecs, void *cb, void *usrdata) = (int (*)(int,int,int,int,int,struct iovec*,void*,void*))0xFFF0BAF4;
	int (*IOS_WaitReply)(void *usrdata, int arg1, int arg2) = (int (*)(void*,int,int))0xFFF18150;
	void* (*memmove)(void *dst, void *src, int len) = (void* (*)(void*,void*,int))0xFFF09C7C;
#elif VER == 550
	void (*spinlock)(void *lock) = (void (*)(void*))0x0;
	void (*spunlock)(void *lock) = (void (*)(void*))0x0;
	int (*fd_to_handle)(unsigned int *ipcbuf, int rampid) = (int (*)(unsigned int*,int))0x0;
	int (*IOS_IoctlvAsync)(int arg0, int fd, int cmd, int cnt_in, int cnt_out, struct iovec *vecs, void *cb, void *usrdata) = (int (*)(int,int,int,int,int,struct iovec*,void*,void*))0x0;
	int (*IOS_WaitReply)(void *usrdata, int arg1, int arg2) = (int (*)(void*,int,int))0x0;
	void* (*memmove)(void *dst, void *src, int len) = (void* (*)(void*,void*,int))0x0;
#else
	void (*spinlock)(void *lock) = (void (*)(void*))0x0;
	void (*spunlock)(void *lock) = (void (*)(void*))0x0;
	int (*fd_to_handle)(unsigned int *ipcbuf, int rampid) = (int (*)(unsigned int*,int))0x0;
	int (*IOS_IoctlvAsync)(int arg0, int fd, int cmd, int cnt_in, int cnt_out, struct iovec *vecs, void *cb, void *usrdata) = (int (*)(int,int,int,int,int,struct iovec*,void*,void*))0x0;
	int (*IOS_WaitReply)(void *usrdata, int arg1, int arg2) = (int (*)(void*,int,int))0x0;
	void* (*memmove)(void *dst, void *src, int len) = (void* (*)(void*,void*,int))0x0;
#endif
	
	/* Only do this if socket open */
	int sockfd = *((int*)0xFFF00B48);
	if (sockfd != -1)
	{
		/* Lock a spinlock */
		spinlock((void*)(0xA0000000 + (0xFFF00B4C - 0xC0000000)));

		/* Get socket FD and convert to handle */
		unsigned int ipcbuf[3] = {0, 0, *((unsigned int*)0xFFF00B44)};
		fd_to_handle(ipcbuf, 4);

		/* Copy socket data to 0x40-aligned buffer */
		memmove((void*)0xDFF01500, addr, len);

		/* Execute send() request and wait for completion */
		unsigned int reqbuf[2] = {(unsigned int)sockfd, 0};
		int sendlen = ((len / 0x40) * 0x40) + ((len % 0x40 == 0) ? 0 : 0x40);
		struct iovec vecs[4];
		vecs[0].buffer = (void*)&reqbuf[0]; vecs[0].len = 8; vecs[0].unknown8[0] = vecs[0].unknown8[1] = vecs[0].unknown8[2] = vecs[0].unknown8[3] = 0;
		vecs[1].buffer = (void*)0xDFF01500; vecs[1].len = sendlen; vecs[1].unknown8[0] = vecs[1].unknown8[1] = vecs[1].unknown8[2] = vecs[1].unknown8[3] = 0;
		vecs[2].buffer = (void*)0; vecs[2].len = 0; vecs[2].unknown8[0] = vecs[2].unknown8[1] = vecs[2].unknown8[2] = vecs[2].unknown8[3] = 0;
		vecs[3].buffer = (void*)0; vecs[3].len = 0; vecs[3].unknown8[0] = vecs[3].unknown8[1] = vecs[3].unknown8[2] = vecs[3].unknown8[3] = 0;
		IOS_IoctlvAsync(0, (int)ipcbuf[2], 0xE, 4, 0, vecs, (void*)0, (void*)0x1234ABC0);
		IOS_WaitReply((void*)0x1234ABC0, 0xF4240, 0);

		/* Release the spinlock */
		spunlock((void*)(0xA0000000 + (0xFFF00B4C - 0xC0000000)));
	}

	/* Pass through to real function */
#if VER == 532
	int (*real_func)(int arg0, int arg1, unsigned int arg2, void *addr, int len, int arg5) = (int (*)(int,int,unsigned int,void*,int,int))0xFFF1AB94;
#elif VER == 550
	int (*real_func)(int arg0, int arg1, unsigned int arg2, void *addr, int len, int arg5) = (int (*)(int,int,unsigned int,void*,int,int))0x0;
#else
	int (*real_func)(int arg0, int arg1, unsigned int arg2, void *addr, int len, int arg5) = (int (*)(int,int,unsigned int,void*,int,int))0x0;
#endif
	return real_func(arg0, arg1, arg2, addr, len, arg5);
}
