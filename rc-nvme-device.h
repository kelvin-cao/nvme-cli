#ifndef _RC_NVME_DEVICE_H
#define _RC_NVME_DEVICE_H

#include "nvme-device.h"

struct rc_nvme_device {
	struct nvme_device device;
};

extern struct nvme_device_ops rc_ops;

#endif
