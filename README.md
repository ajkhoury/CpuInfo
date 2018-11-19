# CpuInfo 

This repository contains the code of a small user-mode utility for identifying and determining CPU features.
More importantly this repository is dedicated to the extremely lightweight and portable PIF (Processor Information and Features) library, and various pieces from my private architecure specific library I call "baselib", shamelessly taken from the EDK2's internal library with the same name.

# Building

This project uses the compiler-independent CMake build system. Currently this project should compile with the GCC, Clang, and MSVC compilers on Windows. I have not tested compilation under any Linux distributions.

To build simply run CMake to generate build files of your choice. Please see https://cmake.org/cmake-tutorial/ and/or https://cmake.org/runningcmake/ or simply use CLion which integrates with CMake very well.

# License

Licenses pertaining to this project can be found under the LICENSES sub-folder.
