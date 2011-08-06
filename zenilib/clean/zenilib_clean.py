#!/usr/bin/python

import os, sys, zenilib

def main():
  os.chdir( zenilib.dir )
  zenilib.auto_accept = True
  
  zenilib.destroy("Visual Studio 2010/SoarLibrary")
  zenilib.destroy("Visual Studio 2010/Win32")
  zenilib.destroy("Visual Studio 2010/x64")
  zenilib.destroy("Visual Studio 2010/ipch")
  
  zenilib.destroy("Visual Studio 2010/Application_Name.h")
  
  zenilib.destroy("Visual Studio 2010/*.opensdf")
  zenilib.destroy("Visual Studio 2010/*.sdf")
  
  zenilib.destroy("smltrace.txt")
  zenilib.destroy("stderr.txt")
  zenilib.destroy("stdout.txt")
  
  
  zenilib.destroy("Xcode/build")
  
  
  zenilib.destroy(".sconsign.dblite")
  
  zenilib.destroy("src/Zeni/*.o")
  zenilib.destroy("src/Zeni/*.os")
  zenilib.destroy("src/*.o")
  zenilib.destroy("src/*.os")
  
  zenilib.destroy("bin/x64/*.exp")
  zenilib.destroy("bin/x64/*.ilk")
  zenilib.destroy("bin/x64/*.lib")
  zenilib.destroy("bin/x64/*.manifest")
  zenilib.destroy("bin/x64/*.map")
  zenilib.destroy("bin/x64/*.pdb")
  
  zenilib.destroy("bin/*.exp")
  zenilib.destroy("bin/*.ilk")
  zenilib.destroy("bin/*.lib")
  zenilib.destroy("bin/*.manifest")
  zenilib.destroy("bin/*.map")
  zenilib.destroy("bin/*.pdb")
  
  zenilib.destroy("src/TinyXML/*.obj")
  zenilib.destroy("src/Zeni/*.obj")
  zenilib.destroy("src/*.obj")
  
  zenilib.destroy("*.idb")
  zenilib.destroy("*.ilk")
  zenilib.destroy("*.lib")
  zenilib.destroy("*.manifest")
  zenilib.destroy("*.map")
  zenilib.destroy("*.pdb")
  
  zenilib.destroy("Visual Studio 2010/*.obj")

if __name__ == '__main__':
  main(*sys.argv[1:])
