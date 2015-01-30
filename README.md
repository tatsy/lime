lime
---

[![Build Status](https://travis-ci.org/tatsy/lime.svg?branch=master)](https://travis-ci.org/tatsy/lime)

> Lime (Library for IMage Editing) is an OpenCV-dependent image editing library for C++.
> Since this library is comprised of only headers, you can use it simply by
> including 'lime.hpp'.

## Installation Guide

This library is comprised of only header files. Therefore, you do not
need to do anything other than installing OpenCV in your computer.
The library depends on **OpenCV 3.0** or later.

## Functions

This library supports following methods.

### NPR (Non-Photorealistic Rendering)

* NPR edge detector with Difference of Gaussian (XDoG and FDoG supported)
* NPR image filters
    * Anisotropic diffusion
    * Mean curvature flow
    * Shock filter
    * Kuwahara filter (and its variations)
    * Mathematical morphology
* Line integral convolution
* Vector field detection
    * Smoothed stractural tensor
    * Edge tangent field
    * Vector field singularity detector
* Poisson disk sampling
    * Simple random-queue-based sampler
    * Fast parallel sampler
* Noise generation
    * Uniform noise
    * Perlin noise

## Samples

You can compile all the samples using CMake.

## Copyright

* (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura (random number generator codes)
