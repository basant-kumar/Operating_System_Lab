#!/bin/bash

for((i=$1;i<=$2;i++))
do
	dd if=/dev/zero of=f$i.txt count=2048 bs=1024
done
