# FreeRTOS-Sim
FreeRTOS simulator for POSIX (Linux, Mac OS X or some OS with pthread and signal support)

The original simulator is developed by William Davy for FreeRTOS v5.3 included in the FreeRTOS package. I'm trying to keep the simulator working with the new FreeRTOS release as much as possible. Current FreeRTOS kernel version I have tested is v8.2.2 included in this repo.

Directory description
- Demo: builtin demo tasks from FreeRTOS
- Source: FreeRTOS kernel and POSIX simulator source files
- POSIX: main file and FreeRTOS settings header for the POSIX port
