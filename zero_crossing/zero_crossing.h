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
    ExtremumSign extremum_sign = ExtremumSign::UNKNOWN;

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

    std::vector<ZeroCrossing> get_zcs(const std::vector<double>& wdc,
                                      const std::vector<ModulusMaxima>& mms);


    std::vector<ZeroCrossing> get_zcs_in_window(const std::vector<double>& wdc,
                                                const std::vector<ZeroCrossing>& zcs, const std::vector<int>& ids_zcs,
                                                size_t begin_index, size_t end_index);

    std::vector<ZeroCrossing> get_zcs_with_special_mms(const std::vector<double>& wdc, size_t begin_index, size_t end_index, int window);

    int get_closest_zc_id(const std::vector<ZeroCrossing>& zcs,
                          const std::vector<int>& ids_zcs, size_t index);

// DEPRECATED:
    std::vector<double> wdc_tmp;
    ModulusMaxima right_mm = ModulusMaxima(0,wdc_tmp);
    ModulusMaxima left_mm = ModulusMaxima(0,wdc_tmp);

    void init_global_mm_right(size_t right_border_index, std::vector<double> wdc) {
        //size_t right_mm_index = this->index + np.argmax(np.abs(wdc[self.index:right_border_index]))
        // TODO Check result here:
        size_t right_mm_index = this->index + std::distance(wdc.begin(), std::max_element(wdc.begin(), wdc.begin()+right_border_index));
        this->right_mm = ModulusMaxima(right_mm_index, wdc);
        this->mm_amplitude = abs(this->left_mm.value) + abs(this->right_mm.value);
    }


    void init_global_mm_left(size_t left_border_index, std::vector<double> wdc) {
        // TODO Check result here:
        size_t left_mm_index = left_border_index + std::distance(wdc.end(), std::max_element(wdc.begin(), wdc.begin()+left_border_index));
        this->left_mm = ModulusMaxima(left_mm_index, wdc);
        this->mm_amplitude = abs(this->left_mm.value) + abs(this->right_mm.value);
    }


    void init_local_mm_right(size_t right_border_index, std::vector<double> wdc) {
        ModulusMaxima candidate_mm = find_right_mm(this->index, wdc);
        if (candidate_mm.index < right_border_index)
            this->right_mm = candidate_mm;
        else
            this->right_mm = ModulusMaxima(right_border_index, wdc);

        this->mm_amplitude = abs(this->left_mm.value) + abs(this->right_mm.value);
    }


    void init_local_mm_left(size_t left_border_index, std::vector<double> wdc) {
        ModulusMaxima candidate_mm = find_left_mm(this->index, wdc);
        if (candidate_mm.index > left_border_index)
            this->left_mm = candidate_mm;
        else
            this->left_mm = ModulusMaxima(left_border_index, wdc);

        this->mm_amplitude = abs(this->left_mm.value) + abs(this->right_mm.value);
    }


    void init_special_mm_right(size_t right_border_index, std::vector<double> wdc) {
        ModulusMaxima current_mm = find_right_mm(this->index, wdc);
        if (current_mm.index >= right_border_index - 1) {
            // TODO Check result here:
            size_t right_mm_index = this->index + std::distance(wdc.begin(), std::max_element(wdc.begin(), wdc.begin()+right_border_index));
            this->right_mm = ModulusMaxima(right_mm_index, wdc);
        }
        else {
            ModulusMaxima candidate_mm = current_mm;
            while (current_mm.index < right_border_index - 1) {
                if (abs(current_mm.value) > abs(candidate_mm.value))
                    candidate_mm = current_mm;
                current_mm = find_right_mm(current_mm.index + 1, wdc);
            }
            this->right_mm = candidate_mm;
        }
        this->mm_amplitude = abs(this->left_mm.value) + abs(this->right_mm.value);
    }


    void init_special_mm_left(size_t left_border_index, std::vector<double> wdc) {
        ModulusMaxima current_mm = find_left_mm(this->index, wdc);
        if (current_mm.index <= left_border_index + 1) {
            size_t left_mm_index = left_border_index + std::distance(wdc.end(), std::max_element(wdc.begin(), wdc.begin()+left_border_index));
            this->left_mm = ModulusMaxima(left_mm_index, wdc);
        }
        else {
            ModulusMaxima candidate_mm = current_mm;
            while (current_mm.index > left_border_index + 1) {
                if (abs(current_mm.value) > abs(candidate_mm.value))
                    candidate_mm = current_mm;
                current_mm = find_left_mm(current_mm.index - 1, wdc);
            }
            this->left_mm = candidate_mm;
        }
        this->mm_amplitude = abs(this->left_mm.value) + abs(this->right_mm.value);
    }
};
