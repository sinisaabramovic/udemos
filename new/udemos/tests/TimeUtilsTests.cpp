//
//  TimeUtilsTests.cpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#include <gtest/gtest.h>
#include <thread>
#include "TimeUtils.hpp"

TEST(TimeUtilsTests, DurationMillis) {
    TimeUtils::TimePoint start = TimeUtils::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    TimeUtils::TimePoint end = TimeUtils::now();
    int64_t duration = TimeUtils::durationMillis(start, end);
    EXPECT_GE(duration, 100);
    EXPECT_LT(duration, 200);
}
