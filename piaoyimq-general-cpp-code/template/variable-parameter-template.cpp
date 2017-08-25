#include <bits/stdc++.h>
using namespace std;

template<class... Args>
int getArgsNum(Args... args)
{
    return sizeof...(args);
}


template<class T>
void Print(T val)
{
    cout <<val <<endl;
}


template<class Head, class... Tail>
void Print(Head head, Tail... tail)
{
    cout << head << ", ";
    Print(tail...);
}


int main()
{
    int ans=getArgsNum(1, 2, "Mayun", "Japan");

    cout << ans <<endl;


    Print(1);
    Print(1, "Mayuyu", "Japan");
    Print("Japan", "hello", "mayuyu", 21);
    return 0;
}
