#define INTEL_QUARK_LEGBR_PCI_DID	0x095E	/* Legacy Bridge Device ID	*/
#define INTEL_QUARK_LEGBR_PCI_VID	0x8086	/* Legacy Bridge Vendor ID	*/

#define PABCDRC_OFFSET	0x60	/* PIRQ A-D Routing Control offset	*/
#define PEFGHRC_OFFSET	0x64	/* PIRQ E-H Routing control offset	*/

#define PABCDRC_VALUE	0x06050403	/* PIRQ A-D routed to 8259 IRQ 2-5 resp. */
#define PEFGHRC_VALUE	0x0C0B0A09	/* PIRQ E-H routed to 8259 IRQ 6-9 resp. */

#define RCBA_OFFSET	0xF0	/* RCBA offset in PCI config space */
#define RCBA_MASK	0xffffc000

/* IRQAGENTx offset from the Root Complex Base Address */
#define IRQAGENT0_OFFSET	0x3140
#define IRQAGENT1_OFFSET	0x3142
#define IRQAGENT2_OFFSET	0x3144
#define IRQAGENT3_OFFSET	0x3146

#define IRQAGENT0_VALUE	0x0000	/* Remote Management Unit, only PCI INTA, routed to PIRQA   */
#define IRQAGENT1_VALUE	0x3210	/* PCIe* Multi-function, PCI INTA-D, routed to PIRQA-D resp */
#define IRQAGENT2_VALUE 0x0001	/* Reserved						    */
#define IRQAGENT3_VALUE 0x7654	/* IO Fabric Multi-func, PCI INTA-D, routed to PIRQE-H resp */
