/* dhcp_dump.c - dhcp_dump */

#include <xinu.h>

/*------------------------------------------------------------------------
 * dhcp_dump  -  Display the contents of a DHCP message
 *------------------------------------------------------------------------
 */
void	dhcp_dump(
	  struct dhcpmsg* dmsg,		/* Pointer to DHCP message	*/
	  uint32 dmsg_size		/* Size of DHCP message		*/
	)
{
	uint32 i,j;
	unsigned char* opt_tmp;	/* Pointer to current DHCP option	*/
	unsigned char* eom;	/* Pointer to the end of the message	*/

	/* Dump fixed fields in the message */

	kprintf("DHCP Message Dump\n");
	kprintf("OP: %d (0x%02X)\n", dmsg->dc_bop, (uint8)dmsg->dc_bop);
	kprintf("HTYPE: %d (0x%02X)\n", dmsg->dc_htype, (uint8)dmsg->dc_htype);
	kprintf("HLEN: %d (0x%02X)\n", dmsg->dc_hlen, (uint8)dmsg->dc_hlen);
	kprintf("HOPS: %d (0x%02X)\n", dmsg->dc_hops, (uint8)dmsg->dc_hops);
	kprintf("XID: %d (0x%08X)\n", ntohl(dmsg->dc_xid), ntohl((uint32)dmsg->dc_xid));
	kprintf("SECS: %d (0x%04X)\n", dmsg->dc_secs, (uint16)dmsg->dc_secs);
	kprintf("FLAGS: %d (0x%04X)\n", dmsg->dc_flags, (uint16)dmsg->dc_flags);
	kprintf("Client IP: %d (0x%08X)\n", ntohl(dmsg->dc_cip), ntohl((uint32)dmsg->dc_cip));
	kprintf("Your IP: %d (0x%08X)\n", ntohl(dmsg->dc_yip), ntohl((uint32)dmsg->dc_yip));
	kprintf("Server IP: %d (0x%08X)\n", ntohl(dmsg->dc_sip), ntohl((uint32)dmsg->dc_sip));
	kprintf("Gateway IP: %d (0x%08X)\n", ntohl(dmsg->dc_gip), ntohl((uint32)dmsg->dc_gip));
	kprintf("Client Hardware Address:\n  ");
	for(i = 0; i < sizeof(dmsg->dc_chaddr); i++) {
		kprintf("%02X", (uint8)dmsg->dc_chaddr[i]);
	}
	kprintf("\n");

	/* Dump BOOTP fields */

	kprintf("BOOTP Server Name: %.64s\n", dmsg->sname);
	kprintf("BOOTP File Name: %.128s\n", dmsg->bootfile);
	kprintf("Magic Cookie: %d (0x%08X)\n", dmsg->dc_cookie, (uint32)dmsg->dc_cookie);

	eom = (unsigned char*)dmsg + dmsg_size - 1;
	opt_tmp = (unsigned char*)dmsg->dc_opt;

	/* Parse and dump options */

	while(opt_tmp < eom) {

		switch(*opt_tmp) {

			case DHCP_PADDING:
			case DHCP_MESSAGE_END:
				opt_tmp++;
				continue;
				break;
				
			case DHCP_SUBNET_MASK:
				kprintf("DHCP Subnet Mask: %d (0x%08X)\n", ntohl(*(uint32*)(opt_tmp+2)), ntohl(*(uint32*)(opt_tmp+2)));
				break;
				
			case DHCP_ROUTER:
				j = *(uint8*)(opt_tmp+1);
				for(i = 0; i < j/4; i++) {
					kprintf("DHCP Router: %d (0x%08X)\n", ntohl(*(uint32*)(opt_tmp+2+4*i)), ntohl(*(uint32*)(opt_tmp+2+4*i)));
				}
				break;
			
			case DHCP_DNS_SERVER:
				j = *(uint8*)(opt_tmp+1);
				for(i = 0; i < j/4; i++) {
					kprintf("DHCP DNS Server: %d (0x%08X)\n", ntohl(*(uint32*)(opt_tmp+2+4*i)), ntohl(*(uint32*)(opt_tmp+2+4*i)));
				}
				break;

			case DHCP_DOMAIN_NAME:
				kprintf("DHCP Domain Name: ");
				j = *(uint8*)(opt_tmp+1);
				for(i = 0; i < j; i++) {
					kprintf("%c", *((char*)(opt_tmp + 2 + i)));
				}
				kprintf("\n");
				break;
			
			case DHCP_IP_ADDR_LEASE_TIME:
				kprintf("DHCP IP Lease Time: %d (0x%08X)\n", ntohl(*(uint32*)(opt_tmp+2)), ntohl(*(uint32*)(opt_tmp+2)));
				break;
				
			case DHCP_RENEWAL_TIME_VALUE:
				kprintf("DHCP Renewal (T1) Time: %d (0x%08X)\n", ntohl(*(uint32*)(opt_tmp+2)), ntohl(*(uint32*)(opt_tmp+2)));
				break;
			
			case DHCP_REBINDING_TIME_VALUE:
				kprintf("DHCP Rebinding (T2) Time: %d (0x%08X)\n", ntohl(*(uint32*)(opt_tmp+2)), ntohl(*(uint32*)(opt_tmp+2)));
				break;		

			case DHCP_MESSAGE_TYPE:
				kprintf("DHCP Message Type: %d\n", *(uint8*)(opt_tmp+2));
				break;
				
			case DHCP_REQUESTED_IP:
				kprintf("DHCP Requested IP: %d (0x%08X)\n", ntohl(*(uint32*)(opt_tmp+2)), ntohl(*(uint32*)(opt_tmp+2)));
				break;
				
			case DHCP_SERVER_ID:
				kprintf("DHCP Server ID: %d (0x%08X)\n", ntohl(*(uint32*)(opt_tmp+2)), ntohl(*(uint32*)(opt_tmp+2)));
				break;

			case DHCP_VENDER_OPTIONS:
				kprintf("DHCP Vender Options List:\n");
				j = *(uint8*)(opt_tmp+1);
				for(i = 0; i < j; i++) {
					kprintf("  %d\n", *(uint8*)(opt_tmp+i+2));
				}
				break;
				
			case DHCP_PARAMETER_REQUEST_LIST:
				kprintf("DHCP Parameter List:\n");
				j = *(uint8*)(opt_tmp+1);
				for(i = 0; i < j; i++) {
					kprintf("  %d\n", *(uint8*)(opt_tmp+i+2));
				}
				break;
				
			case DHCP_VENDOR_CLASS_ID:
				kprintf("DHCP Class Identifier: %.32s\n", (char*)(opt_tmp+2));
				break;
				
			default:
				kprintf("Unknown DHCP option: %d\n", (unsigned char)(*opt_tmp));
				break;

		}

		opt_tmp++;	/* move to length octet */
		opt_tmp += *(uint8*)opt_tmp + 1;
	}
	kprintf("\n");
}