#include <ctime>
#include <iomanip>

#include "debug.hpp"

using std::cout;
using std::cerr;
using std::map;
using std::ostream;
using std::string;
using std::stringstream;

const string s_esc(1,(char)27);

//how can i do this in compile time
string strcolor(string &&s,string &&color){
    return s_esc+"["+color+";1m"+s+s_esc+"[0m";
}

namespace kdebug{

template <typename Clock, typename Duration>
dbg<Clock, Duration>::dbg(std::string unit)
    : _starttime(Clock::now()), _flag_logged(true), _unit(unit) {}

template <typename Clock, typename Duration>
dbg<Clock, Duration>::~dbg() {
    if (_output_file.is_open()) _output_file.close();
}

template <typename Clock, typename Duration>
dbg<Clock, Duration> &dbg<Clock, Duration>::output(level l,
                                                   const string &file_name,
                                                   int line_number) {
    // 1. write the date time if changes
    // 2. write the logging level
    // 3. write timing
    // 4. write filename and line number

    bool update_time = false;
    update_time = (l == info || l == warning || l == error);

    stringstream out;
    out << std::flush << get_current_date_string() << std::flush
        << '[' << get_level_string(l) << " | "
        << get_current_time_string(update_time) << " | "
        << file_name.substr(file_name.find_last_of("/")+1)
        << ":" << line_number << "] ";

    switch (_level) {
        case null:
            return *this;
        case INFO:
        case info:
        case WARNING:
        case warning:
            cout << std::endl << out.str();
            break;
        case ERROR:
        case error:
            cerr << std::endl << out.str();
            break;
    }
    return *this;
}

template <typename Clock, typename Duration>
string dbg<Clock, Duration>::get_current_date_string() {
    stringstream out;

    auto now = Clock::now();
    time_t current_time = Clock::to_time_t(now);
    struct tm *time_info = std::localtime(&current_time);
    char buffer[1024] = {'\0'};
    std::strftime(buffer, 1024, "[%m/%d/%y %A]\n", time_info);

    if (_file_output) {
        if (current_time - _current_file_time > 86400) {
            _current_file_time = current_time;
            out << buffer;
        }
    } else {
        if (current_time - _current_console_time > 86400) {
            _current_console_time = current_time;
            out << buffer;
        }
    }
    return out.str();
}

template <typename Clock, typename Duration>
string dbg<Clock, Duration>::get_level_string(level l) {
    _level = l;

    map<level, const string> levelstring = {
        {null,    "null    "},
        {info,    strcolor("info    ","32")},
        {warning, strcolor("warning ","33")},
        {error,   strcolor("error   ","31")},
        {INFO,    strcolor("info    ","32")},
        {WARNING, strcolor("warning ","33")},
        {ERROR,   strcolor("error   ","31")},
    };

    stringstream ss;
    ss << levelstring[_level];
    return ss.str();
}

template <typename Clock, typename Duration>
void dbg<Clock, Duration>::set_fileoutput(const string filename) {
    if (_output_file.is_open()) {
        _output_file.close();
    }
    _output_file.open(filename.c_str(), std::ios::out);
}

template <typename Clock, typename Duration>
string dbg<Clock, Duration>::get_current_time_string(bool update) {
    const long timepassed = std::chrono::duration_cast<Duration>
        (Clock::now()-_starttime).count();

    if (update) _starttime = Clock::now();

    stringstream out;
    out << timepassed << _unit;
    return out.str();
}

template <typename Clock, typename Duration>
string dbg<Clock, Duration>::current_timestr() {
    time_t current_time;
    std::time(&current_time);
    struct tm *time_info = std::localtime(&current_time);
    char buffer[1024];
    std::strftime(buffer, 1024, "%m/%d/%y %A %T", time_info);
    return std::string(buffer);
}

template class dbg <std::chrono::high_resolution_clock,
                    std::chrono::microseconds>;
template class dbg <std::chrono::system_clock,
                    std::chrono::seconds>;
template class dbg <std::chrono::system_clock,
                    std::chrono::milliseconds>;

time_t _current_console_time = 0;
time_t _current_file_time = 0;
Debug _debug(" ms");
Timer _timer(" us");
Log _log(" sec");

}
