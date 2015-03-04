#!/usr/bin/env python
import socket, struct

class WiiURpc():
	def listen(self, host, port, bound=False):
		if not bound:
			self.s.bind((host, port))
		self.s.listen(1)
		self.conn = self.s.accept()
		print('Connected by ', self.conn[1])

	def log(self):
		while True:
			data = self.conn[0].recv(512)

			if data:
				print(data)

	def read32(self, address, num_words):
		total_words = num_words
		data = []
		
		while total_words > 0:		  
			# Build an RPC request and send it
			num_words = min(total_words, 125)
			request = struct.pack(">III", 0, address, num_words)
			self.conn[0].sendall(request)

			# Block for a reply
			reply = self.conn[0].recv(512)
			fmt = '>' + str(num_words) + 'I'
			data.extend(struct.unpack(fmt, reply[4:4+(num_words*4)]))		 
			total_words -= num_words
		return data
	
	def read_string(self, address, length):
		if length % 4:
			length += 4 - (length % 4)
		num_words = length // 4
		return struct.pack('>{}I'.format(num_words), *self.read32(address, num_words))
	
	def write32(self, address, data):
		# Build an RPC request and send it
		total_words = len(data)
		
		while total_words > 0:
			#assert num_words <= 125
			num_words = min(total_words, 125)
			fmt = ">III" + str(num_words) + 'I'
			request = struct.pack(fmt, 1, address, num_words, *data[:num_words])
			self.conn[0].sendall(request)
			total_words -= num_words
			data = data[num_words:]
		
	def write_string(self, address, string):
		# Ugly hack, need to make command for this later
		string = string.encode('utf8')
		while len(string) % 4:
			string += b"\x00"
		data = struct.unpack('>{}I'.format(len(string) // 4), string)
		self.write32(address, data)

	def get_symbol(self, rplname, symname):
		# Build an RPC request and send it
		request = struct.pack(">III", 2, 12, 12 + len(rplname) + 1)
		request += rplname.encode('utf-8') + b"\x00"
		request += symname.encode('utf-8') + b"\x00"
		self.conn[0].sendall(request)

		# Block for a reply
		reply = self.conn[0].recv(512)
		address = struct.unpack(">I", reply[4:8])[0]
		return ExportedSymbol(address, self, rplname, symname)
	
	def call(self, address, *args):
		# Turn the arguments into a list and add 0 for unused slots
		arguments = list(args)
		while len(arguments) != 5:
			arguments.append(0)

		# Build an RPC request and send it
		request = struct.pack(">II5I", 3, address, *arguments)
		self.conn[0].sendall(request)

		# Block for a reply
		reply = self.conn[0].recv(512)
		return struct.unpack(">I", reply[4:8])[0]

	def get_logs(self, filename='logdump.bin', save_addr=0x1e800000):
		OSLogRetrieve = self.get_symbol('coreinit.rpl', 'OSLogRetrieve')
		self.write32(save_addr, (0,) * 125)
		result = OSLogRetrieve((1<<11)|(1<<12), save_addr, 0x01000000)
		size = self.read32(save_addr, 1)[0]
		print(result)
		
		logs = []
		for address in range(save_addr, save_addr + size, 500):
			logs += list(self.read32(address, 125))

		dump = open(filename, 'wb')
		for word in logs:
			dump.write(struct.pack(">I", word))
		dump.close()

	def walkThreadList(self):
		symbol('coreinit.rpl','OSGetCurrentThread')
		symbol('coreinit.rpl','OSGetActiveThreadLink')
		symbol('coreinit.rpl','OSGetThreadName')
		symbol('coreinit.rpl','OSCheckActiveThreads')

		threadNum=OSCheckActiveThreads()
		print('Total Number of Active Threads:'+str(threadNum))
		thread=OSGetCurrentThread()
		print('Current thread is located at:'+hex(thread))
		print('Walking backwards...')
		while True:
			name=OSGetThreadName(thread)

			if name>0:
				print('Thread name for thread at:'+hex(thread))
				print(str(rpc.read_string(name,25)))
			else:
				print('No Name Found for thread at:'+hex(thread))
			ret=OSGetActiveThreadLink(thread,0x26800000)
			if ret==0:
				print('Couldn''t grab thread link for thread at:'+hex(thread))
				break
			thread=rpc.read32(0x26800000,2).pop(0)
			if thread==0:
				print('End of thread list')
				break
		print('Done')
		thread=OSGetCurrentThread()
		print('Current thread is located at:'+hex(thread))
		print('Walking forward...')
		while True:
			name=OSGetThreadName(thread)

			if name>0:
				print('Thread name for thread at:'+hex(thread))
				print(str(rpc.read_string(name,25)))
			else:
				print('No Name Found for thread at:'+hex(thread))
			ret=OSGetActiveThreadLink(thread,0x26800000)
			if ret==0:
				print('Couldn''t grab thread link for thread at:'+hex(thread))
				break
			thread=rpc.read32(0x26800000,2).pop(1)
			if thread==0:
				print('End of thread list')
				break
		print('Done')
		
	def dma_copy_mem_and_read(self, dst, src, size):
		# dst = 0x1e900000
		symbol('dmae.rpl', 'DMAECopyMem')
		symbol('dmae.rpl', 'DMAEWaitDone')
		symbol('coreinit.rpl', 'DCFlushRange')
		symbol('coreinit.rpl', 'DCInvalidateRange')
		
		DCFlushRange(src, size)
		DCInvalidateRange(dst, size)
		
		success = DMAEWaitDone(DMAECopyMem(dst, src, size // 4, 0))
		if success:
			return self.read_string(dst, size)
		else:
			print("DMAE copy failed")
		
	def exit(self):
		"""Close browser and disconnect"""
		_Exit = self.get_symbol('coreinit.rpl', '_Exit')
		try:
			_Exit()
		except socket.error:
			#self.listen('0.0.0.0', 12345, True)
			exit()
		
	def __init__(self):
		self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)		
		self.data = []
		self.listen('0.0.0.0', 12345)
		
class ExportedSymbol(object):
	def __init__(self, address, rpc=None, rplname=None, symname=None):
		self.address = address
		self.rpc = rpc
		self.rplname = rplname
		self.symname = symname
		print(symname, address)
		
	def __call__(self, *args):
		return self.rpc.call(self.address, *args)
	
def symbol(rplname, symname):
	if not symname in globals():
		globals()[symname] = rpc.get_symbol(rplname, symname)

rpc = WiiURpc()
#from fs import FS
