#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include "nvme-host.h"
#include "nvme-ioctl.h"
#include "pax-nvme-host.h"

#include <switchtec/switchtec.h>
#include <switchtec/mrpc.h>

#if 0
static int host_nvme_submit_admin_passthru(int fd, struct nvme_passthru_cmd *cmd)
{
	return global_host->ops->submit_admin_passthru(fd, cmd);
}
#endif

int is_blk(void)
{
	return global_host->ops->is_blk();
}
