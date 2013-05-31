#ifndef __STATE_HPP__
#define __STATE_HPP__

#include <string>
#include <map>
#include <memory>
#include <utility>
#include <vector>
#include <sstream>
#include "Event.hpp"

class State;
typedef std::shared_ptr<Event> sh_event;
typedef std::shared_ptr<State> sh_state;
typedef std::pair<sh_event, sh_state> transition_pair;

class State {
public:
    virtual ~State() {}
    virtual sh_state on_event(sh_event event)                               = 0;

    virtual void add_transition(sh_event, sh_state state)                   = 0;
    virtual void add_transition(const transition_pair& pair)                = 0;
    virtual void add_transitions(const std::vector<transition_pair>& pairs) = 0;

    virtual std::string get_id() const                                      = 0;
    virtual std::string get_clazz() const                                   = 0;
    virtual void serialize_transitions(std::ostream& os) const              = 0;

    // hook methods that a derived class can implement to add logic to the state
    virtual void reached_with(sh_event event)                               = 0;
    virtual void left_with(sh_event event)                                  = 0;
};

class BaseState : public State {
public:
    BaseState(const std::string& id) : m_id(id) {}
    BaseState(int i) {
        std::ostringstream oss;
        oss << "q" << i;
        m_id = oss.str();
    }

    virtual sh_state on_event(sh_event event) {
        left_with(event);
        auto new_state = m_transitions[event->get_id()];
        new_state->reached_with(event);
        return new_state;
    }

    void add_transition(sh_event event, sh_state state) {
        m_transitions[event->get_id()] = state;
    }

    void add_transition(const transition_pair& pair) {
        add_transition(pair.first, pair.second);
    }

    void add_transitions(const std::vector<transition_pair>& pairs) {
        for (auto pair : pairs) {
            add_transition(pair);
        }
    }

    virtual std::string get_id() const {
        return m_id;
    }

    virtual std::string get_clazz() const {
        return "BaseState";
    }

protected:
    virtual void reached_with(sh_event event) {}
    virtual void left_with(sh_event event)    {}

private:
    virtual void serialize_transitions(std::ostream& os) const {
        if (m_transitions.empty()) return;
        os << get_id() << ";";
        for (auto p : m_transitions) {
            os << p.first << ":" << p.second->get_id() << ";";
        }
        os << "\n";
    }

    std::map<std::string, sh_state> m_transitions;
    std::string m_id;
};

#endif
