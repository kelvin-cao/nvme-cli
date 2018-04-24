#include <sys/ioctl.h>
#include <sys/stat.h>
#include "linux/nvme_ioctl.h"
#include "nvme-device.h"
#include "nvme-ioctl.h"

extern struct stat nvme_stat;

int rc_nvme_submit_admin_passthru(int fd, struct nvme_passthru_cmd *cmd)
{
	return ioctl(fd, NVME_IOCTL_ADMIN_CMD, cmd);
}

int rc_nvme_io(int fd, struct nvme_user_io *io)
{
	return ioctl(fd, NVME_IOCTL_SUBMIT_IO, &io);
}

int rc_is_blk(void)
{
	return S_ISBLK(nvme_stat.st_mode);
}

int rc_nvme_get_nsid(int fd)
{
	return ioctl(fd, NVME_IOCTL_ID);
}

int rc_nvme_subsystem_reset(int fd)
{
	return ioctl(fd, NVME_IOCTL_SUBSYS_RESET);
}

int rc_nvme_reset_controller(int fd)
{
	return ioctl(fd, NVME_IOCTL_RESET);
}

int rc_nvme_ns_rescan(int fd)
{
	return ioctl(fd, NVME_IOCTL_RESCAN);
}

int rc_nvme_submit_passthru(int fd, int ioctl_cmd, struct nvme_passthru_cmd *cmd)
{
	return ioctl(fd, ioctl_cmd, cmd);
}

int rc_nvme_submit_io_passthru(int fd, struct nvme_passthru_cmd *cmd)
{
	return ioctl(fd, NVME_IOCTL_IO_CMD, cmd);
}

struct nvme_device_ops rc_ops = {
	.nvme_submit_admin_passthru = rc_nvme_submit_admin_passthru,
	.nvme_get_nsid = rc_nvme_get_nsid,
	.nvme_io = rc_nvme_io,
	.nvme_subsystem_reset = rc_nvme_subsystem_reset,
	.nvme_reset_controller = rc_nvme_reset_controller,
	.nvme_ns_rescan = rc_nvme_ns_rescan,
	.nvme_submit_passthru = rc_nvme_submit_passthru,
	.nvme_submit_io_passthru = rc_nvme_submit_io_passthru,
	.is_blk = rc_is_blk,
};

