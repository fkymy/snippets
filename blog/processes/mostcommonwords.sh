#!/bin/bash
# Call this script as, e.g., “./mostcommonwords.sh 1000 < file”

tr -cs A-Za-z '\n' |
tr A-Z a-z |
sort |
uniq -c |
sort -rn |
head -n $1
