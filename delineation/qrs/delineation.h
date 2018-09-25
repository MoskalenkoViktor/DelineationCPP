#pragma once
#include "../../ecg_lead/ecg_lead.h"
#include "../wave_delineation.h"

std::vector<WaveDelineation> get_qrs_dels(const ECGLead& ecg_lead, size_t begin_index, size_t end_index);
size_t get_qrs_wdc_scale_id(const ECGLead& ecg_lead);
size_t get_qrs_aux_wdc_scale_id(const ECGLead& ecg_lead);
