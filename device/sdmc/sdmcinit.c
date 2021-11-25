/* sdmcInit.c  -  sdmcInit */

#include <xinu.h>
#include <sdmc.h>

struct	sdmcblk	sdmctab[Nsdmc];

/*------------------------------------------------------------------------
 *  sdmcinit - initialize the SD memory card device
 *------------------------------------------------------------------------
 */
devcall	sdmcinit (
	  struct dentry	*devptr		/* entry in device switch table	*/
	)
{
	volatile struct	sdmc_csreg *csrptr;	/* address of SD controller's CSR	*/
	uint32	pciinfo;			/* PCI info to read config	*/
	
	/* Search for the SD memory card device on the PCI bus */
	pciinfo = find_pci_device(INTEL_QUARK_SDIO_PCI_DID, INTEL_QUARK_SDIO_PCI_VID, 0);
	if((int)pciinfo == SYSERR) {
		kprintf("[SDMC] Device not found\n");
		return SYSERR;
	}
	
	/* Read PCI config space to get memory base address */
	if(pci_read_config_dword(pciinfo, 0x10, (uint32 *)&devptr->dvcsr) == SYSERR) {
		kprintf("[SDMC] Unable to retrieve CSR\n");
		return SYSERR;
	}
	
	/* Enable CSR Memory Space, Enable Bus Master */
	pci_write_config_word(pciinfo, 0x4, 0x0006);
	
	/* Set interrupt IRQ */
	set_evec(devptr->dvirq, (uint32)sdmcdispatch);
	
	/* Initialize the SD CS register */
	csrptr = (struct sdmc_csreg *)devptr->dvcsr;
	
	/* Enable and register for card insertion and removal interrupts */
	csrptr->nrm_int_status_en = SDMC_CRD_INS_STAT_EN | SDMC_CRD_RMV_STAT_EN  | SDMC_CRD_INT_STAT_EN;
	csrptr->nrm_int_sig_en = SDMC_CRD_INS_SIG_EN | SDMC_CRD_RMV_SIG_EN  | SDMC_CRD_INT_SIG_EN;
	csrptr->err_int_stat_en = 0;
	csrptr->err_int_sig_en = 0;
		
	/*
	csrptr->nrm_int_status_en = 0x1FF;
	csrptr->nrm_int_sig_en = 0x1FF;
	
	csrptr->err_int_stat_en = 0x7FF;
	csrptr->err_int_sig_en = 0x7FF;
	*/
	
	return OK;
}
