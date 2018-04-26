#ifndef ENCODER_H
#define ENCODER_H
#include <iostream>
#include <tuple>
#include <string>

using namespace std;

class Encoder
{
    public:
        Encoder();
        virtual ~Encoder();
tuple<double,double,string> E1Rescale(double L,double U,string code);
tuple<double,double,string> E1Rescale(double L,double U,string code);
string encode(char arr[],int ip[],double CDF[],int Size);

    protected:
    private:
};

#endif // ENCODER_H
