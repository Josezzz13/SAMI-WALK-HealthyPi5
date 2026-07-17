#include "ecg_hr_algorithm.h"
#include <stdlib.h>

#define ECG_FS_HZ 128
#define HR_MIN_BPM 40
#define HR_MAX_BPM 180

#define REFRACTORY_SAMPLES 70      // 300 ms * 128 Hz
#define THRESHOLD 20000

static int32_t prev_sample = 0;
static int32_t prev_diff = 0;
static uint32_t sample_index = 0;
static uint32_t last_peak_sample = 0;
static uint8_t last_hr = 0;

uint8_t ecg_hr_update(int32_t *ecg_samples, uint8_t n_samples)
{
    for (uint8_t i = 0; i < n_samples; i++)
    {
        sample_index++;

        int32_t sample = ecg_samples[i];
        int32_t diff = sample - prev_sample;

        if (prev_diff > 0 && diff <= 0 && labs(sample) > THRESHOLD)
        {
            uint32_t rr_samples = sample_index - last_peak_sample;

            if (last_peak_sample > 0 &&
                rr_samples > REFRACTORY_SAMPLES &&
                rr_samples < 384)
            {
                uint16_t hr = (60 * ECG_FS_HZ) / rr_samples;

                if (hr >= HR_MIN_BPM && hr <= HR_MAX_BPM)
                {
                    last_hr = (uint8_t)hr;
                }
            }

            last_peak_sample = sample_index;
        }

        prev_sample = sample;
        prev_diff = diff;
    }

    return last_hr;
}
