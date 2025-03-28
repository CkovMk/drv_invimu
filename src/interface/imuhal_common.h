#ifndef IMUHAL_COMMON_H
#define IMUHAL_COMMON_H

// Include imuhal_config.h if not using Kconfig
#ifndef CONFIG_IMUHAL
#include "imuhal_config.h"
#endif // CONFIG_IMUHAL

#include <stdint.h>

typedef int mStatus_t;

#if defined(CONFIG_IMUHAL_SINGLE_PRECISION)
typedef float imuhal_real_t;
#elif defined(CONFIG_IMUHAL_DOUBLE_PRECISION)
typedef double imuhal_real_t;
#else // CONFIG_IMUHAL_DOUBLE_PRECISION
#error "floatpoint precision not selected"
#endif // CONFIG_IMUHAL_DOUBLE_PRECISION

typedef uint8_t imuhal_relConfig_t;

#endif // IMUHAL_COMMON_H
