﻿#include "drv_imu_inv_icm20600.h"
#include "__drv_imu_syslog.h"
#if defined(INV_ICM20600_ENABLE) && (INV_ICM20600_ENABLE > 0U)

const inv_imu_vector_table_t icm20600_VectorTable =
        {
                .Init = (void *) ICM20602_Init,
                .Detect =(void *) ICM20600_Detect,
                .SelfTest =(void *) ICM20602_SelfTest,
                .Report =(void *) ICM20600_Report,
                .DataReady =(void *) ICM20602_DataReady,
                .EnableDataReadyInt =(void *) ICM20602_EnableDataReadyInt,
                .SoftReset =(void *) ICM20602_SoftReset,
                .ReadSensorBlocking =(void *) ICM20602_ReadSensorBlocking,
                .ReadSensorNonBlocking =(void *) ICM20602_ReadSensorNonBlocking,
                .Convert =(void *) ICM20602_Convert,
                .ConvertRaw =(void *) ICM20602_ConvertRaw,
                .ConvertTemp =(void *) ICM20602_ConvertTemp,
                .IsOpen =(void *) _IMU_IsOpen,
                .Destruct = (void *) ICM20600_Destruct,
                .Dump = (void *) _IMU_Dump
        };

inv_icm20600_handle_t ICM20600_ConstructI2C(inv_i2c_t _i2c, uint8_t _addr) {
    inv_icm20600_handle_t rtv = (inv_icm20600_handle_t) ICM20602_ConstructI2C(_i2c, _addr);
    rtv->parents.parents.vtable = &icm20600_VectorTable;
    return rtv;
}
inv_icm20600_handle_t ICM20600_ConstructSPI(inv_spi_t _spi) {
    inv_icm20600_handle_t rtv = (inv_icm20600_handle_t) ICM20602_ConstructSPI(_spi);
    rtv->parents.parents.vtable = &icm20600_VectorTable;
    return rtv;
}
bool ICM20600_Detect(inv_icm20602_handle_t _this) {
    uint8_t val = 0;
    if (_this->parents.addrAutoDetect) { _this->parents.i2cTransfer.slaveAddress = 0x68; }
    IMU_ReadReg((inv_imu_handle_t) _this, (uint8_t) ICM20602_WHO_AM_I, &val);
    if (0x11 == val) {
        return true;
    }
    val = 0;
    if (_this->parents.addrAutoDetect) { _this->parents.i2cTransfer.slaveAddress = 0x69; }
    IMU_ReadReg((inv_imu_handle_t) _this, (uint8_t) ICM20602_WHO_AM_I, &val);
    if (0x11 == val) {
        return true;
    }
    return false;
}
const char *ICM20600_Report(inv_icm20602_handle_t _this) { return " icm20600"; }
void ICM20600_Destruct(inv_icm20600_handle_t _this) { ICM20602_Destruct((void *) _this); }

//#if defined(__cplusplus) || defined(c_plusplus)
//}
//#endif

#endif //INV_XXX_ENABLE

