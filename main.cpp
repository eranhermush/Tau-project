//
// Created by Eran on 31/03/2019.
//
#include <iostream>

using namespace std;
#include <bits/stdc++.h>

int main()
{

    vector<int> vect1{ 10, 20, 30 };
    vect1 = {30};
    vector<int> vect2(vect1.begin(), vect1.end());

    for (int x : vect2)
        cout << x << " ";

    return 0;

}