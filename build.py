import os,sys,platform
import subprocess

def main():
	argc=len(sys.argv)
	ar1=""
	ar2=""
	ar3=""
	ver=""
	rootDir=os.path.dirname(os.path.realpath(sys.argv[0]))
	frameworkDir=os.path.join(rootDir,"framework")
	libsDir=os.path.join(rootDir,"libwiiu")
	wwwDir=os.path.join(rootDir,"www")
	try:
		ar1=sys.argv[1]
		ar1=sys.argv[2]
	except:
		pass
	projectDir=os.path.join(rootDir,ar1)
	platformStr=platform.system().lower()
	print("Building for your "+platformStr+" platform...")
	if not os.path.exists(wwwDir):
	    os.mkdir(wwwDir)
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
	code300 = os.path.join(projectDir, 'bin', 'code') + str(300) + '.bin'
	subprocess.call(['python', gen_html, code532, str(532), wwwDir])
	subprocess.call(['python', gen_html, code500, str(500), wwwDir])
	subprocess.call(['python', gen_html, code410, str(410), wwwDir])
	subprocess.call(['python', gen_html, code400, str(400), wwwDir])
	subprocess.call(['python', gen_html, code300, str(300), wwwDir])

if __name__ == '__main__':
	main()
