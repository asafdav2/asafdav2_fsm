fsm: fsm.cpp Event.hpp Machine.hpp State.hpp
	g++ -Wall -o fsm fsm.cpp -std=c++11
clean:
	rm -f fsm
