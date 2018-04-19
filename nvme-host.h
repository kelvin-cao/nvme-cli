#ifndef _NVME_HOST_H
#define _NVME_HOST_H

#include "nvme-ioctl.h"

struct nvme_host_ops {
	int (*submit_admin_passthru)(int fd, struct nvme_passthru_cmd *cmd);
	int (*is_blk)(void);
};

#define NVME_HOST_TYPE_RC	0
#define NVME_HOST_TYPE_PAX	1

struct nvme_host {
	int type;
	struct nvme_host_ops *ops;
};

extern struct nvme_host *global_host;

extern int is_blk(void);
#endif 
