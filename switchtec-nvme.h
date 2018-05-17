#undef CMD_INC_FILE
#define CMD_INC_FILE switchtec-nvme

#if !defined(SWITCHTEC_NVME) || defined(CMD_HEADER_MULTI_READ)
#define SWITCHTEC_NVME

#include "cmd.h"

PLUGIN(NAME("switchtec", "Switchtec specific extensions"),
	COMMAND_LIST(
		ENTRY("list", "List all NVMe devices and namespaces attached to Switchtec PAX switches", switchtec_pax_list)
	)
);

#endif

#include "define_cmd.h"
