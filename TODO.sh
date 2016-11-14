#!/bin/sh

find . -name "*.c" -print | xargs grep "TODO.*"
