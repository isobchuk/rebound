#include "engine/framework/log/time.hpp"

using namespace isoeng::log;

const Time::TimePoint Time::_start = std::chrono::steady_clock::now();
