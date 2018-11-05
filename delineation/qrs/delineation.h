#pragma once

#include "beta/beta.h"
#include "gamma/gamma.h"

std::pair<std::vector<WaveDelineation>, std::vector<Morphology>> get_qrs_dels(ECGLead& ecg_lead, size_t begin_index, size_t end_index);
size_t get_qrs_wdc_scale_id(const ECGLead& ecg_lead);
size_t get_qrs_aux_wdc_scale_id(const ECGLead& ecg_lead);
