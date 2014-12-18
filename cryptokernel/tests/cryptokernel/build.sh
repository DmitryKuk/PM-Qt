#!/bin/bash

# Author: Dmitry Kukovinets (d1021976@gmail.com)

# Simple script for compile test

g++ -std=c++11 -Wall -o test main_test.cpp project/generator/generator.cpp project/generator/dev_random.cpp project/cryptokernel.cpp project/types.cpp
