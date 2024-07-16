set +o xtrace
ircc resources.txt -o ircc_resources.gen.cpp
ircc resources.txt -o ircc_resources.gen.c --c_only 
g++ -o runtest1 main.cpp ircc_resources.gen.cpp -I . -g
gcc -o runtest2 main.c ircc_resources.gen.c -I . -g