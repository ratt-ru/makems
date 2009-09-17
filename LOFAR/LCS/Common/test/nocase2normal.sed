#!/bin/sh
sed -e 's,\(^\[[^]]*\]\),\L\1,' \
    -e 's,^\(\([[:alpha:]]\.\)\{1\,\}[[:alpha:]]\+\),\L\1,' \
tParameterSet.stdout 
