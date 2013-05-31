#include "Machine.hpp"
#include <cstring>
#include <iterator>

using namespace std;

Factory<State> Machine::states_factory;

void Machine::serialize(std::ostream& os) {
    for (auto state : m_states) {
        os << state->get_clazz() << ":" << state->get_id() << ";";
    }
    os << "\n" << m_current_state->get_id() << "\n";
    for (auto event : m_events) {
        os << event->get_id() << ";";
    }
    os << "\n";
    for (auto state : m_states) {
        state->serialize_transitions(os);
    }
}

template <typename M, typename V> 
void map2vec(const M& m, V& v) {
    for (typename M::const_iterator it = m.begin(); it != m.end(); ++it) {
        v.push_back(it->second);
    }
}

Machine Machine::deserialize(std::istream& is) {
    string line, id, clazz, typed_state;
    int colon_pos;
    map<string, sh_state> states_map;
    map<string, sh_event> events_map;

    // read states
    getline(is, line);
    istringstream states_line_iss(line);
    while  (getline(states_line_iss, typed_state, ';')) {
        colon_pos = typed_state.find(":");
        clazz = typed_state.substr(0,colon_pos), 
           id = typed_state.substr(colon_pos+1);
        states_map[id] = states_factory.createObject(clazz, id);
    }

    // read current state
    getline(is, id);
    sh_state current_state = states_map[id];

    // read events
    getline(is, line);
    istringstream events_line_iss(line);
    while (getline(events_line_iss, id, ';')) {
        events_map[id] = make_shared<Event>(id);
    }

    // read transitions
    string trans_pair, e_id, s_id;
    sh_state state;
    while (getline(is, line)) {
        istringstream transitions_line_iss(line);
        getline(transitions_line_iss, id, ';');
        state = states_map[id];
        while (getline(transitions_line_iss, trans_pair, ';')) {
            colon_pos = trans_pair.find(":");
            e_id = trans_pair.substr(0,colon_pos), 
            s_id = trans_pair.substr(colon_pos+1);
            state->add_transition(events_map[e_id], states_map[s_id]);
        }
    }

    vector<sh_event> events;
    vector<sh_state> states;
    map2vec(events_map, events);
    map2vec(states_map, states);
    return Machine(events, states, current_state);
}
