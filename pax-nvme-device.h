#ifndef _PAX_NVME_DEVICE_H
#define _PAX_NVME_DEVICE_H

#include <stddef.h>
#include <switchtec/switchtec.h>
#include <switchtec/mrpc.h>
#include "nvme-device.h"

struct pax_nvme_device {
	struct switchtec_dev *dev;
	uint16_t pdfid;
	int is_blk;
	uint32_t ns_id;
	struct nvme_device device;
};

#define to_pax_nvme_device(d)  \
	((struct pax_nvme_device*) \
	 ((char *)d - offsetof(struct pax_nvme_device, device)))

extern struct nvme_device_ops pax_ops;

#endif
