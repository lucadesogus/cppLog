/*
 * cppLog - Log helper class written in modern C++
 *
 * Copyright (c) 2018 Luca Desogus
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* ---------------------------------------------------------------------
 * Version: 0.1.0
 *
 * Author:
 * Luca Desogus <luca.desogus@outlook.com>
 * ---------------------------------------------------------------------
 */
#pragma once
#include <mutex>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <utility>
#ifdef _WIN32
#include <windows.h>
#endif
#define CPPLOG_DECIMAL_PRECISION(x) \
    std::fixed, std::setprecision(x)
// std::fixed, "\b\b", std::setprecision(x), "\b\b"
#define __DEC_PREC__ CPPLOG_DECIMAL_PRECISION
using namespace std::chrono;

class cppLog
{
    enum class log_level : unsigned char
    {
        __DEBUG = 0,
        __INFO,
        __WARNING,
        __ERROR,
    };
public:
    //Log Debug
    template< typename ... Args>
    static void LogD(Args ... args)
    {
        __log(log_level::__DEBUG,std::forward<Args>(args)... );
    }

    //Log Info
    template< typename ... Args>
    static void LogI(Args ... args)
    {
        __log(log_level::__INFO,std::forward<Args>(args)... );
    }

    //Log Warning
    template< typename ... Args>
    static void LogW(Args ... args)
    {
        __log(log_level::__WARNING,std::forward<Args>(args)... );
    }

    //Log Error
    template< typename ... Args>
    static void LogE(Args ... args)
    {
        __log(log_level::__ERROR,std::forward<Args>(args)... );
    }

    //set destination of logs (iostream, ofstream, ostringstream)
    static void setOutput(const std::ostream & p_out)
    {
        std::lock_guard <std::mutex>l_k(m_mtx_logger);
        m_out = const_cast<std::ostream*>(&p_out);
    }

    //reset output to cout
    static void resetOutput()
    {
        std::lock_guard <std::mutex>l_k(m_mtx_logger);
        m_out = &std::cout;
    }

    //get colors status
    static bool printColors(){return m_colors;}
    //set colors on/off (set to off when you are logging to file)
    static void printColors(const bool & p_c)
    {
        std::lock_guard <std::mutex>l_k(m_mtx_logger);
        m_colors = p_c;
    }

private:
    static std::mutex m_mtx_logger;

    static std::ostream * m_out;

    static bool m_colors;

    template<typename ... Args>
    static void __log(const log_level & p_ll,Args ...args)
    {
        std::chrono::system_clock::time_point p = std::chrono::system_clock::now();

        auto ms = std::chrono::duration_cast<milliseconds>(p.time_since_epoch()) % 1000;

        { //mutex scope begin
            std::lock_guard <std::mutex>l_k(m_mtx_logger);
            std::time_t t = std::chrono::system_clock::to_time_t(p);
            std::ostringstream l_stream;
            l_stream << std::boolalpha;
            writeArgsToStream(l_stream,args...);
#ifdef _WIN32
            HANDLE consolehwnd = nullptr;
            if(p_ll != log_level::__DEBUG && m_colors)
                consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
            *m_out
                    << std::put_time(std::localtime(&t), "%F %T")
                    << '.' << std::setfill('0') << std::setw(3) << ms.count() << " "
                    << std::setfill(' ')  << std::setw(8) << std::right;
            if(p_ll == log_level::__DEBUG)
            {
                *m_out<< "[DEBUG]"<< l_stream.str();
            }
            else if(p_ll == log_level::__INFO)
            {
                if(!m_colors)
                    *m_out << "[INFO]"<< l_stream.str();
                else
                {
#ifdef _WIN32
                    SetConsoleTextAttribute(consolehwnd,2); //green
#else
                    *m_out << "\033[0;32m"; //green
#endif
                    *m_out << "[INFO]"<< l_stream.str();
#ifdef _WIN32
                    SetConsoleTextAttribute(consolehwnd,7); //reset color
#else
                    *m_out << "\033[0m"; //reset color
#endif
                }
            }
            else if( p_ll == log_level::__WARNING)
            {
                if(!m_colors)
                    *m_out << "[WARN]"<< l_stream.str();
                else
                {

#ifdef _WIN32
                    SetConsoleTextAttribute(consolehwnd,14); //yellow
#else
                    *m_out << "\033[0;33m"; //yellow
#endif
                    *m_out << "[WARN]"<< l_stream.str();
#ifdef _WIN32
                    SetConsoleTextAttribute(consolehwnd,7); //reset color
#else
                    *m_out << "\033[0m"; //reset color
#endif
                }
            }
            else if( p_ll == log_level::__ERROR)
            {
                if(!m_colors)
                    *m_out << "[ERROR]"<< l_stream.str();
                else
                {
#ifdef _WIN32
                    SetConsoleTextAttribute(consolehwnd,4); //red
#else
                    *m_out << "\033[0;31m"; //red
#endif
                    *m_out << "[ERROR]"<< l_stream.str();
#ifdef _WIN32
                    SetConsoleTextAttribute(consolehwnd,7); //reset color
#else
                    *m_out << "\033[0m"; //reset color
#endif
                }
            }

            *m_out << std::endl;
        } // mutex scope end
    }

    static void writeArgsToStream(std::ostringstream & p_stream)
    {
        return;
    }

    template <typename U, typename... Args >
    static void writeArgsToStream(std::ostringstream & p_stream,U p_first, Args... p_others)
    {
        p_stream << " "<< p_first;
        writeArgsToStream(p_stream,p_others...);
    }
};

std::mutex cppLog::m_mtx_logger;
//output destination init to cout
std::ostream * cppLog::m_out {&std::cout};

bool cppLog::m_colors = true;
