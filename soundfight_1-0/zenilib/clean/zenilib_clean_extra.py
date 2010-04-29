#!/usr/bin/python

import os, sys, zenilib, zenilib_clean

def main():
  zenilib_clean.main()
  
  os.chdir( os.path.dirname( os.path.realpath( __file__ ) ) + "/.." )
  zenilib.auto_accept = True
  
  zenilib.destroy("Visual Studio 2008/*.suo")
  zenilib.destroy("Visual Studio 2008/*.user", ["Application_2008.vcproj.user", "Application_SoarSML_2008.vcproj.user"])
  
  zenilib.destroy("Visual Studio 2008/Win32/*_d")
  zenilib.destroy("Visual Studio 2008/x64/*_d")
  
  zenilib.destroy("bin/*_d.exe")
  zenilib.destroy("bin/x64/*_x64d.exe")
  zenilib.destroy("*_d.exe")
  zenilib.destroy("*_x64d.exe")
  zenilib.destroy("*_d")
  zenilib.destroy("*_x64d")
  
  zenilib.destroy("../*_d.app")

if __name__ == '__main__':
  main(*sys.argv[1:])
