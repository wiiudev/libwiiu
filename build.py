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
	os.system("make")
	os.chdir(libsDir)
	os.system("make")
	os.chdir(projectDir)
	os.system("make")
	os.system("python "+frameworkDir+"/generate_html.py "+projectDir+"/bin/code"+str(500)+".bin "+str(500)+" "+wwwDir)
	os.system("python "+frameworkDir+"/generate_html.py "+projectDir+"/bin/code"+str(410)+".bin "+str(410)+" "+wwwDir)
	os.system("python "+frameworkDir+"/generate_html.py "+projectDir+"/bin/code"+str(400)+".bin "+str(400)+" "+wwwDir)

if __name__ == '__main__':
	main()
