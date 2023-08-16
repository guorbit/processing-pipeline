#Compiler definition
CC := g++

#Set flags
CFLAGS := -Wall -Werror -Wpedantic

#Set stbi header file location
STBI_HEADER_IMAGE = stb_image/stb_image.h
STBI_HEADER_WRITE = stb_image/stb_image_write.h

#Define object files 
OBJS = exportimage.o test.object
OUTPUT = test
#Create object files 

exportimage.o: exportimage.cpp exportimage.hpp $(STBI_HEADER_IMAGE) $(STBI_HEADER_WRITE)
	${CC} ${CFLAGS} -c exportimage.cpp -Istb_image

#Clean
clean: 
	rm -f *.o
	rm -f ${OUTPUT}