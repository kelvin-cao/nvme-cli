#ifndef _RC_NVME_HOST_H
#define _RC_NVME_HOST_H

#include "nvme-host.h"

struct rc_nvme_host {
	struct nvme_host host;
};

extern struct nvme_host_ops rc_ops;

#endif
