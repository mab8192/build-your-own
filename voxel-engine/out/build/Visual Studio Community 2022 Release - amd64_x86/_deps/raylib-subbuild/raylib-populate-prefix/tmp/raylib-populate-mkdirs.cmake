# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/dev/mab8192/build-your-own/voxel-engine/out/build/Visual Studio Community 2022 Release - amd64_x86/_deps/raylib-src")
  file(MAKE_DIRECTORY "C:/dev/mab8192/build-your-own/voxel-engine/out/build/Visual Studio Community 2022 Release - amd64_x86/_deps/raylib-src")
endif()
file(MAKE_DIRECTORY
  "C:/dev/mab8192/build-your-own/voxel-engine/out/build/Visual Studio Community 2022 Release - amd64_x86/_deps/raylib-build"
  "C:/dev/mab8192/build-your-own/voxel-engine/out/build/Visual Studio Community 2022 Release - amd64_x86/_deps/raylib-subbuild/raylib-populate-prefix"
  "C:/dev/mab8192/build-your-own/voxel-engine/out/build/Visual Studio Community 2022 Release - amd64_x86/_deps/raylib-subbuild/raylib-populate-prefix/tmp"
  "C:/dev/mab8192/build-your-own/voxel-engine/out/build/Visual Studio Community 2022 Release - amd64_x86/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp"
  "C:/dev/mab8192/build-your-own/voxel-engine/out/build/Visual Studio Community 2022 Release - amd64_x86/_deps/raylib-subbuild/raylib-populate-prefix/src"
  "C:/dev/mab8192/build-your-own/voxel-engine/out/build/Visual Studio Community 2022 Release - amd64_x86/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/dev/mab8192/build-your-own/voxel-engine/out/build/Visual Studio Community 2022 Release - amd64_x86/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/dev/mab8192/build-your-own/voxel-engine/out/build/Visual Studio Community 2022 Release - amd64_x86/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
