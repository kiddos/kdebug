#pragma once
#include <sstream>
#include <string>
#include <list>
#include <chrono>
#include <map>
#include <iostream>
#include <fstream>

namespace kdebug {

enum level {
    null,
    file,
    info,
    warning,
    error
};

template <typename Clock>
struct dbg_log {
    dbg_log();
    dbg_log(const dbg_log&);
    dbg_log(dbg_log&&);
    dbg_log(typename Clock::rep, level, std::string);
    typename Clock::rep time;
    level l;
    std::string text;
};

template <typename Clock>
std::ostream& operator<<(std::ostream&, const dbg_log<Clock>&);

template <typename Clock, typename Duration>
class dbg {
public:
    using log_t = dbg_log<Clock>;
    using iterator = typename std::list<log_t>::iterator;

    dbg(std::string unit);
    ~dbg();

    dbg &set_level(level l);
    void set_fileoutput(const std::string filename);
    template<typename T> dbg& operator<<(T t);

    std::string time();
    static std::string current_timestr();
    void log();
    void list(std::ostream&,level l=null);

    iterator begin();
    iterator end();
    log_t back();

private:
    typename Clock::time_point _starttime;
    typename Clock::time_point _lastlogtime;
    typename Clock::rep _time;

    level _level;
    bool _flag_logged;
    std::string unit;
    std::stringstream _ss;
    std::ofstream _output_file;
    std::list<log_t> _log;

    std::ostream& outstream(level);
};

// global variables
using timer_t = dbg<std::chrono::high_resolution_clock, std::chrono::microseconds>;
using debug_t = dbg<std::chrono::system_clock, std::chrono::milliseconds>;
using log_t = dbg<std::chrono::system_clock, std::chrono::seconds>;

extern timer_t timer;
extern debug_t debug;
extern log_t log;

extern std::map<level, const std::string> levelstring;


extern std::ostream nullout;

}


#undef DBG
#undef TIMER
#undef LOG
// macros
#ifdef DEBUG_MESSAGE
#define DBG(level) kdebug::debug.set_level(kdebug::level)
#define TIMER(level) kdebug::timer.set_level(kdeug::level)
#define LOG(level) kdebug::log.set_level(kdebug::level)
#else
#define DBG(level) kdebug::debug.set_level(kdebug::null)
#define TIMER(level) kdebug::timer.set_level(kdebug::null)
#define LOG(level) kdebug::log.set_level(kdebug::null)
#endif

#include "debug_imp.hpp"
