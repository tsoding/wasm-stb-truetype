#!/bin/sh

set -xe

clang -Os -fno-builtin -Wall -Wextra -Wswitch-enum --target=wasm32 --no-standard-libraries -Wl,--export=app_init -Wl,--export=app_render_char -Wl,--export=malloc -Wl,--no-entry -Wl,--allow-undefined -o index.wasm index.c
