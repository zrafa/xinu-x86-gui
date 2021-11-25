/* sdmcinterrupt.c - sdmcinterrupt */

#include <xinu.h>
#include <sdmc.h>

/*------------------------------------------------------------------------
 *  sdmcinterrupt - handle an interrupt for a SD memory card device
 *------------------------------------------------------------------------
 */
void sdmcinterrupt(void) {
	struct	dentry	*devptr;	/* address of device control blk*/
	volatile struct	sdmc_csreg *csrptr;	/* address of SD controller's CSR	*/
	struct	sdmcblk	*sdmcptr;	/* Pointer to sdmctab entry	*/

	/* Get CSR address of the device */

	devptr = (struct dentry *) &devtab[SDMC];
	csrptr = (struct sdmc_csreg *) devptr->dvcsr;
	sdmcptr = &sdmctab[devptr->dvminor];
	
	kprintf("SDMC INT: %04X %04X\n", csrptr->nml_int_status, csrptr->err_int_status);
	
	/* Check for card insertion interrupt */
	if(csrptr->nml_int_status & SDMC_NML_INT_CRD_INS) {
	
		/* Clear the card insertion interrupt */
		csrptr->nml_int_status |= SDMC_NML_INT_CRD_INS;
	
		/* Check current card present status to see if it is actually inserted */
		if(csrptr->pre_state & SDMC_PRE_STATE_CRD_INS) {
			
			/* Open the device */
			sdmcopen(devptr, "", "");
		}
		return;
	}
	
	/* Check for card removal interrupt */
	if(csrptr->nml_int_status & SDMC_NML_INT_CRD_RM) {
	
		/* Clear the card removal interrupt */
		csrptr->nml_int_status |= SDMC_NML_INT_CRD_RM;
		
		/* Check current card present status to see if it is actually removed */
		if(!(csrptr->pre_state & SDMC_PRE_STATE_CRD_INS)) {
		
			/* Close the device */
			//sdmcClose
		}
		return;
	}
	
	/* Check for command complete interrupt */
	if(csrptr->nml_int_status & SDMC_NML_INT_CMD_COMP) {
		
		/* Clear the command complete interrupt */
		csrptr->nml_int_status |= SDMC_NML_INT_CMD_COMP;
		
		/* Signal the command semaphore */
		signal(sdmcptr->cmd_sem);
	}
	
	/* Check for transfer complete interrupt */
	if(csrptr->nml_int_status & SDMC_NML_INT_TX_COMP) {
	
		/* Clear the transfer complete interrupt */
		csrptr->nml_int_status |= SDMC_NML_INT_TX_COMP;
		
		/* Signal the transfer semaphore */
		signal(sdmcptr->tx_sem);
	}
}
