#!/bin/bash

# Generate Makefile

# cat skeleton of Makefile
cat make_skel >Makefile

# dependencies for .c files
for i in src/*.c
do
	deps=$(grep -e '^#include ".*"' $i | cut -d ' ' -f 2 | tr -d '"')
	echo -n "bin/$(basename ${i/.c/.o}) : $i" >>Makefile
	for j in $deps
	do
		echo -n " include/$j" >>Makefile
	done
	echo -e "\n\t\$(CC) \$(CFLAGS) -c -T \$(LD) -o \$@ \$<\n" >>Makefile
done

# dependencies for .s files
for i in src/*.s
do
	echo "bin/$(basename ${i/.s/.o}) : $i" >>Makefile
	echo -e "\t\$(AS) -o \$@ \$<\n" >>Makefile
done
