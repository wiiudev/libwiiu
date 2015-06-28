#!/usr/bin/env python

import os,sys,platform
import subprocess

def main():
	argc=len(sys.argv)
	ar1=""
	ar2=""
	ar3=""
	ver=""
	rootDir=os.getcwd()
	frameworkDir=os.path.join(rootDir,"framework")
	libsDir=os.path.join(rootDir,"libwiiu")
	try:
		ar1=sys.argv[1]
		ar1=sys.argv[2]
	except:
		pass
	projectDir=os.path.join(rootDir,ar1)
	wwwDir=os.path.join(rootDir,"www",os.path.basename(os.path.normpath(projectDir)))
	platformStr=platform.system().lower()
	print("Building for your "+platformStr+" platform...")
	if not os.path.exists(wwwDir):
	    os.makedirs(wwwDir)
	os.chdir(frameworkDir)
	subprocess.call(['make'])
	os.chdir(libsDir)
	subprocess.call(['make'])
	os.chdir(projectDir)
	subprocess.call(['make'])
	gen_html = os.path.join(frameworkDir, 'generate_html.py')
	code532 = os.path.join(projectDir, 'bin', 'code') + str(532) + '.bin'
	code500 = os.path.join(projectDir, 'bin', 'code') + str(500) + '.bin'
	code410 = os.path.join(projectDir, 'bin', 'code') + str(410) + '.bin'
	code400 = os.path.join(projectDir, 'bin', 'code') + str(400) + '.bin'
	code310 = os.path.join(projectDir, 'bin', 'code') + str(310) + '.bin'
	code300 = os.path.join(projectDir, 'bin', 'code') + str(300) + '.bin'
	code210 = os.path.join(projectDir, 'bin', 'code') + str(210) + '.bin'
	code200 = os.path.join(projectDir, 'bin', 'code') + str(200) + '.bin'
	subprocess.call(['python', gen_html, code532, str(532), wwwDir])
	subprocess.call(['python', gen_html, code500, str(500), wwwDir])
	subprocess.call(['python', gen_html, code410, str(410), wwwDir])
	subprocess.call(['python', gen_html, code400, str(400), wwwDir])
	subprocess.call(['python', gen_html, code310, str(310), wwwDir])
	subprocess.call(['python', gen_html, code300, str(300), wwwDir])
	subprocess.call(['python', gen_html, code210, str(210), wwwDir])
	subprocess.call(['python', gen_html, code200, str(200), wwwDir])

if __name__ == '__main__':
	main()
