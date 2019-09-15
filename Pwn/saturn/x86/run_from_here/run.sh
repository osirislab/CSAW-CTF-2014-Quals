#!/bin/bash
LD_LIBRARY_PATH=$(pwd):$LD_LIBRARY_PATH ncat -e "./saturn" -lvvp 8888
