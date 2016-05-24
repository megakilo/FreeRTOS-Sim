# FreeRTOS-Sim
FreeRTOS simulator for POSIX (Linux, OS X or maybe other POSIX OS with pthread & signal support)

This work is based on the original simulator developed by William Davy for FreeRTOS v5.3 extracted from the official FreeRTOS release. The goal of the work is to make FreeRTOS simulator a seperate package that is easy to build & use and also to keep the simulator updated with newer FreeRTOS releases (v8.2.2 is the latest kernel tested and included in the repo).

Directory description
- Source: FreeRTOS kernel and POSIX simulator source files
- POSIX: the project directory that includes main() and FreeRTOS settings for the POSIX port
- Demo: tested builtin demo tasks from the official FreeRTOS release
