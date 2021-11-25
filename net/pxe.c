/* pxe.c - Pre-execution Boot Environment */

#include <xinu.h>

/*------------------------------------------------------------------------
 * get_sysid_entry_point - Retrieve the system ID entry point table address
 *                           from BIOS space.  This is located somewhere
 *                           between SYSID_ENTRY_POINT_SADDR and
 *                           SYSID_ENTRY_POINT_EADDR
 *------------------------------------------------------------------------
 */
struct sysid_entry_point* get_sysid_entry_point()
{
	/* Search for the SYSID Entry Point Table */
	/*   The table is 16 byte aligned and is located somewhere between */
	/*     the starting address and the ending address                 */
	uint32 i;
	for(i = SYSID_ENTRY_POINT_SADDR; i < SYSID_ENTRY_POINT_EADDR; i += 16) {
	    struct sysid_entry_point* sysid_entry = (struct sysid_entry_point*)i;
		if(memcmp(sysid_entry->header, SYSID_ENTRY_POINT_HDR, sizeof(sysid_entry->header)) == 0) {
			return (struct sysid_entry_point*)i;
		}
	}
	return NULL;
}

/*------------------------------------------------------------------------
 * get_uuid - Retrieve the UUID from the system ID table in BIOS
 *------------------------------------------------------------------------
 */
char* get_uuid(uint16* uuid_len)
{
	uint16 i;
	struct sysid_bios_struct* sysid_bios_entry;
	struct sysid_entry_point* sysid_entry = get_sysid_entry_point();
	if(NULL == sysid_entry) {
		(*uuid_len) = 0;
		return NULL;
	}
	
	sysid_bios_entry = (struct sysid_bios_struct*)sysid_entry->bios_structure_addr;
	for(i = 0; i < sysid_entry->bios_struct_count; i++)
	{
		if(memcmp(sysid_bios_entry->header, SYSID_ENTRY_UUID_HDR, sizeof(sysid_bios_entry->header)) == 0) {
			(*uuid_len) = sysid_bios_entry->len - sizeof(struct sysid_bios_struct);
			return (char*)(sysid_bios_entry+1);
		}
		
		sysid_bios_entry = (struct sysid_bios_struct*)(((char*)sysid_bios_entry) + sysid_bios_entry->len);
	}
	
	(*uuid_len) = 0;
	return NULL;
}

/*------------------------------------------------------------------------
 * build_dhcp_discover_pxe - handcraft a DHCP Discover message in dmsg
 *------------------------------------------------------------------------
 */
int32 build_dhcp_discover_pxe(struct dhcpmsg* dmsg)
{
	uint32  i,j;
	uint32	xid;			/* xid used for the exchange	*/
		
	memcpy(&xid, NetData.ethucast, 4); /* use 4 bytes from MAC as XID */
	memset(dmsg, 0x00, sizeof(struct dhcpmsg));
	
	dmsg->dc_bop = 0x01;	     	/* Outgoing request		*/
	dmsg->dc_htype = 0x01;		/* hardware type is Ethernet	*/
	dmsg->dc_hlen = 0x06;		/* hardware address length	*/
	dmsg->dc_hops = 0x00;		/* Hop count			*/
	dmsg->dc_xid = htonl(xid);	/* xid (unique ID)		*/
	dmsg->dc_secs = 0x0000;		/* seconds			*/
	dmsg->dc_flags = 0x0000;		/* flags			*/
	dmsg->dc_cip = 0x00000000;	/* Client IP address		*/
	dmsg->dc_yip = 0x00000000;	/* Your IP address		*/
	dmsg->dc_sip = 0x00000000;	/* Server IP address		*/
	dmsg->dc_gip = 0x00000000;	/* Gateway IP address		*/
	memset(&dmsg->dc_chaddr,'\0',16);/* Client hardware address	*/
	memcpy(&dmsg->dc_chaddr, NetData.ethucast, ETH_ADDR_LEN);
	memset(&dmsg->dc_bootp,'\0',192);/* zero the bootp area		*/
	dmsg->dc_cookie = htonl(0x63825363); /* Magic cookie for DHCP	*/
	
	j = 0;
	
	uint16 uuid_len = 0;
	char* uuid = get_uuid(&uuid_len);
	if(uuid_len == 0) {
		kprintf("build_dhcp_discover_pxe: Unable to retrieve UUID\r\n");
		return SYSERR;
	}
	
	dmsg->dc_opt[j++] = 0xff & 97;  /* Client UUID/GUID Type */
	dmsg->dc_opt[j++] = 0xff & (uuid_len + 1);  /* option length         */
	dmsg->dc_opt[j++] = 0xff &  0;  /* UUID                  */
	for(i = 0; i < uuid_len; i++) {
		dmsg->dc_opt[j++] = 0xff & uuid[i];
	}
	dmsg->dc_opt[j++] = 0xff & 94;  /* Client network type   */
	dmsg->dc_opt[j++] = 0xff &  3;  /* option length         */
	dmsg->dc_opt[j++] = 0xff &  1;  /* Type = UNDI           */
	dmsg->dc_opt[j++] = 0xff &  1;  /* UNDI Major ver        */
	dmsg->dc_opt[j++] = 0xff &  2;  /* UNDI Minor ver        */
	
	dmsg->dc_opt[j++] = 0xff & 93;  /* Client architecture   */
	dmsg->dc_opt[j++] = 0xff &  2;  /* option length         */
	dmsg->dc_opt[j++] = 0xff &  0;  /* Arch - byte 1         */
	dmsg->dc_opt[j++] = 0xff &  0;  /* Arch - byte 2         */

	dmsg->dc_opt[j++] = 0xff & 53;	/* DHCP message type option	*/
	dmsg->dc_opt[j++] = 0xff &  1;	/* option length		*/
	dmsg->dc_opt[j++] = 0xff &  1;	/* DHCP Discover message		*/
	dmsg->dc_opt[j++] = 0xff &  0;	/* Options padding		*/

	dmsg->dc_opt[j++] = 0xff & 55;	/* DHCP parameter request list	*/
	dmsg->dc_opt[j++] = 0xff &  4;	/* option length		*/
	dmsg->dc_opt[j++] = 0xff &  1;	/* request subnet mask 		*/
	dmsg->dc_opt[j++] = 0xff &  3;	/* request default router addr->	*/
	dmsg->dc_opt[j++] = 0xff & 43; /* Vendor options */
	dmsg->dc_opt[j++] = 0xff & 60; /* Vendor class identifier */
	dmsg->dc_opt[j++] = 0xff & 66; /* request TFTP Server */
	dmsg->dc_opt[j++] = 0xff & 67; /* request Boot file */
	
	dmsg->dc_opt[j++] = 0xff & 60; /* DHCP PXE Support     */
	dmsg->dc_opt[j++] = 0xff & 32; /* option length        */
	
	char* pxe_client_tmp = PXE_CLIENT_STRING;
	for(i = 0; i < PXE_CLIENT_STRING_LEN; i++) {
		dmsg->dc_opt[j++] = 0xff & pxe_client_tmp[i];
	}
	
	dmsg->dc_opt[j++] = 0xff &  0;	/* options padding		*/
	dmsg->dc_opt[j++] = 0xff &  0;	/* options padding		*/
	dmsg->dc_opt[j++] = 0xff &  0;	/* options padding		*/

	return (uint32)((char *)&dmsg->dc_opt[j] - (char *)dmsg + 1);
}
