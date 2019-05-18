#!/usr/bin/bash


../archiver c archiv Study
mv Study Study.old
../archiver x archiv
diff -r Study Study.old
rm -rf Study
rm -f archiv
mv Study.old Study
