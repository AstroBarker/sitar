#!/bin/usr/env python3

import numpy as np
import matplotlib.pyplot as plt


def main():
  fn = "../data/ca.dat"
  temp, nk, CaI, CaII, CaIII = np.loadtxt(fn, unpack=True)
  temp /= 1.0e4

  fig, ax = plt.subplots()
  ax.plot(temp, CaI, color="rebeccapurple", label=r"Ca")
  ax.plot(temp, CaII, color="#996633", label=r"Ca$^{+}$", ls="--")
  ax.plot(temp, CaIII, color="#339966", label=r"Ca$^{+}$", ls=":")

  ax.legend(frameon=True)
  ax.set(xlabel=r"Temperature [10$^{4}$ K]", ylabel="Ionization Fraction")

  plt.tight_layout()
  plt.savefig("ca.png", dpi=300)


# End main

if __name__ == "__main__":
  main()
