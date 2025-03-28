#ifndef IMUHAL_ACCL_H
#define IMUHAL_ACCL_H

#include "imuhal_bus.h"

#define IMUHAL_ACCL_MEAS_DIM (3U)

/**
 * forward declaration
 */

typedef struct _imuhal_accl imuhal_accl_t;

/**
 * definition
 */

typedef uint16_t imuhal_accl_rawData_t;


/**
 * fullScale API group
 */
#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE
typedef enum _imuhal_accl_fullScaleAbs {

} imuhal_accl_fullScaleAbs_t;
#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE

// mandatory api, set relative
typedef imuhal_relConfig_t (*imuhal_accl_fullScaleGetCount_t)(imuhal_accl_t* _subDev);
typedef mStatus_t (*imuhal_accl_fullScaleSetRel_t)(imuhal_accl_t* _subDev, imuhal_relConfig_t _rel);
typedef mStatus_t (*imuhal_accl_fullScaleGetRel_t)(imuhal_accl_t* _subDev, imuhal_relConfig_t &_rel);

// optional api, set absolute, will increase size as it includes a lut
// can be excluded with kconfig option
#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE
typedef void (*imuhal_accl_fullScaleGetAbsSupportedList_t)(imuhal_accl_t* _subDev, imuhal_accl_fullScaleAbs_t const *list, size_t *count);
typedef mStatus_t (*imuhal_accl_fullScaleSetAbs_t)(imuhal_accl_t* _subDev, imuhal_accl_fullScaleAbs_t _abs);
typedef imuhal_accl_fullScaleAbs_t (*imuhal_accl_fullScaleGetAbs_t)(imuhal_accl_t* _subDev, imuhal_accl_dataRateAbs_t &_abs);
typedef imuhal_real_t (*imuhal_accl_fullScaleConvertSI_t)(imuhal_accl_t* _subDev, imuhal_accl_rawData_t _meas);
#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE

// api struct
typedef struct _imuhal_accl_fullScaleApi {
    imuhal_accl_fullScaleGetCount_t getCount;
    imuhal_accl_fullScaleSetRel_t setRel;
    imuhal_accl_fullScaleGetRel_t getRel;
#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE
    imuhal_accl_fullScaleGetAbsSupportedList_t getAbsList;
    imuhal_accl_fullScaleSetAbs_t setAbs;
    imuhal_accl_fullScaleGetAbs_t getAbs;
    imuhal_accl_fullScaleConvertSI_t convertSI;
#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE
} imuhal_accl_fullScaleApi_t;


/**
 * bandwidth API group
 */
#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE
typedef enum _imuhal_accl_bandwidthAbs {

} imuhal_accl_bandwidthAbs_t;
#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE

// mandatory api, set relative
typedef imuhal_relConfig_t (*imuhal_accl_bandwidthGetCount_t)(imuhal_accl_t* _subDev);
typedef mStatus_t (*imuhal_accl_bandwidthSetRel_t)(imuhal_accl_t* _subDev, imuhal_relConfig_t _rel);
typedef mStatus_t (*imuhal_accl_bandwidthGetRel_t)(imuhal_accl_t* _subDev, imuhal_relConfig_t &_rel);

// api struct
typedef struct _imuhal_accl_bandwidthApi {
    imuhal_accl_bandwidthGetCount_t getCount;
    imuhal_accl_bandwidthSetRel_t setRel;
    imuhal_accl_bandwidthGetRel_t getRel;
} imuhal_accl_bandwidthApi_t;


/**
 * dataRate API group
 */

#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE
typedef enum _imuhal_accl_dataRateAbs {

} imuhal_accl_dataRateAbs_t;
#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE

// mandatory api, set relative
typedef imuhal_relConfig_t (*imuhal_accl_dataRateGetCount_t)(imuhal_accl_t* _subDev);
typedef mStatus_t (*imuhal_accl_dataRateSetRel_t)(imuhal_accl_t* _subDev, imuhal_relConfig_t _rel);
typedef mStatus_t (*imuhal_accl_dataRateGetRel_t)(imuhal_accl_t* _subDev, imuhal_relConfig_t &_rel);

// optional api, set absolute, will increase size as it includes a lut
// can be excluded with kconfig option
#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE
typedef void (*imuhal_accl_dataRateGetAbsSupportedList_t)(imuhal_accl_t* _subDev, imuhal_accl_dataRateAbs_t const *list, size_t *count);
typedef mStatus_t (*imuhal_accl_dataRateSetAbs_t)(imuhal_accl_t* _subDev, imuhal_accl_dataRateAbs_t _abs);
typedef imuhal_accl_dataRateAbs_t (*imuhal_accl_dataRateGetAbs_t)(imuhal_accl_t* _subDev, imuhal_accl_dataRateAbs_t &_abs);
typedef imuhal_real_t (*imuhal_accl_dataRateConvertSI_t)(imuhal_accl_t* _subDev, imuhal_accl_rawData_t _meas);
#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE

// api struct
typedef struct _imuhal_accl_dataRateApi {
    imuhal_accl_dataRateGetCount_t getCount;
    imuhal_accl_dataRateSetRel_t setRel;
    imuhal_accl_dataRateGetRel_t getRel;
#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE
    imuhal_accl_dataRateGetAbsSupportedList_t getAbsList;
    imuhal_accl_dataRateSetAbs_t setAbs;
    imuhal_accl_dataRateGetAbs_t getAbs;
    imuhal_accl_dataRateConvertSI_t convertSI;
#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE
} imuhal_accl_dataRateApi_t;

/**
 * sampleEvent API group
 */

typedef enum _imuhal_accl_sampleEventType {

} imuhal_accl_sampleEventType_t;

typedef uint32_t imuhal_accl_sampleEventFlag_t;

typedef imuhal_accl_sampleEventFlag_t (*imuhal_accl_sampleEventGetAvailable)(imuhal_accl_t* _subDev);
typedef mStatus_t (*imuhal_accl_sampleEventSet)(imuhal_accl_t* _subDev, imuhal_accl_sampleEventFlag_t _flags);
typedef mStatus_t (*imuhal_accl_sampleEventGet)(imuhal_accl_t* _subDev, imuhal_accl_sampleEventFlag_t &_flags);

// api struct
typedef struct _imuhal_accl_sampleEventApi {

} imuhal_accl_sampleEventApi_t;


/**
 * accl core
 */

typedef struct _imuhal_accl_status {
    imuhal_relConfig_t fullScaleRel;
    imuhal_relConfig_t bandwidthRel;
    imuhal_relConfig_t dataRateRel;
    imuhal_accl_sampleEventFlag_t eventEnabled;
} imuhal_accl_status_t;

typedef struct _imuhal_accl_measure {
    union {
        struct {
            imuhal_accl_rawData_t x;
            imuhal_accl_rawData_t y;
            imuhal_accl_rawData_t z;
        };
        imuhal_accl_rawData_t vec3d[IMUHAL_ACCL_MEAS_DIM];
    } data;
    imuhal_accl_rawData_t ts;
} imuhal_accl_measure_t;

typedef struct _imuhal_accl_api {
    imuhal_accl_fullScaleApi_t  fs;
    imuhal_accl_bandwidthApi_t  bw;
    imuhal_accl_dataRateApi_t   dr;
    imuhal_accl_sampleEventApi_t se;
} imuhal_accl_api_t;

struct _imuhal_accl {
    imuhal_accl_status_t status;
    imuhal_accl_measure_t meas;
    imuhal_accl_api_t const *api;
    imuhal_busApi_t const *bus;
    void *priv;
};


mStatus_t IMUHAL_ACCL_FullScaleSetRelMax(imuhal_accl_t* _subDev);

mStatus_t IMUHAL_ACCL_FullScaleSetRelMin(imuhal_accl_t* _subDev);

imuhal_relConfig_t IMUHAL_ACCL_FullScaleGetCount(imuhal_accl_t* _subDev);

mStatus_t IMUHAL_ACCL_FullScaleSetRel(imuhal_accl_t* _subDev, imuhal_relConfig_t _rel);

mStatus_t IMUHAL_ACCL_FullScaleGetRel(imuhal_accl_t* _subDev, imuhal_relConfig_t &_rel);

#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE

mStatus_t IMUHAL_ACCL_FullScaleSetAbs(imuhal_accl_t* _subDev, imuhal_accl_fullScaleAbs_t _abs);

mStatus_t IMUHAL_ACCL_FullScaleGetAbs(imuhal_accl_t* _subDev, imuhal_accl_fullScaleAbs_t &_abs);

void IMUHAL_ACCL_FullScaleGetAbsSupportedList(imuhal_accl_t* _subDev, imuhal_accl_fullScaleAbs_t const *list, size_t *count);

imuhal_real_t IMUHAL_ACCL_FullScaleConvertSI(imuhal_accl_t* _subDev, IMUHAL_ACCL_rawData_t _meas);

#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE

mStatus_t IMUHAL_ACCL_BandWidthSetRelMax(imuhal_accl_t* _subDev);

mStatus_t IMUHAL_ACCL_BandWidthSetRelMin(imuhal_accl_t* _subDev);

imuhal_relConfig_t IMUHAL_ACCL_BandWidthGetCount(imuhal_accl_t* _subDev);

mStatus_t IMUHAL_ACCL_BandWidthSetRel(imuhal_accl_t* _subDev, imuhal_relConfig_t _rel);

mStatus_t IMUHAL_ACCL_BandWidthGetRel(imuhal_accl_t* _subDev, imuhal_relConfig_t &_rel);

mStatus_t IMUHAL_ACCL_DataRateSetRelMax(imuhal_accl_t* _subDev);

mStatus_t IMUHAL_ACCL_DataRateSetRelMin(imuhal_accl_t* _subDev);

imuhal_relConfig_t IMUHAL_ACCL_DataRateGetCount(imuhal_accl_t* _subDev);

mStatus_t IMUHAL_ACCL_DataRateSetRel(imuhal_accl_t* _subDev, imuhal_relConfig_t _rel);

mStatus_t IMUHAL_ACCL_DataRateGetRel(imuhal_accl_t* _subDev, imuhal_relConfig_t &_rel);

#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE

mStatus_t IMUHAL_ACCL_DataRateSetAbs(imuhal_accl_t* _subDev, imuhal_accl_dataRateAbs_t _abs);

mStatus_t IMUHAL_ACCL_DataRateGetAbs(imuhal_accl_t* _subDev, imuhal_accl_fullScaleAbs_t &_abs);

void IMUHAL_ACCL_DataRateGetAbsSupportedList(imuhal_accl_t* _subDev, imuhal_accl_dataRateAbs_t const *list, size_t *count);

imuhal_real_t IMUHAL_ACCL_DataRateConvertSI(imuhal_accl_t* _subDev, IMUHAL_ACCL_rawData_t _meas);

#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE

#endif // IMUHAL_ACCL_H
