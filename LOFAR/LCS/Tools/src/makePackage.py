#! /usr/bin/env python
#
# makePackage.py: Script to make an initial Packagedir in the LOFAR
# development tree
#
# Usage:
#        ./makePackageDir [PackageName]
# Args:
#       PackageName	The name of the Package that will be created
#
#
# Revisions:
#
#  25-01-2005   Initial Release.
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
#  $Id: makePackage.py 10490 2007-08-31 09:39:48Z diepen $

#
# import all packages we need
#
import os
import sys
import getopt
import shutil

def usage():
  print "usage: "+sys.argv[0]+" [-h] [-s] packageName [packageName...]"
  print "args:  -h,--help                    - print usage"
  print "args:  -s,--super                   - super(toplevel) package"  
  print "       packageName [packageName...] - name of the package to be created."
  sys.exit(2)

def openFile(name,mode):
  try:
    file = open (name,mode)
  except IOError, message:
    sys.exit("Error opening file: %s" % message)
  return file

def createDir(name):
  try:
    file = os.mkdir(name)
  except IOError, message:
    sys.exit("Error creating directory: %s" % message)

def createPackageDoc(packageName,dirLevel):
  if os.path.isfile(packageName):
    return
  else:
    writeFile=openFile(packageName+"/package.dox","w")
    if dirLevel>0:
      baseGroup=os.path.basename(os.environ["PWD"])
      writeFile.write("// \ingroup "+baseGroup+"\n")
    writeFile.write("// \defgroup "+packageName+" "+packageName+"\n")
    writeFile.write("//\n")
    writeFile.write("//  ... description ...\n")
    writeFile.close()

def replacePackageName(readFile,writeFile,packageName,dirLevel):
  levelString=""
  for i in range(0,dirLevel):
    levelString +="../"
  aLine=readFile.readline()
  while aLine != "":
    newLine=aLine
    if aLine.find("%PACKAGE%") > -1:
      newLine = str.replace(aLine,"%PACKAGE%",packageName)
    if aLine.find("%PACKAGELOWER%") > -1:
      newLine = str.replace(aLine,"%PACKAGELOWER%",packageName.lower())
    if aLine.find("%LEVEL%") > -1:
      newLine = str.replace(aLine,"%LEVEL%",levelString)
    writeFile.write(newLine)
    aLine=readFile.readline()
  
def createConfigureIn(lofarDir,packageName,dirLevel):
  #
  # Create configure.in in Package dir
  #
  readFile=openFile(lofarDir+"/templates/package_configure.in_template","r")
  writeFile=openFile(packageName+"/configure.in","w")
  replacePackageName(readFile,writeFile,packageName,dirLevel)
  writeFile.close()
  readFile.close()

def createLofarconfIn(lofarDir,packageName):
  #
  # Create lofarconf.in in (super) Package dir
  #
  readFile=openFile(lofarDir+"/templates/package_lofarconf.in_template","r")
  writeFile=openFile(packageName+"/lofarconf.in","w")
  aLine=readFile.readline()
  while aLine != "":
    writeFile.write(aLine)
    aLine=readFile.readline()
  writeFile.close()
  readFile.close()

def addToLofarconfIn(lofarDir,packageName):
  #
  # add new package to lofarconf.in in Package dir
  #

  if os.path.isfile("lofarconf.in"):
    shutil.copyfile("lofarconf.in","lofarconf.in.old")
  f = openFile("lofarconf.in","a")
  f.write(packageName+"\t\t\t#short description\n")
  f.close()

def createBootstrap(lofarDir,packageName,dirLevel):
  #
  # Create Bootstrap in Package dir
  #
  readFile=openFile(lofarDir+"/templates/package_bootstrap_template","r")
  fileName=packageName+"/bootstrap"
  writeFile=openFile(fileName,"w")
  replacePackageName(readFile,writeFile,packageName,dirLevel)
  writeFile.close()
  readFile.close()
  os.chmod(fileName, os.stat(fileName).st_mode | 0111)

def createSpecin(lofarDir,packageName,dirLevel):
  #
  # Create .spec.in in Package dir
  #
  readFile=openFile(lofarDir+"/templates/package_spec.in_template","r")
  fileName=packageName+"/"+packageName+".spec.in"
  writeFile=openFile(fileName,"w")
  replacePackageName(readFile,writeFile,packageName,dirLevel)
  writeFile.close()
  readFile.close()

def createMakefiles(lofarDir,packageName,incDir,incpkgDir,
                    srcDir,testDir,dirLevel):
  #
  # Create Makefile.am in Package dir
  #
  readFile=openFile(lofarDir+"/templates/package_makefile.am_template","r")
  writeFile=openFile(packageName+"/Makefile.am","w")
  replacePackageName(readFile,writeFile,packageName,dirLevel)
  writeFile.close()
  readFile.close()

  #
  # Create Makefile.am in incDir
  #
  readFile=openFile(lofarDir+"/templates/include_makefile.am_template","r")
  writeFile=openFile(incDir+"/Makefile.am","w")
  replacePackageName(readFile,writeFile,packageName,dirLevel)
  writeFile.close()
  readFile.close()

  #
  # Create Makefile.am in incpkgDir
  #
  readFile=openFile(lofarDir+"/templates/include_pkg_makefile.am_template","r")
  writeFile=openFile(incpkgDir+"/Makefile.am","w")
  replacePackageName(readFile,writeFile,packageName,dirLevel)
  writeFile.close()
  readFile.close()

  #
  # Create Makefile.am in srcDir
  #
  readFile=openFile(lofarDir+"/templates/src_makefile.am_template","r")
  writeFile=openFile(srcDir+"/Makefile.am","w")
  replacePackageName(readFile,writeFile,packageName,dirLevel)
  writeFile.close()
  readFile.close()

  #
  # Create Makefile.am in testDir
  #
  readFile=openFile(lofarDir+"/templates/test_makefile.am_template","r")
  writeFile=openFile(testDir+"/Makefile.am","w")
  replacePackageName(readFile,writeFile,packageName,dirLevel)
  writeFile.close()
  readFile.close()


def main(argv):
  packageName = "None"
  baseDir = os.environ["PWD"]
  super=0
  #
  # get Lofar base dir
  #
  file= os.popen("echo $PWD | sed -e 's%/LOFAR/.*%/LOFAR%'")
  baseLofarDir=str.replace(file.readline(),"\n","")
  file.close()
  if "LOFARROOT" in os.environ:
    lofarDir = os.environ["LOFARROOT"] + "/share"
  else:
    file= os.popen("echo $PWD | sed -e 's%/LOFAR/.*%/LOFAR%'")
    lofarDir=baseLofarDir

  # find out the directory sublevel
  dirLevel=len(baseDir.split('/'))-len(baseLofarDir.split('/'))

  # Get lofar data dir.
  if "LOFARDATAROOT" in os.environ:
    lofarDataDir = os.environ["LOFARDATAROOT"]
  else:
    lofarDir+="/LCS/Tools/src"
    
  try:
    opts, args = getopt.getopt(argv, "hs",["help","super"])
  except getopt.GetoptError:
    usage()
  for opt, arg in opts:
    if opt in ("-h", "--help"):
      usage()
    elif opt in ("-s", "--super"):
      super = 1
    

  if len(args) <= 0 and packageName == "None":
    usage()
    
  for packageName in args:
    
    #
    # print LOFAR Package and basetree
    #
    if super ==0:
      print "Trying to set up Package: " + baseDir + "/" + packageName +"\n"
    else:
      print "Trying to set up Super Package: " + baseDir + "/" + packageName +"\n"
      
    #
    # Check of given package name already exists in the working directory as
    # directory or as file
    #
    if os.path.isdir(packageName) | os.path.isfile(packageName):
      print "Sorry, that name already exists. Please take another one\n"
      sys.exit(1)

    #
    # if configure.in exists in the root directory it is forbidden to extend
    # this directory with (sub)packages
    #
    if os.path.isfile("configure.in"):
      print "Sorry, it is not allowed to create subpackages in this packagedir\n"
      sys.exit(1)

    #
    # Create the directory
    #
    createDir(packageName)
    
    if super == 0:
      incDir = packageName+"/include"
      incpkgDir = packageName+"/include/"+packageName
      srcDir = packageName+"/src"
      testDir = packageName+"/test"
    
      #
      # create the src and test directories in package
      #
      createDir(incDir)
      createDir(incpkgDir)
      createDir(srcDir)
      createDir(testDir)
    
      #
      # Create all initial Makefile.am files from templates
      #
      createMakefiles(lofarDir,packageName,incDir,incpkgDir,
                      srcDir,testDir,dirLevel)
      
      #
      # Create all other initial files from templates
      #
      createConfigureIn(lofarDir,packageName,dirLevel)
      createBootstrap(lofarDir,packageName,dirLevel)
      createSpecin(lofarDir,packageName,dirLevel)
      
    else:
      #
      # create lofarconf.in
      #
      createLofarconfIn(lofarDir,packageName)

    #
    # Add package to lofarconf.in
    #
    addToLofarconfIn(lofarDir,packageName)

    createPackageDoc(packageName,dirLevel)
  
#
# this is the main entry
#
if __name__ == "__main__":
  main(sys.argv[1:])
  print "Done"
