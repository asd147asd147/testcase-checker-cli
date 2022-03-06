#!/bin/sh

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

inpList=$(find $sourceDir -name '*.inp')
tcNum=0
for inp in $inpList
do
    ((tcNum++))
    echo "TC #$tcNum"
    tcName=$(basename -s '.inp' $inp)
    out=$(find $sourceDir -name $tcName.out)
    if [ -z $out ]; then
        echo "tcc: FileError: No exist $tcName.out"
        continue
    fi
    elapsedTime=$((time "$executeFile" < $sourceDir$tcName.inp > $sourceDir$tcName.ans.out) 2>&1 | head -2)
    elapsedTime=`echo $elapsedTime | cut -d 'm' -f2`
    
    lastLine=$(tail $sourceDir$tcName.ans.out)
    sed '$d' $sourceDir$tcName.ans.out > $sourceDir$tcName.ans.out
    echo $lastLine >> $sourceDir$tcName.ans.out
    result=$(diff --brief $sourceDir$tcName.ans.out $sourceDir$tcName.out)

    if [[ -z $result ]]; then
        printf "PASS "
    else
        printf "FAIL "
    fi
    echo $elapsedTime
done
