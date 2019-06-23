#!/bin/bash
make
make test
make load
chmod a+wrx /dev/booga*

#alias "test-booga"="./test-booga"

echo "------------------/sbin/lsmod | grep booga------------------"
/sbin/lsmod | grep booga

echo "------------------cat /proc/driver/booga------------------"
cat /proc/driver/booga

echo "------------------cat /dev/booga1------------------"
cat /dev/booga1
echo ""

echo "------------------test-booga------------------"
./test-booga

echo "------------------test-booga a 100 raed------------------"
./test-booga a 100 raed

echo "------------------test-booga 1 10a read------------------"
./test-booga 1 10a read

echo "------------------test-booga 1 100 raed------------------"
./test-booga 1 100 raed

echo "------------------test-booga 1 100 read------------------"
./test-booga 1 100 read

echo "------------------test-booga 1 100 read------------------"
./test-booga 1 100 write

echo "------------------cat /proc/driver/booga------------------"
cat /proc/driver/booga

echo "------------------test-booga 3 100 write------------------"
./test-booga 3 100 write

make unload
