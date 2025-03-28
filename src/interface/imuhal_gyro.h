#ifndef IMUHAL_GYRO_H
#define IMUHAL_GYRO_H

#include "imuhal_bus.h"

#define IMUHAL_GYRO_MEAS_DIM (3U)

/**
 * forward declaration
 */

typedef struct _imuhal_gyro imuhal_gyro_t;
/**
 * definition
 */

typedef uint16_t imuhal_gyro_rawData_t;


/**
 * fullScale API group
 */
#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE
typedef enum _imuhal_gyro_fullScaleAbs {

} imuhal_gyro_fullScaleAbs_t;
#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE

// mandatory api, set relative
typedef imuhal_relConfig_t (*imuhal_gyro_fullScaleGetCount_t)(imuhal_gyro_t* _subDev);
typedef mStatus_t (*imuhal_gyro_fullScaleSetRel_t)(imuhal_gyro_t* _subDev, imuhal_relConfig_t _rel);
typedef mStatus_t (*imuhal_gyro_fullScaleGetRel_t)(imuhal_gyro_t* _subDev, imuhal_relConfig_t &_rel);

// optional api, set absolute, will increase size as it includes a lut
// can be excluded with kconfig option
#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE
typedef mStatus_t (*imuhal_gyro_fullScaleSetAbs_t)(imuhal_gyro_t* _subDev, imuhal_gyro_fullScaleAbs_t _abs);
typedef mStatus_t (*imuhal_gyro_fullScaleGetAbs_t)(imuhal_gyro_t* _subDev, imuhal_accl_dataRateAbs_t &_abs);
typedef void (*imuhal_gyro_fullScaleGetAbsSupportedList_t)(imuhal_gyro_t* _subDev, imuhal_gyro_fullScaleAbs_t const *list, size_t *count);
typedef imuhal_real_t (*imuhal_gyro_fullScaleConvertSI_t)(imuhal_gyro_t* _subDev, imuhal_gyro_rawData_t _meas);
#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE

// api struct
typedef struct _imuhal_gyro_fullScaleApi {
    imuhal_gyro_fullScaleGetCount_t getCount;
    imuhal_gyro_fullScaleSetRel_t setRel;
    imuhal_gyro_fullScaleGetRel_t getRel;
#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE
    imuhal_gyro_fullScaleSetAbs_t setAbs;
    imuhal_gyro_fullScaleGetAbs_t getAbs;
    imuhal_gyro_fullScaleGetAbsSupportedList_t getAbsList;
    imuhal_gyro_fullScaleConvertSI_t convertSI;
#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE
} imuhal_gyro_fullScaleApi_t;


/**
 * bandwidth API group
 */
#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE
typedef enum _imuhal_gyro_bandwidthAbs {

} imuhal_gyro_bandwidthAbs_t;
#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE

// mandatory api, set relative
typedef imuhal_relConfig_t (*imuhal_gyro_bandwidthGetCount_t)(imuhal_gyro_t* _subDev);
typedef mStatus_t (*imuhal_gyro_bandwidthSetRel_t)(imuhal_gyro_t* _subDev, imuhal_relConfig_t _rel);
typedef mStatus_t (*imuhal_gyro_bandwidthGetRel_t)(imuhal_gyro_t* _subDev, imuhal_relConfig_t &_rel);

// api struct
typedef struct _imuhal_gyro_bandwidthApi {
    imuhal_gyro_bandwidthGetCount_t getCount;
    imuhal_gyro_bandwidthSetRel_t setRel;
    imuhal_gyro_bandwidthGetRel_t getRel;
} imuhal_gyro_bandwidthApi_t;


/**
 * dataRate API group
 */

#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE
typedef enum _imuhal_gyro_dataRateAbs {

} imuhal_gyro_dataRateAbs_t;
#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE

// mandatory api, set relative
typedef imuhal_relConfig_t (*imuhal_gyro_dataRateGetCount_t)(imuhal_gyro_t* _subDev);
typedef mStatus_t (*imuhal_gyro_dataRateSetRel_t)(imuhal_gyro_t* _subDev, imuhal_relConfig_t _rel);
typedef mStatus_t (*imuhal_gyro_dataRateGetRel_t)(imuhal_gyro_t* _subDev, imuhal_relConfig_t &_rel);

// optional api, set absolute, will increase size as it includes a lut
// can be excluded with kconfig option
#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE
typedef mStatus_t (*imuhal_gyro_dataRateSetAbs_t)(imuhal_gyro_t* _subDev, imuhal_gyro_dataRateAbs_t _abs);
typedef mStatus_t (*imuhal_gyro_dataRateGetAbs_t)(imuhal_gyro_t* _subDev, imuhal_accl_dataRateAbs_t &_abs);
typedef void (*imuhal_gyro_dataRateGetAbsSupportedList_t)(imuhal_gyro_t* _subDev, imuhal_gyro_dataRateAbs_t const *list, size_t *count);
typedef imuhal_real_t (*imuhal_gyro_dataRateConvertSI_t)(imuhal_gyro_t* _subDev, imuhal_gyro_rawData_t _meas);
#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE

// api struct
typedef struct _imuhal_gyro_dataRateApi {
    imuhal_gyro_dataRateGetCount_t getCount;
    imuhal_gyro_dataRateSetRel_t setRel;
    imuhal_gyro_dataRateGetRel_t getRel;
#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE
    imuhal_gyro_dataRateSetAbs_t setAbs;
    imuhal_gyro_dataRateGetAbs_t getAbs;
    imuhal_gyro_dataRateGetAbsSupportedList_t getAbsList;
    imuhal_gyro_dataRateConvertSI_t convertSI;
#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE
} imuhal_gyro_dataRateApi_t;

/**
 * sampleEvent API group
 */

// api struct
typedef struct _imuhal_gyro_sampleEventApi {

} imuhal_gyro_sampleEventApi_t;

/**
 * gyro core
 */

typedef struct _imuhal_gyro_status {
    imuhal_gyro_fullScaleRel_t  fs;
    imuhal_gyro_bandwidthRel_t  bw;
    imuhal_gyro_dataRateRel_t   dr;
} imuhal_gyro_status_t;

typedef struct _imuhal_gyro_measure {
    union {
        struct {
            imuhal_gyro_rawData_t x;
            imuhal_gyro_rawData_t y;
            imuhal_gyro_rawData_t z;
        };
        imuhal_gyro_rawData_t vec3d[IMUHAL_GYRO_MEAS_DIM];
    } data;
    imuhal_gyro_rawData_t ts;
} imuhal_gyro_measure_t;

typedef struct _imuhal_gyro_api {
    imuhal_gyro_fullScaleApi_t  fs;
    imuhal_gyro_bandwidthApi_t  bw;
    imuhal_gyro_dataRateApi_t   dr;
    imuhal_gyro_sampleEventApi_t se;
} imuhal_gyro_api_t;

struct _imuhal_gyro {
    imuhal_gyro_status_t status;
    imuhal_gyro_measure_t meas;
    imuhal_gyro_api_t const *api;
    imuhal_busApi_t const *bus;
};


mStatus_t IMUHAL_GYRO_FullScaleSetRelMax(imuhal_gyro_t* _subDev);

mStatus_t IMUHAL_GYRO_FullScaleSetRelMin(imuhal_gyro_t* _subDev);

imuhal_relConfig_t IMUHAL_GYRO_FullScaleGetCount(imuhal_gyro_t* _subDev);

mStatus_t IMUHAL_GYRO_FullScaleSetRel(imuhal_gyro_t* _subDev, imuhal_relConfig_t _rel);

imuhal_relConfig_t IMUHAL_GYRO_FullScaleGetRel(imuhal_gyro_t* _subDev);

#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE

mStatus_t IMUHAL_GYRO_FullScaleSetAbs(imuhal_gyro_t* _subDev, imuhal_gyro_fullScaleAbs_t _abs);

mStatus_t IMUHAL_GYRO_FullScaleGetAbs_t(imuhal_gyro_t* _subDev, imuhal_accl_dataRateAbs_t &_abs);

void IMUHAL_GYRO_FullScaleGetAbsSupportedList(imuhal_gyro_t* _subDev, imuhal_gyro_fullScaleAbs_t const *list, size_t *count);

imuhal_real_t IMUHAL_GYRO_FullScaleConvertSI(imuhal_gyro_t* _subDev, IMUHAL_GYRO_rawData_t _meas);

#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE

mStatus_t IMUHAL_GYRO_BandWidthSetRelMax(imuhal_gyro_t* _subDev);

mStatus_t IMUHAL_GYRO_BandWidthSetRelMin(imuhal_gyro_t* _subDev);

imuhal_relConfig_t IMUHAL_GYRO_BandWidthGetCount(imuhal_gyro_t* _subDev);

mStatus_t IMUHAL_GYRO_BandWidthSetRel(imuhal_gyro_t* _subDev, imuhal_relConfig_t _rel);

imuhal_relConfig_t IMUHAL_GYRO_BandWidthGetRel(imuhal_gyro_t* _subDev);

mStatus_t IMUHAL_GYRO_DataRateSetRelMax(imuhal_gyro_t* _subDev);

mStatus_t IMUHAL_GYRO_DataRateSetRelMin(imuhal_gyro_t* _subDev);

imuhal_relConfig_t IMUHAL_GYRO_DataRateGetCount(imuhal_gyro_t* _subDev);

mStatus_t IMUHAL_GYRO_DataRateSetRel(imuhal_gyro_t* _subDev, imuhal_relConfig_t _rel);

imuhal_relConfig_t IMUHAL_GYRO_DataRateGetRel(imuhal_gyro_t* _subDev);

#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE

mStatus_t IMUHAL_GYRO_DataRateSetAbs(imuhal_gyro_t* _subDev, imuhal_gyro_dataRateAbs_t _abs);

mStatus_t IMUHAL_GYRO_DataRateGetAbs(imuhal_gyro_t* _subDev, imuhal_accl_dataRateAbs_t &_abs);

void IMUHAL_GYRO_DataRateGetAbsSupportedList(imuhal_gyro_t* _subDev, imuhal_gyro_dataRateAbs_t const *list, size_t *count);

imuhal_real_t IMUHAL_GYRO_DataRateConvertSI(imuhal_gyro_t* _subDev, IMUHAL_GYRO_rawData_t _meas);

#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE

#endif // IMUHAL_GYRO_H
