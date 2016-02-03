#include <iostream>
#include <cstdint>


class Person{
public:
    Person(const char* itsSex, uint32_t itsAge, uint32_t itsHeight):
        sex(itsSex), age(itsAge), height(itsHeight) {}
   ~Person() {}

    void setSex(const char* sex) {this->sex = std::string(sex);}
    void setAge(uint32_t age) {this->age = age;}
    void setHeight(uint32_t height) {this->height = height;}

    void calcuateSum() {_sum = age + height;}

protected:
    uint64_t _sum;

private:
    std::string sex;
    uint32_t age;
    uint32_t height;
};


int main(){
    Person person("LiLei", 15, 178);
    std::cout << "--------End-------\n";
}
