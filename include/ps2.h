/* ps2.h - definitions for the PS/2 controller */

#define PS2_DATA_PORT           0x60
#define PS2_CTRL_STATUS_PORT    0x64

/* Status Register Bits */
#define OBF                     0x01    // (Output Buffer Full) - Indicates when it's okay to write to output buffer
                                        // 0: Output buffer empty - Okay to write to port 0x60
                                        // 1: Output buffer full - Don't write to port 0x60
#define IBF                     0x02    // (Input Buffer Full) - Indicates when input is available in the input buffer.
                                        // 0: Input buffer empty - No unread input at port 0x60
                                        // 1: Input buffer full - New input can be read from port 0x60

/* Controller Commands */
#define READ_COMMAND            0x20    // Read 8042 Command Byte: current 8042 command byte is placed in port 60h.
#define WRITE_COMMAND           0x60    // Write 8042 Command Byte: next data byte written to port 60h is
	                                    // placed in 8042 command register.	Format:

	                                    // |7|6|5|4|3|2|1|0|	8042 Command Byte
	                                    // 	| | | | | | | `---- 1=enable output register full interrupt
	                                    // 	| | | | | | `----- should be 0
	                                    // 	| | | | | `------ 1=set status register system, 0=clear
	                                    // 	| | | | `------- 1=override keyboard inhibit, 0=allow inhibit
	                                    // 	| | | `-------- disable keyboard I/O by driving clock line low
	                                    // 	| | `--------- disable auxiliary device, drives clock line low
	                                    // 	| `---------- IBM scancode translation 0=AT, 1=PC/XT
	                                    // 	 ----------- reserved, should be 0


#define ENABLE_AUX_INTERFACE    0xA8    // Enable Auxiliary Interface: clears Bit 5 of command register
#define ENABLE_KBD_INTERFACE    0xAE    // Enable Keyboard Interface: clears Bit 4 of command register
	                                    // enabling keyboard interface.
#define WRITE_AUX_DEVICE        0xD4    // Write Auxiliary Device: on PS/2 systems the next data byte
                                        // written to input register a port at 60h is sent to the
                                        // auxiliary device

/* Mouse Command Set */
#define SET_DEFAULTS            0xF6    // Disables streaming, sets the packet rate to 100 per second,
                                        // and resolution to 4 pixels per mm.
#define ENABLE_PACKET_STREAMING 0xF4    // The mouse starts sending automatic packets when the mouse
                                        // moves or is clicked.

/* Command Keyboard Controller bits definitions */
#define INT                     0x01    // INT (Input Buffer Full Interrupt) - When set, IRQ 1 is generated when data is
                                        // available in the input buffer
#define INT2                    0x02    // INT2 (Mouse Input Buffer Full Interrupt) - When set, IRQ 12 is generated when
                                        //  mouse data is available.
