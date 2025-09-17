#include<iostream>
int main()
{
    int x[5]{1,2,3,4,5};
    std::cout<<&x[0]<<'\n';
    std::cout<<&x[1]<<'\n';
    std::cout<<&x[2]<<'\n';
    return 0;
}