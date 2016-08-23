#pragma once
#include "debug.hpp"
#include <iostream>
#include <streambuf>
#include <ctime>
#include <iomanip>

namespace kdebug{

template <typename Clock>
dbg_log<Clock>::dbg_log()
{
}

template <typename Clock>
dbg_log<Clock>::dbg_log(typename Clock::rep rep, level l, std::string s)
{
    log = std::move(std::make_tuple(rep,l,s));
}

template <typename Clock>
std::ostream& operator<<(std::ostream& os, const dbg_log<Clock>& l)
{
    os<< "\n["<< std::get<0>(l.log)<< " : "<< std::get<1>(l.log)<<"] "<< std::get<2>(l.log);
    return os;
}

template <typename Clock, typename Duration>
dbg<Clock, Duration>::dbg(std::string unit)
    : _starttime(Clock::now()), _flag_logged(true), unit(unit)
{
}

template <typename Clock, typename Duration>
dbg<Clock, Duration>::~dbg()
{
    std::cout << std::endl;
    _output_file << '\n';
    if (_output_file.is_open()) _output_file.close();
}

template <typename Clock, typename Duration>
dbg<Clock, Duration>& dbg<Clock, Duration>::set_level(level l)
{
    _level = l;
    std::stringbuf buf;
    std::ostream output(&buf);
    output << "\n[" << levelstring[_level]
        << current_timestr()
        << " | " << time() << unit << "]: ";

    switch (_level) {
        case null:
            return *this;
        case file:
            if (_output_file.is_open()) {
                _output_file << buf.str();
            }
            break;
        case info:
        case warning:
            std::cout << buf.str();
            break;
        case error:
            std::cerr << buf.str();
            break;
    }
    return *this;
}

template <typename Clock, typename Duration>
void dbg<Clock, Duration>::set_fileoutput(const std::string filename)
{
    _output_file.open(filename.c_str(), std::ios::out);
}

template <typename Clock, typename Duration>
template<typename T>
dbg<Clock, Duration> &dbg<Clock, Duration>::operator<< (T t)
{
    _ss << t;
    switch (_level) {
        case null:
            return *this;
            break;
        case file:
            if (_output_file.is_open()) {
                _output_file << t;
            }
            break;
        case info:
            break;
        case warning:
            std::cout << t;
            break;
        case error:
            std::cerr << t;
            break;
    }
    return *this;
}

template <typename Clock, typename Duration>
void dbg<Clock, Duration>::log()
{
    std::string s;
    _ss >> s;

    _log.push_back(log_t(_time, _level, s));
    _ss.str(std::string());
    _ss.clear();
    s.clear();
    _flag_logged = true;
}

template <typename Clock, typename Duration>
std::string dbg<Clock, Duration>::time()
{
    const long timepassed = std::chrono::duration_cast<Duration>
        (Clock::now()-_starttime).count();
    //_starttime = Clock::now();

    std::stringstream ss;
    ss << timepassed;
    return ss.str();
}


template <typename Clock, typename Duration>
std::string dbg<Clock, Duration>::current_timestr()
{
    time_t current_time;
    std::time(&current_time);
    struct tm *time_info = std::localtime(&current_time);
    char buffer[1024];
    std::strftime(buffer, 1024, "%m/%d/%y %A %T", time_info);
    return std::string(buffer);
}

template <typename Clock, typename Duration>
void dbg<Clock, Duration>::list()
{
    if(!_flag_logged)
        log();
    for(auto i = _log.begin() ; i!= _log.end() ; ++i) {
        std::cout <<*i<<'\n';
    }
}

template <typename Clock, typename Duration>
typename dbg<Clock, Duration>::iterator dbg<Clock, Duration>::begin()
{
    if(!_flag_logged)
        log();
    return _log.begin();
}

template <typename Clock, typename Duration>
typename dbg<Clock, Duration>::iterator dbg<Clock, Duration>::end()
{
    if(!_flag_logged)
        log();
    return _log.end();
}

template <typename Clock, typename Duration>
typename dbg<Clock, Duration>::log_t dbg<Clock, Duration>::back()
{
    if(!_flag_logged)
        log();
    if (_log.size() > 0) {
        return _log.back();
    } else {
        std::string empt = "empty";
        return log_t(_time, null, empt);
    }
}


}
