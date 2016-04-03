#!/bin/bash

qemu-system-arm -M raspi2 -bios kernel.img  -serial stdio
