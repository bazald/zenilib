#!/usr/bin/python

import os, sys, zenilib, zenilib_clean

def main():
  zenilib_clean.main()
  
  os.chdir( zenilib.dir )
  zenilib.auto_accept = True
  
  zenilib.destroy("Visual Studio 2010/*.suo")
  zenilib.destroy("Visual Studio 2010/*.user", ["Application.vcxproj.user", "Application_SoarSML.vcxproj.user", "Launcher.vcxproj.user"])
  
  zenilib.destroy("bin/*_d.dll")
  zenilib.destroy("bin/*_d.so")
  zenilib.destroy("bin/x64/*_d.dll")
  zenilib.destroy("bin/x64/*_d.so")
  zenilib.destroy("lib_win/*_d.exp")
  zenilib.destroy("lib_win/*_d.lib")
  zenilib.destroy("lib_win/x64/*_d.exp")
  zenilib.destroy("lib_win/x64/*_d.lib")
  zenilib.destroy("bin/*_d.exe")
  zenilib.destroy("bin/x64/*_d.exe")
  zenilib.destroy("*_d.exe")
  zenilib.destroy("*_x64d.exe")
  zenilib.destroy("*_d")
  zenilib.destroy("*_x64d")
  
  zenilib.destroy("*_d.app")
  
  zenilib.destroy("docs/Zeni")

if __name__ == '__main__':
  main(*sys.argv[1:])
