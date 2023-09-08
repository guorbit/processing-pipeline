#Compiler definition
CC := g++

#Set flags
CFLAGS := -Wall -Werror -Wpedantic -std=c++17 

LINKERFLAGS := -lstdc++ 


#Define object files 
OBJS = test.o exportimage.o 
OUTPUT = test
#Create object files 

test: ${OBJS}
	${CC} ${CFLAGS} -o ${OUTPUT} ${OBJS} ${LINKERFLAGS}

test.o: test.cpp 
	${CC} ${CFLAGS} -c test.cpp

exportimage.o: exportimage.cpp exportimage.hpp 
	${CC} ${CFLAGS} -c exportimage.cpp 

#Clean
clean: 
	rm -f *.o
	rm -f ${OUTPUT}