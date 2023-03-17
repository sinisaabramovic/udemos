//
//  TimeUtils.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef TimeUtils_hpp
#define TimeUtils_hpp

#include <chrono>

namespace TimeUtils {

using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<Clock>;

TimePoint now();
int64_t durationMillis(const TimePoint& start, const TimePoint& end);

}  // namespace TimeUtils

#endif /* TimeUtils_hpp */
