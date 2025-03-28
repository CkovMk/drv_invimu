#ifndef IMUHAL_BUS_H
#define IMUHAL_BUS_H

#include "imuhal_common.h"

typedef _imuhal_bus imuhal_bus_t;

typedef enum _imuhal_bus_eventSource {
    imuhal_bus_eventSource_controller = 0,
    imuhal_bus_eventSource_target,
} imuhal_bus_eventSource_t;


typedef mStatus_t (*imuhal_bus_init_t)(imuhal_bus_t *const _bus);

typedef struct _imuhal_busApi {
    bus_init
    bus_deinit
    bus_xfer
    bus_xfer_async
    bus_eventHandler
} imuhal_busApi_t;



struct _imuhal_bus {

};

#endif // IMUHAL_BUS_H
