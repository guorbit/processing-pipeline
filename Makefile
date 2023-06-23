# Object files
OBJECTS = main.o IFilter.o segfilter.o model.o

# Executable
pipeline: $(OBJECTS)
	g++ -Wall -Werror -Wpedantic $(OBJECTS) -ltensorflow -o pipeline -lstdc++

# Object file rules
main.o: main.cpp ./src/filter/IFilter.hpp ./src/filter/segfilter.hpp
	g++ -Wall -Werror -Wpedantic -c main.cpp -o main.o

segfilter.o: ./src/filter/segfilter.cpp ./src/filter/segfilter.hpp IFilter.o model.o
	g++ -Wall -Werror -Wpedantic -c ./src/filter/segfilter.cpp -o segfilter.o

model.o: ./src/model/model.cpp ./src/model/model.hpp
	g++ -Wall -Werror -Wpedantic -c ./src/model/model.cpp -o model.o

IFilter.o: ./src/filter/IFilter.cpp ./src/filter/IFilter.hpp
	g++ -Wall -Werror -Wpedantic -c ./src/filter/IFilter.cpp -o IFilter.o

# Clean rule
clean:
	rm -f $(OBJECTS) pipeline
