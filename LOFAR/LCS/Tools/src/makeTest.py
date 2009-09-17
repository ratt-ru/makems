#! /usr/bin/env python
#
#  makeTest.py: Script to make default class files in a Package/test dir
#  in the LOFAR development tree. sh scripts and main program are covered
#
#  Usage:
#         ./makeTest [ -h] [testName]
#  Args:
#        testName 	    The name of the testClass that will be created
#        h,--help            usage
#  Revisions:
#
#  15-02-2005   Initial Release.
#
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
#  $Id: makeTest.py 10490 2007-08-31 09:39:48Z diepen $
#

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

def changeName(aName):
  return "t"+aName[0].upper()+aName[1:len(aName)]

def replacePackageAndClassName(readFile,writeFile,packageName,
                               testName,subDirName,shortName):
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
        aLine = str.replace(aLine,"%SUBUPPER%",subDirName+"/")
      else:
        aLine = str.replace(aLine,"%SUBUPPER%",subDirName)

    # replace PACKAGE with real name
    if aLine.find("%PACKAGE%") > -1:
      aLine = str.replace(aLine,"%PACKAGE%",packageName)

    # replace PACKAGEUPPER with uppercase Package name
    if aLine.find("%PACKAGEUPPER%") > -1:
      aLine = str.replace(aLine,"%PACKAGEUPPER%",packageName.upper())

    # replace TEST with testname
    if aLine.find("%TEST%") > -1:
      aLine = str.replace(aLine,"%TEST%",testName)

    # replace TESTSHORT with real given name
    if aLine.find("%TESTSHORT%") > -1:
      aLine = str.replace(aLine,"%TESTSHORT%",shortName)
      
    # replace TESTUPPER with uppercase testname
    if aLine.find("%TESTUPPER%") > -1:
      aLine = str.replace(aLine,"%TESTUPPER%",testName.upper())
      
      
    writeFile.write(aLine)
    aLine=readFile.readline()
  
def makeTestFiles(lofarDir,testName,packageName,testDir,subDirName,shortName,dirLevel):
  # main test (.cc file)
  readFile=openFile(lofarDir+"/templates/test.cc_template","r")
  writeFile=openFile(testName+".cc","w")
  replacePackageAndClassName(readFile,writeFile,packageName,testName,subDirName,shortName)
  writeFile.close()
  readFile.close()
  addToMakefile("cc",testName,testDir,subDirName,packageName,dirLevel)

  # test run script (.sh file)
  readFile=openFile(lofarDir+"/templates/test.sh_template","r")
  writeFile=openFile(testName+".sh","w")
  replacePackageAndClassName(readFile,writeFile,packageName,testName,subDirName,shortName)
  writeFile.close()
  readFile.close()
  addToMakefile("sh",testName,testDir,subDirName,packageName,dirLevel)


    # test input  (.in file)
  readFile=openFile(lofarDir+"/templates/test.in_template","r")
  writeFile=openFile(testName+".in","w")
  replacePackageAndClassName(readFile,writeFile,packageName,testName,subDirName,shortName)
  writeFile.close()
  readFile.close()

  # test output (.out file)
  readFile=openFile(lofarDir+"/templates/test.out_template","r")
  writeFile=openFile(testName+".out","w")
  replacePackageAndClassName(readFile,writeFile,packageName,testName,subDirName,shortName)
  writeFile.close()
  readFile.close()


def addToMakefile(type,testName,testDir,subDirName,packageName,dirLevel):
  checkPattern=re.compile('^([ \t]*)check_PROGRAMS[ \t]*=.*$',re.IGNORECASE)
  testPattern=re.compile('^([ \t]*)TESTSCRIPTS[ \t]*=.*$',re.IGNORECASE)

  startPattern=re.compile('^([ \t]*)XFAIL_TESTS[ \t]*=.*$',re.IGNORECASE)
  
  os.rename(testDir+"/Makefile.am",testDir+"/Makefile.am.old")
  readFile=openFile(testDir+"/Makefile.am.old","r")
  writeFile=openFile(testDir+"/Makefile.am","w")
  searchEnd=0

  levelString="../"
  for i in range(0,dirLevel):
    levelString +="../"

  aLine=readFile.readline()

  while aLine != "":
    if subDirName != "":
      extendedTestName=subDirName+"/t"+testName
    else:
      extendedTestName=testName
      
    if type == "cc":
      # add testprogram to check_PROGRAMS
      if checkPattern.search(aLine):
        #find / to see if the line already contains another source
        front,end = aLine.split("=")
        if re.search("[a-zA-Z]",end):
          writeFile.write(front+" = "+extendedTestName+" \\\n")
          writeFile.write("\t\t"+end)
        else :
          writeFile.write(front+" = "+extendedTestName+"\n")
      elif startPattern.search(aLine):
        writeFile.write(aLine)
        writeFile.write("\n")
        writeFile.write(testName+"_SOURCES      = "+extendedTestName+".cc\n")
        writeFile.write(testName+"_LDADD        = "+levelString+"src/lib"+packageName+".la\n")
        writeFile.write(testName+"_DEPENDENCIES = "+levelString+"src/lib"+packageName+".la $(LOFAR_DEPEND)\n")        

      else:
        writeFile.write(aLine)


    if type == "sh":
      # add testprogram  script to TESTSCRIPTS
      if testPattern.search(aLine):
        #find / to see if the line already contains another source
        front,end = aLine.split("=")
        if re.search("[a-zA-Z]",end):
          writeFile.write(front+" = "+extendedTestName+".sh \\\n")
          writeFile.write("\t\t"+end)
        else :
          writeFile.write(front+" = "+extendedTestName+".sh\n")
      else:
        writeFile.write(aLine)

    #else:
      #      writeFile.write(aLine)
    
    aLine=readFile.readline()

  writeFile.close()
  readFile.close()
  os.unlink(testDir+"/Makefile.am.old")

  
  

def usage():
  print "usage: "+sys.argv[0]+" [-h] testName [testName...]"
  print "args:  -h,--help               - print usage"
  print "       testName [testName...]- name of the testClass(es) to be created."
  sys.exit(2)

def main(argv):
  testName = "None"
  #
  # get Lofar base dir
  #
  if "LOFARROOT" in os.environ:
    lofarDir = os.environ["LOFARROOT"] + "/share"
  else:
    file= os.popen("echo $PWD | sed -e 's%/LOFAR/.*%/LOFAR%'")
    lofarDir=str.replace(file.readline(),"\n","")
    file.close()
  baseDir = os.environ["PWD"]
  subDirName = ""
  packageName = ""
  testDir = ""
  os.path.basename(os.path.dirname(baseDir));
  
  # look if we are in a subdir within test
  if baseDir.find("test") > -1 :
    if os.path.basename(os.path.dirname(baseDir)) == "test":
      testDir,subDirName=os.path.split(baseDir)
      packageName=os.path.basename(os.path.dirname(testDir))
    elif os.path.split(baseDir)[1] != "test":
      print "Sorry, only one level of subdirs is allowed in test."
      usage()
    else:
      packageName=os.path.basename(os.path.dirname(baseDir))
      testDir=baseDir
  else:
    print "You have to be in the testdir or one of its subdirs to run this program."
    usage()

  try:
    opts, args = getopt.getopt(argv, "h",
                               ["help"])
  except getopt.GetoptError:
    usage()
  for opt, arg in opts:
    if opt in ("-h", "--help"):
      usage()
  if len(args) <= 0 and testName == "None":
    usage()
    
  if len(sys.argv) < 1:
    usage()

  # find out the directory sublevel
  dirLevel=len(baseDir.split('/'))-len(testDir.split('/'))

  print "Level: "+`dirLevel`
  
  #
  # Make a backup from the Original Makefile(s)
  #
  os.system("cp "+testDir+"/Makefile.am "+testDir+"/Makefile.am.save")

  for aName in args:
    testName=changeName(aName)
    shortName=aName

  
    #
    # print info
    #
    print "Trying to set up test files and programs for " + shortName + " in package " + packageName

    #
    # Check of given testname already exists in the working directory as
    # directory or as file
    #
  
    if os.path.isfile(testName+".cc"):
      print "Sorry, that test already exists. Please take another name"
      sys.exit(1)

    #
    # Create all initial files from templates
    #
    makeTestFiles(lofarDir,testName,packageName,testDir,subDirName,shortName,dirLevel)

#
# this is the main entry
#
if __name__ == "__main__":
  main(sys.argv[1:])
  print "Done"
