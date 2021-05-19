#!/usr/bin/env python
import sys
import os
import json
import numpy as np


#test_dirs = ["simple_test"]
all_dirs = [name for name in os.listdir(".") if os.path.isdir(name)]


arg_strs = sys.argv
if len(arg_strs) == 1 or (len(arg_strs) == 2 and arg_strs[1] == "all"):
  test_dirs = all_dirs
else :
  test_dirs = arg_strs[1:]



#test_dirs = all_dirs
for dir in test_dirs :
  cmd = 'cd ' + dir + "; " + "./run_test.py > test.log";
  print(cmd)
  ret = os.system(cmd)
  if ret != 0 :
    print(dir + ' fail')
  else :
    print(dir + ' pass')  
