/* pxe.h - Definitions for Pre-Execution Boot Environment */

/* BIOS SYSID Structure Table */
#define SYSID_ENTRY_POINT_SADDR 0x000E0000
#define SYSID_ENTRY_POINT_EADDR 0x000FFFFF

#define SYSID_ENTRY_POINT_HDR   "_SYSID_"
#define SYSID_ENTRY_UUID_HDR    "_UUID_"

#pragma pack(1)
struct sysid_bios_struct
{
	byte header[6];
	byte checksum;
	uint16 len;
};
#pragma pack()

#pragma pack(1)
struct sysid_entry_point
{
	byte   header[7];
	byte   checksum;
	uint16 len;
	uint32 bios_structure_addr;
	uint16 bios_struct_count;
	byte   sysid_bios_revision;
};
#pragma pack()

struct sysid_entry_point* get_sysid_entry_point();
char* get_uuid(uint16* uuid_len);

/* PXE client specification string                               */
/*   Tells the boot server information about client architecture */
/*   and supported PXE version                                   */
/*   Client architecture - 000000 - IA x86 PC                    */
/*   PXE Version - 2.1                                           */
#define PXE_CLIENT_STRING      "PXEClient:Arch:00000:UNDI:002001"
#define PXE_CLIENT_STRING_LEN  32

int32 build_dhcp_discover_pxe(struct dhcpmsg* dmsg);
int32 build_dhcp_request_pxe(struct dhcpmsg* dmsg, const struct dhcpmsg* dmsg_offer);
