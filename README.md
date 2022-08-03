# turing_cmd

[![Русская версия](forReadme/RU-icon.png)](README-RU.md)

Turing machine with TUI

![1.gif](forReadme/1.gif)

## Features:
- Endless tape
- Step by step execution
- Continuous exectuion
- Save/load from file.

### Example of save/load:
![2.gif](forReadme/2.gif)

You can navigate with keyboard arrows or with mouse, exit by Esc. There are hotkeys.

### Full reference text:
![3.png](forReadme/3.png) 

## Known issues:
- Cyrillic and/or widechars (like greek symbols) are working not properly. Use english.

## Build:
There are two deps, but if you have CMake - you're ready to go: make a "build" dir and execute "run.sh".

P.S.: project are working under FTXUI V2.0, but there is V3.0 already

## Thanks for libraries creators:
- [nlohmann/json](https://github.com/nlohmann/json) - JSON (are used now)
- [ArthurSonzogni/FTXUI](https://github.com/ArthurSonzogni/FTXUI) - TUI
- [taocpp/json](https://github.com/taocpp/json) - JSON (was used in first version)