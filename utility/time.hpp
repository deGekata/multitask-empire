#ifndef MULTITASK_EMPIRE_UTILITY_TIME_HPP
#define MULTITASK_EMPIRE_UTILITY_TIME_HPP

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

namespace utility {

using sf::Clock;
using sf::Time;

using sf::seconds;
using sf::milliseconds;
using sf::microseconds;

void Delay(Time delay);

}  // namespace utility

#endif  // MULTITASK_EMPIRE_UTILITY_TIME_HPP
