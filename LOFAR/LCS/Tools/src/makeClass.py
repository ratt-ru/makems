#! /usr/bin/env python
#
# makeClass.py: Script to make default class files in a Package/srcdir in the
# LOFAR development tree. normal class files, main program and templates
# are covered
#
# Usage:
#        ./makeClass [-h] [-t list [-d] | -m] [ClassName]
# Args:
#       ClassName 	    The name of the Class that will be created
#       h,--help            usage
#       t,--templated list  This is an automated templated class,
#                           list can contain a comma seperated list
#                           with the template parameters. Example:
#                           makeClass -t T,U className
#       d,--diy             Do it yourself (manual template instanciation)
#                           Only together with -t
#       m,--main            This is a main program for a class
#
# Revisions:
#
#  26-01-2005   Initial Release.
#
#  Copyright (C) 2005
#  ASTRON (Netherlands Foundation for Research in Astronomy)
#  P.O.Box 2, 7990 AA Dwingeloo, The Netherlands, seg@astron.nl
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#  $Id: makeClass.py 10490 2007-08-31 09:39:48Z diepen $

#
# import all packages we need
#
import os
import sys
import getopt
import re
from datetime import date

def openFile(name,mode):
  try:
    file = open (name,mode)
  except IOError, message:
    sys.exit("Error opening file: %s" % message)
  return file

def replacePackageAndClassName(readFile,writeFile,packageName,
                               className,subDirName):
  aLine=readFile.readline()
  year=`date.today().year`
  while aLine != "":
    #set start of copyright year
    if aLine.find("%YEAR%") > -1:
      aLine = str.replace(aLine,"%YEAR%",year)

    # replace SUB with Subdir when needed
    if aLine.find("%SUB%") > -1:
      if subDirName != "":
        aLine = str.replace(aLine,"%SUB%",subDirName+"/")
      else:
        aLine = str.replace(aLine,"%SUB%",subDirName)

    # replace SUBUPPER with Subdir in uppercase when needed
    if aLine.find("%SUBUPPER%") > -1:
      if subDirName != "":
        aLine = str.replace(aLine,"%SUBUPPER%",subDirName.upper()+"_")
      else:
        aLine = str.replace(aLine,"%SUBUPPER%",subDirName.upper())

    # replace PACKAGE with real name
    if aLine.find("%PACKAGE%") > -1:
      aLine = str.replace(aLine,"%PACKAGE%",packageName)

    # replace PACKAGEUPPER with uppercase Package name
    if aLine.find("%PACKAGEUPPER%") > -1:
      aLine = str.replace(aLine,"%PACKAGEUPPER%",packageName.upper())

    # replace CLASS with real name
    if aLine.find("%CLASS%") > -1:
      aLine = str.replace(aLine,"%CLASS%",className)
      
    # replace CLASSUPPER with uppercase classname
    if aLine.find("%CLASSUPPER%") > -1:
      aLine = str.replace(aLine,"%CLASSUPPER%",className.upper())
      
    writeFile.write(aLine)
    aLine=readFile.readline()
  
def addTemplates(type,readFile,writeFile,className,packageName,templateList,autoTemplate,subDirName):
  aLine=readFile.readline()
  year=`date.today().year`
  
  while aLine != "":
    #set start of copyright year
    if aLine.find("%YEAR%") > -1:
      aLine = str.replace(aLine,"%YEAR%",year)
      
    # replace SUB with Subdir when needed
    if aLine.find("%SUB%") > -1:
      if subDirName != "":
        aLine = str.replace(aLine,"%SUB%",subDirName+"/")
      else:
        aLine = str.replace(aLine,"%SUB%",subDirName)

    # replace SUBUPPER with Subdir in uppercase when needed
    if aLine.find("%SUBUPPER%") > -1:      
      if subDirName != "":
        aLine = str.replace(aLine,"%SUBUPPER%",subDirName.upper()+"_")
      else:
        aLine = str.replace(aLine,"%SUBUPPER%",subDirName.upper())

    # replace PACKAGE with real name
    if aLine.find("%PACKAGE%") > -1:
      aLine= str.replace(aLine,"%PACKAGE%",packageName)
      
    # replace PACKAGEUPPER with uppercase Package name
    if aLine.find("%PACKAGEUPPER%") > -1:
      aLine = str.replace(aLine,"%PACKAGEUPPER%",packageName.upper())

    # replace CLASS with real name
    if aLine.find("%CLASS%") > -1:
      aLine = str.replace(aLine,"%CLASS%",className)
      
    # replace CLASSUPPER with uppercase classname
    if aLine.find("%CLASSUPPER%") > -1:
      aLine = str.replace(aLine,"%CLASSUPPER%",className.upper())

    tmpltype = "<"
    tmplparm = "<"
    i=0
    while i < len(templateList):
      if i > 0:
        tmpltype += ", "
        tmplparm += ","
      tmpltype += "typename " + templateList[i]
      tmplparm += templateList[i]
      i+=1
    tmpltype += ">"
    tmplparm += ">"

    # replace TEMPLATETYPE and TEMPLATEPARAM
    if aLine.find("%TEMPLATETYPE%") > -1:
      aLine = str.replace(aLine,"%TEMPLATETYPE%",tmpltype)
    if aLine.find("%TEMPLATEPARAM%") > -1:
      aLine = str.replace(aLine,"%TEMPLATEPARAM%",tmplparm)

    # Check if !diy, template and .h file, if so include tcc in header file
    if aLine.find("%INCLUDETCC%") > -1:
      incstr = ""
      if autoTemplate == 1:
        if subDirName != "":
          incstr = "#include <"+packageName+"/"+subDirName+"/"+className+".tcc>"
        else:
          incstr = "#include <"+packageName+"/"+className+".tcc>"
      aLine = str.replace(aLine,"%INCLUDETCC%",incstr)
      
    writeFile.write(aLine)
    aLine=readFile.readline()


def makeDefaultClass(lofarDir,className,packageName,srcDir,incDir,subDirName):
  # default.h file
  readFile=openFile(lofarDir+"/LCS/Tools/src/templates/header.h_template","r")
  incHDir=incDir
  if subDirName != "":
    incHDir = incDir+"/"+subDirName
  writeFile=openFile(incHDir+"/"+className+".h","w")
  replacePackageAndClassName(readFile,writeFile,packageName,className,subDirName)
  writeFile.close()
  readFile.close()
  addToMakefile("h",packageName,className,incDir,subDirName)
  #default.cc file
  readFile=openFile(lofarDir+"/LCS/Tools/src/templates/header.cc_template","r")
  writeFile=openFile(className+".cc","w")
  replacePackageAndClassName(readFile,writeFile,packageName,className,subDirName)
  writeFile.close()
  readFile.close()
  addToMakefile("cc",packageName,className,srcDir,subDirName)

def makeTemplatedClass(lofarDir,className,packageName,templateList,autoTemplate,srcDir,incDir,subDirName):
  #default h file
  readFile=openFile(lofarDir+"/LCS/Tools/src/templates/templated_header.h_template","r")
  incHDir=incDir
  if subDirName != "":
    incHDir = incDir+"/"+subDirName
  writeFile=openFile(incHDir+"/"+className+".h","w")
  addTemplates("h",readFile,writeFile,className,packageName,templateList,autoTemplate,subDirName)
  writeFile.close()
  readFile.close()
  addToMakefile("h",packageName,className,incDir,subDirName)

  #default tcc template file
  readFile=openFile(lofarDir+"/LCS/Tools/src/templates/templated_header.tcc_template","r")
  writeFile=openFile(incHDir+"/"+className+".tcc","w")
  addTemplates("tcc",readFile,writeFile,className,packageName,templateList,autoTemplate,subDirName)
  writeFile.close()
  readFile.close()
  addToMakefile("tcc",packageName,className,incDir,subDirName)

  if autoTemplate==0:
    #default diy-cc template file
    readFile=openFile(lofarDir+"/LCS/Tools/src/templates/templated_header.cc_template","r")
    writeFile=openFile(className+".cc","w")
    addTemplates("diy",readFile,writeFile,className,packageName,templateList,autoTemplate,subDirName)
    writeFile.close()
    readFile.close()
    addToMakefile("diy",packageName,className,srcDir,subDirName)

def makeMainClass(lofarDir,className,packageName,srcDir,subDirName):
  readFile=openFile(lofarDir+"/LCS/Tools/src/templates/main.cc_template","r")
  writeFile=openFile(className+"Main.cc","w")
  replacePackageAndClassName(readFile,writeFile,packageName,className,subDirName)
  writeFile.close()
  readFile.close()
  addToMakefile("maincc",packageName,className+"Main",srcDir,subDirName)


def addToMakefile(type,packageName,className,srcDir,subDirName):
  hPattern=re.compile('^([ \t]*)INSTHDRS[ \t]*=.*$',re.IGNORECASE)
  ccPattern=re.compile('^(.*)_la_SOURCES[ \t]*=.*$',re.IGNORECASE)
  mainccPattern=re.compile('^(.*)bin_PROGRAMS[ \t]*=.*$',re.IGNORECASE)
  tccPattern=re.compile('^([ \t]*)TCCHDRS[ \t]*=.*$',re.IGNORECASE)
  os.rename(srcDir+"/Makefile.am",srcDir+"/Makefile.am.old")
  readFile=openFile(srcDir+"/Makefile.am.old","r")
  writeFile=openFile(srcDir+"/Makefile.am","w")
  searchEnd=0
  aLine=readFile.readline()

  while aLine != "":
    if subDirName != "":
      extendedClassName=subDirName+"/"+className
    else:
      extendedClassName=className
      
    if type == "h":
      # find INSTHDRS to start inserting headerfiles
      if hPattern.search(aLine):
        #find / to see if the line already contains another header
        front,end = aLine.split("=")
        if re.search("[a-zA-Z]",end):
          writeFile.write(front+" = "+extendedClassName+".h \\\n")
          writeFile.write("\t"+end)
        elif end.find('\\') > -1:
          writeFile.write(front+" = "+extendedClassName+".h \\\n")
        else :
          writeFile.write(front+" = "+extendedClassName+".h\n")
          
      else:
        writeFile.write(aLine)

    elif type == "cc" or type == "diy":
      # find _la_SOURCES to start inserting sourcefiles
      if ccPattern.search(aLine):
        #find / to see if the line already contains another source
        front,end = aLine.split("=")
        if re.search("[a-zA-Z]",end):
          writeFile.write(front+" = "+extendedClassName+".cc \\\n")
          writeFile.write("\t\t"+end)
        elif end.find('\\') > -1:
          writeFile.write(front+" = "+extendedClassName+".cc \\\n")
        else :
          writeFile.write(front+" = "+extendedClassName+".cc\n")
          
      else:
        writeFile.write(aLine)

    elif type == "maincc":
      pkgLower=packageName.lower()
      # find bin_PROGRAMS to start inserting mainsourcefiles
      # they are inserted in reverse order
      if mainccPattern.search(aLine):        
        front,end = aLine.split("=")
        # the line already contains another source, so that becomes next line
        if re.search("[a-zA-Z]",end):
          writeFile.write(front+" = "+extendedClassName+" \\\n")
          writeFile.write("\t"+end)
          if end.find('\\') > -1:
            # a backslash, so search further (for more program names)
            searchEnd=1
        elif end.find('\\') > -1:
          # only a backslash (no name), so write and search further
          writeFile.write(front+"="+end);
          writeFile.write("\t"+extendedClassName+" \\\n")
          searchEnd=1
        else:
          # nothing yet, so write program name
          writeFile.write(front+" = "+extendedClassName+"\n")
        if searchEnd == 0:
          writeFile.write("\n")
          writeFile.write(className+"_SOURCES      = "+extendedClassName+".cc\n")
          writeFile.write(className+"_LDADD        = lib"+pkgLower+".la\n")
          writeFile.write(className+"_DEPENDENCIES = lib"+pkgLower+".la $(LOFAR_DEPEND)\n")

      elif searchEnd > 0:
        # there have been other mainprograms, so we need to look
        # for the last program name (thus without backslash).
        writeFile.write(aLine)
        if aLine.find('\\') < 0:
          writeFile.write("\n")
          writeFile.write(className+"_SOURCES      = "+extendedClassName+".cc\n")
          writeFile.write(className+"_LDADD        = lib"+pkgLower+".la\n")
          writeFile.write(className+"_DEPENDENCIES = lib"+pkgLower+".la $(LOFAR_DEPEND)\n")
          searchEnd=0
      else:
        writeFile.write(aLine)

    elif type == "tcc":
      # find TCCHDRS to start inserting templatefiles
      if tccPattern.search(aLine):
        #find / to see if the line already contains another source
        front,end = aLine.split("=")
        if re.search("[a-zA-Z]",end):
          writeFile.write(front+" = "+extendedClassName+".tcc \\\n")
          writeFile.write("\t"+end)
        elif end.find('\\') > -1:
          writeFile.write(front+" = "+extendedClassName+".tcc \\\n")
        else :
          writeFile.write(front+" = "+extendedClassName+".tcc\n")
          
      else:
        writeFile.write(aLine)

    else:
      writeFile.write(aLine)
    
    aLine=readFile.readline()

  writeFile.close()
  readFile.close()
  os.unlink(srcDir+"/Makefile.am.old")
  
  
def usage():
  print "usage: "+sys.argv[0]+" [-h] [-m | -t list [-d]] className [className...]"
  print "args:  -h,--help               - print usage"
  print "       -m,--main               - make main program for a class" 
  print "       -t,--templated list     - automated templated class"
  print "                                 list can contain a comma seperated list"
  print "                                 with the template parameters. Example:"
  print "                                 makeClass -t T,U className"
  print "       -d,--diy                - Do it yourself (manual template "
  print "                                 instanciation) Only together with -t"
  print "       className [className...]- name of the class(es) to be created."
  sys.exit(2)


def main(argv):
  noMain=1
  noTemplated=1
  autoTemplate=1
  className = "None"
  #
  # get Lofar base dir
  #
  file = os.popen("echo $PWD | sed -e 's%/LOFAR/.*%/LOFAR%'")
  lofarDir = str.replace(file.readline(),"\n","")
  file.close()
  baseDir = os.environ["PWD"]
  subDirName = ""  
  packageName = ""
  srcDir = ""
  os.path.basename(os.path.dirname(baseDir));
  
  # look if we are in a subdir within src
  if baseDir.find("src") > -1 :
    if os.path.basename(os.path.dirname(baseDir)) == "src":
      srcDir,subDirName=os.path.split(baseDir)
      packageName=os.path.basename(os.path.dirname(srcDir))
    elif os.path.split(baseDir)[1] != "src":
      print "Sorry, only one level of subdirs is allowed in src."
      usage()
    else:
      packageName=os.path.basename(os.path.dirname(baseDir))
      srcDir=baseDir
  else:
    print "You have to be in the srcdir or one of its subdirs to run this program."
    usage()

  try:
    opts, args = getopt.getopt(argv, "hdmt:",
                               ["help","diy","templated=","main"])
  except getopt.GetoptError:
    usage()
  for opt, arg in opts:
    if opt in ("-h", "--help"):
      usage()
    elif opt in ("-m", "--main"):
      noMain = 0
    elif opt in ("-t", "--templated"):
      noTemplated = 0
      templateList = str.split(arg,',')
    elif opt in ("-d", "--diy"):
      autoTemplate = 0

  if len(args) <= 0 and className == "None":
    usage()
    

  if noTemplated==0 and noMain==0:
    print "Sorry, no facility to generate a templated mainfile (yet)."
    usage()
  if len(sys.argv) < 1:
    usage()
  if autoTemplate==0 and noTemplated==1:
    print "Diy only makes sense in templated class."
    print "I will forget you gave this option, and continue.."

  # See if an include/PACKAGE directory exists.
  # If so, use that for the .h and .tcc files.
  # Create possible subdirectory if needed.
  incDir = os.path.dirname(srcDir)+"/include/"+packageName
  hdrDir = incDir
  if not os.path.exists(incDir):
    incDir = srcDir
    hdrDir = srcDir
    if subDirName != "":
      hdrDir = incDir+"/"+subDirName
  else:
    if subDirName != "":
      hdrDir = incDir+"/"+subDirName
      if not os.path.exists(hdrDir):
        os.makedirs(hdrDir)
        print "Created subdirectory "+hdrDir
        

  #
  # Make a backup from the Original Makefiles
  #
  os.system("cp "+srcDir+"/Makefile.am "+srcDir+"/Makefile.am.save")
  if incDir != srcDir:
    os.system("cp "+incDir+"/Makefile.am "+incDir+"/Makefile.am.save")


  for className in args:

    #
    # print info
    #
    if noMain and noTemplated: 
      print "Trying to set up default class " + className + " for package " + packageName
    if noMain and noTemplated==0:
      print "Trying to set up default templated class " + className + " for package " + packageName
      if templateList == "":
        print "No templates provided, so only default template class will be created."
    if noMain==0:
      print "Trying to set up main class program " + className + " for package " + packageName

    #
    # Check of given class name already exists in the working directory as
    # directory or as file
    #
    if noMain: 
      if os.path.isfile(hdrDir+"/"+className+".h"):
        print "Sorry, that class already exists. Please take another name"
        sys.exit(1)
    else:
      if os.path.isfile(className+"Main.cc"):
        print "Sorry, that name already exists. Please take another one"
        sys.exit(1)
      if os.path.isfile(hdrDir+"/"+className+".h") == 0:
        print "WARNING: the base classes for which you are creating a Mainprogram"
        print "         are not available yet."
        print "         please remember that you have to create them.\n"

    #
    # Create all initial files from templates
    #
    if noMain and noTemplated:
      makeDefaultClass(lofarDir,className,packageName,srcDir,incDir,subDirName)
    if noMain and noTemplated==0:
      makeTemplatedClass(lofarDir,className,packageName,templateList,autoTemplate,srcDir,incDir,subDirName)
    if noMain==0:
      makeMainClass(lofarDir,className,packageName,srcDir,subDirName)

#
# this is the main entry
#
if __name__ == "__main__":
  main(sys.argv[1:])
  print "Done"
