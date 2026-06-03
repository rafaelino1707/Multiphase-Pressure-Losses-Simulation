# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/multicomplex-src"
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/multicomplex-build"
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/multicomplex-subbuild/multicomplex-populate-prefix"
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/multicomplex-subbuild/multicomplex-populate-prefix/tmp"
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/multicomplex-subbuild/multicomplex-populate-prefix/src/multicomplex-populate-stamp"
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/multicomplex-subbuild/multicomplex-populate-prefix/src"
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/multicomplex-subbuild/multicomplex-populate-prefix/src/multicomplex-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/multicomplex-subbuild/multicomplex-populate-prefix/src/multicomplex-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/multicomplex-subbuild/multicomplex-populate-prefix/src/multicomplex-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
