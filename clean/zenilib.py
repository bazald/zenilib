#!/usr/bin/python

import glob, os, shutil, sys

auto_accept = False
dir = os.path.dirname( os.path.realpath( __file__ ) ) + "/.."

def isspace(c):
  return c == ' ' or c == '\t' or c == '\r' or c == '\n'

def yes_no_prompt(prompt):
  if auto_accept:
    return True
  
  print prompt + " [y/N] " ,
  line = sys.stdin.readline()
  print ""
  while len(line) is not 0 and isspace(line[0]):
    line = line[1:]
  while len(line) is not 0 and isspace(line[len(line)-1]):
    line = line[:len(line)-1]
  
  return line == "y" or line == "Y" or line == "yes" or line == "YES"

def destroy_one(path):
  if os.path.exists(path):
    if os.path.isdir(path):
      if yes_no_prompt("Delete directory '" + path + "'?"):
        shutil.rmtree(path, True)
    else:
      if yes_no_prompt("Delete file '" + path + "'?"):
        try:
          os.remove(path)
        except OSError:
          return

def destroy(path, excludes = []):
  list = glob.glob(path)
  for entry in list:
    skip = False
    for exclude in excludes:
      if entry.find(exclude) is not -1:
        skip = True
    if not skip:
      destroy_one(entry)
