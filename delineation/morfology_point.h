#pragma once


enum class PointName
{
    QRS_ONSET,
    Q,
    R,
    S,
    QRS_OFFSET,

    T_ONSET,
    T_PEAK,
    T_OFFSET,

    P_ONSET,
    P_PEAK,
    P_OFFSET,

    XTD_POINT
};
    


enum class WaveSign
{
    NEGATIVE = -1,
    POSITIVE = 1,
    NONE = 0
};


class Point
{
public:
    Point(PointName name, size_t index, double value, WaveSign sign) :
        name(name), index(index), value(value), sign(sign)
    { }

    PointName name;
    size_t index;
    double value;
    WaveSign sign;
};
