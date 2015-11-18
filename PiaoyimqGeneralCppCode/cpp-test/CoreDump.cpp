/*
 * CoreDump.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: ezhweib
 */

#include <iostream>
#include <cstdint>

class Event;

class IEventReceiver {
public:
    virtual ~IEventReceiver() {
    }

    virtual void processEvent(Event& event) = 0;
};

class Sm: public IEventReceiver {
public:
    virtual ~Sm() {
    }

    virtual void processEvent(Event& event) {
        std::cout << __FUNCTION__ << " invoked.\n";
    }
};

class Event {
public:
    typedef uint32_t EventId;

    Event(EventId eventId, const std::string& name) :
            eventId_(eventId), enabled_(true), destination_(0), name_(name) {
    }

    virtual ~Event() {
    }

    const EventId& getId() const {
        return eventId_;
    }

    IEventReceiver* getDestination() const {
        return destination_;
    }

    void setDestination(IEventReceiver* destination) {
        destination_ = destination;
    }

    bool isEnabled() const {
        return enabled_;
    }

    void setEnabled(bool enabled) {
        enabled_ = enabled;
    }

    const std::string& getName() const {
        return name_;
    }

private:
    EventId eventId_;
    bool enabled_;
    IEventReceiver* destination_;
    std::string name_;
};


class Student {
public:
    Student(const char* name):_name(name) {
    }

    void print() {
        std::cout << "Stuendt name is: " << _name << std::endl;
    }

private:
    std::string _name;
};


int main() {
//    Student s1= new Student("Lucy");
    Sm* sm = new Sm();
    Event* const ev = new Event(119, "test");

    ev->setDestination(sm);

    IEventReceiver* const dest = ev->getDestination();
    delete sm;
    if (dest != 0 && ev->isEnabled()) {
        std::cout << "Start to processEvent" << std::endl;
        dest->processEvent(*ev);
    }

    delete ev;

}
