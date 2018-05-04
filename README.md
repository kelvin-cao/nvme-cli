# Switchtec-nvme-cli
This project is a fork of [nvme-cli][0] project with additional support of NVMe drives within Switchtec Gen3 PAX fabric.

## Background
Switchtec Gen3 PAX is a variant of Microsemi's Switchtec PCIe switch product which enables the SR-IOV device sharing to mutiple hosts. Common SR-IOV devices include NICs, HBAs and IB cards, etc. Around early 2018, SR-IOV capable NVMe drives becomes available to market. With the support Switchtec Gen3 PAX switch, a SR-IOV capable NVMe drive can be managed by host and shared to multiple hosts.

By default, a NVMe drive in Switchtec Gen3 PAX fabric cannot be enumerated by hosts connected to the fabric. With the bind operation supported by Switchtec Gen3 PAX, VFs can be bound to hosts and be enumerated by host.

Unlike some of other SR-IOV devices like NICs, the NVMe drives need to be properly configured before using. Normal operations includes create namespace and attache the namespace to a specific VF (NVMe secondary controller). And these operations need to be done on host. So we need a method to configure the NVMe drives before we can bind it to a host. 

As stated above, before bind operation, a host cannot even enumerate the NVMe drives, not to say use nvme-cli to manage them. To bridge this gap, Switchtec Gen3 PAX provides a special MRPC command to host to forward NVMe admin commands to the NVMe drives behind PAX switch. 

Switchtec-nvme-cli is responsible to format NVMe admin commands and deliver them to NVMe drives behind PAX switch through the special MRPC command.

## Features
Swichtec-nvme-cli supports all features of nvme-cli for drives connected directly to host.

Swichtec-nvme-cli supports following features for NVMe drives behind PAX.
- List all NVMe devices and namespaces
- Create namespace
- Delete namespace
- Attaches a namespace to requested controller(s)
- Deletes a namespace from the controller
- Send NVMe Identify Controller
- Send NVMe Identify Namespace, display structure
- Send NVMe Identify List, display structure

## Examples
Here are some examples of managing the NVMe drives behind a PAX switch. For the NVMe drives connected directly to host, all commands from original nvme-cli are supported without any change.
1. List all NVMe devices and namespaces
```
#sudo ./nvme microsemi list
Node                       SN                   Model                                    Namespace Usage                      Format           FW Rev
-------------------------- -------------------- ---------------------------------------- --------- -------------------------- ---------------- --------
0x3300n1@/dev/switchtec0   S3HCNX0JC00648       SAMSUNG MZWLL800HEHP-00003               1           6.55  MB /   6.55  MB    512   B +  0 B   GPNA6B3T
0x3300n2@/dev/switchtec0   S3HCNX0JC00648       SAMSUNG MZWLL800HEHP-00003               2          13.11  MB /  13.11  MB    512   B +  0 B   GPNA6B3T
0x3300n3@/dev/switchtec0   S3HCNX0JC00648       SAMSUNG MZWLL800HEHP-00003               3         131.07  MB / 131.07  MB    512   B +  0 B   GPNA6B3T
0x3300n4@/dev/switchtec0   S3HCNX0JC00648       SAMSUNG MZWLL800HEHP-00003               4           2.15  GB /   2.15  GB    512   B +  0 B   GPNA6B3T
0x3300n5@/dev/switchtec0   S3HCNX0JC00648       SAMSUNG MZWLL800HEHP-00003               5           2.15  GB /   2.15  GB    512   B +  0 B   GPNA6B3T

```
2. Create a 4GB namespace
```
#sudo ./nvme  create-ns 0x3300@/dev/switchtec0 -c 1048576 -s 1048576 -f 2
create-ns: Success, created nsid:6
```
3. Attach a namespace to a controller
```
#sudo ./nvme attach-ns 0x3300@/dev/switchtec0 -n 6 -c 0x21
attach-ns: Success, nsid:6
```
4. List all NVMe devices and namespaces
```
#sudo ./nvme microsemi list
Node                       SN                   Model                                    Namespace Usage                      Format           FW Rev
-------------------------- -------------------- ---------------------------------------- --------- -------------------------- ---------------- --------
0x3300n1@/dev/switchtec0   S3HCNX0JC00648       SAMSUNG MZWLL800HEHP-00003               1           6.55  MB /   6.55  MB    512   B +  0 B   GPNA6B3T
0x3300n2@/dev/switchtec0   S3HCNX0JC00648       SAMSUNG MZWLL800HEHP-00003               2          13.11  MB /  13.11  MB    512   B +  0 B   GPNA6B3T
0x3300n3@/dev/switchtec0   S3HCNX0JC00648       SAMSUNG MZWLL800HEHP-00003               3         131.07  MB / 131.07  MB    512   B +  0 B   GPNA6B3T
0x3300n4@/dev/switchtec0   S3HCNX0JC00648       SAMSUNG MZWLL800HEHP-00003               4           2.15  GB /   2.15  GB    512   B +  0 B   GPNA6B3T
0x3300n5@/dev/switchtec0   S3HCNX0JC00648       SAMSUNG MZWLL800HEHP-00003               5           2.15  GB /   2.15  GB    512   B +  0 B   GPNA6B3T
0x3300n6@/dev/switchtec0   S3HCNX0JC00648       SAMSUNG MZWLL800HEHP-00003               6           4.29  GB /   4.29  GB      4 KiB +  0 B   GPNA6B3T
```
5. Detach a namespace from a controller
```
#sudo ./nvme detach-ns 0x3300@/dev/switchtec0 -n 6 -c 0x21
detach-ns: Success, nsid:6
```
6. Delete a namespace
```
#sudo ./nvme delete-ns 0x3300@/dev/switchtec0 -n 6
delete-ns: Success, deleted nsid:6
```
7. List all NVMe devices and namespaces
```
#sudo ./nvme microsemi list
Node                       SN                   Model                                    Namespace Usage                      Format           FW Rev
-------------------------- -------------------- ---------------------------------------- --------- -------------------------- ---------------- --------
0x3300n1@/dev/switchtec0   S3HCNX0JC00648       SAMSUNG MZWLL800HEHP-00003               1           6.55  MB /   6.55  MB    512   B +  0 B   GPNA6B3T
0x3300n2@/dev/switchtec0   S3HCNX0JC00648       SAMSUNG MZWLL800HEHP-00003               2          13.11  MB /  13.11  MB    512   B +  0 B   GPNA6B3T
0x3300n3@/dev/switchtec0   S3HCNX0JC00648       SAMSUNG MZWLL800HEHP-00003               3         131.07  MB / 131.07  MB    512   B +  0 B   GPNA6B3T
0x3300n4@/dev/switchtec0   S3HCNX0JC00648       SAMSUNG MZWLL800HEHP-00003               4           2.15  GB /   2.15  GB    512   B +  0 B   GPNA6B3T
0x3300n5@/dev/switchtec0   S3HCNX0JC00648       SAMSUNG MZWLL800HEHP-00003               5           2.15  GB /   2.15  GB    512   B +  0 B   GPNA6B3T
```
8. List namespace of a NVMe drive
```
#sudo ./nvme list-ns 0x3300@/dev/switchtec0
[   0]:0x1
[   1]:0x2
[   2]:0x3
[   3]:0x4
[   4]:0x5
```
9. List controllers of a NVMe drive
```
#sudo ./nvme list-ctrl 0x3300@/dev/switchtec0
[   0]:0x1
[   1]:0x2
[   2]:0x3
[   3]:0x4
[   4]:0x5
[   5]:0x6
[   6]:0x7
[   7]:0x8
[   8]:0x9
[   9]:0xa
[  10]:0xb
[  11]:0xc
[  12]:0xd
[  13]:0xe
[  14]:0xf
[  15]:0x21
```
[0]: https://github.com/linux-nvme/nvme-cli
