﻿#include "drv_imu_inv_icm20948.h"
#include "__drv_imu_syslog.h"
#if defined(INV_ICM20948_ENABLE) && (INV_ICM20948_ENABLE > 0U)


#define magUnit  0.15f//固定量程4900uT 0.15µT/LSB

#define MPU9250_I2C_SLV4_EN  0x80
#define MPU9250_I2C_SLV4_DONE  0x40
#define MPU9250_I2C_SLV4_NACK  0x10
#define ICM20948_AK09916_I2C_ADDR  0x0C
#define MPU9250_AK8963_DATA_READY  (0x01)
#define MPU9250_AK8963_DATA_OVERRUN  (0x02)
// const unsigned int MPU9250_AK8963_OVERFLOW = (0x80);
#define MPU9250_AK8963_OVERFLOW  (0x08)
static const uint16_t sSelfTestEquation[256] = {
        2620, 2646, 2672, 2699, 2726, 2753, 2781, 2808,
        2837, 2865, 2894, 2923, 2952, 2981, 3011, 3041,
        3072, 3102, 3133, 3165, 3196, 3228, 3261, 3293,
        3326, 3359, 3393, 3427, 3461, 3496, 3531, 3566,
        3602, 3638, 3674, 3711, 3748, 3786, 3823, 3862,
        3900, 3939, 3979, 4019, 4059, 4099, 4140, 4182,
        4224, 4266, 4308, 4352, 4395, 4439, 4483, 4528,
        4574, 4619, 4665, 4712, 4759, 4807, 4855, 4903,
        4953, 5002, 5052, 5103, 5154, 5205, 5257, 5310,
        5363, 5417, 5471, 5525, 5581, 5636, 5693, 5750,
        5807, 5865, 5924, 5983, 6043, 6104, 6165, 6226,
        6289, 6351, 6415, 6479, 6544, 6609, 6675, 6742,
        6810, 6878, 6946, 7016, 7086, 7157, 7229, 7301,
        7374, 7448, 7522, 7597, 7673, 7750, 7828, 7906,
        7985, 8065, 8145, 8227, 8309, 8392, 8476, 8561,
        8647, 8733, 8820, 8909, 8998, 9088, 9178, 9270,
        9363, 9457, 9551, 9647, 9743, 9841, 9939, 10038,
        10139, 10240, 10343, 10446, 10550, 10656, 10763, 10870,
        10979, 11089, 11200, 11312, 11425, 11539, 11654, 11771,
        11889, 12008, 12128, 12249, 12371, 12495, 12620, 12746,
        12874, 13002, 13132, 13264, 13396, 13530, 13666, 13802,
        13940, 14080, 14221, 14363, 14506, 14652, 14798, 14946,
        15096, 15247, 15399, 15553, 15709, 15866, 16024, 16184,
        16346, 16510, 16675, 16842, 17010, 17180, 17352, 17526,
        17701, 17878, 18057, 18237, 18420, 18604, 18790, 18978,
        19167, 19359, 19553, 19748, 19946, 20145, 20347, 20550,
        20756, 20963, 21173, 21385, 21598, 21814, 22033, 22253,
        22475, 22700, 22927, 23156, 23388, 23622, 23858, 24097,
        24338, 24581, 24827, 25075, 25326, 25579, 25835, 26093,
        26354, 26618, 26884, 27153, 27424, 27699, 27976, 28255,
        28538, 28823, 29112, 29403, 29697, 29994, 30294, 30597,
        30903, 31212, 31524, 31839, 32157, 32479, 32804
};


const inv_imu_vector_table_t icm20948_VectorTable =
        {
                .Init = (void *) ICM20948_Init,
                .Detect =(void *) ICM20948_Detect,
                .SelfTest =(void *) ICM20948_SelfTest,
                .Report =(void *) ICM20948_Report,
                .DataReady =(void *) ICM20948_DataReady,
                .EnableDataReadyInt =(void *) ICM20948_EnableDataReadyInt,
                .SoftReset =(void *) ICM20948_SoftReset,
                .ReadSensorBlocking =(void *) ICM20948_ReadSensorBlocking,
                .ReadSensorNonBlocking =(void *) ICM20948_ReadSensorNonBlocking,
                .Convert =(void *) ICM20948_Convert,
                .ConvertRaw =(void *) ICM20948_ConvertRaw,
                .ConvertTemp =(void *) ICM20948_ConvertTemp,
                .IsOpen =(void *) _IMU_IsOpen,
                .Destruct = (void *) ICM20948_Destruct,
                .Dump = (void *) ICM20948_Dump
        };


inv_icm20948_handle_t ICM20948_ConstructI2C(inv_i2c_t _i2c, uint8_t _addr) {
    inv_icm20948_handle_t rtv = (void *) INV_MALLOC(sizeof(inv_icm20948_t));
    inv_imu_handle_t p2parent = _IMU_ConstructI2C(_i2c, _addr);
    memset(rtv, 0, sizeof(inv_icm20948_t));
    rtv->parents = *p2parent;
    _IMU_Destruct(p2parent);

    rtv->parents.vtable = &icm20948_VectorTable;
    rtv->buf = rtv->rxbuf + 1;
    rtv->bank = INT32_MAX;//确保一开始就切换bank
    return rtv;
}
inv_icm20948_handle_t ICM20948_ConstructSPI(inv_spi_t _spi) {
    inv_icm20948_handle_t rtv = (void *) INV_MALLOC(sizeof(inv_icm20948_t));
    inv_imu_handle_t p2parent = _IMU_ConstructSPI(_spi);
    memset(rtv, 0, sizeof(inv_icm20948_t));
    rtv->parents = *p2parent;
    _IMU_Destruct(p2parent);

    rtv->parents.vtable = &icm20948_VectorTable;
    rtv->buf = rtv->rxbuf + 1;
    rtv->bank = INT32_MAX;//确保一开始就切换bank
    return rtv;
}
int ICM20948_Init(inv_icm20948_handle_t _this, inv_imu_config_t _cfg) {
    assert(_this);
    assert(_this->parents.vtable);
    _this->parents.cfg = _cfg;
    _this->parents.isOpen = false;
    int res = 0;
    int bw;
    int fs;
    float unit;
    float unit_from;

    if (!IMU_Detect((inv_imu_handle_t) _this)) { return -1; }
    //软复位
    res |= IMU_SoftReset((inv_imu_handle_t) _this);

    //打开所有传感器
    res |= ICM20948_WriteRegVerified(_this, (uint16_t) ICM20948_PWR_MGMT_2, 0);

    //1.125khz采样率
    res |= ICM20948_WriteRegVerified(_this, (uint16_t) ICM20948_GYRO_SMPLRT_DIV, 0);
    res |= ICM20948_WriteRegVerified(_this, (uint16_t) ICM20948_ACCEL_SMPLRT_DIV_1, 0);
    res |= ICM20948_WriteRegVerified(_this, (uint16_t) ICM20948_ACCEL_SMPLRT_DIV_2, 0);

    //配置陀螺仪lpf
    _InvGetMapVal(ICM20948_GBW_MAP, _this->parents.cfg.gyroBandwidth, bw);
    res |= ICM20948_WriteRegVerified(_this, (uint16_t) ICM20948_GYRO_CONFIG_1, 1 | (bw << 3));

    //配置陀螺仪量程和单位
    _InvGetMapVal(mpu_gyro_unit_dps_map, _this->parents.cfg.gyroFullScale, unit);
    _InvGetMapVal(mpu_gyro_unit_from_dps_map, _this->parents.cfg.gyroUnit, unit_from);
    _this->gyroUnit = unit * unit_from;

    _InvGetMapVal(mpu_gyro_fs_map, _this->parents.cfg.gyroFullScale, fs);
    res |= ICM20948_ModifyReg(_this, (uint16_t) ICM20948_GYRO_CONFIG_1, fs << 1, 3 << 1);

    //配置加速度计lpf
    _InvGetMapVal(ICM20948_ABW_MAP, _this->parents.cfg.accelBandwidth, bw);
    res |= ICM20948_WriteRegVerified(_this, (uint16_t) ICM20948_ACCEL_CONFIG, 1 | (bw << 3));

    //配置加速度计量程和单位
    _InvGetMapVal(mpu_accel_unit_G_map, _this->parents.cfg.accelFullScale, unit);
    _InvGetMapVal(mpu_accel_unit_from_G_map, _this->parents.cfg.accelUnit, unit_from);
    _this->accelUnit = unit * unit_from;

    _InvGetMapVal(mpu_accel_fs_map, _this->parents.cfg.accelFullScale, fs);
    res |= ICM20948_ModifyReg(_this, (uint16_t) ICM20948_ACCEL_CONFIG, fs << 1, 3 << 1);

    //设置温度输出lpf
    res |= ICM20948_WriteRegVerified(_this, (uint16_t) ICM20948_TEMP_CONFIG, 6); //8hz

    //开启数据更新中断
    res |= IMU_EnableDataReadyInt((inv_imu_handle_t) _this);

    if (res != 0) { return res; }
    _this->parents.isOpen = false;

    //设置内部i2c
    res |= ICM20948_WriteRegVerified(_this, (uint16_t) ICM20948_I2C_MST_CTRL, 1 << 4 | 12);//471khz，连续读模式
    res |= ICM20948_WriteRegVerified(_this, (uint16_t) ICM20948_USER_CTRL, 1 << 5);//开启i2c主模式

    //开始设置ak8963
    res |= ICM20948_SubI2cRead(_this, ICM20948_AK09916_I2C_ADDR, (uint8_t) AK09916_WIA2, &_this->ak09916DeviceId, 1);
    if (res != 0) { return res; }

    //复位
    uint8_t val = 0x01;
    res |= ICM20948_SubI2cWrite(_this, ICM20948_AK09916_I2C_ADDR, (uint8_t) AK09916_CNTL3, &val, 1);
    val = 0;
    res |= ICM20948_SubI2cWrite(_this, ICM20948_AK09916_I2C_ADDR, (uint8_t) AK09916_CNTL2, &val, 1);

    //设置连续读ak8963到fifo
    val = 0x5D;
    res |= ICM20948_WriteRegVerified(_this, (uint16_t) ICM20948_I2C_MST_CTRL, val);

    val = ICM20948_AK09916_I2C_ADDR | 0x80;
    res |= ICM20948_WriteRegVerified(_this, (uint16_t) ICM20948_I2C_SLV0_ADDR, val);

    val = (uint8_t) AK09916_ST1;
    res |= ICM20948_WriteRegVerified(_this, (uint16_t) ICM20948_I2C_SLV0_REG, val);


    //唤醒ak09916
    val = (1 << 3);
    res |= ICM20948_SubI2cWrite(_this, ICM20948_AK09916_I2C_ADDR, (uint8_t) AK09916_CNTL2, &val, 1);

    val = 0x89;
    res |= ICM20948_WriteRegVerified(_this, (uint16_t) ICM20948_I2C_SLV0_CTRL, val);

    val = 0x09;
    res |= ICM20948_WriteRegVerified(_this, (uint16_t) ICM20948_I2C_SLV4_CTRL, val);

    val = 0x81;
    res |= ICM20948_WriteRegVerified(_this, (uint16_t) ICM20948_I2C_MST_DELAY_CTRL, val);


    if (res == 0) {
        _this->parents.isOpen = true;
        SYSLOG_D("Init with the config↓👇，%s is ready!", IMU_Report((inv_imu_handle_t) _this));
        SYSLOG_D(IMU_ConfigFormat2String(_this->parents.cfg));
    }
    return res;
}
bool ICM20948_Detect(inv_icm20948_handle_t _this) {
    uint8_t val = 0;
    if (_this->parents.addrAutoDetect) { _this->parents.i2cTransfer.slaveAddress = 0x68; };
    ICM20948_SwitchBank(_this, 0);
    ICM20948_ReadReg(_this, (uint16_t) ICM20948_WHO_AM_I, &val);
    if (0xEA == val) {
        return true;
    }
    val = 0;
    if (_this->parents.addrAutoDetect) { _this->parents.i2cTransfer.slaveAddress = 0x69; };
    ICM20948_SwitchBank(_this, 0);
    ICM20948_ReadReg(_this, (uint16_t) ICM20948_WHO_AM_I, &val);
    if (0xEA == val) {
        return true;
    }
    return false;
}
int ICM20948_SelfTest(inv_icm20948_handle_t _this) {
    if (!IMU_IsOpen((inv_imu_handle_t) _this)) { return -1; }
    int res = 0;
    inv_imu_config_t backup_cfg = _this->parents.cfg;
    inv_imu_config_t st_cfg = IMU_ConfigDefault();
    st_cfg.gyroFullScale = MPU_FS_250dps;
    st_cfg.accelFullScale = MPU_FS_2G;
    st_cfg.accelBandwidth = MPU_ABW_99;
    st_cfg.gyroBandwidth = MPU_GBW_92;
    if (0 != IMU_Init((inv_imu_handle_t) _this, st_cfg)) {
        IMU_Init((inv_imu_handle_t) _this, backup_cfg);
        return -1;
    }
    int32_t gyro_bias_st[3], gyro_bias_regular[3];
    int32_t accel_bias_st[3], accel_bias_regular[3];
    int16_t abuf[9];
    int16_t *gbuf = &abuf[3];
    int accel_result = 0;
    int gyro_result = 0;
//        uint8_t val;
    memset(gyro_bias_st, 0, sizeof(gyro_bias_st));
    memset(gyro_bias_regular, 0, sizeof(gyro_bias_regular));
    memset(accel_bias_st, 0, sizeof(accel_bias_st));
    memset(accel_bias_regular, 0, sizeof(accel_bias_regular));

    int times;
    times = 50;
    while (times--) { while (!IMU_DataReady((inv_imu_handle_t) _this)) {}}//丢弃前20个数据
    times = 50;
    while (times--) {
        while (!IMU_DataReady((inv_imu_handle_t) _this)) {}
        res |= IMU_ReadSensorBlocking((inv_imu_handle_t) _this);
        IMU_ConvertRaw((inv_imu_handle_t) _this, abuf);
        for (int i = 0; i < 3; ++i) {
            gyro_bias_regular[i] += gbuf[i];
            accel_bias_regular[i] += abuf[i];
        }
    }

    //打开自检输出
    res |= ICM20948_ModifyReg(_this, (uint16_t) ICM20948_GYRO_CONFIG_2, 0x7/*0b111*/ << 3, 0x7/*0b111*/ << 3);
    res |= ICM20948_ModifyReg(_this, (uint16_t) ICM20948_ACCEL_CONFIG_2, 0x7/*0b111*/ << 3, 0x7/*0b111*/ << 3);

    times = 50;
    while (times--) { while (!IMU_DataReady((inv_imu_handle_t) _this)) {}}//丢弃前20个数据
    times = 50;
    while (times--) {
        while (!IMU_DataReady((inv_imu_handle_t) _this)) {}
        res |= IMU_ReadSensorBlocking((inv_imu_handle_t) _this);
        IMU_ConvertRaw((inv_imu_handle_t) _this, abuf);
        for (int i = 0; i < 3; ++i) {
            gyro_bias_st[i] += gbuf[i];
            accel_bias_st[i] += abuf[i];
        }
    }

    for (int i = 0; i < 3; ++i) {
        gyro_bias_regular[i] /= 50;
        accel_bias_regular[i] /= 50;
        gyro_bias_st[i] /= 50;
        accel_bias_st[i] /= 50;
    }


    //计算加速度计自检结果
    uint8_t regs[3];
    int otp_value_zero = 0;
    int st_shift_prod[3], st_shift_cust[3], i;
    res |= ICM20948_ReadReg(_this, (uint16_t) ICM20948_SELF_TEST_X_ACCEL, regs);
    res |= ICM20948_ReadReg(_this, (uint16_t) ICM20948_SELF_TEST_Y_ACCEL, regs + 1);
    res |= ICM20948_ReadReg(_this, (uint16_t) ICM20948_SELF_TEST_Z_ACCEL, regs + 2);
    for (i = 0; i < 3; i++) {
        if (regs[i] != 0) {
            st_shift_prod[i] = sSelfTestEquation[regs[i] - 1];
        } else {
            st_shift_prod[i] = 0;
            otp_value_zero = 1;
        }
    }

    if (!otp_value_zero) {
        for (i = 0; i < 3; i++) {
            st_shift_cust[i] = accel_bias_st[i] - accel_bias_regular[i];
            if (st_shift_cust[i] < (st_shift_prod[i] >> 1)
                || st_shift_cust[i] > ((st_shift_prod[i] >> 1) + st_shift_prod[i])) {
                //加速度计自检未通过
                accel_result = 1;
                SYSLOG_W("accel[%d] st fail,result = %f,ref:0.5<result<1.5", i, (float) st_shift_cust[i] / st_shift_prod[i]);
            } else {
                SYSLOG_D("accel[%d] st result = %f,ref:0.5<result<1.5", i, (float) st_shift_cust[i] / st_shift_prod[i]);
            }
        }
    } else {
        accel_result = 1;
        SYSLOG_W("accel[%d] st fail,otp_value=0", i);
    }

    //计算陀螺仪自检结果
    res |= ICM20948_ReadReg(_this, (uint16_t) ICM20948_SELF_TEST_X_GYRO, regs);
    res |= ICM20948_ReadReg(_this, (uint16_t) ICM20948_SELF_TEST_Y_GYRO, regs + 1);
    res |= ICM20948_ReadReg(_this, (uint16_t) ICM20948_SELF_TEST_Z_GYRO, regs + 2);
    for (i = 0; i < 3; i++) {
        if (regs[i] != 0) {
            st_shift_prod[i] = sSelfTestEquation[regs[i] - 1];
        } else {
            st_shift_prod[i] = 0;
            otp_value_zero = 1;
        }
    }


    if (!otp_value_zero) {
        for (i = 0; i < 3; i++) {
            st_shift_cust[i] = gyro_bias_st[i] - gyro_bias_regular[i];
            if (st_shift_cust[i] < (st_shift_prod[i] >> 1)) {
                //陀螺仪自检未通过
                gyro_result = 1;
                SYSLOG_W("gyro[%d] st fail,result = %f,ref:0.5<result<1.5", i, (float) st_shift_cust[i] / st_shift_prod[i]);
            } else {
                SYSLOG_D("gyro[%d] st result = %f,ref:0.5<result<1.5", i, (float) st_shift_cust[i] / st_shift_prod[i]);
            }
        }
    } else {
        gyro_result = 1;
        SYSLOG_W("gyro[%d] st fail,otp_value=0", i);
    }

    //恢复原来的配置
    res |= IMU_Init((inv_imu_handle_t) _this, backup_cfg);
    return res | (gyro_result << 1) | accel_result;
}
bool ICM20948_DataReady(inv_icm20948_handle_t _this) {
    uint8_t val = 0;
    ICM20948_ReadReg(_this, (uint16_t) ICM20948_INT_STATUS_1, &val);
    return (val & 0x01) == 0x01;
}
int ICM20948_EnableDataReadyInt(inv_icm20948_handle_t _this) {
    return ICM20948_ModifyReg(_this, (uint16_t) ICM20948_INT_ENABLE_1, 0x01, 0x01);
}
int ICM20948_SoftReset(inv_icm20948_handle_t _this) {
    if (!IMU_Detect((inv_imu_handle_t) _this)) { return -1; }
    int res = 0;
    int times;
    uint8_t val;
    //先选择bank0
    ICM20948_SwitchBank(_this, 0);

    //复位
    res |= IMU_WriteReg((inv_imu_handle_t) _this, (uint8_t) ICM20948_PWR_MGMT_1, 0x80);
    //等待复位成功
    times = 100;//尝试100次
    do {
        INV_DELAY(5);
        res |= IMU_ReadReg((inv_imu_handle_t) _this, (uint8_t) ICM20948_PWR_MGMT_1, &val);
    } while (val != 0x41 && res == 0 && --times);
    if (times == 0) {
        SYSLOG_E("Time out!! 0x%x at PWR_MGMT_1,when waiting it get 0x41", val);
        return -1;
    }
    //唤起睡眠
    res |= IMU_WriteReg((inv_imu_handle_t) _this, (uint8_t) ICM20948_PWR_MGMT_1, 0x1);
    //等待唤起
    times = 100;//尝试100次
    do {
        INV_DELAY(5);
        res |= IMU_ReadReg((inv_imu_handle_t) _this, (uint8_t) ICM20948_PWR_MGMT_1, &val);
    } while (val != 0x1 && res == 0 && --times);
    if (times == 0) {
        SYSLOG_E("Time out!! 0x%x at PWR_MGMT_1,when waiting it get 0x1", val);
        return -1;
    }
    return res;
}
int ICM20948_ReadSensorBlocking(inv_icm20948_handle_t _this) {
    if (_this->bank != 0) {
        ICM20948_SwitchBank(_this, 0);
    }
    int res;
    if (!_this->parents.isSPI) {
        _this->parents.i2cTransfer.subAddress = (uint8_t) ICM20948_ACCEL_XOUT_H;
        _this->parents.i2cTransfer.data = _this->buf;
        _this->parents.i2cTransfer.dataSize = 23;
        _this->parents.i2cTransfer.direction = inv_i2c_direction_Read;
        res = _this->parents.i2c.masterTransferBlocking(&_this->parents.i2cTransfer);
        if (res != 0) {
            SYSLOG_E("i2c read return code = %d", res);
        }
    } else {
        _this->txbuf[0] = (1U << 7U) | ((uint8_t) ICM20948_ACCEL_XOUT_H & 0x7fU);
        _this->parents.spiTransfer.dataSize = 24;
        _this->parents.spiTransfer.rxData = _this->rxbuf;
        _this->parents.spiTransfer.txData = _this->txbuf;
        res = _this->parents.spi.masterTransferBlocking(&_this->parents.spiTransfer);
        if (res != 0) {
            SYSLOG_E("spi read return code = %d", res);
        }
    }
    return res;
}
int ICM20948_ReadSensorNonBlocking(inv_icm20948_handle_t _this) {
    if (_this->bank != 0) {
        ICM20948_SwitchBank(_this, 0);
    }
    int res;
    if (!_this->parents.isSPI) {
        _this->parents.i2cTransfer.subAddress = (uint8_t) ICM20948_ACCEL_XOUT_H;
        _this->parents.i2cTransfer.data = _this->buf;
        _this->parents.i2cTransfer.dataSize = 23;
        _this->parents.i2cTransfer.direction = inv_i2c_direction_Read;
        res = _this->parents.i2c.masterTransferNonBlocking(&_this->parents.i2cTransfer);
        if (res != 0) {
            SYSLOG_E("i2c read return code = %d", res);
        }
    } else {
        _this->txbuf[0] = (1U << 7U) | ((uint8_t) ICM20948_ACCEL_XOUT_H & 0x7fU);
        _this->parents.spiTransfer.dataSize = 24;
        _this->parents.spiTransfer.rxData = _this->rxbuf;
        _this->parents.spiTransfer.txData = _this->txbuf;
        res = _this->parents.spi.masterTransferNonBlocking(&_this->parents.spiTransfer);
        if (res != 0) {
            SYSLOG_E("spi read return code = %d", res);
        }
    }
    return res;
}
int ICM20948_Convert(inv_icm20948_handle_t _this, float *array) {
    uint8_t *buf = _this->buf;
    array[0] = _this->accelUnit * ((int16_t) ((buf[0] << 8) | buf[1]));
    array[1] = _this->accelUnit * ((int16_t) ((buf[2] << 8) | buf[3]));
    array[2] = _this->accelUnit * ((int16_t) ((buf[4] << 8) | buf[5]));
    array[3] = _this->gyroUnit * ((int16_t) ((buf[6] << 8) | buf[7]));
    array[4] = _this->gyroUnit * ((int16_t) ((buf[8] << 8) | buf[9]));
    array[5] = _this->gyroUnit * ((int16_t) ((buf[10] << 8) | buf[11]));

    if (!(buf[14 + 0] & MPU9250_AK8963_DATA_READY) || (buf[14 + 0] & MPU9250_AK8963_DATA_OVERRUN)) {
//        if (!(buf[14 + 0] & MPU9250_AK8963_DATA_READY)) {
//            SYSLOG_V("0x%x at buf[14 + 0]", (int) buf[14 + 0]);
        return -1;
    }
    if (buf[14 + 8] & MPU9250_AK8963_OVERFLOW) {
//            SYSLOG_V("0x%x at buf[14 + 7]", (int) buf[14 + 7]);
        return -1;
    }
    array[6] = magUnit * ((int16_t) (buf[14 + 2] << 8) | buf[14 + 1]);
    array[7] = magUnit * ((int16_t) (buf[14 + 4] << 8) | buf[14 + 3]);
    array[8] = magUnit * ((int16_t) (buf[14 + 6] << 8) | buf[14 + 5]);
    return 0;
}
int ICM20948_ConvertRaw(inv_icm20948_handle_t _this, int16_t *raw) {
    uint8_t *buf = _this->buf;
    raw[0] = ((int16_t) ((buf[0] << 8) | buf[1]));
    raw[1] = ((int16_t) ((buf[2] << 8) | buf[3]));
    raw[2] = ((int16_t) ((buf[4] << 8) | buf[5]));
    raw[3] = ((int16_t) ((buf[6] << 8) | buf[7]));
    raw[4] = ((int16_t) ((buf[8] << 8) | buf[9]));
    raw[5] = ((int16_t) ((buf[10] << 8) | buf[11]));

    if (!(buf[14 + 0] & MPU9250_AK8963_DATA_READY) || (buf[14 + 0] & MPU9250_AK8963_DATA_OVERRUN)) {
//        if (!(buf[14 + 0] & MPU9250_AK8963_DATA_READY)) {
//            SYSLOG_V("0x%x at buf[14 + 0]", (int) buf[14 + 0]);
        return -1;
    }
    if (buf[14 + 8] & MPU9250_AK8963_OVERFLOW) {
//            SYSLOG_V("0x%x at buf[14 + 7]", (int) buf[14 + 7]);
        return -1;
    }
    raw[6] = ((int16_t) (buf[14 + 2] << 8) | buf[14 + 1]);
    raw[7] = ((int16_t) (buf[14 + 4] << 8) | buf[14 + 3]);
    raw[8] = ((int16_t) (buf[14 + 6] << 8) | buf[14 + 5]);
    return 0;
}
int ICM20948_ConvertTemp(inv_icm20948_handle_t _this, float *temp) {
    if (temp) { *temp = (float) ((int16_t) ((_this->buf[12] << 8) | _this->buf[13])) / 333.87f + 21; }
    return 0;
}
int ICM20948_SubI2cRead(inv_icm20948_handle_t _this, uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len) {
    uint8_t index = 0;
    uint8_t status = 0;
    uint32_t timeout = 0;
    uint8_t tmp = 0;
    int res = 0;
    tmp = addr | 0x80;
    res |= ICM20948_WriteReg(_this, (uint16_t) ICM20948_I2C_SLV4_ADDR, tmp);
    while (index < len) {
        tmp = reg + index;
        res |= ICM20948_WriteReg(_this, (uint16_t) ICM20948_I2C_SLV4_REG, tmp);
        tmp = MPU9250_I2C_SLV4_EN;
        res |= ICM20948_WriteReg(_this, (uint16_t) ICM20948_I2C_SLV4_CTRL, tmp);
        do {
            if (timeout++ > 5000) {
                SYSLOG_E("SubI2cRead time out");
                return -2;
            }
            res |= ICM20948_ReadReg(_this, (uint16_t) ICM20948_I2C_MST_STATUS, &status);
        } while ((status & MPU9250_I2C_SLV4_DONE) == 0);
        if (status & MPU9250_I2C_SLV4_NACK) {
            SYSLOG_E("SubI2cRead no ack");
            return -3;
        }
        res |= ICM20948_ReadReg(_this, (uint16_t) ICM20948_I2C_SLV4_DI, val + index);
        index++;
    }
    return res;
}
int ICM20948_SubI2cWrite(inv_icm20948_handle_t _this, uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len) {
    uint32_t timeout = 0;
    uint8_t status = 0;
    uint8_t tmp = 0;
    uint8_t index = 0;
    int res = 0;
    tmp = addr;
    res |= ICM20948_WriteReg(_this, (uint16_t) ICM20948_I2C_SLV4_ADDR, tmp);
    while (index < len) {
        tmp = reg + index;
        res |= ICM20948_WriteReg(_this, (uint16_t) ICM20948_I2C_SLV4_REG, tmp);
        tmp = val[index];
        res |= ICM20948_WriteReg(_this, (uint16_t) ICM20948_I2C_SLV4_DO, tmp);
        tmp = MPU9250_I2C_SLV4_EN;
        res |= ICM20948_WriteReg(_this, (uint16_t) ICM20948_I2C_SLV4_CTRL, tmp);
        do {
            if (timeout++ > 5000) {
                SYSLOG_E("SubI2cWrite time out");
                return -2;
            }
            res |= ICM20948_ReadReg(_this, (uint16_t) ICM20948_I2C_MST_STATUS, &status);
        } while ((status & MPU9250_I2C_SLV4_DONE) == 0);
        if (status & MPU9250_I2C_SLV4_NACK) {
            SYSLOG_E("SubI2cWrite no ack");
            return -3;
        }
        index++;
    }
    return res;
}
int ICM20948_WriteReg(inv_icm20948_handle_t _this, uint16_t reg, const uint8_t val) {
    if (_this->bank != reg >> 8) {
        ICM20948_SwitchBank(_this, reg >> 8);
    }
    return IMU_WriteReg((inv_imu_handle_t) _this, reg, val);
}
int ICM20948_WriteRegVerified(inv_icm20948_handle_t _this, uint16_t reg, const uint8_t val) {
    if (_this->bank != reg >> 8) {
        ICM20948_SwitchBank(_this, reg >> 8);
    }
    return IMU_WriteRegVerified((inv_imu_handle_t) _this, reg, val);
}
int ICM20948_ReadReg(inv_icm20948_handle_t _this, uint16_t reg, uint8_t *val) {
    if (_this->bank != reg >> 8) {
        ICM20948_SwitchBank(_this, reg >> 8);
    }
    return IMU_ReadReg((inv_imu_handle_t) _this, reg, val);
}
int ICM20948_ModifyReg(inv_icm20948_handle_t _this, uint16_t reg, const uint8_t val, const uint8_t mask) {
    if (_this->bank != reg >> 8) {
        ICM20948_SwitchBank(_this, reg >> 8);
    }
    return IMU_ModifyReg((inv_imu_handle_t) _this, reg, val, mask);
}
int ICM20948_SwitchBank(inv_icm20948_handle_t _this, int _bank) {
    _this->bank = _bank;
    return IMU_WriteRegVerified((inv_imu_handle_t) _this, (uint8_t) ICM20948_REG_BANK_SEL, _bank << 4);
}
void ICM20948_Destruct(inv_icm20948_handle_t _this) { _IMU_Destruct((void *) _this); }
const char *ICM20948_Report(inv_icm20948_handle_t _this) { return " icm20948"; }
void ICM20948_Dump(inv_icm20948_handle_t _this, int (*_printf_)(const char *, ...)) {
    for (int i = 0; i < 4; ++i) {
        ICM20948_SwitchBank(_this, i);
        _printf_("\r\nBank=%d\r\n", i);
        _IMU_Dump((inv_imu_handle_t) _this, _printf_);
    }
}

//#if defined(__cplusplus) || defined(c_plusplus)
//}
//#endif

#endif //INV_XXX_ENABLE

