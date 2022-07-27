//
// Utility/Timer.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Double-precision timer.
//
#pragma once

#include<chrono> 



struct Timer
{
    double total { 0.0 };
    std::chrono::steady_clock::time_point begin {};
   

    void start()
    {
        begin = std::chrono::steady_clock::now();
    }

    void stop()
    {
        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        const std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - begin);
        total += duration.count();
    }
};