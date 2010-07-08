#!/usr/bin/python

import os, sys, zenilib

def main():
  os.chdir( zenilib.dir )
  zenilib.auto_accept = True
  
  zenilib.destroy("Visual Studio 2008/x64")
  zenilib.destroy("Visual Studio 2010/x64")
  
  zenilib.destroy("bin/x64")
  zenilib.destroy("lib_win/x64")
  
  zenilib.destroy("*_x64.exe")
  zenilib.destroy("*_x64d.exe")
  zenilib.destroy("*_x64")
  zenilib.destroy("*_x64d")

if __name__ == '__main__':
  main(*sys.argv[1:])
