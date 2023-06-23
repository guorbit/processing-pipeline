pipeline: main.cpp
	gcc -Wall -Werror -Wpedantic main.cpp -ltensorflow -o pipeline.o -lstdc++