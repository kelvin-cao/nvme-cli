#undef CMD_INC_FILE
#define CMD_INC_FILE microsemi-nvme

#if !defined(MICROSEMI_NVME) || defined(CMD_HEADER_MULTI_READ)
#define MICROSEMI_NVME

#include "cmd.h"

PLUGIN(NAME("microsemi", "Microsemi vendor specific extensions"),
	COMMAND_LIST(
		ENTRY("list", "List all NVMe devices and namespaces attached to Microsemi PAX switches", microsemi_list)
	)
);

#endif

#include "define_cmd.h"

