#!/usr/bin/env python
#Used for fs and platform identification
import os,sys,platform
#Used to call external python process
import subprocess
#Used for parsing out information
import re

def main():
	#Get arguments
	argc=len(sys.argv)
	ar1=""
	ar2=""
	ar3=""
	ver=""
	#Set root directory
	rootDir=os.getcwd()
	#Set framework directory
	frameworkDir=os.path.join(rootDir,"framework")
	#Set library directory
	libsDir=os.path.join(rootDir,"libwiiu")
	#Try to grab args
	try:
		ar1=sys.argv[1]
		ar1=sys.argv[2]
	except:
		pass
	#Set project dir
	projectDir=os.path.join(rootDir,ar1)
	#Set root www dir
	wwwDir=os.path.join(rootDir,"www")
	#Set project www dir
	wwwSubDir=os.path.join(wwwDir,os.path.basename(os.path.normpath(projectDir)))
	#Get sys info
	platformStr=platform.system().lower()
	#Build
	print("Building for your "+platformStr+" platform...")
	if not os.path.exists(wwwSubDir):
	    os.makedirs(wwwSubDir)
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
	subprocess.call(['python', gen_html, code532, str(532), wwwSubDir])
	subprocess.call(['python', gen_html, code500, str(500), wwwSubDir])
	subprocess.call(['python', gen_html, code410, str(410), wwwSubDir])
	subprocess.call(['python', gen_html, code400, str(400), wwwSubDir])
	subprocess.call(['python', gen_html, code310, str(310), wwwSubDir])
	subprocess.call(['python', gen_html, code300, str(300), wwwSubDir])
	subprocess.call(['python', gen_html, code210, str(210), wwwSubDir])
	subprocess.call(['python', gen_html, code200, str(200), wwwSubDir])
	#Generate root index file for all projects.
	rootIndexGen(wwwDir);
	
def rootIndexGen(wwwDir):
	#Open master index file for writing
	f = open(os.path.join(wwwDir,'index.html'), 'w')
	#Write html header and body tags
	f.write("<html>\n<head>\n\t</head>\n\t<body>\n")
	f.write("""<style type='text/css'>a { display: block; padding:20px; margin:10px; border: 1px solid; width:33%; float:left; }</style>""")
	#Get list of all project subdirs inside root www dir.
	for subdir in subDirPath(wwwDir):
		#get a list of all payload files for each project.
		payloadVersionFiles=os.listdir(os.path.join(wwwDir,subdir))
		#Keep a list of ints of versions supported
		versionsSupported=[]
		#Look at each file in project directory.
		for fileName in payloadVersionFiles:
			#Look for payload files
			search=re.search('payload(.*?).html', fileName)
			#If payload file is found
			if search:
				#Extract just the version number and add it to versionsSupported list.
				payloadVersion=search.group(1)
				versionsSupported.append(payloadVersion)
		#Check that project isn't empty
		if len(versionsSupported) > 0:
			#Write project index in root index file.
			f.write("\t<a href=\""+os.path.basename(subdir)+"\\index.html\">"+os.path.basename(subdir).replace('_',' ').title()+" (")
			#Add version numbers supported for project.
			for versionNumber in versionsSupported[:-1]:
				f.write(versionNumber+",")
			f.write(versionsSupported[-1])
			f.write(")</a>\n")
		#Write closing html tags.
		f.write("\t</body>\n</html>")
	
def subDirPath (d):
    return filter(os.path.isdir, [os.path.join(d,f) for f in os.listdir(d)])
	
if __name__ == '__main__':
	main()
