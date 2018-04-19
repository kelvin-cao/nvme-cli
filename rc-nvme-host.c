#include <sys/ioctl.h>
#include <sys/stat.h>
#include "linux/nvme_ioctl.h"
#include "nvme-host.h"
#include "nvme-ioctl.h"

#if 0
int rc_nvme_submit_admin_passthru(int fd, struct nvme_passthru_cmd *cmd)
{
	return ioctl(fd, NVME_IOCTL_ADMIN_CMD, cmd);
}
#endif

extern struct stat nvme_stat;

int rc_is_blk(void)
{
	return S_ISBLK(nvme_stat.st_mode);
}

struct nvme_host_ops rc_ops = {
	.submit_admin_passthru = nvme_submit_admin_passthru,
	.is_blk = rc_is_blk,
};


