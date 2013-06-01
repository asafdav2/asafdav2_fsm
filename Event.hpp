#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include <string>
#include <iostream>

class Event {
public:
    virtual ~Event() {}
    Event(const std::string& id, const std::string& data = "") : m_id(id), m_data(data) {}

    std::string get_id() const {
        return m_id;
    }

    std::string get_data() const {
        return m_data;
    }

    virtual std::string get_clazz() const {
        return "Event";
    }

    friend std::ostream& operator<< (std::ostream& os, const Event& event) {
        return os << "Event " << event.m_id << " of type " << event.get_clazz();
    }

private:
    const std::string m_id, m_data;
};

#endif
