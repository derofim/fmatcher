# `fmatcher`: Filename matcher

## Usage

```bash
./fmatcher -f "filename.some.ext" -p "\pattern/here\?***"
```

Some versions of bash may need escaping of "\":

```bash
./fmatcher -f "pattern/" -p "pattern\\"
```

## Running unit tests

Normal mode: print only on errors

```bash
cmake --build . --target TEST_check
```

Verbose mode: print all messages

```bash
cmake --build . --target TEST_check_verbose
```

## Exit codes
0 - match
1 - don`t match
2 - wrong arguments

## Before build (Tested on Ubuntu)

* install git
* install CMake 3.8.2 or higher
```bash
cd $HOME
wget https://cmake.org/files/v3.12/cmake-3.12.0-Linux-x86_64.sh
sudo sh cmake-3.12.0-Linux-x86_64.sh --prefix=/usr/bin --exclude-subdir
source /etc/environment
cmake --version | head -n1
```
* install C++ compiler, like g++
```bash
sudo apt-get install build-essential
```

## How to Clone?

```bash
git clone https://github.com/blockspacer/fmatcher.git
cd fmatcher
git submodule update --init --recursive
```

## How to build/run?

```bash
mkdir out
cd out
cmake clean ..
cmake --build .
```

## Pattern syntax

```text
? - 1 symbol (any), but not delim
* - 0+ symbols, but not delim
** - 0+ symbols can be delim
```

delim: / or \

## Prohibited filenames

Prohibited filenames that contain one of (with space):
*, ?

## dependencies

* Clara - A simple to use, composable, command line parser for C++ 11 and beyond

## Help

```
./fmatcher -h
usage:
  fmatcher  options

where options are:
  -f, --filename <filename>    What is file name?
  -p, --pattern <pattern>      What is pattern?
  -?, -h, --help               display usage information
```