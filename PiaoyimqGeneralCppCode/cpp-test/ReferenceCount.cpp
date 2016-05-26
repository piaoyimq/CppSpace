#include <iostream>
#include <memory>

class HeapOnly {
public:
    HeapOnly();
private:
    //Give auto_ptr<> access to destructor
    friend class std::auto_ptr<HeapOnly>;
    //Prohibit static or automatic allocation
    ~HeapOnly();    //Dynamic allocation only
};

class Handle;
//Forward reference

class Body {     //Base class for body classes
    friend class Handle;
public:
    Body();
protected:
    virtual ~Body();
private:
    void addReference();
    void removeReference();
    size_t count_;
};

Body::Body() :
        count_(0) {
    std::cout << __FUNCTION__  << ":\tcount_ = " << count_ << std::endl;
}

Body::~Body() {
}

void Body::addReference() {
    ++count_;
    std::cout << __FUNCTION__  << ":\tcount_ = " << count_ << std::endl;
}

void Body::removeReference() {
    if (0 == --count_) {
        std::cout << __FUNCTION__  << ":\tcount_ = " << count_ << std::endl;
        delete this;
    }
}

class Handle {
public:
    Handle(Body* body);
    Handle(const Handle& other);
    Handle& operator=(const Handle& other);
    ~Handle();
private:
    Body* body_;
};

Handle::Handle(Body* body) :
        body_(body) {
    body_->addReference();
}

Handle::Handle(const Handle& handle) :
        body_(handle.body_) {
    body_->addReference();
}

Handle& Handle::operator=(const Handle& other) {
    Body* temp = body_;
    other.body_->addReference();
    body_ = other.body_;
    temp->removeReference();
    return *this;
}

Handle::~Handle() {
    body_->removeReference();
}

int main() {
//    Body b;   // Not allowed!
    Handle first(new Body());   // Ok!
    Handle second(first);   // Makes a copy
}

