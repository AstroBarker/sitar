#!/usr/bin/env python3

import os
from subprocess import call
import shutil
import sys

import numpy as np

# ============
# Utility
# ============

NUM_PROCS = 1


def soft_equiv(val, ref, tol=1.0e-5):
  """
  Soft equivalence of value and reference.
  """
  tiny = 1.0e-10
  numerator = np.fabs(val - ref)
  denominator = max(np.fabs(ref), tiny)

  if numerator / denominator > tol:
    return False
  else:
    return True


# End soft_equiv


class Regression:
  """
  regression testing class
  """

  def __init__(
    self,
    src_dir,
    build_dir,
    run_dir = "./run",
    build_type="Release",
    num_procs=os.environ["OMP_NUM_PROCS"],
  ):
    self.src_dir = src_dir
    self.build_dir = build_dir
    self.run_dir = run_dir
    self.build_type = build_type
    self.num_procs = num_procs

  # End __init__

  def __str__(self):
    print(f"Source Dir : {self.src_dir}")
    print(f"Build Dir  : {self.build_dir}")
    print(f"Build Type : {self.build_type}")
    print(f"Num Procs  : {self.num_procs}")
    return "\n"

  # End __str__

  def build_code(self):
    if os.path.isdir(self.build_dir):
      print(
        f"BUILD_DIR '{self.build_dir}' already exists! Clean up before regression testing!"
      )
      sys.exit(os.EX_SOFTWARE)
    os.mkdir(self.build_dir)
    os.chdir(self.build_dir)

    # Base configure options
    configure_options = []

    if self.build_type == "Release":
      configure_options.append("-DCMAKE_BUILD_TYPE=Release")
    elif self.build_type == "Debug":
      configure_options.append("-DCMAKE_BUILD_TYPE=Debug")
    else:
      print(f"Build type '{self.build_type}' not known!")
      sys.exit(os.EX_SOFTWARE)
    configure_options.append("-DENABLE_UNIT_TESTS=OFF")

    cmake_call = []
    cmake_call.append("cmake")
    for option in configure_options:
      cmake_call.append(option)
    # cmake opts
    cmake_call.append(self.src_dir)

    # Configure
    call(cmake_call)

    # Compile
    call(["cmake", "--build", ".", "--parallel", str(self.num_procs)])

    # Return to standard directory
    os.chdir("..")

  # End build_code


  def cleanup(self):
    """
    Clean up working directory after testing
    """
    if (
      os.getcwd().split(os.sep)[-1] == self.build_dir
      or os.getcwd().split(os.sep)[-1] == self.run_dir
    ):
      os.chdir("..")

    if os.path.isabs(self.build_dir):
      print(
        f"Absolute paths not allowed for build directory '{self.build_dir}' -- unsafe when cleaning up!"
      )
      sys.exit(os.EX_SOFTWARE)

    if os.path.isabs(self.run_dir):
      print(
        f"Absolute paths not allowed for run directory '{self.run_dir}' -- unsafe when cleaning up!"
      )
      sys.exit(os.EX_SOFTWARE)

    if os.path.exists(self.build_dir):
      try:
        shutil.rmtree(self.build_dir)
      except:
        print(f"Error cleaning up build directory '{self.build_dir}'!")

    if os.path.exists(self.run_dir):
      try:
        shutil.rmtree(self.run_dir)
      except:
        print(f"Error cleaning up build directory '{self.run_dir}'!")
  # End cleanup


# End Regression
