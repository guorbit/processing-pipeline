# Object files


OBJECTS = main.o segfilter.o TFLiteModel.o

# Executable
pipeline: $(OBJECTS)
	g++ -Wall -Werror -Wpedantic $(OBJECTS) -ltensorflow -o pipeline -lstdc++

# Object file rules
main.o: main.cpp ./src/filter/IFilter.hpp ./src/filter/segfilter.hpp
	g++ -Wall -Werror -Wpedantic -c main.cpp -o main.o

segfilter.o: ./src/filter/segfilter.cpp ./src/filter/segfilter.hpp IFilter.o TFLiteModel.o
	g++ -Wall -Werror -Wpedantic -c ./src/filter/segfilter.cpp -o segfilter.o

TFLiteModel.o: ./src/model/TFLiteModel.cpp ./src/model/TFLiteModel.hpp
	g++ -Wall -Werror -Wpedantic -c ./src/model/TFLiteModel.cpp -o TFLiteModel.o

# Clean rule
clean:
	rm -f $(OBJECTS) pipeline

