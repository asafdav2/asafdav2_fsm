#ifndef __STATE_HPP__
#define __STATE_HPP__

#include <string>
#include <map>
#include <memory>
#include <utility>
#include <vector>
#include "Event.hpp"

class State {
public:

    virtual ~State() {}

    virtual std::shared_ptr<State> on_event(const Event& event) {
        left_with(event);
        auto new_state = m_transitions[event.get_data()];
        new_state->reached_with(event);
        return new_state;
    }

    void add_transition(const Event& event, std::shared_ptr<State> state) {
        m_transitions[event.get_data()] = state;
    }

    void add_transition(const std::pair<Event, std::shared_ptr<State>>& pair) {
        add_transition(pair.first, pair.second);
    }

    void add_transitions(const std::vector<std::pair<Event, std::shared_ptr<State>>>& pairs) {
        for (auto pair : pairs) {
            add_transition(pair);
        }
    }

protected:
    // hook methods
    virtual void reached_with(const Event& event) {};
    virtual void left_with(const Event& event) {};

private:
    std::map<std::string, std::shared_ptr<State> > m_transitions;
};

#endif
