pipeline.o: main.o IFilter.o segfilter.o
	g++ -Wall -Werror -Wpedantic main.o IFilter.o segfilter.o -ltensorflow -o pipeline.o -lstdc++

main.o: main.cpp IFilter.h segfilter.h
	g++ -Wall -Werror -Wpedantic -c main.cpp -ltensorflow -o main.o -lstdc++

segfilter.o: segfilter.cpp segfilter.h IFilter.h
	g++ -Wall -Werror -Wpedantic -c segfilter.cpp -ltensorflow -o segfilter.o -lstdc++

IFilter.o: IFilter.cpp IFilter.h
	g++ -Wall -Werror -Wpedantic -c IFilter.cpp -ltensorflow -o IFilter.o -lstdc++