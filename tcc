#!/bin/sh

CYAN='\033[0;36m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m'

if [ $# -lt 1 ]; then
    echo 'tcc: FileError: No cpp file'
    exit 1
fi

sourceFile=$1
extension=${sourceFile##*.}
sourceDir=${sourceFile%/*}/

if [ $extension != 'cpp' ]; then
    echo "tcc: FileError: File isn't cpp format"
    exit 1
fi

sourceName=$(basename -s '.cpp' $sourceFile)
executeFile=$sourceDir$sourceName
g++ -std=c++17 -o "$executeFile" "$sourceFile"

parallelJudge() {
    local elapsedTime=$((time "$executeFile" < $sourceDir$1.inp > $sourceDir$1.ans.out) 2>&1 | head -2)
    local elapsedTime=`echo $elapsedTime | cut -d 'm' -f2`
    local lastLine=$(tail $sourceDir$tcName.ans.out)
    sed '$d' $sourceDir$tcName.ans.out > $sourceDir$tcName.ans.out
    echo $lastLine >> $sourceDir$tcName.ans.out
    local result=$(diff --brief $sourceDir$tcName.ans.out $sourceDir$tcName.out)
    judge=""
    if [[ -z $result ]]; then
        judge="${BLUE}PASS${NC}\t"
    else
        judge="${RED}FAIL${NC}\t"
    fi
    printf "#$2\t$1\t$judge$elapsedTime\n" >> result.txt
    rm $sourceDir$tcName.ans.out 
}

inpList=$(basename -s '.inp' $(find $sourceDir -name '*.inp') | sort -n)
tcNum=0
printf "${CYAN}NO.\tNAME\tSTATUS\tTIME${NC}\n"
for tcName in $inpList
do
    ((tcNum++))
    out=$(find $sourceDir -name $tcName.out)
    if [ -z $out ]; then
        echo "tcc: FileError: No exist $tcName.out"
        continue
    fi
    parallelJudge $tcName $tcNum&
done
wait

cat result.txt | sort
rm "$executeFile" result.txt
