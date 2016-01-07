/*
 * CoreDump.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: ezhweib
 */

#include <iostream>
#include <cstdint>

namespace esc {
class IEventReceiver;

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

    virtual void setDestination(Event* event) {
        std::cout << "Sm this = " << this << std::endl;
        event->setDestination(this);
    }
    virtual bool handleEvent(Event& event) = 0;

    virtual void processEvent(Event& event) {
        std::cout << __FUNCTION__ << " invoked.\n";
    }
private:
    int _test;
};

class Student {
public:
    Student(const char* name) :
            _name(name) {
    }

    void print() {
        std::cout << "Stuendt name is: " << _name << std::endl;
    }

private:
    std::string _name;
};
}

namespace AgentSupport {
class Sm: public esc::Sm {
public:
    bool handleEvent(esc::Event& ev) {
        static_cast<void>(ev);
        return true;
    }
};
}

int main() {
//    Student s1= new Student("Lucy");
//    esc::Sm* itsSm = new Sm(); //error, abstract class can't declare a object
    AgentSupport::Sm* itsSm = new AgentSupport::Sm();
    esc::Event* const ev = new esc::Event(119, "test");
    itsSm->setDestination(ev);

    esc::IEventReceiver* const dest = ev->getDestination();

    delete itsSm;
    if (dest != 0 && ev->isEnabled()) {
        std::cout << "Start to processEvent" << std::endl;
        dest->processEvent(*ev);
    }

    delete ev;

}
