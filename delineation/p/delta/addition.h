#pragma once;
#include "../../../zero_crossing/zero_crossing.h"
#include "../../../zero_crossing/routines.h"
#include "../gamma/gamma.h"
#include "../delta/removal.h"
#include "../../../params/params.h"

// What elements?!
void add_complex(leads, corr_mtx, size_t g_id, size_t mean_p_on, size_t mean_p_off) {

    size_t num_leads = leads.size();

    for (size_t lead_id = 0; lead_id < num_leads; lead_id++) {
        // Add only in missed leads
        if (corr_mtx[lead_id][g_id] == -1)
            auto lead = leads[lead_id];

        if (lead.qrs_dels.size() > g_id) {
            WaveDelineation p_del_extra;
            p_del_extra.onset_index = mean_p_on;
            p_del_extra.offset_index = mean_p_off;
            p_del_extra.specification = WaveSpecification.exist;


//Check for allowed borders
//First check offset and qrs border
            size_t qrs_id = g_id;
            while (p_del_extra.offset_index > lead.qrs_dels[qrs_id].offset_index) {
                if (qrs_id + 1 > lead.qrs_dels.size() - 1)
                    break;
                else
                    qrs_id += 1;
            }

            if (p_del_extra.offset_index > lead.qrs_dels[qrs_id].onset_index)
                p_del_extra.offset_index = lead.qrs_dels[qrs_id].onset_index;

// First check onset and t border
            size_t t_id = g_id;
            bool is_t_id_dec = false;
            if (t_id < lead.t_dels.size()) {
                while (p_del_extra.onset_index > lead.t_dels[t_id].onset_index) {
                    is_t_id_dec = true;
                    if (t_id + 1 > lead.t_dels.size() - 1)
                        break;
                    else
                        t_id += 1;
                }
                if (is_t_id_dec)
                    t_id -= 1;
            }

            if (t_id < lead.t_dels.size())
                if (p_del_extra.onset_index < lead.t_dels[t_id].offset_index)
                    p_del_extra.onset_index = lead.t_dels[t_id].offset_index;

            size_t p_on = p_del_extra.onset_index;
            size_t p_off = p_del_extra.offset_index;

// Search ZCSs in averaged interval
            ZeroCrossing p_del_extra_zcs = get_zcs_with_global_mms(lead.wdc[int(WDC_SCALE_ID)], p_del_extra.onset_index, p_del_extra.offset_index);

// If ZCSs exist in averaged interval
            if (p_del_extra_zcs) {

// Search ZCS with maximum mm_amplitude
                ZeroCrossing p_del_extra_zc = p_del_extra_zcs[0];
                for (size_t zc_id = 1; zc_id<p_del_extra_zcs.size(); zc_id++) {
                    if (p_del_extra_zcs[zc_id].mm_amplitude > p_del_extra_zc.mm_amplitude)
                        p_del_extra_zc = p_del_extra_zcs[zc_id];
                }

                p_del_extra.peak_index = p_del_extra_zc.index;
                size_t p_del_id = 0;

                if (!lead.p_dels)
                    lead.p_dels.append(p_del_extra);
                else
                    if (p_del_extra.peak_index < lead.p_dels[0].peak_index)
                        lead.p_dels.insert(p_del_id, p_del_extra);
                else
                    if (p_del_extra.peak_index > lead.p_dels[-1].peak_index)
                        p_del_id = lead.p_dels.size();
                        lead.p_dels.push_back(p_del_extra);
                else
                    for (size_t del_id = 1; del_id < lead.p_dels.size(); del_id++)
                        if (lead.p_dels[del_id - 1].peak_index < p_del_extra.peak_index < lead.p_dels[del_id].peak_index) {
                            p_del_id = del_id
                            lead.p_dels.insert(p_del_id, p_del_extra)
                        }

// Init morphology for new P
                Morphology morphology = get_p_morph(lead, p_del_id, lead.p_dels[p_del_id]);
// We should not change del_id in morphology of new P
                morphology.del_id = -1;
// Add morphology with mock del_id
                lead.p_morphs.insert(p_del_id, morphology);

                p_del_extra.onset_index = p_on;
                p_del_extra.offset_index = p_off;
            }
        }
    }
}