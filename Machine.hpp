#ifndef __MACHINE_HPP__
#define __MACHINE_HPP__

#include "State.hpp"
#include <vector>
#include <iostream>

class Machine {
public:
    Machine(std::shared_ptr<State> initial_state) : m_current_state(initial_state) {}

    void apply(const Event& event) {
        m_current_state = m_current_state->on_event(event);
    }

    void apply(const std::vector<Event>& events) {
        for (auto event : events) {
            apply(event);
        }
    }

private:
    std::shared_ptr<State> m_current_state;
};



#endif
