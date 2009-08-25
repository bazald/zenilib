#!/usr/bin/python

import os, sys, zenilib_clean, zenilib_strip_x64

def main():
  os.chdir( os.path.dirname( os.path.realpath( __file__ ) ) + "/.." )
  
  zenilib_clean.main()
  zenilib_strip_x64.main()

if __name__ == '__main__':
  main(*sys.argv[1:])
