#!/bin/bash

if [[ $# -ne 2 ]]; then
	echo
	echo "Usage: $0 [prefix] [object-file]"
	echo
	exit -1
fi;

if [ "$UNAME" == "" ]; then
UNAME=`uname -s`
fi
if [ "$ARCH" == "" ]; then
ARCH=`uname -m`
fi
if [ "$NM" == "" ]; then
NM=nm
fi
if [ "$LD" == "" ]; then
NM=ld
fi
if [ "$UNAME" == "Linux" ]; then
	$OBJCOPY  `cat crypto/$1/symbols.map | sed -e "s/^\(.*\) \(.*\)$/--redefine-sym \1=\2/g" | tr '\n' ' '` $2
    symbols=(`$NM --defined-only -f p $2 | cut -f1 -d' ' | grep -v "^$1"`)
    for item in "${symbols[@]}"; do
        $OBJCOPY --redefine-sym $item="$1_$item" $2
    done
    # Remove weak symbols if a strong one is available
    # Currently it affect defunc_pk_from_sk module only
    count=$(nm -f p $2 | grep public_key_from_private_key | awk '{print $2}' | sort | uniq | tr -d '\n' | tr '[[:lower:]]' '[[:upper:]]')
    if [ "$count" == "TW" ]; then
        ar dv $2 defunc_pk_from_sk.o
    fi
elif [ "$UNAME" == "Darwin" ]; then
    $LD -r -arch $ARCH -o $2 `cat crypto/$1/symbols.map | sed -e 's/^\(.*\) \(.*\)$/-alias _\1 _\2 -exported_symbol _\2/g' | tr '\n' ' '` $2
else
    echo "Unsupported platform $UNAME"
    exit -1
fi
