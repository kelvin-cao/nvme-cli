#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include "nvme-device.h"
#include "pax-nvme-device.h"

#include <switchtec/switchtec.h>
#include <switchtec/mrpc.h>

struct fabiov_device_manage_nvme_req
{
	struct fabiov_device_manage_req_hdr hdr;
	uint32_t nvme_sqe[16];
	uint8_t nvme_data[MRPC_MAX_DATA_LEN -
		sizeof(struct fabiov_device_manage_req_hdr) -
		(16 * 4)];
};

struct fabiov_device_manage_nvme_rsp
{
	struct fabiov_device_manage_rsp_hdr hdr;
	uint32_t nvme_cqe[4];
	uint8_t nvme_data[MRPC_MAX_DATA_LEN -
		sizeof(struct fabiov_device_manage_rsp_hdr) -
		(4 * 4)];
};

int pax_nvme_submit_admin_passthru(int fd, struct nvme_passthru_cmd *cmd)
{
	int ret;

	struct pax_nvme_device *pax;
	struct fabiov_device_manage_nvme_req req;
	struct fabiov_device_manage_nvme_rsp rsp;
	int data_len;
	int status;
	bool write = nvme_is_write((struct nvme_command *)cmd);

	memset(&req, 0, sizeof(req));
	memset(&rsp, 0, sizeof(rsp));
	pax = to_pax_nvme_device(global_device);

	req.hdr.pdfid = htole16(pax->pdfid);
	memcpy(&req.nvme_sqe, cmd, 16 * 4);

	/* sqe[9] should be 0 per spec */
	req.nvme_sqe[9] = 0;

	if (cmd->data_len > sizeof(req.nvme_data))
		data_len = sizeof(req.nvme_data);
	else
		data_len = cmd->data_len;

	memcpy(req.nvme_data, (void *)cmd->addr, data_len);

	req.hdr.req_len = htole16(((data_len + 3) & ~3) / 4 + 16);

	ret = switchtec_device_manage(pax->dev,
				     (struct fabiov_device_manage_req *)&req,
				     (struct fabiov_device_manage_rsp *)&rsp);
	if (ret) {
		switchtec_perror("device_manage_cmd");
		return ret;
	}

	status = (rsp.nvme_cqe[3] & 0xfffe0000) >> 17;
	if (!status) {
		cmd->result = rsp.nvme_cqe[0];
		if (!write) {
			memcpy((uint64_t *)cmd->addr,
				rsp.nvme_data,
				(rsp.hdr.rsp_len - 4) * 4);
		}
	}

	return status;
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
	.nvme_submit_admin_passthru = pax_nvme_submit_admin_passthru,
	.nvme_get_nsid = pax_nvme_get_nsid,
	.nvme_io = pax_nvme_io,
	.nvme_subsystem_reset = pax_nvme_subsystem_reset,
	.nvme_reset_controller = pax_nvme_reset_controller,
	.nvme_ns_rescan = pax_nvme_ns_rescan,
	.nvme_submit_passthru = pax_nvme_submit_passthru,
	.nvme_submit_io_passthru = pax_nvme_submit_io_passthru,
	.is_blk = pax_is_blk,
};

