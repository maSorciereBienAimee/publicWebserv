#!/bin/bash

mkdir TMP &&
mv Makefile TMP/ &&
mv LEAKS/Makefile . &&
mv LEAKS/AllServers_leaks.cpp manageServer/ &&
make leaks &&
echo "Usage: valgrind ./leaks conf/config.conf" &&
echo "To return to previous state: bash revert_leaks.sh"
