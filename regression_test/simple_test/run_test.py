#!/usr/bin/env python
import sys
import os
import json
import numpy as np

test_bin = '../test_bin'
input_file = 'test_in.json'
out_file = "output.json"
out_file_ref = "output_ref.json"
rtol = 1e-5
atol = 1e-3

os.system(test_bin + " " + input_file)
data = json.load(open(out_file))
data_ref = json.load(open(out_file_ref))

names = np.array(data["item_name"])
val = np.array(data["item_metric_val"])
val_ref = np.array(data_ref["item_metric_val"])

comp_res = np.isclose(val, val_ref, rtol=rtol, atol=atol)

diff = np.where(comp_res == False)[0]

print("compare output with ref...\n")
if diff.size > 0 :
  print("------------------------------------")
  print("item_name", "item_metric_val",  "item_metric_val_ref") 
  print("------------------------------------")
  for indx in diff :
    print(names[indx], Tmax[indx], Tmax_ref[indx])
  sys.exit(1)
else :
  print("OK")
  sys.exit(0)
