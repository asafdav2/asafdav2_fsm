fsm: fsm.cpp Machine.cpp Event.hpp Machine.hpp State.hpp Factory.hpp
	g++ -Wall -o fsm fsm.cpp Machine.cpp -std=c++11
clean:
	rm -f fsm machine_state.txt
