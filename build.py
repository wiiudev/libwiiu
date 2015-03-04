import os,sys,platform
import subprocess

def main():
	argc=len(sys.argv)
	ar1=""
	ar2=""
	ar3=""
	ver=""
	rootDir=os.path.dirname(os.path.realpath(sys.argv[0]))
	frameworkDir=rootDir+"\\framework"
	libsDir=rootDir+"\\libwiiu"
	wwwDir=rootDir+"\\www"
	try:
		ar1=sys.argv[1]
		ar1=sys.argv[2]
	except:
		pass
	projectDir=rootDir+"\\"+ar1
	platformStr=platform.system().lower()
	print("Building for your "+platformStr+" platform...")
	os.mkdir(wwwDir)
	os.chdir(frameworkDir)
	os.system("Make")
	os.chdir(libsDir)
	os.system("Make")
	os.chdir(projectDir)
	os.system("Make")
	os.system("python "+frameworkDir+"/generate_html.py "+projectDir+"/bin/code"+str(500)+".bin "+str(500)+" "+wwwDir)
	os.system("python "+frameworkDir+"/generate_html.py "+projectDir+"/bin/code"+str(410)+".bin "+str(410)+" "+wwwDir)
	os.system("python "+frameworkDir+"/generate_html.py "+projectDir+"/bin/code"+str(400)+".bin "+str(400)+" "+wwwDir)

if __name__ == '__main__':
	main()