#pragma once

#include "../../ecg_lead/ecg_lead.h"
#include "../wave_delineation.h"

std::pair<std::vector<WaveDelineation>, std::vector<Morphology>> get_p_dels(const ECGLead& ecg_lead);