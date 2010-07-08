#!/usr/bin/python

import os, sys, zenilib, zenilib_clean

def main():
  zenilib_clean.main()
  
  os.chdir( zenilib.dir )
  zenilib.auto_accept = True
  
  zenilib.destroy("Visual Studio 2008/*.suo")
  zenilib.destroy("Visual Studio 2008/*.user", ["Application.vcproj.user", "Application_SoarSML.vcproj.user", "Launcher.vcproj.user"])
  zenilib.destroy("Visual Studio 2010/*.suo")
  zenilib.destroy("Visual Studio 2010/*.user", ["Application.vcxproj.user", "Application_SoarSML.vcxproj.user", "Launcher.vcxproj.user"])
  
  zenilib.destroy("bin/*_d.exe")
  zenilib.destroy("bin/x64/*_x64d.exe")
  zenilib.destroy("*_d.exe")
  zenilib.destroy("*_x64d.exe")
  zenilib.destroy("*_d")
  zenilib.destroy("*_x64d")
  
  zenilib.destroy("*_d.app")
  
  zenilib.destroy("docs/Zeni")

if __name__ == '__main__':
  main(*sys.argv[1:])
