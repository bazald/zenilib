#!/usr/bin/python

import os, sys, zenilib, zenilib_clean, zenilib_strip_x64

def main():
  zenilib_clean.main()
  zenilib_strip_x64.main()
  
  os.chdir( os.path.dirname( os.path.realpath( __file__ ) ) + "/.." )
  zenilib.auto_accept = False
  
  zenilib.destroy("Frameworks")

if __name__ == '__main__':
  main(*sys.argv[1:])
