#pragma once


enum class WaveSpecification
{
    ABSENCE = 0,
    EXIST = 1,
    BIPHASIC = 2,
    FLECTURE = 3,
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
