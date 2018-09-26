#pragma once
#include "cpplog.hpp"
class luca
{
public:
    luca()
    {
        cppLog::LogW("HELLO FROM NEW LUCA INSTANCE, my mem address is: ", (unsigned long)this);
    }
};
