# Ray Tracing In One Weekend

<img align="center" src="https://github.com/GPSnoopy/RayTracingInOneWeekend/blob/master/Book1.jpg">

## Overview

My take on [Peter Shirley's Ray Tracing in One Weekend](https://github.com/RayTracing/raytracing.github.io) first book. The book is an excellent introduction to ray tracing; yet as the title implies, short enough you can write your own ray tracer in a weekend.

On top of my own programming style, I have added multi-threading support (using a separate random number generator for each thread).

This program is a fun way to explore new programming languages and approaches. Thus I have also implemented it in Rust and [ISPC](https://ispc.github.io/index.html).

## Building

Visual Studio 2019 project files provided as-is. Use `build_linux.sh` for a GCC build, be sure to change the `-march=skylake` to match your  processor.

## Performance

The cover picture was rendered in about 1h running 16 threads on a Core i9-9900K @ 4.7GHz at 3840x2160 with 1024 samples per pixel. Clocking the CPU at 4.8GHz when running AVX code would cause it to reach over 90C, so be careful when running this code on an overclocked processor. I've used [GIMP](https://www.gimp.org/) to open the resulting PPM image and convert it to more standard formats such as PNG and JPEG.

For best performance, I recommend the Linux build even on Windows (I've used Ubuntu 18.04 running via [WSL on Windows 10 1809](https://en.wikipedia.org/wiki/Windows_Subsystem_for_Linux)). For some reason, GCC (7.3) seems better at generating AVX FMA instructions than Visual Studio 2019 (16.3) on this code.

About the other implementations I have tried: the ISPC version is >2.7x faster than either the Rust and C++ version. None of these come even close to approaching the performance of the Vulkan implementation using RTX, which you can find [here]( https://github.com/GPSnoopy/RayTracingInVulkan/).
