# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/refprop_headers-src"
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/refprop_headers-build"
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/refprop_headers-subbuild/refprop_headers-populate-prefix"
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/refprop_headers-subbuild/refprop_headers-populate-prefix/tmp"
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/refprop_headers-subbuild/refprop_headers-populate-prefix/src/refprop_headers-populate-stamp"
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/refprop_headers-subbuild/refprop_headers-populate-prefix/src"
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/refprop_headers-subbuild/refprop_headers-populate-prefix/src/refprop_headers-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/refprop_headers-subbuild/refprop_headers-populate-prefix/src/refprop_headers-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/refprop_headers-subbuild/refprop_headers-populate-prefix/src/refprop_headers-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
