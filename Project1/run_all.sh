sed -i '' -e '5s/.*/#include "doalgQuickSelect.c"/' ./MAIN.c 

gcc -o MAIN ./MAIN.c

./MAIN >output.auto.txt

sed -i '' -e '5s/.*/#include \"doalgMaxHeap.c\"/' ./MAIN.c 

gcc -o MAIN ./MAIN.c

./MAIN >>output.auto.txt

sed -i '' -e '5s/.*/#include \"doalgMinOfMaxes.c\"/' ./MAIN.c 

gcc -o MAIN ./MAIN.c

./MAIN >>output.auto.txt

sed -i '' -e '5s/.*/#include \"doalgMinOfMaxesWithCaching.c\"/' ./MAIN.c 

gcc -o MAIN ./MAIN.c

./MAIN >>output.auto.txt

sed -i '' -e '5s/.*/#include \"avlWithShaunsEdits.c\"/' ./MAIN.c 

gcc -o MAIN ./MAIN.c

./MAIN >>output.auto.txt