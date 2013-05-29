#include "Event.hpp"
#include "State.hpp"
#include "Machine.hpp"
#include <algorithm>
#include <sstream>

using namespace std;

class Seq1State : public State {
public:
    virtual void reached_with(const Event& event) {
        cout << "Seq1\n";
    }
};

class Seq2State : public State {
public:
    virtual void reached_with(const Event& event) {
        cout << "Seq2\n";
    }
};

class Seq3State : public State {
    public:
    virtual void left_with(const Event& event) {
        cout << "Seq3: " << event << "\n";
    }
};

int main() {
    Event A("A"), B("B"), C("C");
    vector<shared_ptr<State>> q(10);
    generate(q.begin(), q.end(), make_shared<State>);
    
    q[6] = make_shared<Seq1State>();
    q[7] = make_shared<Seq3State>();

    // to support Seq1
    q[1]->add_transitions({make_pair(A, q[2]), make_pair(B, q[1]), make_pair(C, q[1])});
    q[2]->add_transitions({make_pair(A, q[3]), make_pair(B, q[8]), make_pair(C, q[7])});
    q[3]->add_transitions({make_pair(A, q[2]), make_pair(B, q[4]), make_pair(C, q[7])});
    q[4]->add_transitions({make_pair(A, q[2]), make_pair(B, q[1]), make_pair(C, q[5])});
    q[5]->add_transitions({make_pair(A, q[6]), make_pair(B, q[1]), make_pair(C, q[1])});
    q[6]->add_transitions({make_pair(A, q[2]), make_pair(B, q[1]), make_pair(C, q[7])});
    
    // to support Seq3
    q[7]->add_transitions({make_pair(A, q[2]), make_pair(B, q[1]), make_pair(C, q[1])});

    // to support Seq2
    shared_ptr<State> last_state = q[8], new_state;
    for (int i = 0; i != 1000; ++i) {
        new_state = make_shared<State>();
        last_state->add_transitions({make_pair(A, q[2]), make_pair(B, q[1]), make_pair(C, new_state)});
        q.push_back(last_state);
        last_state = new_state;
    }
    q[9] = make_shared<Seq2State>();
    q[9]->add_transitions({make_pair(A, q[3]), make_pair(B, q[1]), make_pair(C, q[7])});
    new_state->add_transitions({make_pair(A, q[9]), make_pair(B, q[1]), make_pair(C, q[1])});


    // build and run the machine
    Machine machine(q[1]);

    machine.apply({A, B, A, B});
    machine.apply({A, A, B, C, A}); // should trigger Seq1
    machine.apply({C, A}); // should trigger Seq3 with A
    machine.apply({C, A}); // should trigger Seq3 with A
    machine.apply(B);
    for (int i = 0; i != 1000; ++i) {
        machine.apply(C);
    }
    machine.apply(A); // should trigger Seq2
    machine.apply({C, B}); // should trigger Seq3 with B
}
    
