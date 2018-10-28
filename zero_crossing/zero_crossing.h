#pragma once
#include <vector>
#include "../modulus_maxima/modulus_maxima.h"
#include <memory>


class ModulusMaxima;


enum class ExtremumSign
{
    NEGATIVE = -1,
    POSITIVE = 1,
    UNKNOWN = 0
};


class ZeroCrossing
{
public:
    ZeroCrossing(size_t index, int id, const std::vector<ModulusMaxima>& l_mms, 
        const std::vector<ModulusMaxima>& r_mms);

    size_t index;
    int id;
    ExtremumSign extremum_sign;

    std::vector<ModulusMaxima> l_mms;
    std::vector<ModulusMaxima> r_mms;
    
    std::shared_ptr<ModulusMaxima> g_l_mm;
    std::shared_ptr<ModulusMaxima> g_r_mm;
    double g_ampl = 0;

    std::shared_ptr<ModulusMaxima> l_l_mm;
    std::shared_ptr<ModulusMaxima> l_r_mm;
    double l_ampl = 0;

    std::shared_ptr<ModulusMaxima> s_l_mm;
    std::shared_ptr<ModulusMaxima> s_r_mm;
    double s_ampl = 0;

    double mm_amplitude = 0.0;

    void zc_proc();
};


std::vector<ZeroCrossing> get_zcs(const std::vector<double>& wdc, 
    const std::vector<ModulusMaxima>& mms);

std::vector<ZeroCrossing> get_zcs_in_window(const std::vector<double>& wdc, 
    const std::vector<ZeroCrossing>& zcs, const std::vector<int>& ids_zcs, 
    size_t begin_index, size_t end_index);

std::vector<ZeroCrossing> get_zcs_with_special_mms(const std::vector<double>& wdc, size_t begin_index, size_t end_index, int window);

int get_closest_zc_id(const std::vector<ZeroCrossing>& zcs, 
    const std::vector<int>& ids_zcs, size_t index);
