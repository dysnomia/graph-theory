#!/bin/sh

cd dict
for i in *.txt
do
	../word_graph < $i > ../out/${i%.txt}.out
done

