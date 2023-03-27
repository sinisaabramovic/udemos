//
//  TimeUtils.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include "TimeUtils.hpp"

namespace TimeUtils {

TimePoint now() {
    return Clock::now();
}

int64_t durationMillis(const TimePoint& start, const TimePoint& end) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

}  // namespace TimeUtils
