/*
 * delete_vector_report.cpp
 *
 *  Created on: Aug 23, 2018
 *      Author: azhweib
 */

#include<set>
#include<vector>
#include<iostream>
using namespace std;

int main()
{
    vector<int> vec;
    vec =
    {   1, 2, 3, 4, 8, 9, 3, 2, 1, 0, 4, 8};
    set<int> st(vec.begin(), vec.end());
    vec.assign(st.begin(), st.end());

    vector<int>::iterator it;
    for (it = vec.begin(); it != vec.end(); it++)
        cout << *it << endl;
    return 0;
}
