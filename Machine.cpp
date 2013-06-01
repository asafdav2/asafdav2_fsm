#include "Machine.hpp"
#include <cstring>
#include <iterator>
#include <fstream>

using namespace std;

Factory<State> Machine::states_factory;
Factory<Event> Machine::events_factory;

std::string Machine::STATE_FILENAME = "machine_state.txt";

void Machine::serialize(std::ostream& os) {
    for (auto state : m_states) {
        os << state->get_clazz() << ":" << state->get_id() << ";";
    }
    os << "\n" << m_current_state->get_id() << "\n";
    for (auto event : m_events) {
        os << event->get_clazz() << ":" << event->get_id() << ";";
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

void separate_in_colon(const std::string& str, std::string& left, std::string& right) {
    const int colon_pos = str.find(":");
    left  = str.substr(0,colon_pos);
    right = str.substr(colon_pos+1);
}

Machine Machine::deserialize(std::istream& is) {
    string line, id, clazz, typed_state, typed_event;
    map<string, sh_state> states_map;
    map<string, sh_event> events_map;

    // read states
    getline(is, line);
    istringstream states_line_iss(line);
    while  (getline(states_line_iss, typed_state, ';')) {
        separate_in_colon(typed_state, clazz, id);
        states_map[id] = states_factory.create(clazz, id);
    }

    // read current state
    getline(is, id);
    sh_state current_state = states_map[id];

    // read events
    getline(is, line);
    istringstream events_line_iss(line);
    while (getline(events_line_iss, typed_event, ';')) {
        separate_in_colon(typed_event, clazz, id);
        events_map[id] = events_factory.create(clazz, id);
    }

    // read transitions
    string trans_pair, e_id, s_id;
    sh_state state;
    while (getline(is, line)) {
        istringstream transitions_line_iss(line);
        getline(transitions_line_iss, id, ';');
        state = states_map[id];
        while (getline(transitions_line_iss, trans_pair, ';')) {
            separate_in_colon(trans_pair, e_id, s_id);
            state->add_transition(events_map[e_id], states_map[s_id]);
        }
    }

    vector<sh_event> events;
    vector<sh_state> states;
    map2vec(events_map, events);
    map2vec(states_map, states);
    return Machine(events, states, current_state);
}

void Machine::save_to_file() {
    ofstream ofs(STATE_FILENAME);
    serialize(ofs);
}

Machine Machine::load_from_file() {
    ifstream ifs(STATE_FILENAME);
    return Machine::deserialize(ifs);
}
