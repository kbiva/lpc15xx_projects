/*
 * uart.c
 *
 *  Created on: 2014.07.04
 *      Author: Kestutis Bivainis
 */

#include "board.h"
#include "uart.h"
#include <string.h>

/* UART handle and memory for ROM API */
static UART_HANDLE_T *uartHandle;

/* Use a buffer size larger than the expected return value of
   uart_get_mem_size() for the static UART handle type */
static uint32_t uartHandleMEM[0x10];

/* Receive buffer */
#define RECV_BUFF_SIZE 32
static char recv_buf[RECV_BUFF_SIZE];

/* ASCII code for escapre key */
#define ESCKEY 27

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* UART Pin mux function - note that SystemInit() may already setup your
   pin muxing at system startup */
void init_UART0_pinmux(void)
{
	/* UART signals on pins PIO0_13 (FUNC0, U0_TXD) and PIO0_18 (FUNC0, U0_RXD) */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 13, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGMODE_EN));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 18, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGMODE_EN));

	/* UART signal muxing via SWM */
	Chip_SWM_MovablePortPinAssign(SWM_UART0_RXD_I, 0, 13);
	Chip_SWM_MovablePortPinAssign(SWM_UART0_TXD_O, 0, 18);
}

/* Turn on LED to indicate an error */
static void errorUART(void)
{
	Board_LED_Set(0, true);
	while (1) {}
}

/* Setup UART handle and parameters */
void setup_UART(void)
{
	uint32_t errCode;

	/* 115.2KBPS, 8N1, ASYNC mode, no errors, clock filled in later */
	UART_CONFIG_T cfg = {
		0,				/* U_PCLK frequency in Hz */
		115200,		/* Baud Rate in Hz */
		1,				/* 8N1 */
		0,				/* Asynchronous Mode */
		NO_ERR_EN	/* Enable No Errors */
	};

	/* Initialize UART0 */
	Chip_UART_Init(LPC_USART0);

	Chip_Clock_SetUARTFRGDivider(1);

	/* Perform a sanity check on the storage allocation */
	if (LPC_UARTD_API->uart_get_mem_size() > sizeof(uartHandleMEM)) {
		/* Example only: this should never happen and probably isn't needed for
		   most UART code. */
		errorUART();
	}

	/* Setup the UART handle */
	uartHandle = LPC_UARTD_API->uart_setup((uint32_t) LPC_USART0, (uint8_t *) &uartHandleMEM);
	if (uartHandle == NULL) {
		errorUART();
	}

	/* Need to tell UART ROM API function the current UART peripheral clock
	     speed */
	cfg.sys_clk_in_hz = Chip_Clock_GetSystemClockRate();

	/* Initialize the UART with the configuration parameters */
	errCode = LPC_UARTD_API->uart_init(uartHandle, &cfg);
	if (errCode != LPC_OK) {
		/* Some type of error handling here */
		errorUART();
	}
}

/* Send a string on the UART terminated by a NULL character using
   polling mode. */
void putline_UART(const char *send_data)
{
	UART_PARAM_T param;

	param.buffer = (uint8_t *) send_data;
	param.size = strlen(send_data);

	/* Polling mode, do not append CR/LF to sent data */
	param.transfer_mode = TX_MODE_SZERO;
	param.driver_mode = DRIVER_MODE_POLLING;

	/* Transmit the data */
	if (LPC_UARTD_API->uart_put_line(uartHandle, &param)) {
		errorUART();
	}
}

/* Receive a string on the UART terminated by a LF character using
   polling mode. */
static void getLineUART(char *receive_buffer, uint32_t length)
{
	UART_PARAM_T param;

	param.buffer = (uint8_t *) receive_buffer;
	param.size = length;

	/* Receive data up to the CR/LF character in polling mode. Will
	   truncate at length if too long.	*/
	param.transfer_mode = RX_MODE_CRLF_RECVD;
	param.driver_mode = DRIVER_MODE_POLLING;

	/* Receive the data */
	if (LPC_UARTD_API->uart_get_line(uartHandle, &param)) {
		errorUART();
	}
}
