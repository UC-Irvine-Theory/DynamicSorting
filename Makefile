FLAGS := -std=c++17 -Wall -Wpedantic -Wextra -O3

all: Sort

Sort: testRunner.cpp evolvingList.o initialConfiguration.o sorts.o
	g++ $(FLAGS) testRunner.cpp evolvingList.o initialConfiguration.o sorts.o -o Sort

evolvingList.o: evolvingList.cpp evolvingList.h
	g++ $(FLAGS) -c evolvingList.cpp

initialConfiguration.o: initialConfiguration.cpp
	g++ $(FLAGS) -c initialConfiguration.cpp

sorts.o: sorts.cpp sorts.h evolvingList.o
	g++ $(FLAGS) -c sorts.cpp

PrintConfig: initialConfiguration.cpp printConfigurations.cpp
	g++ $(FLAGS) initialConfiguration.cpp printConfigurations.cpp -o PrintConfigs

clean:
	rm -f Sort initialConfiguration.o PrintConfigs evolvingList.o initialConfiguration.o sorts.o
