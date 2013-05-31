#include "Event.hpp"
#include "State.hpp"
#include "Machine.hpp"
#include <algorithm>
#include <sstream>

using namespace std;

// define some custom states
class Seq1State : public BaseState {
public:
    Seq1State(int i) : BaseState(i) {}
    Seq1State(const string& id) : BaseState(id) {}
    virtual void reached_with(sh_event event) {
        cout << "Seq1\n";
    }
    virtual string get_clazz() const {
        return "Seq1State";
    }
};

class Seq2State : public BaseState {
public:
    Seq2State(int i) : BaseState(i) {}
    Seq2State(const string& id) : BaseState(id) {}
    virtual void reached_with(sh_event event) {
        cout << "Seq2\n";
    }
    virtual string get_clazz() const {
        return "Seq2State";
    }
};

class Seq3State : public BaseState {
    public:
    Seq3State(int i) : BaseState(i) {}
    Seq3State(const string& id) : BaseState(id) {}
    virtual void left_with(sh_event event) {
        cout << "Seq3: " << *event << "\n";
    }
    virtual string get_clazz() const {
        return "Seq3State";
    }
};

int main() {

    // register our states, so they can be deserialized
    Machine::register_state_class<BaseState>("BaseState");
    Machine::register_state_class<Seq1State>("Seq1State");
    Machine::register_state_class<Seq2State>("Seq2State");
    Machine::register_state_class<Seq3State>("Seq3State");

    // prepare the machine ingridients
    sh_event A = make_shared<Event>("A"), 
             B = make_shared<Event>("B"), 
             C = make_shared<Event>("C");
    vector<sh_state> Q(10);
    for (int i = 0; i != 10; ++i) {
        Q[i] = make_shared<BaseState>(i);
    }
    
    Q[6] = make_shared<Seq1State>(6);
    Q[7] = make_shared<Seq3State>(7);

    // support Seq1
    Q[1]->add_transitions({make_pair(A, Q[2]), make_pair(B, Q[1]), make_pair(C, Q[1])});
    Q[2]->add_transitions({make_pair(A, Q[3]), make_pair(B, Q[8]), make_pair(C, Q[7])});
    Q[3]->add_transitions({make_pair(A, Q[2]), make_pair(B, Q[4]), make_pair(C, Q[7])});
    Q[4]->add_transitions({make_pair(A, Q[2]), make_pair(B, Q[1]), make_pair(C, Q[5])});
    Q[5]->add_transitions({make_pair(A, Q[6]), make_pair(B, Q[1]), make_pair(C, Q[1])});
    Q[6]->add_transitions({make_pair(A, Q[2]), make_pair(B, Q[1]), make_pair(C, Q[7])});
    
    // support Seq3
    Q[7]->add_transitions({make_pair(A, Q[2]), make_pair(B, Q[1]), make_pair(C, Q[1])});

    // support Seq2
    sh_state last_state = Q[8], new_state;
    const int LEN = 5;
    for (int i = 0; i != LEN; ++i) {
        new_state = make_shared<BaseState>(i + 10);
        last_state->add_transitions({make_pair(A, Q[2]), make_pair(B, Q[1]), make_pair(C, new_state)});
        Q.push_back(last_state);
        last_state = new_state;
    }
    Q[9] = make_shared<Seq2State>(9);
    Q[9]->add_transitions({make_pair(A, Q[3]), make_pair(B, Q[1]), make_pair(C, Q[7])});
    new_state->add_transitions({make_pair(A, Q[9]), make_pair(B, Q[1]), make_pair(C, Q[1])});
    Q.push_back(new_state);

    // construct and run the machine
    Machine m1({A,B,C}, Q, Q[1]);

    m1.apply({A, B, A, B});
    m1.apply({A, A, B, C, A}); // should trigger Seq1
    m1.apply({C, A}); // should trigger Seq3 with A

    // persist the state of the machine to string
    ostringstream oss;
    m1.serialize(oss);
    const string serialized = oss.str();
    
    // build a new machine and continue the run
    istringstream iss(serialized);
    Machine m2 = Machine::deserialize(iss);
    m2.apply({C, A}); // should trigger Seq3 with A
    m2.apply(B);
    for (int i = 0; i != LEN; ++i) {
        m2.apply(C);
    }
    m2.apply(A); // should trigger Seq2
    m2.apply({C, B}); // should trigger Seq3 with B
}
    
