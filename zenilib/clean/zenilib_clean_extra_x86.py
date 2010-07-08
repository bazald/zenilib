#!/usr/bin/python

import os, sys, zenilib, zenilib_clean_extra, zenilib_strip_x64

def main():
  zenilib_clean_extra.main()
  zenilib_strip_x64.main()
  
  os.chdir( zenilib.dir )
  zenilib.auto_accept = False
  
  zenilib.destroy("Frameworks")

if __name__ == '__main__':
  main(*sys.argv[1:])
