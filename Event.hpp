#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include <string>
#include <iostream>

class Event {
public:
    Event(const std::string& id) : m_id(id) {}
    std::string get_id() const {
        return m_id;
    }

    friend std::ostream& operator<< (std::ostream& os, const Event& event) {
        return os << "Event " << event.m_id;
    }

private:
    const std::string m_id;
};

#endif
