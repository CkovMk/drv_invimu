#include "imuhal_common.h"
#include "imuhal_accl.h"
#include "imuhal_gyro.h"
#include "imuhal_temp.h"
#include "imuhal_mpu6050.h"
#include "imuhal_mpu6050_reg.h"

static imuhal_accl_fullScaleApi_t const imuhal_device_acclFullScaleApi = {
    .getCount = _IMPL_ACCL_FullScaleGetCount;
    .setRel = _IMPL_ACCL_FullScaleSetRel;
    .getRel = _IMPL_ACCL_FullScaleGetRel;
#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE
    .setAbs = _IMPL_ACCL_FullScaleSetAbs;
    .getAbs = _IMPL_ACCL_FullScaleGetAbs;
    .getAbsList = _IMPL_ACCL_FullScaleGetAbsList;
    .convertSI = _IMPL_ACCL_FullScaleConvertSI;
#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE
};

static imuhal_relConfig_t imu_accl_fullScale_count = 7;

#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE
static imuhal_accl_fullScaleAbs_t imuhal_accl_fullScale_abs_supported_list = {

};
#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE

static imuhal_accl_bandwidthApi_t const imuhal_device_acclBandwidthApi = {
    .getCount = _IMPL_ACCL_BandwidthGetCount;
    .setRel = _IMPL_ACCL_BandwidthSetRel;
    .getRel = _IMPL_ACCL_BandwidthGetRel;
};

static imuhal_relConfig_t imu_accl_bandwidth_count = 7;


static imuhal_accl_dataRateApi_t const imuhal_device_acclDataRateApi = {
    .getCount = _IMPL_ACCL_DataRateGetCount;
    .setRel = _IMPL_ACCL_DataRateSetRel;
    .getRel = _IMPL_ACCL_DataRateGetRel;
#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE
    .setAbs = _IMPL_ACCL_DataRateSetAbs;
    .getAbs = _IMPL_ACCL_DataRateGetAbs;
    .getAbsList = _IMPL_ACCL_DataRateGetAbsList;
    .convertSI = _IMPL_ACCL_DataRateConvertSI;
#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE
};

static imuhal_relConfig_t imu_accl_dataRate_count = 7;

#ifdef CONFIG_IMUHAL_SCALE_ABSOLUTE
static imuhal_accl_dataRateAbs_t imuhal_accl_dataRate_abs_supported_list = {

};
#endif // CONFIG_IMUHAL_SCALE_ABSOLUTE

static imuhal_accl_api_t const imuhal_mpu6050_api = {
    .fs = &imuhal_device_acclFullScaleApi,
    .bw = &imuhal_device_acclBandwidthApi,
    .dr = &imuhal_device_acclDataRateApi,
    .se = &imuhal_device_acclSampleEventApi,
};
