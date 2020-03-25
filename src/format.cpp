#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
    long HH{seconds/3600};
    long H{seconds%3600};
    long MM{H/60};
    long SS{H%60};
    std::string formatted_minutes = MM < 10 ? "0" + std::to_string(MM) : std::to_string(MM);
    std::string formatted_seconds = SS < 10 ? "0" + std::to_string(SS) : std::to_string(SS);
    string time{std::to_string(HH) + ":" + formatted_minutes + ":" + formatted_seconds};
    return time; 
}