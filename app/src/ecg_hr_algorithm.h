#ifndef ECG_HR_ALGORITHM_H
#define ECG_HR_ALGORITHM_H

#include <stdint.h>

uint8_t ecg_hr_update(int32_t *ecg_samples, uint8_t n_samples);

#endif
