#!/bin/sh


find . -name *.c -print | xargs egrep -n -o "TODO.*" | grep "TODO.*"
