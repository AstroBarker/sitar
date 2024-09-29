import unittest
import shutil
import subprocess
import os
import sys
import warnings

import numpy as np


class CppRegressionTest(unittest.TestCase):
  def __init__(
    self,
    test_name="runTest",
    src_dir="",
    build_dir="./build",
    executable="./main",
    run_dir="./run",
    build_type="Release",
    num_procs=1,
    goldfile=None,
  ):
    super(CppRegressionTest, self).__init__(test_name)
    self.src_dir = src_dir
    self.build_dir = build_dir
    self.build_type = build_type
    self.executable = os.path.join(self.build_dir, "src", executable)
    self.run_dir = os.path.relpath(run_dir)
    self.num_procs = num_procs
    self.goldfile = goldfile

  # End __init__

  def build_code(self):
    if os.path.isdir(self.build_dir):
      print("Build dir already exists! Clean up before regression testing!")
      sys.exit(os.EX_SOFTWARE)
    os.mkdir(self.build_dir)
    os.chdir(self.build_dir)

    # Base configure options
    configure_options = ""

    if self.build_type == "Release":
      configure_options += "-DCMAKE_BUILD_TYPE=Release "
    elif self.build_type == "Debug":
      configure_options += "-DCMAKE_BUILD_TYPE=Debug "
    else:  # TODO: more cmake build types
      print(f"Build type '{self.build_type}' not known!")
      sys.exit(os.EX_SOFTWARE)
    configure_options += "-DENABLE_UNIT_TESTS=OFF "

    cmake_call = ""
    cmake_call += "cmake "
    for option in configure_options:
      cmake_call += option
    # cmake opts
    cmake_call += self.src_dir

    # Configure
    print("Configuring the source...")
    try:
      subprocess.run(
        cmake_call,
        shell=True,
        check=True,
        stderr=subprocess.PIPE,
        stdout=subprocess.PIPE,
      )
    except subprocess.CalledProcessError as e:
      self.fail(f"Configure failed with error: {e.stderr.decode()}")

    # Compile
    print("Compiling the source...")
    try:
      subprocess.run(
        "cmake --build . --parallel " + str(self.num_procs),
        shell=True,
        check=True,
        stderr=subprocess.PIPE,
        stdout=subprocess.PIPE,
      )
    except subprocess.CalledProcessError as e:
      self.fail(f"Compilation failed with error: {e.stderr.decode()}")

    # Return to standard directory
    os.chdir("..")

  # End build_code

  def setUp(self):
    """Set up the test environment by compiling the code."""

    self.build_code()

  # end setUp

  def tearDown(self):
    """Clean up the generated executable after the test."""
    self.cleanup()

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

    run_cmd = ""  # empty now, can accomodate mpi runs
    outfile = open("out.dat", "w")
    try:
      if os.path.isabs(self.executable):
        subprocess.run(
          run_cmd + self.executabe,
          shell=True,
          check=True,
          stdout=outfile,
          stderr=subprocess.PIPE,
        )
      else:
        subprocess.run(
          run_cmd + os.path.join("../", self.executable),
          shell=True,
          check=True,
          stdout=outfile,
          stderr=subprocess.PIPE,
        )
    except subprocess.CalledProcessError as e:
      self.fail(f"Execution failed with error: {e.stderr.decode()}")
    outfile.close()

  # End run_code

  def load_output(self, fn):
    """
    load simulation output
    """
    temp, nk, CaI, CaII, CaIII = np.loadtxt(fn, unpack=True)
    return temp, nk, CaI, CaII, CaIII

  # End load_output

  def compare_gold(self):
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
      self.load_output(self.goldfile)
    )

    # cleanup before compare
    self.tearDown()

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
      return True
    else:
      print("Some test failed! :(")
      print(f"Temperature : {temperature_status}")
      print(f"nk          : {nk_status}")
      print(f"CaI         : {CaI_status}")
      print(f"CaII        : {CaII_status}")
      print(f"CaIII       : {CaIII_status}")
      return False

  # End compare_gold

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

  def test_saha_solver(self):
    """Test Saha solver on CaII ionization."""

    self.run_code()
    self.assertTrue(self.compare_gold())

  # End test_saha_solver


def main():
  # Example usage:
  """
  run regression tests for CaI, CaII, CaIII
  """
  # Define test parameters
  src_dir = "../../../"
  build_dir = "./build"
  run_dir = "./run"
  executable = "./saha"
  goldfile = "/home/barker/code/sitar/data/ca.dat"

  # Create and run the test suite
  suite = unittest.TestSuite()
  suite.addTest(
    CppRegressionTest(
      src_dir=src_dir,
      build_dir=build_dir,
      run_dir=run_dir,
      test_name="test_saha_solver",
      executable=executable,
      goldfile=goldfile,
      num_procs=2,
    )
  )

  runner = unittest.TextTestRunner(verbosity=2)
  runner.run(suite)


if __name__ == "__main__":
  main()
