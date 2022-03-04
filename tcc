#!/bin/sh

if [ $# -lt 1 ]; then
    echo 'tcc: FileError: No cpp file'
    exit 1
fi

sourceFile=$1

echo $1

