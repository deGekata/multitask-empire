#include <utility/time.hpp>

namespace utility {

void Delay(Time delay) {
	Clock clock;
	while (clock.getElapsedTime() < delay) {}
}

}  // namespace utility
