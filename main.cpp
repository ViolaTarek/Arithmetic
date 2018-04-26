#include <bits/stdc++.h>
using namespace std;
tuple<double,double,string> E2Rescale(double L,double U,string code);
tuple<double,double,string> E1Rescale(double L,double U,string code);
tuple<double,double,string> E1RescaleDecode(double L,double U,string input);
tuple<double,double,string> E2RescaleDecode(double L,double U,string input);
string encode(char arr[],int ip[],double CDF[],int Size);
string Decode(char arr[],string input,double CDF[],int WL,int n);
double string2double(string tag);
double E1L=0.0,E1U=0.5,E2L=0.5,E2U=1.0;
string code;
string decode;
double L,U;

int main()
{
    string Input;
    string Output;
    int option;
    double smallest=1.0;
    double sub=0.0;
    int WL;
    int Size;
    int SeqSize;
    int Seq;

    cout<<"Enter Size of your Set "<<endl;
    cin>>Size;
    char Set[Size+1];
    double prob[Size+1],cdf[Size+1];
    cout<< "Start filling the set : ";

    for( int i=0 ; i<Size+1 ; i++)
    {
        if(i==0)
        {
            prob[i]=0.0;
            cdf[i]=0.0;
            Set[i]='0';
        }
        else
        {
            cout<<"enter a letter in set"<<endl;
            cin>>Set[i];
            cout<<"enter probability of letter"<<endl;
            cin>>prob[i];
            cdf[i] =cdf[i-1]+prob[i];        //calculate CDF
        }
    }

    for(int i=0; i<Size+1; i++)
    {
        cout<<"Set of "<<i<<" = "<<Set[i]<<" and its cdf = " <<cdf[i]<<endl;
    }
    cout<<"Now Enter 1 for Encoding , 2 for decoding" << endl;
    cin>>option;


    switch (option)
    {
    //Encoding
    case 1:
    {
        cout<<"Enter the sequence size!" << endl;
        cin>>SeqSize;
        char ip[SeqSize];
        int Input[SeqSize];
        cout<<"if the sequence is indexes press 1 , if sequence is like Set press 2 : "<<endl;
        cin>> Seq;
        if(Seq==2)          //if sequence is like the SET
        {
            for( int i=0 ; i<SeqSize; i++)
            {
                cin>>ip[i];
            }
            for(int i=0; i<SeqSize; i++)
            {
                for(int j=0; j<Size+1; j++)
                {
                    if(ip[i]==Set[j])
                    {
                        Input[i]=j;
                    }
                }
            }
        }
        else if(Seq==1)              //if sequence is number of indexes
        {
            for( int i=0 ; i<SeqSize; i++)
            {
                cin>>Input[i];

            }
        }

        Output=  encode(Set,Input,cdf,SeqSize);
        cout<<"output in binary = "<<Output<<endl;
        cout<<"output in decimal = "<<string2double(Output)<<endl;
        string finalOP=Output+'1';
        for(int i=0; i<32-finalOP.size(); i++)
        {
            finalOP+='0';
        }
        cout<<"Final output = "<<finalOP<<endl;

    }
    //Decoding
    case 2:
    {

        int NumberOfEvents;
        cout<<"Please enter your input as binary number ";
        cin>>Input;
        string finalIP=Input+'1';
        for(int i=0; i<32; i++)
        {
            finalIP+='0';
        }
        cout <<"Enter number of events "<<endl;
        cin>>NumberOfEvents;
        for(int i=1; i<Size; i++)
        {
            sub=cdf[i]-cdf[i-1];
            if(sub < smallest)
                smallest=sub;

        }

        cout<<smallest<<endl;
        WL=ceil(abs(log2(1/smallest)));

        Output= Decode(Set,finalIP,cdf,WL,NumberOfEvents);
        cout<<Output;

    }
    }


}

//////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
//Encoding functions
tuple<double,double,string> E2Rescale(double L,double U,string code)
{

    L=2*(L-0.5);
    U=2*(U-0.5);

    if((L>=E2L && L<E2U ) && (U>=E2L &&U<E2U))
    {
        code+='1';
        auto p = E2Rescale(L,U,code);
        L=get<0>(p);
        U=get<1>(p);
        code=get<2>(p);

    }
    else if((L>=E1L &&L<E1U ) && (U>=E1L &&U<E1U))
    {
        code+='0';
        auto p = E1Rescale(L,U,code);
        L=get<0>(p);
        U=get<1>(p);
        code=get<2>(p);
    }

    return make_tuple(L,U,code);

}

tuple<double,double,string> E1Rescale(double L,double U,string code)
{
    L=2*L;
    U=2*U;

    if((L>=E1L &&L<E1U ) && (U>=E1L &&U<E1U))
    {
        code+='0';
        auto p = E1Rescale(L,U,code);
        L=get<0>(p);
        U=get<1>(p);
        code=get<2>(p);

    }
    else if ((L>=E2L &&U<E2U ) && (L>=E2L &&U<E2U))
    {
        code+='1';
        auto p = E2Rescale(L,U,code);
        L=get<0>(p);
        U=get<1>(p);
        code=get<2>(p);
    }

    return make_tuple(L,U,code);

}

string encode(char arr[],int ip[],double CDF[],int Size)
{
    double factor=0.0,Range=1.0;
    double L=0.0,U=1.0;

    for (int i=0; i < Size ; i++)
    {
        L=factor+(Range*CDF[ip[i]-1]);
        U=factor+(Range*CDF[ip[i]]);

        if((L>=E1L &&L<E1U ) && (U>=E1L &&U<E1U))   //in lower half
        {
            code += '0';
            auto p = E1Rescale(L,U,code);
            L=get<0>(p);
            U=get<1>(p);
            code=get<2>(p);

        }

        else if(((L>=E2L &&L<E2U ) && (U>=E2L &&U<E2U)))  //in upper half
        {
            code +='1';

            auto p = E2Rescale(L,U,code);

            L=get<0>(p);
            U=get<1>(p);
            code=get<2>(p);


        }
        factor=L;
        Range=(U-L);

    }

    return code;

}
////////////////////////////////////////////////////////////////////
//convert Tag value from binary to decimal value
double string2double(string tag)
{
    int length=tag.size();

    int power = 1;
    double value =0.0;

    for ( int i=0; i < length ; i++)
    {
        if (tag[i] == '1')
        {
            value += 1/(pow(2,power));
        }
        power+=1;

    }

    return value;
}


/////////////////////////////////////////////////////////////////////////////////////////
//Decoding functions
string Decode(char arr[],string input, double CDF[], int WL,int number)
{
    double low=0.0,Range=1.0;
    string Tag= input.substr(0, WL);         //take the first work length bits only from the input
    double Tagval= string2double(Tag);       //calculate tag in decimal
    int x;
    for(int i=0 ; i<number ; i++)
    {
        x=1;
        while(Tagval < low+Range*CDF[x-1] || Tagval > low+Range*CDF[x])
        {
            x+=1;
        }
        L=low+Range*CDF[x-1];
        U=low+Range*CDF[x];
        decode+=arr[x];
        if((L>=E1L &&L<E1U ) && (U>=E1L &&U<E1U))           //in lower half
        {
            input = input.erase(0,1);
            auto p = E1RescaleDecode(L,U,input);
            L=get<0>(p);
            U=get<1>(p);
            input=get<2>(p);

        }

        else if(((L>=E2L &&L<E2U ) && (U>=E2L &&U<E2U)))  //in upper half
        {
            input = input.erase(0,1);
            auto p = E2RescaleDecode(L,U,input);
            L=get<0>(p);
            U=get<1>(p);
            input=get<2>(p);

        }
        low =L;
        Range=U-L;
        Tag= input.substr(0, WL);         //take the first work length bits only from the input
        Tagval= string2double(Tag);

    }

    return decode;
}

tuple<double,double,string> E1RescaleDecode(double L,double U,string input)
{
    L=2*L;
    U=2*U;
    if((L>=E2L && L<E2U ) && (U>=E2L &&U<E2U))
    {
        input = input.erase(0,1);
        auto p = E2RescaleDecode(L,U,input);
        L=get<0>(p);
        U=get<1>(p);
        input=get<2>(p);

    }
    else if((L>=E1L &&L<E1U ) && (U>=E1L &&U<E1U))
    {
        input = input.erase(0,1);
        auto p = E1RescaleDecode(L,U,input);
        L=get<0>(p);
        U=get<1>(p);
        input=get<2>(p);
    }

    return make_tuple(L,U,input);

}
tuple<double,double,string> E2RescaleDecode(double L,double U,string input)
{

    L=2*(L-0.5);
    U=2*(U-0.5);
    if((L>=E2L && L<E2U ) && (U>=E2L &&U<E2U))
    {
        input = input.erase(0,1);
        auto p = E2RescaleDecode(L,U,input);
        L=get<0>(p);
        U=get<1>(p);
        input=get<2>(p);


    }
    else if((L>=E1L &&L<E1U ) && (U>=E1L &&U<E1U))
    {
        input = input.erase(0,1);
        auto p = E1RescaleDecode(L,U,input);
        L=get<0>(p);
        U=get<1>(p);
        input=get<2>(p);

    }

    return make_tuple(L,U,input);
}

