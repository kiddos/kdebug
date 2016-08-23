#include <streambuf>
#include <ctime>
#include <iomanip>

#include "debug.hpp"

using std::cout;
using std::cerr;
using std::map;
using std::ostream;
using std::string;
using std::streambuf;
using std::stringbuf;
using std::stringstream;

namespace kdebug{

map<level, const string> levelstring = {
    {null,    "null:    "},
    {file,    "file:    "},
    {info,    "info:    "},
    {warning, "warning: "},
    {error,   "error:   "},
};

timer_t timer(" us");
debug_t debug(" ms");
log_t log(" sec");

}
