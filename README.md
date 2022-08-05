<a name="turing-cmd"/>

# turing_cmd

[![Русская версия](forReadme/RU-icon.png)](README-RU.md)

Turing machine with TUI

![1.gif](forReadme/1.gif)

Table of contents
- [turing_cmd](#turing-cmd)
  * [Features:](#features)
    + [Example of save/load:](#example-of-save-load)
    + [Full reference text:](#full-reference-text)
  * [Known issues:](#known-issues)
  * [Installation:](#installation)
    + [Ubuntu/Debian:](#ubuntu-debian)
  * [Build:](#build)
  * [Thanks for libraries creators:](#thanks-for-libraries-creators)
  * [Changelog:](#changelog)


<a name="features"/>

## Features:
- Endless tape
- Step by step execution
- Continuous exectuion
- Save/load from file.

<a name="example-of-save-load"/>

### Example of save/load:
![2.gif](forReadme/2.gif)

You can navigate with keyboard arrows or with mouse, exit by Esc. There are hotkeys.

<a name="full-reference-text"/>

### Full reference text:
![3.png](forReadme/3.png) 

<a name="known-issues"/>

## Known issues:
- Cyrillic and/or widechars (like greek symbols) are working not properly. Use english.\

<a name="installation"/>

## Installation:

<a name="ubuntu-debian"/>

### Ubuntu/Debian:
[The PPA repo is provided](https://launchpad.net/~danarmor/+archive/ubuntu/ppa).
* `sudo add-apt-repository ppa:danarmor/ppa`
* `sudo apt update`
* `sudo apt install turing-cmd`
Done - you can start program by `turing-cmd` command in terminal.

<a name="build"/>

## Build:
There are two deps, but if you have CMake - you're ready to go: make a "build" dir and execute "run.sh".

<a name="thanks-for-libraries-creators"/>

## Thanks for libraries creators:
- [nlohmann/json](https://github.com/nlohmann/json) - JSON (are used now)
- [ArthurSonzogni/FTXUI](https://github.com/ArthurSonzogni/FTXUI) - TUI
- [taocpp/json](https://github.com/taocpp/json) - JSON (was used in first version)

<a name="changelog"/>

## Changelog:
v0.2 - moved to nlohmann/json, replaced wchar/wstring with char/string, changed save/load notification, moved to FTXUI V3.0.0

v0.1 - init version
