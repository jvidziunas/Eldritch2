#!/bin/bash
# Pre-build script for the Eldritch2 game engine responsible for ensuring FlatBuffer schema files are current

echo "Compiling FlatBuffer schemas..." 1>&2
for inputFile in flatbuffers/*.schema;
do
	echo "Compiling $inputFile..." 1>&2
	tools/flatc -c -o src/ $inputFile 1>&2
done