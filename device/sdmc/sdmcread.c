/* sdmcRead.c  -  sdmcread */

#include <xinu.h>
#include <sdmc.h>

devcall sdmcread_nodma (
	  struct dentry	*devptr,	/* entry in device switch table	*/
	  char	*buff,			/* buffer to hold disk block	*/
	  int32	blk			/* block number of block to read*/
	)
{
	volatile struct	sdmc_csreg *csrptr;	/* address of SD controller's CSR	*/
	//struct	sdmcblk	*sdmcptr;	/* Pointer to sdmctab entry	*/
	uint16	error_sts = 0;		/* SDMC command error status	*/
	uint32	cmd_arg = 0;
	char* buff_ptr = buff;
	uint32 i;
	
	//sdmcptr = &sdmctab[devptr->dvminor];
	csrptr = (struct sdmc_csreg *) devptr->dvcsr;
	
	/* Issue command CMD16 - set block size */
	if(sdmc_issue_cmd_sync(csrptr, SDMC_CMD16, 512, &error_sts, SDMC_CMD_NO_FLAGS) != SDMC_RC_OK) {
		kprintf("[SDMC] Error in CMD16: %04X %04X", SDMC_CMD16, error_sts);
		return SYSERR;
	}
	
	csrptr->nrm_int_status_en = 0x1FF;
	csrptr->err_int_stat_en = 0x7FF;
	
	csrptr->blk_size = 0x00000200;
	csrptr->blk_count = 1;
	cmd_arg = blk;
	
	/* Set transmit mode 		*/
	/*   Single block read no DMA	*/
	csrptr->tx_mode = 0x0016;
	
	if(sdmc_issue_cmd_sync(csrptr, SDMC_CMD17, cmd_arg, &error_sts, SDMC_CMD_NO_FLAGS) != SDMC_RC_OK) {
		kprintf("[SDMC] Error in CMD17: %04X %04X\n", SDMC_CMD17, error_sts);
		return SYSERR;
	}
	
	kprintf("INTSTS %08X %08X\n", csrptr->nrm_int_status_en, csrptr->err_int_stat_en);
	
	/*
	if(sdmc_issue_cmd_sync(csrptr, SDMC_CMD12, 0x00000000, &error_sts, SDMC_CMD_NO_FLAGS) != SDMC_RC_OK) {
		kprintf("[SDMC] Error in CMD12: %04X %04X\n", SDMC_CMD17, error_sts);
		return SYSERR;
	}
	*/
	
	for(i = 0; i < 512/sizeof(csrptr->buf_data_port); i++) {
		while(!(csrptr->nml_int_status & SDMC_NML_INT_BUF_RD_RDY)) {
			//kprintf("INT %08X %04X %04X %08X\n", csrptr->pre_state, csrptr->nml_int_status, csrptr->err_int_status, csrptr->buf_data_port);
			DELAY(SDMC_CMD_DELAY);
		}
		csrptr->nml_int_status |= SDMC_NML_INT_BUF_RD_RDY;
		
		memcpy(buff_ptr, (char*)&csrptr->buf_data_port, sizeof(csrptr->buf_data_port));
		buff_ptr += sizeof(csrptr->buf_data_port);
		
		kprintf("Read block: %d\n", i);
	}
	
	return OK;
}


devcall sdmcread_dma (
	  struct dentry	*devptr,	/* entry in device switch table	*/
	  char	*buff,			/* buffer to hold disk block	*/
	  int32	blk			/* block number of block to read*/
	)
{
	volatile struct	sdmc_csreg *csrptr;	/* address of SD controller's CSR	*/
	struct	sdmcblk	*sdmcptr;	/* Pointer to sdmctab entry	*/
	uint16	error_sts = 0;		/* SDMC command error status	*/
	uint32	cmd_arg = 0;
	char* buff_ptr = buff;
	uint32 i;
	
	sdmcptr = &sdmctab[devptr->dvminor];
	csrptr = (struct sdmc_csreg *) devptr->dvcsr;
	
	/* Issue command CMD16 - set block size */
	if(sdmc_issue_cmd_sync(csrptr, SDMC_CMD16, 512, &error_sts, SDMC_CMD_NO_FLAGS) != SDMC_RC_OK) {
		kprintf("[SDMC] Error in CMD16: %04X %04X", SDMC_CMD16, error_sts);
		return SYSERR;
	}
	
	csrptr->nrm_int_status_en = 0x1FF;
	csrptr->err_int_stat_en = 0x7FF;
	csrptr->err_int_stat_en &= ~(SDMC_ERR_INT_DATA_TIMEOUT_ERR);
	
	csrptr->sys_adr = (uint32)buff;
	csrptr->blk_size = 0x00000200;
	csrptr->blk_count = 1;
	cmd_arg = blk;
	
	/* Set transmit mode 		*/
	/*   Single block read DMA	*/
	csrptr->tx_mode = 0x0011;
	
	if(sdmc_issue_cmd_sync(csrptr, SDMC_CMD17, cmd_arg, &error_sts, SDMC_CMD_DAT_TRNS) != SDMC_RC_OK) {
		kprintf("[SDMC] Error in CMD17: %04X %04X\n", SDMC_CMD17, error_sts);
		//return SYSERR;
	}
	
	kprintf("INTSTS %08X %08X\n", csrptr->nrm_int_status_en, csrptr->err_int_stat_en);
	
	cmd_arg = sdmcptr->rca;
	if(sdmc_issue_cmd_sync(csrptr, SDMC_CMD13, cmd_arg, &error_sts, SDMC_CMD_NO_FLAGS) != SDMC_RC_OK) {
		kprintf("[SDMC] Error in CMD13: %04X %04X\n", SDMC_CMD13, error_sts);
		return SYSERR;
	}
	
	return SYSERR;
	
	/*
	if(sdmc_issue_cmd_sync(csrptr, SDMC_CMD12, 0x00000000, &error_sts, SDMC_CMD_NO_FLAGS) != SDMC_RC_OK) {
		kprintf("[SDMC] Error in CMD12: %04X %04X\n", SDMC_CMD17, error_sts);
		return SYSERR;
	}
	*/
	
	for(i = 0; i < 512/sizeof(csrptr->buf_data_port); i++) {
		while(!(csrptr->nml_int_status & SDMC_NML_INT_BUF_RD_RDY)) {
			//kprintf("INT %08X %04X %04X %08X\n", csrptr->pre_state, csrptr->nml_int_status, csrptr->err_int_status, csrptr->buf_data_port);
			DELAY(SDMC_CMD_DELAY);
		}
		csrptr->nml_int_status |= SDMC_NML_INT_BUF_RD_RDY;
		
		memcpy(buff_ptr, (char*)&csrptr->buf_data_port, sizeof(csrptr->buf_data_port));
		buff_ptr += sizeof(csrptr->buf_data_port);
		
		kprintf("Read block: %d\n", i);
	}
	
	return OK;
}


/*------------------------------------------------------------------------
 * sdmcread - Read data from an SD memory card
 *------------------------------------------------------------------------
 */
devcall	sdmcread (
	  struct dentry	*devptr,	/* entry in device switch table	*/
	  char	*buff,			/* buffer to hold disk block	*/
	  int32	blk			/* block number of block to read*/
	)
{
	volatile struct	sdmc_csreg *csrptr;	/* address of SD controller's CSR	*/
	struct	sdmcblk	*sdmcptr;	/* Pointer to sdmctab entry	*/
	uint16	error_sts = 0;		/* SDMC command error status	*/
	uint32	cmd_arg = 0;
	
	/* Initialize structure pointers */
	sdmcptr = &sdmctab[devptr->dvminor];
	csrptr = (struct sdmc_csreg *) devptr->dvcsr;
	
	/* Retrieve the current card status to ensure it is ready for I/O */
	cmd_arg = sdmcptr->rca;
	if(sdmc_issue_cmd_sync(csrptr, SDMC_CMD13, cmd_arg, &error_sts, SDMC_CMD_NO_FLAGS) != SDMC_RC_OK) {
		kprintf("[SDMC] Error in CMD13: %04X %04X", SDMC_CMD13, error_sts);
		return SYSERR;
	}
	
	switch(csrptr->response0 & SDMC_R1_CURRENT_STATE) {
	
		/* Card is in initial state the device 	*/
		/*   must be reopened to allow I/O	*/
		case SDMC_R1_IDLE_STATE:
		case SDMC_R1_READY_STATE:
		case SDMC_R1_IDENT_STATE:
			/* TODO close and reopen the device	*/
			return SYSERR;
			break;
	
		/* Standby state, issue CMD7 to select the card */
		case SMDC_R1_STBY_STATE:
			cmd_arg = sdmcptr->rca;
			if(sdmc_issue_cmd_sync(csrptr, SDMC_CMD7, cmd_arg, &error_sts, SDMC_CMD_NO_FLAGS) != SDMC_RC_OK) {
				kprintf("[SDMC] Error in CMD7: %04X %04X", SDMC_CMD7, error_sts);
				return SYSERR;
			}
			break;
			
		/* Card is already in data transfer state 	*/
		case SDMC_R1_TRAN_STATE:
		case SDMC_R1_DATA_STATE:
		case SDMC_R1_RCV_STATE:
			break;
		
		/* Card is in some other state  	*/
		/*   must be reopened to allow I/O	*/
		case SDMC_R1_PRG_STATE:
		case SMDC_R1_DIS_STATE:
			/* TODO close and reopen the device	*/
			return SYSERR;
			break;
		
		default:
			return SYSERR;
	}
	
	sdmcread_dma(devptr, buff, blk);

	return OK;
}
