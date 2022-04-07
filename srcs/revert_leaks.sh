#!/bin/bash

make fclean &&
mv Makefile LEAKS/ &&
mv TMP/Makefile . &&
mv manageServer/AllServers_leaks.cpp LEAKS/ &&
rmdir TMP &&

echo "files reverted"
