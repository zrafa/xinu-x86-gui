#!/bin/bash

# Linux compile
gcc -o demo-sdl2 main.c renderer.c microui.c -O3 -Wall -std=c11 -pedantic -lSDL2 -lGL -lm -g

