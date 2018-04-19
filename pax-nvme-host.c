#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include "nvme-host.h"
#include "pax-nvme-host.h"

#include <switchtec/switchtec.h>
#include <switchtec/mrpc.h>

int pax_nvme_submit_admin_passthru(int fd, struct nvme_passthru_cmd *cmd)
{
	int ret;

//	printf("here\n");
	struct pax_nvme_host *pax;
	struct fabiov_device_manage_req req;
	struct fabiov_device_manage_rsp rsp;

	memset(&req, 0, sizeof(req));
	memset(&rsp, 0, sizeof(rsp));
	pax = to_pax_nvme_host(global_host);

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

int pax_is_blk(void)
{
	struct pax_nvme_host *pax;
	pax = to_pax_nvme_host(global_host);

	return pax->is_blk;
}

struct nvme_host_ops pax_ops = {
	.submit_admin_passthru = pax_nvme_submit_admin_passthru,
	.is_blk = pax_is_blk,
};

