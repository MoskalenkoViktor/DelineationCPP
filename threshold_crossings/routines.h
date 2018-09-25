#pragma once
#include <vector>
#include <algorithm>
#include "../modulus_maxima/modulus_maxima.h"


size_t find_left_thc_index(const std::vector<double>& wdc, size_t begin_searching_index,
    int end_searching_index, double threshold)
{
    if (begin_searching_index == 0)
    {
        return begin_searching_index;
    }

    size_t current_index = begin_searching_index;
    size_t prev_index = begin_searching_index - 1;
    
    size_t left_border = std::max(0, end_searching_index);

    while (prev_index > left_border)
    {
        double criterion = (wdc[current_index] - threshold) * (wdc[prev_index] - threshold);

        if (criterion < 0)
        {
            break;
        }
        else if (std::abs(wdc[prev_index] - threshold) < EPSILON)
        {
            size_t threshold_period = 0;
            while ((prev_index >= left_border) && (std::abs(wdc[prev_index] - threshold) < EPSILON))
            {
                ++threshold_period;
                --prev_index;
            }
        }
        else
        {
            --current_index;
            --prev_index;
        }
    }

    return prev_index;
}


size_t find_right_thc_index(const std::vector<double>& wdc, size_t begin_searching_index,
    int end_searching_index, double threshold)
{
    if (begin_searching_index == wdc.size() - 1)
    {
        return begin_searching_index;
    }

    size_t current_index = begin_searching_index;
    size_t next_index = begin_searching_index + 1;

    int right_border = std::min(static_cast<int>(wdc.size()) - 1, end_searching_index);

    while (next_index < right_border)
    {
        double criterion = (wdc[current_index] - threshold) * (wdc[next_index] - threshold);

        if (criterion < 0)
        {
            break;
        }
        else if (std::abs(wdc[next_index] - threshold) < EPSILON)
        {
            size_t threshold_period = 0;
            while ((next_index <= right_border) && (std::abs(wdc[next_index] - threshold) < EPSILON))
            {
                ++threshold_period;
                ++next_index;
            }
        }
        else
        {
            ++current_index;
            ++next_index;
        }
    }

    return next_index;
}
