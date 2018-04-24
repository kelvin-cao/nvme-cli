#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include "nvme-device.h"
#include "pax-nvme-device.h"

#include <switchtec/switchtec.h>
#include <switchtec/mrpc.h>

int pax_nvme_submit_admin_passthru(int fd, struct nvme_passthru_cmd *cmd)
{
	int ret;

//	printf("here\n");
	struct pax_nvme_device *pax;
	struct fabiov_device_manage_req req;
	struct fabiov_device_manage_rsp rsp;

	memset(&req, 0, sizeof(req));
	memset(&rsp, 0, sizeof(rsp));
	pax = to_pax_nvme_device(global_device);

	req.pdfid = pax->pdfid;
	req.cmd_data[0] = 0x2;
	memcpy(&req.cmd_data[1], cmd, 64 - 4);
	for (int i = 0; i < 16; i++) {
		fprintf(stdout, "0x%x ", req.cmd_data[i]);
	}
	fprintf(stdout, "\n");
	req.max_rsp_len = sizeof(rsp.rsp_data)/4;
	ret = switchtec_device_manage(pax->dev, &req, &rsp);
	if (ret) {
		switchtec_perror("device_manage_cmd");
		return ret;
	}

	memcpy((uint64_t *)cmd->addr, rsp.rsp_data, rsp.rsp_len * 4);

	return ret;
}

int pax_nvme_io(int fd, struct nvme_user_io *io)
{
	fprintf(stderr, "%s not implemented.\n", __FUNCTION__);
	return -1;
}

int pax_nvme_subsystem_reset(int fd)
{
	fprintf(stderr, "%s not implemented.\n", __FUNCTION__);
	return -1;
}

int pax_nvme_reset_controller(int fd)
{
	fprintf(stderr, "%s not implemented.\n", __FUNCTION__);
	return -1;
}

int pax_nvme_ns_rescan(int fd)
{
	fprintf(stderr, "%s not implemented.\n", __FUNCTION__);
	return -1;
}

int pax_nvme_submit_passthru(int fd, int ioctl_cmd, struct nvme_passthru_cmd *cmd)
{
	fprintf(stderr, "%s not implemented.\n", __FUNCTION__);
	return -1;
}

int pax_nvme_submit_io_passthru(int fd, struct nvme_passthru_cmd *cmd)
{
	fprintf(stderr, "%s not implemented.\n", __FUNCTION__);
	return -1;
}

int pax_is_blk(void)
{
	struct pax_nvme_device *pax;
	pax = to_pax_nvme_device(global_device);

	return pax->is_blk;
}

int pax_nvme_get_nsid(int fd)
{
	struct pax_nvme_device *pax;
	pax = to_pax_nvme_device(global_device);

	return pax->ns_id;
}

struct nvme_device_ops pax_ops = {
	.submit_admin_passthru = pax_nvme_submit_admin_passthru,
	.is_blk = pax_is_blk,
	.nvme_get_nsid = pax_nvme_get_nsid,
	.nvme_io = pax_nvme_io,
	.nvme_subsystem_reset = pax_nvme_subsystem_reset,
	.nvme_reset_controller = pax_nvme_reset_controller,
	.nvme_ns_rescan = pax_nvme_ns_rescan,
	.nvme_submit_passthru = pax_nvme_submit_passthru,
	.nvme_submit_io_passthru = pax_nvme_submit_io_passthru,
};

