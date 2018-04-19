#ifndef _PAX_NVME_HOST_H
#define _PAX_NVME_HOST_H

#include <stddef.h>
#include <switchtec/switchtec.h>
#include <switchtec/mrpc.h>
#include "nvme-host.h"

struct pax_nvme_host {
	struct switchtec_dev *dev;
	uint16_t pdfid;
	int is_blk;
	uint32_t ns_id;
	struct nvme_host host;
};

#define to_pax_nvme_host(d)  \
	((struct pax_nvme_host *) \
	 ((char *)d - offsetof(struct pax_nvme_host, host)))

extern struct nvme_host_ops pax_ops;

#endif
