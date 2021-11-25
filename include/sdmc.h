#define INTEL_QUARK_SDIO_PCI_DID	0x08A7	/* Intel Quark SDIO PCI Device ID	*/
#define INTEL_QUARK_SDIO_PCI_VID	0x8086	/* Intel Quark SDIO MAC PCI Vendor ID	*/

#ifndef	Nsdmc
#define	Nsdmc		1		/* number of SD Memory Cards		*/
#endif

struct sdmcblk {
	sid32	cmd_sem;		/* command semaphore 			*/
	sid32	tx_sem;			/* transfer semaphore			*/
	uint32	rca;			/* relative card address		*/
	char	cid[16];		/* Card identifier			*/
	uint8	cmd8	:1;		/* Card supports CMD8			*/
	uint8	sdio	:1;		/* Card is an SDIO card			*/
	uint8	rsvd	:6;
};
extern	struct	sdmcblk	sdmctab[];

#pragma pack(1)
struct sdmc_csreg {
	uint32	sys_adr;		/* sdma system address register 	*/
	uint16	blk_size;		/* block size register 			*/
	uint16	blk_count;		/* block count register 		*/
	uint32	argument;		/* argument register 			*/
	uint16	tx_mode;		/* transfer mode register 		*/
	uint16	cmd;			/* command register 			*/
	uint32	response0;		/* response register 0 			*/
	uint32	response2;		/* response register 2 			*/
	uint32	response4;		/* response register 4 			*/
	uint32	response6;		/* response register 6 			*/
	uint32	buf_data_port;		/* buffer data port register 		*/
	uint32	pre_state;		/* present state register 		*/
	uint8	host_ctl;		/* host control register 		*/
	uint8	pwr_ctl;		/* power control register 		*/
	uint8	blk_gap_ctl;		/* block gap control register 		*/
	uint8	wakeup_ctl;		/* wakeup control register 		*/
	uint16	clk_ctl;		/* clock control register 		*/
	uint8	timeout_ctl;		/* timeout control register 		*/
	uint8	sw_rst;			/* software reset register 		*/
	uint16	nml_int_status;		/* normal interrupt status register 	*/
	uint16	err_int_status;		/* error interrupt status register 	*/
	uint16	nrm_int_status_en;	/* normal interrupt status enable 	*/
	uint16	err_int_stat_en;	/* error interrupt status enable register */
	uint16	nrm_int_sig_en;		/* normal interrupt signal enable register */
	uint16	err_int_sig_en;		/* error interrupt signal enable register */
	uint16	cmd12_err_stat;		/* auto cmd12 error status register 	*/
	uint16	host_ctrl_2;		/* host control 2 register 		*/
	uint32	capabilities;		/* capabilities register 		*/
	uint32	capabilities_2;		/* capabilities register 2 		*/
	uint32	max_cur_cap;		/* maximum current capabilities register */
	byte	rsvd1[4];
	uint16	force_event_cmd12_err_stat;	/* force event register for auto cmd12 error status */
	uint16	force_event_err_int_stat;	/* force event register for error interrupt status */
	uint8	adma_err_stat;		/* adma error status register 		*/
	byte	rsvd2[3];
	uint32	adma_sys_addr;		/* adma system address register */
	byte	rsvd3[4];
	uint16	preset_value_0;		/* initialization preset values register */
	uint16	preset_value_1;		/* default speed preset values register */
	uint16	preset_value_2;		/* high speed preset values register */
	uint16	preset_value_3;		/* sdr12 preset values register */
	uint16	preset_value_4;		/* sdr25 preset values register */
	uint16	preset_value_5;		/* sdr50 preset values register */
	uint16	preset_value_6;		/* sdr104 preset values register */
	uint16	preset_value_7;		/* ddr50 preset values register */
	uint32	boot_timeout_ctrl;	/* boot time-out control register */
	uint8	debug_sel;		/* debug selection register */
	byte	rsvd4[107];
	uint32	shared_bus;		/* shared bus control register */
	byte	rsvd5[12];
	uint8	spi_int_sup;		/* spi interrupt support register */
	byte	rsvd6[11];
	uint16	slot_int_stat;		/* slot interrupt status register */
	uint16	host_ctrl_ver;		/* host controller version register */
};
#pragma pack()

struct sdmc_cmd {
	uint16	resp_type_sel	:2;
	uint16	cmd_crc_chk_en	:1;
	uint16	cmd_index_chk_en:1;
	uint16	dat_pr_sel	:2;
	uint16	cmd_type	:2;
	uint16	cmd_index	:6;
	uint16	rsvd		:2;
};

struct sdmc_capabilities {
	uint32	timeout_clock_frequency	:6;	/* Timeout Clock Frequency		*/
	uint32	rsvd6			:1;	/* RSVD6				*/
	uint32	timeout_clock_unit	:1;	/* Timeout Clock Unit			*/
	uint32	base_clock_frequency_sd_clk:8;	/* Base Clock Frequency for SD Clock	*/
	uint32	max_block_length	:2;	/* Max Block Length			*/
	uint32	extended_media_bus_support:1;	/* Extended Media Bus Support		*/
	uint32	adma2_support		:1;	/* ADMA2 Support			*/
	uint32	rsvd5			:1;	/* RSVD5				*/
	uint32	high_speed_support	:1;	/* High Speed Support			*/
	uint32	sdma_support		:1;	/* SDMA Support				*/
	uint32	suspend_resume_support	:1;	/* Suspend / Resume Support		*/
	uint32	volt_support_3p3v	:1;	/* Voltage Support 3.3V			*/
	uint32	volt_support_3p0v	:1;	/* Voltage Support 3.0V			*/
	uint32	volt_support_1p8v	:1;	/* Voltage Support 1.8V			*/
	uint32	rsvd4			:1;	/* RSVD4				*/
	uint32	sys_bus_support_64b	:1;	/* 64-bit System Bus Support		*/
	uint32	async_int_support	:1;	/* Asynchronous Interrupt Support	*/
	uint32	slot_type		:2;	/* Slot Type				*/
};

/* Individual Bits in Control and Status Registers	*/

/* Normal Interrupt Status Enable */
#define SDMC_CMD_COMP_STAT_EN		0x0001	/* Command Complete Status Enable 	*/
#define SDMC_TX_COMP_STAT_EN		0x0002	/* Transfer Complete Status Enable	*/
#define SDMC_BLK_GAP_EVENT_STAT_EN	0x0004	/* Block Gap Event Status Enable	*/
#define SDMC_DMA_INT_STAT_EN		0x0008	/* DMA Interrupt Status Enable		*/
#define SDMC_BUF_WR_RDY_STAT_EN		0x0010	/* Buffer Write Ready Status Enable	*/
#define SDMC_BUF_RD_RDY_STAT_EN		0x0020	/* Buffer Read Ready Status Enable	*/
#define SDMC_CRD_INS_STAT_EN		0x0040	/* Card Insertion Status Enable		*/
#define SDMC_CRD_RMV_STAT_EN		0x0080	/* Card Removal Status Enable		*/
#define SDMC_CRD_INT_STAT_EN		0x0100	/* Card Interrupt Status Enable		*/
#define SDMC_INT_A_STAT_EN		0x0200	/* INT_A Status Enable			*/
#define SDMC_INT_B_STAT_EN		0x0400	/* INT_A Status Enable			*/
#define SDMC_INT_C_STAT_EN		0x0800	/* INT_A Status Enable			*/
#define SDMC_RE_TUNE_STAT_EN		0x1000	/* Re-Tuning Event Status Enable	*/

/* Normal Interrupt Signal Enable */
#define SDMC_CMD_COMP_SIG_EN		0x0001	/* Command Complete Signal Enable 	*/
#define SDMC_TX_COMP_SIG_EN		0x0002	/* Transfer Complete Signal Enable	*/
#define SDMC_BLK_GAP_EVENT_SIG_EN	0x0004	/* Block Gap Event Signal Enable	*/
#define SDMC_DMA_INT_SIG_EN		0x0008	/* DMA Interrupt Signal Enable		*/
#define SDMC_BUF_WR_RDY_SIG_EN		0x0010	/* Buffer Write Ready Signal Enable	*/
#define SDMC_BUF_RD_RDY_SIG_EN		0x0020	/* Buffer Read Ready Signal Enable	*/
#define SDMC_CRD_INS_SIG_EN		0x0040	/* Card Insertion Signal Enable		*/
#define SDMC_CRD_RMV_SIG_EN		0x0080	/* Card Removal Signal Enable		*/
#define SDMC_CRD_INT_SIG_EN		0x0100	/* Card Interrupt Signal Enable		*/
#define SDMC_INT_A_SIG_EN		0x0200	/* INT_A Signal Enable			*/
#define SDMC_INT_B_SIG_EN		0x0400	/* INT_A Signal Enable			*/
#define SDMC_INT_C_SIG_EN		0x0800	/* INT_A Signal Enable			*/
#define SDMC_RE_TUNE_SIG_EN		0x1000	/* Re-Tuning Event Signal Enable	*/

/* Normal Interrupt Status Register */
#define SDMC_NML_INT_CMD_COMP		0x0001	/* Command Complete			*/
#define SDMC_NML_INT_TX_COMP		0x0002	/* Transfer Complete			*/
#define SDMC_NML_INT_BLK_GAP_EVENT	0x0004	/* Block Gap Event			*/
#define SDMC_NML_INT_DMA_INT		0x0008	/* DMA Interrupt			*/
#define SDMC_NML_INT_BUF_WR_RDY		0x0010	/* Buffer Write Ready			*/
#define SDMC_NML_INT_BUF_RD_RDY		0x0020	/* Buffer Read Ready			*/
#define SDMC_NML_INT_CRD_INS		0x0040	/* Card Insertion			*/
#define SDMC_NML_INT_CRD_RM		0x0080	/* Card Removal				*/
#define SDMC_NML_INT_CRD_INT		0x0100	/* Card Interrupt			*/
#define SDMC_NML_INT_INT_A		0x0200	/* INT_A				*/
#define SDMC_NML_INT_INT_B		0x0400	/* INT_B				*/
#define SDMC_NML_INT_INT_C		0x0800	/* INT_C				*/
#define SDMC_NML_INT_RE_TUNE		0x1000	/* Re-Tuning Event			*/
#define SDMC_NML_INT_BOOT_CK_RCV	0x2000	/* Boot Acknowledge Received		*/
#define SDMC_NML_INT_BOOT_TER_INT	0x4000	/* Boot Terminate Interrupt		*/
#define SDMC_NML_INT_ERR_INT		0x8000	/* Error Interrupt			*/

/* Error Interrupt Status Enable */
#define SDMC_ERR_INT_CMD_TIMEOUT_ERR_STAT_EN	0x0001 	/* Command Timeout Error Status Enable	*/
#define SDMC_ERR_INT_CMD_CRC_ERR_STAT_EN	0x0002	/* Command CRC Error Status Enable	*/
#define SDMC_ERR_INT_CMD_END_BIT_ERR_STAT_EN	0x0004	/* Command End Bit Error Status Enable	*/
#define SDMC_ERR_INT_CMD_IND_ERR_STAT_EN	0x0008	/* Command Index Error Status Enable 	*/
#define SDMC_ERR_INT_DATA_TIMEOUT_ERR_STAT_EN	0x0010	/* Data Timeout Error Status Enable	*/
#define SDMC_ERR_INT_DATA_CRC_ERR_STAT_EN	0x0020	/* Data CRC Error Status Enable		*/
#define SDMC_ERR_INT_DATA_END_BIT_ERR_STAT_EN	0x0040	/* Data End Bit Error Status Enable	*/
#define SDMC_ERR_INT_CUR_LIMIT_ERR_STAT_EN	0x0080	/* Current Limit Error Status Enable	*/
#define SDMC_ERR_INT_CMD12_ERR_STAT_EN		0x0100	/* Auto CMD12 Error Status Enable	*/
#define SDMC_ERR_INT_ADMA_ERR_STAT_EN		0x0200	/* ADMA Error Status Enable		*/
#define SDMC_ERR_INT_TUNE_ERR_STAT_EN		0x0400	/* Tuning Error Status Enable		*/
#define SDMC_ERR_INT_TGT_RSP_ERR_EN		0x1000	/* Target Response Error Status Enable	*/
#define SDMC_ERR_INT_CEATA_ERR_EN		0x2000	/* CEATA Error Status Enable		*/

/* Error Interrupt Signal Enable */
#define SDMC_ERR_INT_CMD_TIMEOUT_ERR_SIG_EN	0x0001 	/* Command Timeout Error Signal Enable	*/
#define SDMC_ERR_INT_CMD_CRC_ERR_SIG_EN		0x0002	/* Command CRC Error Signal Enable	*/
#define SDMC_ERR_INT_CMD_END_BIT_ERR_SIG_EN	0x0004	/* Command End Bit Error Signal Enable	*/
#define SDMC_ERR_INT_CMD_IND_ERR_SIG_EN		0x0008	/* Command Index Error Signal Enable 	*/
#define SDMC_ERR_INT_DATA_TIMEOUT_ERR_SIG_EN	0x0010	/* Data Timeout Error Signal Enable	*/
#define SDMC_ERR_INT_DATA_CRC_ERR_SIG_EN	0x0020	/* Data CRC Error Signal Enable		*/
#define SDMC_ERR_INT_DATA_END_BIT_ERR_SIG_EN	0x0040	/* Data End Bit Error Signal Enable	*/
#define SDMC_ERR_INT_CUR_LIMIT_ERR_SIG_EN	0x0080	/* Current Limit Error Signal Enable	*/
#define SDMC_ERR_INT_CMD12_ERR_SIG_EN		0x0100	/* Auto CMD12 Error Signal Enable	*/
#define SDMC_ERR_INT_ADMA_ERR_SIG_EN		0x0200	/* ADMA Error Signal Enable		*/
#define SDMC_ERR_INT_TUNE_ERR_SIG_EN		0x0400	/* Tuning Error Signal Enable		*/
#define SDMC_ERR_INT_TGT_RSP_ERR_SIG_EN		0x1000	/* Target Response Error Signal Enable	*/
#define SDMC_ERR_INT_CEATA_ERR_SIG_EN		0x2000	/* CEATA Error Signal Enable		*/

/* Error Interrupt Status Register */
#define SDMC_ERR_INT_CMD_TIMEOUT_ERR		0x0001	/* Command Timeout Error		*/
#define SDMC_ERR_INT_CMD_CRC_ERR		0x0002	/* Command CRC Error			*/
#define SDMC_ERR_INT_CMD_END_BIT_ERR		0x0004	/* Command End Bit Error		*/
#define SDMC_ERR_INT_CMD_INDEX_ERR		0x0008	/* Command Index Error			*/
#define SDMC_ERR_INT_DATA_TIMEOUT_ERR		0x0010	/* Data Timeout Error			*/
#define SDMC_ERR_INT_DATA_CRC_ERR		0x0020	/* Data CRC Error			*/
#define SDMC_ERR_INT_DATA_END_BIT_ERR		0x0040	/* Data End Bit Error			*/
#define SDMC_ERR_INT_CUR_LIMIT_ERR		0x0080	/* Current Limit Error			*/
#define SDMC_ERR_INT_CMD12_ERR			0x0100	/* Auto CMD Error			*/
#define SDMC_ERR_INT_ADMA_ERR			0x0200	/* ADMA Error				*/
#define SDMC_ERR_INT_TUNE_ERR			0x0400	/* Tuning Error				*/
#define SDMC_ERR_INT_TGT_RSP_ERR		0x1000	/* Target Response Error		*/
#define SDMC_ERR_INT_CEATA_ERR			0x2000	/* CEATA Error				*/

/* Software Reset Register */
#define SDMC_SW_RST_ALL			0x01	/* Software Reset For All		*/
#define SDMC_SW_RST_CMD_LN		0x02	/* Software Reset For CMD Line		*/
#define SDMC_SW_RST_DAT_LN		0x04	/* Software Reset for DAT Line		*/

/* Capabilities Register */
#define SDMC_CAP_VOLT_SUPPORT_3P3V	0x01000000	/* Voltage Support 3.3V		*/
#define SDMC_CAP_VOLT_SUPPORT_3P0V	0x02000000	/* Voltage Support 3.0V		*/
#define SDMC_CAP_VOLT_SUPPORT_1P8V	0x04000000	/* Voltage Support 1.8V		*/

/* Present State Status Register */
#define SDMC_PRE_STATE_CMD_INHIBIT_CMD	0x00000001	/* Command Inhibit		*/
#define SDMC_PRE_STATE_CMD_INHIBIT_DAT	0x00000002	/* Command Inhibit DAT		*/
#define SDMC_PRE_STATE_CRD_INS		0x00010000	/* Card Inserted		*/
#define SDMC_PRE_STATE_DATA_LN_SIG_LVL	0x00F00000	/* Line Signal Level		*/

/* Clock Control Register */
#define SDMC_CLK_CTL_INT_CLK_EN		0x0001	/* Internal Clock Enable		*/
#define SDMC_CLK_CTL_INT_CLK_STABLE	0x0002	/* Internal Clock Stable		*/
#define SDMC_CLK_CTL_SD_CLK_EN		0x0004	/* SD Clock Enable			*/
#define SDMC_CLK_CTL_CLK_GEN_SEL	0x0020	/* Clock Generator Select		*/
#define SDMC_CLK_CTL_SD_FREQ_HIGH_MASK	0x00FF	/* Mask to set SD clock frequency to max*/

/* Power Control Register */
#define SDMC_PWR_CTL_SD_BUS_PWR			0x01	/* SD Bus Power			*/
#define SDMC_PWR_CTL_HW_RST			0x10	/* HW Reset			*/
#define SDMC_PWR_CTL_SD_BUS_VOL_SEL_CLR		0xF1	/* Clear mask for SD bus voltage*/
#define SDMC_PWR_CTL_SD_BUS_VOL_SEL_3P3V	0x0E	/* Set mask for 3.3V		*/
#define SDMC_PWR_CTL_SD_BUS_VOL_SEL_3P0V	0x0C	/* Set mask for 3.0V		*/
#define SDMC_PWR_CTL_SD_BUS_VOL_SEL_1P8V	0x0A	/* Set mask for 1.8V		*/

/* Card Status Response (R1) */
#define SDMC_R1_AKE_SEQ_ERROR		0x00000008	/* Authentication Error		*/
#define SDMC_R1_APP_CMD			0x00000020	/* Next command is application	*/
#define SDMC_R1_READY_FOR_DATA		0x00000100	/* Card ready for data		*/
#define SDMC_R1_CURRENT_STATE		0x00001E00	/* Current card state		*/
#define SDMC_R1_ERASE_RESET		0x00002000	/* Erase processes reset	*/
#define SDMC_R1_CARD_ECC_DISABLED	0x00004000	/* Command without ECC		*/
#define SDMC_R1_WP_ERASE_SKIP		0x00008000	/* Write protected		*/
#define SDMC_R1_CSD_OVERWRITE		0x00010000	/* Error in CSD overwrite	*/
#define SDMC_R1_ERROR			0x00080000	/* Unknown error		*/
#define SDMC_R1_CC_ERROR		0x00100000	/* Internal controller error	*/
#define SDMC_R1_CARD_ECC_FAILED		0x00200000	/* ECC correction failed	*/
#define SDMC_R1_ILLEGAL_COMMAND		0x00400000	/* Not a legal command		*/
#define SDMC_R1_COM_CRC_ERROR		0x00800000	/* Previous command CRC failed	*/
#define SDMC_R1_LOCK_UNLOCK_FAILED	0x01000000	/* Lock/unlock of card failed	*/
#define SDMC_R1_CARD_IS_LOCKED		0x02000000	/* Card is locked		*/
#define SDMC_R1_WP_VIOLATION		0x04000000	/* Write to protected block	*/
#define SDMC_R1_ERASE_PARAM		0x08000000	/* Invalid erase parameter	*/
#define SDMC_R1_ERASE_SEQ_ERROR		0x10000000	/* Invalid erase sequence	*/
#define SDMC_R1_BLOCK_LEN_ERROR		0x20000000	/* TX block length not allowed	*/
#define SDMC_R1_ADDRESS_ERROR		0x40000000	/* Misaligned address		*/
#define SDMC_R1_OUT_OF_RANGE		0x80000000	/* Argument out of range	*/

/* Card state */
#define SDMC_R1_IDLE_STATE		0x00000000	/* Idle State			*/
#define SDMC_R1_READY_STATE		0x00000200	/* Ready State			*/
#define SDMC_R1_IDENT_STATE		0x00000400	/* Identification State		*/
#define SMDC_R1_STBY_STATE		0x00000600	/* Standby State		*/
#define SDMC_R1_TRAN_STATE		0x00000800	/* Transfer State		*/
#define SDMC_R1_DATA_STATE		0x00000A00	/* Sending-data State		*/
#define SDMC_R1_RCV_STATE		0x00000C00	/* Receive-data State		*/
#define SDMC_R1_PRG_STATE		0x00000E00	/* Programming State		*/
#define SMDC_R1_DIS_STATE		0x00001000	/* Disconnect State		*/

/* Operating Conditions Register Response (R3) */
#define SDMC_R3_2P7_2P8			0x00008000	/* Voltage Range 2.7V - 2.8V	*/
#define SDMC_R3_2P8_2P9			0x00010000	/* Voltage Range 2.8V - 2.9V	*/
#define SDMC_R3_2P9_3P0			0x00020000	/* Voltage Range 2.9V - 3.0V	*/
#define SDMC_R3_3P0_3P1			0x00040000	/* Voltage Range 3.0V - 3.1V	*/
#define SDMC_R3_3P1_3P2			0x00080000	/* Voltage Range 3.1V - 3.2V	*/
#define SDMC_R3_3P2_3P3			0x00100000	/* Voltage Range 3.2V - 3.3V	*/
#define SDMC_R3_3P3_3P4			0x00200000	/* Voltage Range 3.3V - 3.4V	*/
#define SDMC_R3_3P4_3P5			0x00400000	/* Voltage Range 3.4V - 3.5V	*/
#define SDMC_R3_3P5_3P6			0x00800000	/* Voltage Range 3.5V - 3.6V	*/
#define SDMC_R3_S18A			0x01000000	/* Switching to 1.8V accepted	*/
#define SDMC_R3_UHS_II_STS		0x20000000	/* UHS-II card status		*/
#define SDMC_R3_CCS			0x40000000	/* Card capacity status		*/
#define SDMC_R3_BUSY			0x80000000	/* Card power up status (busy)	*/

/* Published RCA Response (R6) */
#define SDMC_R6_RCA_MASK		0xFFFF0000	/* Relative card address mask	*/
#define SDMC_R6_AKE_SEQ_ERROR		0x00000008	/* Authentication Error		*/
#define SDMC_R6_APP_CMD			0x00000020	/* Next command is application	*/
#define SDMC_R6_READY_FOR_DATA		0x00000100	/* Card ready for data		*/
#define SDMC_R6_CURRENT_STATE		0x00001E00	/* Current card state		*/
#define SDMC_R6_ERROR			0x00002000	/* Unknown error		*/
#define SDMC_R6_ILLEGAL_COMMAND		0x00004000	/* Not a legal command		*/
#define SDMC_R6_COM_CRC_ERROR		0x00008000	/* Previous command CRC failed	*/

/* ACMD41 Argument flags */
#define SDMC_OCR_MASK			0x00FFFF00	/* OCR set mask			*/
#define SDMC_ACMD41_S18R		0x01000000	/* Switching to 1.8V request	*/
#define SDMC_ACMD41_XPC			0x10000000	/* Extended capacity power ctl	*/
#define SDMC_ACMD41_HCS			0x40000000	/* High capacity card support	*/

#define SDMC_ABT			0x0CC0	/* CMD12 - Abort			*/
#define SDMC_CMD0			0x0000	/* CMD0 - Go Idle State	(Reset)		*/
#define SDMC_CMD2			0x0209	/* CMD2 - All send card identifier	*/
#define SDMC_CMD3			0x031A	/* CMD3 - Send relative card address	*/
#define SDMC_CMD7			0x071B	/* CMD7 - Select/Deselect card		*/
#define SDMC_CMD8			0x081A	/* CMD8 - Voltage check			*/
#define SDMC_CMD9			0x0909	/* CMD9 - Send CSD			*/
#define SDMC_CMD10			0x0A09	/* CMD10 - Send CID			*/
#define SDMC_CMD12			0x0C1B	/* CMD12 - Stop Transmission		*/
#define SDMC_CMD13			0x0D1A	/* CMD13 - Send status			*/
#define SDMC_CMD16			0x101A	/* CMD16 - Set block length		*/
#define SDMC_CMD17			0x113A	/* CMD17 - Single block read		*/
#define SDMC_CMD55			0x371A	/* CMD55 - Application specific command	*/
#define SDMC_ACMD41			0x2902	/* ACMD41 - Card Initialization/Inquiry	*/

#define SDMC_CMD_DELAY			100000		/* Delay in micro secs	*/

#define SDMC_RC_OK			(0)
#define SDMC_RC_RECOVERABLE_ERR		(-1)
#define SDMC_RC_NON_RECOVERABLE_ERROR	(-2)

#define SDMC_CMD_NO_FLAGS		0x00	/* No flags used with the command	*/
#define SDMC_CMD_NO_ERR_RCVY		0x01	/* Do not perform error recovery	*/
#define SDMC_CMD_DAT_TRNS		0x02	/* Wait for data transmission		*/

devcall sdmc_issue_cmd_sync (volatile struct sdmc_csreg *csrptr, uint16 cmd_value, uint32 arg_value, uint16* error_sts, uint8 flags);
devcall sdmc_issue_cmd_async (volatile struct sdmc_csreg *csrptr, uint16 cmd_value, uint32 arg_value);
devcall sdmc_finalize_cmd_async (volatile struct sdmc_csreg *csrptr, uint16* error_sts);