#include <utility/metric.hpp>

namespace metrics {

bool CheckDuration(TimeStorage& timestamp, double sec) {
    std::chrono::high_resolution_clock::time_point cur_timestamp = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(cur_timestamp - timestamp);

    if (duration.count() < sec) {
        return true;
    }

    timestamp = std::chrono::high_resolution_clock::now();
    return false;
}

TimeStorage CurTime() {
    return std::chrono::high_resolution_clock::now();
}

};  // namespace metrics
