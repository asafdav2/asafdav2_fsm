#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include <string>
#include <iostream>

class Event {
public:
    Event(const std::string& data) : m_data(data) {}
    std::string get_data() const {
        return m_data;
    }

    friend std::ostream& operator<< (std::ostream& os, const Event& event) {
        return os << "Event " << event.m_data;
    }

private:
    std::string m_data;
};

#endif
