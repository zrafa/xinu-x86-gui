/* sdmccmd.c - sdmc_cmd_err_rcvy, sdmc_issue_cmd_async,			*/
/*		 sdmc_finalize_cmd_async, sdmc_issue_cmd_sync		*/

#include <xinu.h>
#include <sdmc.h>

devcall sdmc_cmd_err_rcvy (
	 volatile struct sdmc_csreg *csrptr,	/* SD controlelr CSR	*/
	 uint16* error_sts			/* command error status	*/
	)
{
	uint16	cmd12_err_sts = 0;		/* CMD12 error status	*/

	kprintf("[SDMC] Error recovery %04X\n", csrptr->err_int_status);
	
	/* Set software reset bit for error type */
	
	/* Command Error - Command software reset */
	if(csrptr->err_int_status &
		(SDMC_ERR_INT_CMD_TIMEOUT_ERR |
		SDMC_ERR_INT_CMD_CRC_ERR |
		SDMC_ERR_INT_CMD_END_BIT_ERR |
		SDMC_ERR_INT_CMD_INDEX_ERR)) {
		
		csrptr->sw_rst |= SDMC_SW_RST_CMD_LN;
		while(csrptr->sw_rst & SDMC_SW_RST_CMD_LN) {
			DELAY(SDMC_CMD_DELAY);
		}
	}
	
	/* Data error - Data software reset */
	if(csrptr->err_int_status &
		(SDMC_ERR_INT_DATA_TIMEOUT_ERR |
		SDMC_ERR_INT_DATA_CRC_ERR |
		SDMC_ERR_INT_DATA_END_BIT_ERR)) {

		csrptr->sw_rst |= SDMC_SW_RST_DAT_LN;
		while(csrptr->sw_rst & SDMC_SW_RST_DAT_LN) {
			DELAY(SDMC_CMD_DELAY);
		}
	}
	
	kprintf("After software reset\n");
	
	/* Save and clear the error status */
	(*error_sts) = csrptr->err_int_status;
	csrptr->err_int_status |= (*error_sts);
	
	/* Issue the abort command (CMD12) */
	/*   Do not perform error recovery */
	sdmc_issue_cmd_sync(csrptr, SDMC_ABT, 0x00000000, &cmd12_err_sts,
						SDMC_CMD_NO_ERR_RCVY);

	/* Wait for command and DAT inhibit signals to be cleared */
	while(csrptr->pre_state & SDMC_PRE_STATE_CMD_INHIBIT_CMD) {
		DELAY(SDMC_CMD_DELAY);
	}
	while(csrptr->pre_state & SDMC_PRE_STATE_CMD_INHIBIT_DAT) {
		DELAY(SDMC_CMD_DELAY);
	}
	
	/* Clear the command complete interrupt */
	if(csrptr->nml_int_status & SDMC_NML_INT_CMD_COMP) {
		csrptr->nml_int_status |= SDMC_NML_INT_CMD_COMP;
	}
	
	/* Check status of abort command */
	if(csrptr->err_int_status &
		(SDMC_ERR_INT_CMD_TIMEOUT_ERR |
		SDMC_ERR_INT_CMD_CRC_ERR |
		SDMC_ERR_INT_CMD_END_BIT_ERR |
		SDMC_ERR_INT_CMD_INDEX_ERR)) {
		
		/* Clear the error status */
		csrptr->err_int_status |= csrptr->err_int_status;
		
		/* Error in abort, error is non recoverable */
		return SDMC_RC_NON_RECOVERABLE_ERROR;
	}
	
	/* Check for data timeout */
	if(csrptr->err_int_status & SDMC_ERR_INT_DATA_TIMEOUT_ERR) {
	
		/* Clear the error status */
		csrptr->err_int_status |= csrptr->err_int_status;
		
		/* Data timeout in abort, error is non recoverable */
		return SDMC_RC_NON_RECOVERABLE_ERROR;
	}
	
	/* Check data line signal level */
	DELAY(1); 	/* Must wait at least 40 microseconds */
	if(csrptr->pre_state & SDMC_PRE_STATE_DATA_LN_SIG_LVL) {
		return SDMC_RC_RECOVERABLE_ERR;
	}
	return SDMC_RC_NON_RECOVERABLE_ERROR;
}

devcall sdmc_issue_cmd_async (
	 volatile struct sdmc_csreg *csrptr,	/* SD controller CSR	*/
	 uint16 cmd_value,			/* SDMC Command to issue*/
	 uint32 arg_value			/* Command argument	*/
	)
{
	/* Wait for command inhibit bits to be cleared */
	
	kprintf("State Prior to CMD: %08X %08X %08X %08X %08X %04X %04X\n",
		csrptr->pre_state, csrptr->response0, csrptr->response2,
		csrptr->response4, csrptr->response6,
		csrptr->nml_int_status, csrptr->err_int_status);
	
	while(csrptr->pre_state & SDMC_PRE_STATE_CMD_INHIBIT_CMD) {
		DELAY(SDMC_CMD_DELAY);
	}
	while(csrptr->pre_state & SDMC_PRE_STATE_CMD_INHIBIT_DAT) {
		DELAY(SDMC_CMD_DELAY);
	}
	
	kprintf("Issuing command %04X %08X\n", cmd_value, arg_value);
	
	/* Ensure that the command complete interrupt status are enabled*/
	csrptr->nrm_int_status_en |= SDMC_CMD_COMP_STAT_EN |
			SDMC_TX_COMP_STAT_EN | SDMC_CRD_INT_STAT_EN;
	csrptr->err_int_stat_en |= SDMC_ERR_INT_CMD_TIMEOUT_ERR_STAT_EN |
			SDMC_ERR_INT_CMD_CRC_ERR_STAT_EN |
			SDMC_ERR_INT_CMD_END_BIT_ERR_STAT_EN |
			SDMC_ERR_INT_CMD_IND_ERR_STAT_EN;
	
	/* Asynchronous command execution, ensure that interrupt	*/
	/*		signals are enabled				*/
	csrptr->nrm_int_sig_en |= SDMC_CMD_COMP_SIG_EN |
		SDMC_TX_COMP_SIG_EN | SDMC_CRD_INT_SIG_EN;
	csrptr->err_int_sig_en |= SDMC_ERR_INT_CMD_TIMEOUT_ERR_SIG_EN |
		SDMC_ERR_INT_CMD_CRC_ERR_SIG_EN |
		SDMC_ERR_INT_CMD_END_BIT_ERR_SIG_EN |
		SDMC_ERR_INT_CMD_IND_ERR_SIG_EN;
	
	/* Issue the command */
	csrptr->argument = arg_value;
	csrptr->cmd = cmd_value;
	
	return SDMC_RC_OK;
}

devcall sdmc_finalize_cmd_async (
	 volatile struct sdmc_csreg *csrptr,	/* SD controller CSR	*/
	 uint16* error_sts			/* Error status		*/
	)
{
	kprintf("CMD INT: %04X %04X\n", csrptr->nml_int_status,
						csrptr->err_int_status);
	
	/* Clear the command complete interrupt */
	if(csrptr->nml_int_status & SDMC_NML_INT_CMD_COMP) {
		csrptr->nml_int_status |= SDMC_NML_INT_CMD_COMP;
	}
	
	/* Clear the error interrupt and perform error recovery */
	if(csrptr->nml_int_status & SDMC_NML_INT_ERR_INT) {
		csrptr->nml_int_status |= SDMC_NML_INT_ERR_INT;
		return sdmc_cmd_err_rcvy(csrptr, error_sts);
	}
	
	kprintf("State After CMD: %08X %08X %08X %08X %08X %04X %04X\n",
		csrptr->pre_state, csrptr->response0, csrptr->response2,
		csrptr->response4, csrptr->response6,
		csrptr->nml_int_status, csrptr->err_int_status);
	
	return SDMC_RC_OK;
}

devcall sdmc_issue_cmd_sync (
	 volatile struct sdmc_csreg *csrptr,	/* SD controller CSR	*/
	 uint16 cmd_value,			/* SDMC Command to issue*/
	 uint32 arg_value,			/* Command argument	*/
	 uint16* error_sts,			/* Error status		*/
	 uint8 flags				/* Command execut. flags*/
	)
{
	uint8 rc = SDMC_RC_OK;

	/* Save old contents of interrupt registers */
	uint16 save_nrm_int_stat_en = csrptr->nrm_int_status_en;
	uint16 save_nrm_int_sig_en = csrptr->nrm_int_sig_en;
	uint16 save_err_int_stat_en = csrptr->err_int_stat_en;
	uint16 save_err_int_sig_en = csrptr->err_int_sig_en;

	/* Wait for command inhibit bits to be cleared */
	
	kprintf("State Prior to CMD: %08X %08X %08X %08X %08X %04X %04X\n",
		csrptr->pre_state, csrptr->response0, csrptr->response2,
		csrptr->response4, csrptr->response6,
		csrptr->nml_int_status, csrptr->err_int_status);
	
	while(csrptr->pre_state & SDMC_PRE_STATE_CMD_INHIBIT_CMD) {
		DELAY(SDMC_CMD_DELAY);
	}
	while(csrptr->pre_state & SDMC_PRE_STATE_CMD_INHIBIT_DAT) {
		DELAY(SDMC_CMD_DELAY);
	}
	
	kprintf("Issuing command %04X %08X\n", cmd_value, arg_value);
	
	/* Ensure that the command complete interrupt status are enabled*/

	csrptr->nrm_int_status_en |= SDMC_CMD_COMP_STAT_EN |
		SDMC_TX_COMP_STAT_EN | SDMC_CRD_INT_STAT_EN;
	csrptr->err_int_stat_en |= SDMC_ERR_INT_CMD_TIMEOUT_ERR_STAT_EN |
		SDMC_ERR_INT_CMD_CRC_ERR_STAT_EN |
		SDMC_ERR_INT_CMD_END_BIT_ERR_STAT_EN |
		SDMC_ERR_INT_CMD_IND_ERR_STAT_EN;
	
	if(flags & SDMC_CMD_DAT_TRNS) {
		csrptr->nrm_int_status_en |= SDMC_TX_COMP_STAT_EN;
		csrptr->err_int_stat_en |=
			SDMC_ERR_INT_DATA_TIMEOUT_ERR_STAT_EN |
			SDMC_ERR_INT_DATA_CRC_ERR_STAT_EN |
			SDMC_ERR_INT_DATA_END_BIT_ERR_STAT_EN;
	}
	
	/* Synchronous command execution, ensure that interrupt		*/
	/*	signals are not enabled					*/
	csrptr->nrm_int_sig_en &= ~(SDMC_CMD_COMP_SIG_EN |
			SDMC_TX_COMP_SIG_EN | SDMC_CRD_INT_SIG_EN);
	csrptr->err_int_sig_en &= ~(
		SDMC_ERR_INT_CMD_TIMEOUT_ERR_SIG_EN |
		SDMC_ERR_INT_CMD_CRC_ERR_SIG_EN |
		SDMC_ERR_INT_CMD_END_BIT_ERR_SIG_EN |
		SDMC_ERR_INT_CMD_IND_ERR_SIG_EN |
		SDMC_ERR_INT_DATA_TIMEOUT_ERR_SIG_EN |
		SDMC_ERR_INT_DATA_CRC_ERR_SIG_EN |
		SDMC_ERR_INT_DATA_END_BIT_ERR_SIG_EN);
	
	/* Issue the command */
	csrptr->argument = arg_value;
	csrptr->cmd = cmd_value;
	
	/* Wait for command to complete */
	while(!(csrptr->nml_int_status & SDMC_NML_INT_CMD_COMP) &&
	      !(csrptr->nml_int_status & SDMC_NML_INT_ERR_INT)) {
		DELAY(SDMC_CMD_DELAY);
	}
	
	/* Clear the command complete interrupt */
	if(csrptr->nml_int_status & SDMC_NML_INT_CMD_COMP) {
		csrptr->nml_int_status |= SDMC_NML_INT_CMD_COMP;
	}
	
	if(flags & SDMC_CMD_DAT_TRNS) {
		
		/* Wait for data transmission to complete */
		while(!(csrptr->nml_int_status & SDMC_NML_INT_TX_COMP) &&
		      !(csrptr->nml_int_status & SDMC_NML_INT_ERR_INT)) {
		   kprintf(
		     "Waiting for data trans %08X %04X %04X %08X %02X\n",
		      csrptr->sys_adr, csrptr->nml_int_status,
		      csrptr->err_int_status, csrptr->pre_state,
		      csrptr->blk_gap_ctl);
				DELAY(SDMC_CMD_DELAY);
		}
		
		/* Clear the data transmission complete interrupt */
		if(csrptr->nml_int_status & SDMC_NML_INT_TX_COMP) {
			csrptr->nml_int_status |= SDMC_NML_INT_TX_COMP;
		}
	}
	
	/* Clear the error interrupt and perform error recovery */
	if(csrptr->nml_int_status & SDMC_NML_INT_ERR_INT) {
		csrptr->nml_int_status |= SDMC_NML_INT_ERR_INT;			
		if(!(flags & SDMC_CMD_NO_ERR_RCVY)) {
			rc = sdmc_cmd_err_rcvy(csrptr, error_sts);
		}
	}
	
	kprintf("State After CMD: %08X %08X %08X %08X %08X %04X %04X\n",
		csrptr->pre_state, csrptr->response0, csrptr->response2,
		csrptr->response4, csrptr->response6,
		csrptr->nml_int_status, csrptr->err_int_status);
	
	/* Restore saved interrupt enable bits */
	csrptr->nrm_int_status_en = save_nrm_int_stat_en;
	csrptr->nrm_int_sig_en = save_nrm_int_sig_en;
	csrptr->err_int_stat_en = save_err_int_stat_en;
	csrptr->err_int_sig_en = save_err_int_sig_en;
	
	return rc;
}
