#!/bin/bash
LD_LIBRARY_PATH=$(pwd):$LD_LIBRARY_PATH qemu-mips -L $(pwd) ./saturn-nostrip-mips
