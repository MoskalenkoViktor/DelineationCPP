#pragma once

#include "../wave_delineation.h"

std::pair<std::vector<WaveDelineation>, std::vector<Morphology>> get_qrs_dels(
    const ECGLead& ecg_lead, size_t begin_index, size_t end_index);
size_t get_qrs_wdc_scale_id(const ECGLead& ecg_lead);
size_t get_qrs_aux_wdc_scale_id(const ECGLead& ecg_lead);
