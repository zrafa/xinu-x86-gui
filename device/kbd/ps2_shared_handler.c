#include <xinu.h>

#define PS2_DATA           0x60
#define PS2_STATUS         0x64
#define PS2_COMMAND        0x64

#define PS2_DISABLE_PORT2  0xA7
#define PS2_ENABLE_PORT2   0xA8
#define PS2_DISABLE_PORT1  0xAD
#define PS2_ENABLE_PORT1   0xAE


int ps2_wait_input(void) {
	uint64 timeout = 100000UL;
	while (--timeout) {
		if (!(inportb(PS2_STATUS) & (1 << 1))) return 0;
	}
	return 1;
}

void ps2_command(uint8 cmdbyte) {
	ps2_wait_input();
	outportb(PS2_COMMAND, cmdbyte);
}

void ps2_shared_handler(void) {
	/* Disable both ports */
	ps2_command(PS2_DISABLE_PORT1);
	ps2_command(PS2_DISABLE_PORT2);
	/* Read the status and data */
	uint8 status = inportb(PS2_STATUS);
	uint8 data_byte = inportb(PS2_DATA);
	/* Re-enable both */
	ps2_command(PS2_ENABLE_PORT1);
	ps2_command(PS2_ENABLE_PORT2);

	// irq_ack(r->int_no-32);

	if (!(status & 0x01)) return ;

	if (!(status & 0x20)) {
		kbdhandler(data_byte);
	} else if (status & 0x21) {
		micehandler(data_byte);
	}
}
