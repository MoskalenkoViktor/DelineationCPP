#pragma once
#include <vector>
#include "../ecg_lead/ecg_lead.h"


const static double EPSILON = 1e-16;
class ECGLead;



class ModulusMaxima
{
public:
    ModulusMaxima(size_t index, int id, const std::vector<double>& wdc);
    size_t index;
    int id;
    double value;
    bool correctness;
};


std::vector<ModulusMaxima> get_mms(const std::vector<double>& wdc);

ModulusMaxima get_left_mm(const std::vector<ModulusMaxima>& mms,
    const std::vector<int>& ids_mms, size_t index);

ModulusMaxima get_right_mm(const std::vector<ModulusMaxima>& mms,
    const std::vector<int>& ids_mms, size_t index);

std::vector<ModulusMaxima> get_lr_mms_in(const ECGLead& ecg_lead, 
    size_t scale_id, int left, int right);

std::vector<ModulusMaxima> get_rl_mms_in(const ECGLead& ecg_lead, 
    size_t scale_id, int left, int right);

std::vector<int> get_correct_mms_ids(const std::vector<ModulusMaxima>& mms);

std::vector<int> get_incorrect_mms_ids(const std::vector<ModulusMaxima>& mms);

std::vector<ModulusMaxima> get_closest_mm_id(const std::vector<double>& wdc);

size_t get_closest_mm_id_left(const std::vector<ModulusMaxima>& mms,
    const std::vector<int>& ids_mms, size_t index);

size_t get_closest_mm_id_right(const std::vector<ModulusMaxima>& mms,
    const std::vector<int>& ids_mms, size_t index);

