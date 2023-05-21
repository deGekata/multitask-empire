#!/bin/bash

find $1 -name "*.[c|h]pp" | xargs wc -l
