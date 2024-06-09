#!/bin/bash
set -e
g++ -Wall -Werror -g main.cpp priority_queue.cpp instance_counter.cpp data_set.cpp linked_list.cpp config.cpp charmatrix.cpp -o lex
./lex $*
