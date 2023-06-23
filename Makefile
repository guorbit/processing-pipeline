pipeline: main.c
	gcc -Wall -Werror -Wpedantic main.c -ltensorflow -o pipeline.o -lstdc++