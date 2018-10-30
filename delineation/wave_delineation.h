#pragma once
#include "morfology_point.h"
#include <vector>


enum class WaveSpecification
{
    ABSENCE = 0,
    EXIST = 1,
    BIPHASIC = 2,
    FLEXURE = 3,
    EXTRA = 5,
    ATRIAL_FIBRILLATION = 6
};


class WaveDelineation
{
public:
    WaveDelineation(WaveSpecification specification = WaveSpecification::ABSENCE);
    size_t onset_index = 0;
    size_t peak_index = 0;
    size_t offset_index = 0;
    WaveSpecification specification;
};


class Morphology
{
public:
    Morphology(int del_id, const std::vector<Point>& points, Degree degree, 
        const std::vector<int>& branch_id):
        del_id(del_id), points(points), degree(degree), branch_id(branch_id)
    { }
    int del_id;
    std::vector<Point> points;
    Degree degree;
    std::vector<int> branch_id;
};
