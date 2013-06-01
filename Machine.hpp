#ifndef __MACHINE_HPP__
#define __MACHINE_HPP__

#include "State.hpp"
#include "Factory.hpp"
#include <vector>
#include <iostream>

class Machine {
public:
    Machine(const std::vector<sh_event>& events, 
            const std::vector<sh_state>& states, 
            sh_state initial_state) 
        : m_events(events), m_states(states), m_current_state(initial_state) {}

    void apply(sh_event event) {
        m_current_state = m_current_state->on_event(event);
    }

    void apply(const std::vector<sh_event> events) {
        for (auto event : events) {
            apply(event);
        }
    }

    void serialize(std::ostream& os);
    static Machine deserialize(std::istream& is);

    void save_to_file();
    static Machine load_from_file();

    template<typename S>
    static void register_state_class(const std::string& name) {
        states_factory.registerClass<S>(name);
    }

    template<typename S>
    static void register_event_class(const std::string& name) {
        events_factory.registerClass<S>(name);
    }

private:
    std::vector<sh_event> m_events;
    std::vector<sh_state> m_states;
    sh_state m_current_state;
    static Factory<State> states_factory;
    static Factory<Event> events_factory;

    static std::string STATE_FILENAME;
};

#endif
