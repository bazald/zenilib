#!/usr/bin/python

import os, sys, zenilib, zenilib_clean_extra, zenilib_strip_x64

def main():
  zenilib_clean_extra.main()
  zenilib_strip_x64.main()
  
  os.chdir( zenilib.dir )
  zenilib.auto_accept = False
  
  zenilib.destroy("bin/*.so")
  zenilib.destroy("bin/x64/*.so")
  
  zenilib.destroy("Xcode/build")
  zenilib.destroy("Xcode/Frameworks")
  zenilib.destroy("Xcode/zenilib_*_frameworks.zip")

if __name__ == '__main__':
  main(*sys.argv[1:])
