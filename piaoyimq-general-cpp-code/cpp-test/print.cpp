#include <iostream>
//using std::ostream;
using namespace std;

void  print(ostream& os) {
        //#[ operation print(std::ostream&) const

        os << "[";
        os << "NotificationId=" << 5;
        os << ", Operation=" << 3;
        os << ", Threshold=" << 2;
        os << ", Data=";
        os << "]";
        //#]
    }

int main(int argc, char **argv)
{
//	std::ostream out;

	print(cout);
	return 0;
//	cout<<out;

}
