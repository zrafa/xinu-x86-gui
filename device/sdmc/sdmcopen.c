/* sdmcopen.c  -  sdmcopen */

#include <xinu.h>
#include <sdmc.h>

/*------------------------------------------------------------------------
 * sdmc_set_bus_power - set SD voltage
 *------------------------------------------------------------------------
 */
devcall sdmc_set_bus_power (
	 volatile struct sdmc_csreg *csrptr	/* address of SD controller's CSR	*/
	)
{
	uint8 sdmc_voltage_select = 0;

	/* Determine max supported voltage */
	if(csrptr->capabilities & SDMC_CAP_VOLT_SUPPORT_3P3V) {
		sdmc_voltage_select = SDMC_PWR_CTL_SD_BUS_VOL_SEL_3P3V;
	} else if(csrptr->capabilities & SDMC_CAP_VOLT_SUPPORT_3P0V) {
		sdmc_voltage_select = SDMC_PWR_CTL_SD_BUS_VOL_SEL_3P0V;
	} else if(csrptr->capabilities & SDMC_CAP_VOLT_SUPPORT_1P8V) {
		sdmc_voltage_select = SDMC_PWR_CTL_SD_BUS_VOL_SEL_1P8V;
	} else {
		/* Unknown supported voltage */
		return SDMC_RC_NON_RECOVERABLE_ERROR;
	}
	
	/* Set the controller voltage */
	csrptr->pwr_ctl &= SDMC_PWR_CTL_SD_BUS_VOL_SEL_CLR;
	csrptr->pwr_ctl |= sdmc_voltage_select;
	
	/* Set bus power flag */
	csrptr->pwr_ctl |= SDMC_PWR_CTL_SD_BUS_PWR;
	
	return SDMC_RC_OK;
}

/*------------------------------------------------------------------------
 * sdmc_set_clock - set SDCLK frequency
 *------------------------------------------------------------------------
 */
devcall sdmc_set_clock (
	 volatile struct sdmc_csreg *csrptr	/* address of SD controller's CSR	*/
	)
{
	/* Change to the SD Clock is not allowed while command inhibit is set */
	while(csrptr->pre_state & SDMC_PRE_STATE_CMD_INHIBIT_CMD) {
		DELAY(SDMC_CMD_DELAY);
	}
	while(csrptr->pre_state & SDMC_PRE_STATE_CMD_INHIBIT_DAT) {
		DELAY(SDMC_CMD_DELAY);
	}
	
	/* Disable the SD Clock */
	csrptr->clk_ctl &= ~(SDMC_CLK_CTL_SD_CLK_EN);
	
	/* Set the SD clock frequency divisor */
	csrptr->clk_ctl &= SDMC_CLK_CTL_SD_FREQ_HIGH_MASK;
	
	/* Internal enable the SD clock */
	csrptr->clk_ctl |= SDMC_CLK_CTL_INT_CLK_EN;
	
	/* Wait for SD clock to be stable */
	while(!(csrptr->clk_ctl & SDMC_CLK_CTL_INT_CLK_STABLE)) {
		DELAY(SDMC_CMD_DELAY);
	}
	
	/* Set SD Clock enable */
	csrptr->clk_ctl |= SDMC_CLK_CTL_SD_CLK_EN;
	
	return SDMC_RC_OK;
}	

/*------------------------------------------------------------------------
 * sdmc_set_dat_timeout - set timeout for data lines
 *------------------------------------------------------------------------
 */
devcall sdmc_set_dat_timeout (
	 volatile struct sdmc_csreg *csrptr	/* address of SD controller's CSR	*/
	)
{
	kprintf("TIMEOUT %08X %02X\n", csrptr->capabilities, csrptr->timeout_ctl);

	return SDMC_RC_OK;
}	

/*------------------------------------------------------------------------
 * sdmcopen - open an SD memory card
 *------------------------------------------------------------------------
 */
devcall	sdmcopen (
	 struct	dentry	*devptr,	/* entry in device switch table	*/
	 char	*name,			/* name to open			*/
	 char	*mode			/* mode argument		*/
	)
{
	volatile struct	sdmc_csreg *csrptr;	/* address of SD controller's CSR	*/
	struct	sdmcblk	*sdmcptr;	/* Pointer to sdmctab entry	*/
	uint16	error_sts = 0;		/* SDMC command error status	*/
	uint32	cmd_arg = 0;		/* Value of argument register	*/
	byte	first_ACMD41 = 1;	/* Set for the first ACMD41	*/
	
	/* Initialize structure pointers */
	sdmcptr = &sdmctab[devptr->dvminor];
	csrptr = (struct sdmc_csreg *) devptr->dvcsr;
	
	/* Initialize card identifiers */
	sdmcptr->rca = 0;
	memset(sdmcptr->cid, 0x00, sizeof(sdmcptr->cid));
	
	sdmcptr->cmd8 = 1;	/* assume card supports CMD8	*/
	sdmcptr->sdio = 0;	/* assume not an SDIO card	*/
	
	/* Set the clock speed */
	sdmc_set_clock(csrptr);
	
	/* Set the bus voltage */
	sdmc_set_bus_power(csrptr);
	
	/* Set the data line timeout value */
	sdmc_set_dat_timeout(csrptr);
	
	/* Issue card reset command (CMD0) */
	if(sdmc_issue_cmd_sync(csrptr, SDMC_CMD0, 0x00000000, &error_sts, SDMC_CMD_NO_FLAGS) != SDMC_RC_OK) {
		kprintf("[SDMC] Error in CMD0: %04X %04X", SDMC_CMD0, error_sts);
		return SYSERR;
	}
	
	/* Issue voltage check command (CMD8) */
	if(sdmc_issue_cmd_sync(csrptr, SDMC_CMD8, 0x00000101, &error_sts, SDMC_CMD_NO_FLAGS) != SDMC_RC_OK) {
		kprintf("[SDMC] Error in CMD8: %04X %04X", SDMC_CMD8, error_sts);
		return SYSERR;
	}
	/* Error in CMD8 - card must not support it */
	if(error_sts & SDMC_ERR_INT_CMD_TIMEOUT_ERR ||
	   csrptr->response0 != 0x00000101) {
		sdmcptr->cmd8 = 0;
	}
	
	/* Issue inquiry voltage state (ACMD41) */
	/* To send an application command (ACMD) a CMD55 must first be sent 	*/
	/*   to tell the controller to expect an application command		*/
	if(sdmc_issue_cmd_sync(csrptr, SDMC_CMD55, 0x00000000, &error_sts, SDMC_CMD_NO_FLAGS) != SDMC_RC_OK) {
		kprintf("[SDMC] Error in CMD55: %04X %04X", SDMC_CMD55, error_sts);
		return SYSERR;
	}
	if(sdmc_issue_cmd_sync(csrptr, SDMC_ACMD41, 0x00000000, &error_sts, SDMC_CMD_NO_FLAGS) != SDMC_RC_OK) {
		kprintf("[SDMC] Error in ACMD41: %04X %04X", SDMC_ACMD41, error_sts);
		return SYSERR;
	}
	
	/* Issue initialize card command (ACMD41) */
	cmd_arg = SDMC_OCR_MASK & csrptr->response0;	/* Set OCR 				*/
	if(csrptr->response0 & SDMC_R3_S18A) {		/* Set switch to 1.8V is card supports	*/
		cmd_arg |= SDMC_ACMD41_S18R;
	}
	cmd_arg |= SDMC_ACMD41_XPC | SDMC_ACMD41_HCS; 	/* Set high capacity support 		*/
							/*   and extended performance control 	*/
	
	/* The card initialization command (ACMD41) must be continuously sent until 	*/
	/*   unitialization has completed						*/
	do {
		if(!first_ACMD41) {
			DELAY(SDMC_CMD_DELAY);
		}
		
		/* To send an application command (ACMD) a CMD55 must first be sent 	*/
		/*   to tell the controller to expect an application command		*/
		if(sdmc_issue_cmd_sync(csrptr, SDMC_CMD55, 0x00000000, &error_sts, SDMC_CMD_NO_FLAGS) != SDMC_RC_OK) {
			kprintf("[SDMC] Error in CMD55: %04X %04X", SDMC_CMD55, error_sts);
			return SYSERR;
		}
		/* Send the card initialization command 	*/
		if(sdmc_issue_cmd_sync(csrptr, SDMC_ACMD41, cmd_arg, &error_sts, SDMC_CMD_NO_FLAGS) != SDMC_RC_OK) {
			kprintf("[SDMC] Error in ACMD41: %04X %04X", SDMC_ACMD41, error_sts);
			return SYSERR;
		}
		
		first_ACMD41 = 0;
	} while(!(csrptr->response0 & SDMC_R3_BUSY));
	
	/* TODO run voltage switch procedure of the card supports 1.8V signaling */
	
	/* Retrieve the card's card identifier (CID) */
	if(sdmc_issue_cmd_sync(csrptr, SDMC_CMD2, 0, &error_sts, SDMC_CMD_NO_FLAGS) != SDMC_RC_OK) {
		kprintf("[SDMC] Error in CMD2: %04X %04X", SDMC_CMD2, error_sts);
		return SYSERR;
	}
	memcpy(sdmcptr->cid, (char*)&csrptr->response0, 16);
	
	/* Retrieve the card's relative card address (RCA) */
	if(sdmc_issue_cmd_sync(csrptr, SDMC_CMD3, 0, &error_sts, SDMC_CMD_NO_FLAGS) != SDMC_RC_OK) {
		kprintf("[SDMC] Error in CMD3: %04X %04X", SDMC_CMD3, error_sts);
		return SYSERR;
	}
	sdmcptr->rca = csrptr->response0 & SDMC_R6_RCA_MASK;
	
	sdmcptr->cmd_sem = semcreate(0);
	if((int)sdmcptr->cmd_sem == SYSERR) {
		return SYSERR;
	}
	
	return devptr->dvnum;
}
