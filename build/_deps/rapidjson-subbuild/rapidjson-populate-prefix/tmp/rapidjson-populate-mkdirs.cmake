# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/rapidjson-src"
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/rapidjson-build"
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/rapidjson-subbuild/rapidjson-populate-prefix"
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/rapidjson-subbuild/rapidjson-populate-prefix/tmp"
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/rapidjson-subbuild/rapidjson-populate-prefix/src/rapidjson-populate-stamp"
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/rapidjson-subbuild/rapidjson-populate-prefix/src"
  "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/rapidjson-subbuild/rapidjson-populate-prefix/src/rapidjson-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/rapidjson-subbuild/rapidjson-populate-prefix/src/rapidjson-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/lino/Multiphase-Pressure-Losses-Simulation/build/_deps/rapidjson-subbuild/rapidjson-populate-prefix/src/rapidjson-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
