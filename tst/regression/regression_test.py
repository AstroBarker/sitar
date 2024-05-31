#!/usr/bin/env python3

import os
from subprocess import call, STDOUT
import shutil
import sys
import warnings

import numpy as np

# ============
# Utility
# ============


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
    build_dir="./build",
    executable="saha",
    run_dir="./run",
    build_type="Release",
    num_procs=os.environ["OMP_NUM_THREADS"],
  ):
    self.src_dir = src_dir
    self.build_dir = os.path.relpath(build_dir)
    self.executable = os.path.join(self.build_dir, "src", executable)
    self.run_dir = os.path.relpath(run_dir)
    self.build_type = build_type
    self.num_procs = num_procs
    self.data_dir = os.path.join(src_dir, "data")

  # End __init__

  def __str__(self):
    print("========== REGRESSION TESTING ==========")
    print(f"Source Dir  : {self.src_dir}")
    print(f"Build Dir   : {self.build_dir}")
    print(f"Executable  : {self.executable}")
    print(f"Run Dir     : {self.run_dir}")
    print(f"Build Type  : {self.build_type}")
    print(f"Num Procs   : {self.num_procs}")
    print("========================================")
    return "\n"

  # End __str__

  def build_code(self):
    if os.path.isdir(self.build_dir):
      print("Build dir already exists! Clean up before regression testing!")
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

  def run_code(self):
    if not os.path.isdir(self.build_dir):
      self.build_code()
    if not os.path.isfile(self.executable):
      print(f"Executable '{self.executable}' does not exist!")
      sys.exit(os.EX_SOFTWARE)

    if os.path.isdir(self.run_dir):
      print("Run dir already exists! Clean up before regression testing!")
      sys.exit(os.EX_SOFTWARE)
    os.mkdir(self.run_dir)
    os.chdir(self.run_dir)

    run_cmd = []  # empty now, can accomodate mpi runs
    outfile = open("out.dat", "w")
    if os.path.isabs(self.executable):
      call(run_cmd + [self.executable], stdout=outfile, stderr=STDOUT)
    else:
      call(
        run_cmd + [os.path.join("..", self.executable)],
        stdout=outfile,
        stderr=STDOUT,
      )
    outfile.close()

  # End run_code

  def load_output(self, fn):
    """
    load simulation output
    """
    temp, nk, CaI, CaII, CaIII = np.loadtxt(fn, unpack=True)
    return temp, nk, CaI, CaII, CaIII

  # End load_output

  def compare_gold(self, fn_gold):
    """
    compare to gold data
    """

    # check if sim data exist
    if not os.path.isfile("out.dat"):
      print("Simulation data do not exist!")
      sys.exit(os.EX_SOFTWARE)

    # load sim data
    temperature, nk, CaI, CaII, CaIII = self.load_output("out.dat")

    # load gold
    temperature_gold, nk_gold, CaI_gold, CaII_gold, CaIII_gold = (
      self.load_output(fn_gold)
    )

    # cleanup before compare
    self.cleanup()

    # comparison
    temperature_status = np.array_equal(temperature, temperature_gold)
    nk_status = np.array_equal(nk, nk_gold)
    CaI_status = np.array_equal(CaI, CaI_gold)
    CaII_status = np.array_equal(CaII, CaII_gold)
    CaIII_status = np.array_equal(CaIII, CaIII_gold)

    success = all(
      [temperature_status, nk_status, CaI_status, CaII_status, CaIII_status]
    )

    if success:
      print("Test Passed! :)")
      return os.EX_OK
    else:
      print("Some test failed! :(")
      print(f"Temperature : {temperature_status}")
      print(f"nk          : {nk_status}")
      print(f"CaI         : {CaI_status}")
      print(f"CaII        : {CaII_status}")
      print(f"CaIII       : {CaIII_status}")
      return os.EX_SOFTWARE

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
        "Absolute paths not allowed for build dir -- unsafe when cleaning up!"
      )
      sys.exit(os.EX_SOFTWARE)

    if os.path.isabs(self.run_dir):
      print(
        "Absolute paths not allowed for run dir -- unsafe when cleaning up!"
      )
      sys.exit(os.EX_SOFTWARE)

    if os.path.exists(self.build_dir):
      try:
        shutil.rmtree(self.build_dir)
      except Exception:
        warnings.warn(f"Error cleaning up build directory '{self.build_dir}'!")

    if os.path.exists(self.run_dir):
      try:
        shutil.rmtree(self.run_dir)
      except Exception:
        warnings.warn(f"Error cleaning up build directory '{self.run_dir}'!")

  # End cleanup


# End Regression

if __name__ == "__main__":
  test = True
  reg = Regression("../../../", "./build")
  # reg.build_code()
  reg.run_code()

  fn_gold = os.path.join(reg.data_dir, "ca.dat")
  reg.compare_gold(fn_gold)
