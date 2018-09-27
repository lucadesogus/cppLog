//#define _LOG2FILE_ //uncomment this line to try log to file
#include "cpplog.hpp"
#include <thread>
#include <vector>
#include <algorithm>
#include <climits>
#ifdef _LOG2FILE_
#include <fstream>
#endif
#include "luca.hpp"
using namespace std;

template <class T>
class Var
{
public:
    Var(const T & val):i(val) {}

    T i;

    friend std::ostream& operator<< (std::ostream &out, const Var<T> &element)
    {
        out << "I'm Var<"<< typeid(T).name()<<">. My value is " <<element.i <<".";
        return out;
    }
};

template <class U>
class VarVect : public Var<std::vector<U>>
{
public:
    VarVect(const std::vector<U> & val):Var<std::vector<U>>(val){}

    friend std::ostream& operator<< (std::ostream &out, const VarVect<U> &element)
    {
        out << "I'm VarVect<"<< typeid(U).name()<<">. ";
        if (!element.i.empty())
        {
            out << "My content is: ";
            for(const auto & item: element.i)
                out << item <<" ";
        }
        else
        {
            out << "I'm empty";
        }

        return out;
    }
};

int main()
{
    cppLog::LogI("cpplog example start.");

#ifdef _LOG2FILE_
    cppLog::printColors(false);
    std::ofstream ofs ("cpplog_example.log", std::ofstream::out);
    cppLog::setOutput(ofs);
#endif
    //lvalue
    long long llm = LLONG_MAX;
    cppLog::LogW(llm);

    // example of decimal precision
    Var<long double> vld(945.776767667678545);
    cppLog::LogD(CPPLOG_DECIMAL_PRECISION(15),vld,CPPLOG_DECIMAL_PRECISION(6),vld);

    //user defined class example
    Var<int> vi(69);
    cppLog::LogE(vi);
    std::vector<int> vc {10,56,99,45,89,11,9,18};
    VarVect<int> vvi(vc);
    cppLog::LogD(vvi);

    VarVect<string> vvs({"Hello","World!"});
    cppLog::LogI(vvs);

    //multithread example
    const int l_numThreads = 5; //try it with bigger values
    const int l_numLoopsInThread = 1; //try it with bigger values
    std::vector<std::thread> l_vectThreads;
    for(int h = 0; h != l_numThreads; ++h)
    {
        l_vectThreads.push_back(std::thread ([h,&l_numLoopsInThread]()
        {
            for (int i =0; i!= l_numLoopsInThread ; ++i)
            {
                cppLog::LogD("loop ",i,"- thread ",h);
                cppLog::LogI("loop ",i,"- thread ",h);
                cppLog::LogW("loop ",i,"- thread ",h);
                cppLog::LogE("loop ",i,"- thread ",h);
            }
        }));
    }
    //join threads
    std::for_each (l_vectThreads.begin(),l_vectThreads.end(),[](std::thread & t){t.join();});
#ifndef _LOG2FILE_
    cppLog::printColors(false);
#endif
    // :)
    luca l,u,c,a;
#ifndef _LOG2FILE_   
    cppLog::printColors(true);
#endif
    cppLog::LogI("cpplog example end.");
#ifdef _LOG2FILE_
    ofs.close();
    cppLog::resetOutput();
#endif
    cppLog::LogD("Goodbye.");
    return 0;
}
