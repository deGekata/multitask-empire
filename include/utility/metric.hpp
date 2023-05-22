#ifndef METRICS_H
#define METRICS_H

#include <ecs/config.hpp>

#include <chrono>
namespace metrics {

using TimeStorage = std::chrono::high_resolution_clock::time_point;

/**
 * @brief checks if time elapsed since setting timestamp is lesser then sec
 * 
 * @return true, if elapsed time lesser then sec. false, otherwise
 */
bool CheckDuration(TimeStorage& timestamp, double sec);

TimeStorage CurTime();

static constexpr double kNSecInDt                   = 0.04;
static constexpr double kNSecForGapBetweenRenders   = 0.04;
static constexpr double kNSecForGapBetweenFrames    = 0.1;

};

#endif // METRICS_H
