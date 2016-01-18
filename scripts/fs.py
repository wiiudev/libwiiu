#!/usr/bin/env python
import socket, struct

FS_RET_NO_ERROR = 0
FS_STATUS_OK = 0
FS_RET_ALL_ERROR = 0xFFFFFFFF

FSClient_size = 0x1700
FSCmdBlock_size = 0xA80

error_base = 0xFFFFFFFF + 1
error_codes = {
	error_base - 1: 'FS_STATUS_CANCELED: Command canceled',
	error_base - 2: 'FS_STATUS_END: Indicates end of file / directory entry',
	error_base - 3: 'FS_STATUS_MAX: Reached to max number of file / directory / client handles / mount points',
	error_base - 4: 'FS_STATUS_ALREADY_OPEN: Target is already opened or locked by another transaction',
	error_base - 5: 'FS_STATUS_EXISTS: Target path already exists or not empty',
	error_base - 6: 'FS_STATUS_NOT_FOUND: Target path is not found',
	error_base - 7: 'FS_STATUS_NOT_FILE: Specified path is directory or quota instead of a file.',
	error_base - 8: 'FS_STATUS_NOT_DIR: Specified path is file instead of a directory or a quota.',
	error_base - 9: 'FS_STATUS_ACCESS_ERROR: Attempted to access file with bad file mode',
	error_base - 10: 'FS_STATUS_PERMISSION_ERROR: Did not have permission to complete operation',
	error_base - 11: 'FS_STATUS_FILE_TOO_BIG: Request would push the file over the size limit (not the quota limit).',
	error_base - 12: 'FS_STATUS_STORAGE_FULL: Request would cause one of the ancestor directories to exceed its quota / Or no free space left in storage',
	error_base - 13: 'FS_STATUS_JOURNAL_FULL: Transaction journal is full, need to flush',
	error_base - 14: 'FS_STATUS_UNSUPPORTED_CMD: Operation is not supported by file system (for only manual mount device)',
	error_base - 15: 'FS_STATUS_MEDIA_NOT_READY: Medium is not ready',
	error_base - 16: 'FS_STATUS_INVALID_MEDIA: Medium is invalid',
	error_base - 17: 'FS_STATUS_MEDIA_ERROR: Medium is in some bad condition',
	error_base - 18: 'FS_STATUS_DATA_CORRUPTED: Data is corrupted',
	error_base - 19: 'FS_STATUS_WRITE_PROTECTED: Medium is write protected',
}

class FS(object):
	rplname = 'coreinit.rpl'
	symnames = ['FSInit', 'FSAddClient', 'FSSetStateChangeNotification', 'FSInitCmdBlock', 'FSOpenDir', 'FSReadDir', 'FSCloseDir']
	
	def __init__(self, rpc):
		self.rpc = rpc
		self.symbols = {symname: rpc.get_symbol(self.rplname, symname) for symname in self.symnames}
		globals().update(self.symbols)
		
		ret = FSInit()
		print("FSInit() returned %d" % ret)
		self.pClient = fake_alloc(FSClient_size)
		self.pCmd = fake_alloc(FSCmdBlock_size)
		print("Allocated client and cmd block")
		
		ret = FSAddClient(self.pClient, FS_RET_NO_ERROR)
		print("FSAddClient() returned %d" % ret)
		ret = FSInitCmdBlock(self.pCmd)
		print("FSInitCmdBlock() returned %d" % ret)
		
	def read_dir(self, path="/vol/content/"):
		rpc = self.rpc
		path_addr = fake_alloc(len(path))
		dh_addr = fake_alloc(4)
		rpc.write_string(path_addr, path)

		self.result = FSOpenDir(self.pClient, self.pCmd, path_addr, dh_addr, FS_RET_ALL_ERROR)
		if self.result != FS_STATUS_OK:
			print('Error opening {}: {}'.format(path, error_codes[self.result]))
			return
		dh = rpc.read32(dh_addr, 1)[0]
		de_addr = fake_alloc(500)
		
		while FSReadDir(self.pClient, self.pCmd, dh, de_addr, FS_RET_ALL_ERROR) == FS_STATUS_OK:
			de = rpc.read_string(de_addr, 500)
			name = de[25*4:].split(b'\x00', 1)[0]
			print(name)
		
		FSCloseDir(self.pClient, self.pCmd, dh, FS_RET_NO_ERROR)
		
class FakeHeap():
	heap = 0x1e900000
	
	def __call__(self, size):
		addr = self.heap		   
		self.heap += size
		if size % 4:
			self.heap += 4 - (size % 4)
		
		return addr

try:
	fake_alloc
except NameError:		 
	fake_alloc = FakeHeap()
