#define TIMEOUT_DELAY	200000

#include <stdio.h>

#include <RTL.h>
#include <rl_usb.h>
#include <stm32f10x.h>

#define  __NO_USB_LIB_C
#include "usb_config.c"

#include "DAP_config.h"
#include "DAP.h"

#include "usbd_user_cdc_acm.h"

uint8_t usbd_hid_process(void);
void CheckUserApplication(void);

void LedConnectedOut(uint16_t bit);
void LedRunningOut(uint16_t bit);

void Delay_ms(uint32_t delay);

void NotifyOnStatusChange (void);
void BoardInit(void);

extern const UserAppDescriptor_t UserAppDescriptor;

UserAppDescriptor_t * pUserAppDescriptor = NULL;

const CoreDescriptor_t CoreDescriptor = {
	&LedConnectedOut,
	&LedRunningOut,
};

#if (USBD_CDC_ACM_ENABLE == 1)
	int32_t usb_rx_ch;
	int32_t usb_tx_ch;
#endif

uint32_t led_count;
uint32_t led_timeout;

/**
  * @brief	LED functions
  *
  */
#if defined( BOARD_V3 )
// active Low
void LedConnectedOn(void) {LED_CONNECTED_PORT->BRR = LED_CONNECTED;}
void LedConnectedOff(void) {LED_CONNECTED_PORT->BSRR  = LED_CONNECTED;}
void LedRunningOn(void) {LED_RUNNING_PORT->BRR =LED_RUNNING;}
void LedRunningOff(void) {LED_RUNNING_PORT->BSRR =LED_RUNNING;}
#else
// active High
void LedConnectedOn(void) {LED_CONNECTED_PORT->BSRR = LED_CONNECTED;}
void LedConnectedOff(void) {LED_CONNECTED_PORT->BRR = LED_CONNECTED;}
void LedRunningOn(void) {LED_RUNNING_PORT->BSRR = LED_RUNNING;}
void LedRunningOff(void) {LED_RUNNING_PORT->BRR = LED_RUNNING;}
#endif

void LedConnectedToggle(void) {LED_CONNECTED_PORT->ODR ^= LED_CONNECTED;}
void LedRunningToggle(void) {LED_RUNNING_PORT->ODR ^= LED_RUNNING;}

void LEDS_SETUP (void)
{
	const GPIO_InitTypeDef LED_CONNECTED_INIT = {
		LED_CONNECTED,
		GPIO_Speed_2MHz,
		GPIO_Mode_Out_PP
	};
	const GPIO_InitTypeDef LED_RUNNING_INIT = {
		LED_RUNNING,
		GPIO_Speed_2MHz,
		GPIO_Mode_Out_PP
	};

	GPIO_INIT(LED_CONNECTED_PORT, LED_CONNECTED_INIT);
	LedConnectedOff();

	GPIO_INIT(LED_RUNNING_PORT, LED_RUNNING_INIT);
	LedRunningOff();
}

void LedConnectedOut(uint16_t bit)
{
	if (bit & 1)
	{
		LedConnectedOn();
	}
	else
	{
		LedConnectedOff();
	}
}
void LedRunningOut(uint16_t bit)
{
	if (bit & 1)
	{
		LedRunningOn();
	}
	else
	{
		LedRunningOff();
	}
}

/**
  * @brief	Main
  *
  */
int main(void)
{
	BoardInit();
	SystemCoreClockUpdate();

	LedConnectedOn();
	if (UserAppDescriptor.UserInit != NULL)
	{
		pUserAppDescriptor = &UserAppDescriptor;
		pUserAppDescriptor->UserInit((CoreDescriptor_t *)&CoreDescriptor);
	}
	LedConnectedOff();

	Delay_ms(10);

	// USB Device Initialization and connect
	usbd_init();
	usbd_connect(__TRUE);

	led_count = 0;
	while (!usbd_configured())	// Wait for USB Device to configure
	{
		if (led_count++ == 0)
		{
			LedConnectedOn();
		}
		else if (led_count == 5)
		{
			LedConnectedOff();
		}
		else if (led_count == 50)
		{
			led_count = 0;
		}
		Delay_ms(10);
	}
	LedConnectedOff();
	Delay_ms(100);				// Wait for 100ms

	led_count = 0;
	led_timeout = TIMEOUT_DELAY;

#if (USBD_CDC_ACM_ENABLE == 1)
	usb_rx_ch = -1;
	usb_tx_ch = -1;
#endif

while (1)
	{
		if (pUserAppDescriptor == NULL)
		{
			// No user application
			if (led_count++ == 1000000)
			{
				led_count = 0;
				LedConnectedToggle();
			}
			usbd_hid_process();
		}
		else if (!usbd_hid_process())
		{
			// No packet processing
			if (led_timeout == 1000)
			{
				LedConnectedOn();
			}
			else if (led_timeout == 0)
			{
				LedConnectedOff();
				led_timeout = TIMEOUT_DELAY;
			}
			led_timeout--;

		}
		else
		{
			led_timeout = TIMEOUT_DELAY;
		}

#if (USBD_CDC_ACM_ENABLE == 1)

		NotifyOnStatusChange();

		// USB -> UART
		if (usb_rx_ch == -1)
		{
			usb_rx_ch = USBD_CDC_ACM_GetChar();
		}

		if (usb_rx_ch != -1)
		{
			if (UART_PutChar (usb_rx_ch) == usb_rx_ch)
			{
				usb_rx_ch = -1;
			}
		}

		// UART -> USB
		if (usb_tx_ch == -1)
		{
			usb_tx_ch = UART_GetChar();
		}

		if (usb_tx_ch != -1)
		{
			if (USBD_CDC_ACM_PutChar(usb_tx_ch) == usb_tx_ch)
			{
				usb_tx_ch = -1;
			}
		}
#endif
	}
}

// Delay for specified time
//    delay:  delay time in ms
void Delay_ms(uint32_t delay)
{
	delay *= (CPU_CLOCK / 1000 + (DELAY_SLOW_CYCLES - 1)) / (2 * DELAY_SLOW_CYCLES);
	PIN_DELAY_SLOW(delay);
}

extern uint32_t __Vectors;

void HardFault_Handler(void);
void NMI_Handler(void)			__attribute((alias("HardFault_Handler")));
void MemManage_Handler(void)	__attribute((alias("HardFault_Handler")));
void BusFault_Handler(void)		__attribute((alias("HardFault_Handler")));
void UsageFault_Handler(void)	__attribute((alias("HardFault_Handler")));
void SVC_Handler(void)			__attribute((alias("HardFault_Handler")));
void DebugMon_Handler(void)		__attribute((alias("HardFault_Handler")));
void PendSV_Handler(void)		__attribute((alias("HardFault_Handler")));

void HardFault_Handler(void)
{
	__disable_irq();
	__set_MSP(__Vectors);
	LEDS_SETUP();
	{
		register int count;
		for (count = 0; count < 5; count++)
		{
			LedRunningOn();
			Delay_ms(250);
			LedRunningOff();

			LedConnectedOn();
			Delay_ms(250);
			LedConnectedOff();

			Delay_ms(1000);
		}
	}
	NVIC_SystemReset();
}


/* Control USB connecting via SW	*/
void PORT_USB_CONNECT_SETUP(void)
{
#ifdef PIN_USB_CONNECT_PORT
	const GPIO_InitTypeDef INIT_PIN_USB_CONNECT = {
		PIN_USB_CONNECT,
		GPIO_Speed_2MHz,
		PIN_USB_MODE
	};

	GPIO_INIT(PIN_USB_CONNECT_PORT, INIT_PIN_USB_CONNECT);
	PIN_USB_CONNECT_ON();
#endif
}

void send_char(char ch)
{
#if (USBD_CDC_ACM_ENABLE == 1)
	if (ch == '\n')
		send_char('\r');
	while (USBD_CDC_ACM_PutChar(ch) != ch)
	{ }
#endif
}

PUTCHAR_PROTOTYPE
{
	send_char(ch);
	return ch;
}


#if ( DAP_SWD != 0 )
/** Setup SWD I/O pins: SWCLK, SWDIO, and nRESET.
	Configures the DAP Hardware I/O pins for Serial Wire Debug (SWD) mode:
	 - SWCLK, SWDIO, nRESET to output mode and set to default high level.
	 - TDI, TDO, nTRST to HighZ mode (pins are unused in SWD mode).
*/
void PORT_SWD_SETUP()
{
	const GPIO_InitTypeDef PIN_SWCLK_TCK_INIT = {
		PIN_SWCLK_TCK,
		GPIO_Speed_50MHz,
		GPIO_Mode_Out_PP
	};
	const GPIO_InitTypeDef PIN_SWDIO_TMS_INIT = {
		PIN_SWDIO_TMS,
		GPIO_Speed_50MHz,
		GPIO_Mode_Out_PP
	};
	const GPIO_InitTypeDef PIN_TDO_INIT = {
		PIN_TDO,
		(GPIOSpeed_TypeDef)0,
		GPIO_Mode_IPU
	};
	const GPIO_InitTypeDef PIN_TDI_INIT = {
		PIN_TDI,
		(GPIOSpeed_TypeDef)0,
		GPIO_Mode_IPU
	};
	const GPIO_InitTypeDef PIN_nRESET_INIT = {
		PIN_nRESET,
		(GPIOSpeed_TypeDef)0,
		GPIO_Mode_IPU
	};

	GPIO_INIT(PIN_SWCLK_TCK_PORT, PIN_SWCLK_TCK_INIT);
	PIN_SWCLK_TCK_PORT->BSRR = PIN_SWCLK_TCK;

	GPIO_INIT(PIN_SWDIO_TMS_PORT, PIN_SWDIO_TMS_INIT);
	PIN_SWDIO_TMS_PORT->BSRR = PIN_SWDIO_TMS;

	GPIO_INIT(PIN_TDO_PORT, PIN_TDO_INIT);
	PIN_TDO_PORT->BSRR = PIN_TDO;

	GPIO_INIT(PIN_TDI_PORT, PIN_TDI_INIT);
	PIN_TDI_PORT->BSRR = PIN_TDI;

	GPIO_INIT(PIN_nRESET_PORT, PIN_nRESET_INIT);
	PIN_nRESET_LOW();
	Delayms(1);
	PIN_nRESET_HIGH();
}
#endif


#if ( DAP_JTAG != 0 )
/** Setup JTAG I/O pins: TCK, TMS, TDI, TDO, nTRST, and nRESET.
	Configures the DAP Hardware I/O pins for JTAG mode:
	- TCK, TMS, TDI, nTRST, nRESET to output mode and set to high level.
	- TDO to input mode.
*/
void PORT_JTAG_SETUP()
{
	const GPIO_InitTypeDef PIN_SWCLK_TCK_INIT = {
		PIN_SWCLK_TCK,
		GPIO_Speed_50MHz,
		GPIO_Mode_Out_PP
	};
	const GPIO_InitTypeDef PIN_SWDIO_TMS_INIT = {
		PIN_SWDIO_TMS,
		GPIO_Speed_50MHz,
		GPIO_Mode_Out_PP
	};
	const GPIO_InitTypeDef PIN_TDO_INIT = {
		PIN_TDO,
		(GPIOSpeed_TypeDef)0,
		GPIO_Mode_IPU
	};
	const GPIO_InitTypeDef PIN_TDI_INIT = {
		PIN_TDI,
		GPIO_Speed_50MHz,
		GPIO_Mode_Out_PP
	};
	const GPIO_InitTypeDef PIN_nRESET_INIT = {
		PIN_nRESET,
		(GPIOSpeed_TypeDef)0,
		GPIO_Mode_IPU
	};

	GPIO_INIT(PIN_SWCLK_TCK_PORT, PIN_SWCLK_TCK_INIT);
	PIN_SWCLK_TCK_PORT->BSRR = PIN_SWCLK_TCK;

	GPIO_INIT(PIN_SWDIO_TMS_PORT, PIN_SWDIO_TMS_INIT);
	PIN_SWDIO_TMS_PORT->BSRR = PIN_SWDIO_TMS;

	GPIO_INIT(PIN_TDO_PORT, PIN_TDO_INIT);
	PIN_TDO_PORT->BSRR = PIN_TDO;

	GPIO_INIT(PIN_TDI_PORT, PIN_TDI_INIT);
	PIN_TDI_PORT->BSRR = PIN_TDI;

	GPIO_INIT(PIN_nRESET_PORT, PIN_nRESET_INIT);
	PIN_nRESET_LOW();
	Delayms(1);
	PIN_nRESET_HIGH();
}
#endif

/** Disable JTAG/SWD I/O Pins.
Disables the DAP Hardware I/O pins which configures:
 - TCK/SWCLK, TMS/SWDIO, TDI, TDO, nTRST, nRESET to High-Z mode.
*/
void PORT_OFF()
{
	const GPIO_InitTypeDef PIN_SWCLK_TCK_OFF_INIT = {
		PIN_SWCLK_TCK,
		(GPIOSpeed_TypeDef)0,
		GPIO_Mode_IPD
	};
	const GPIO_InitTypeDef PIN_SWDIO_TMS_OFF_INIT = {
		PIN_SWDIO_TMS,
		(GPIOSpeed_TypeDef)0,
		GPIO_Mode_IPU
	};
	const GPIO_InitTypeDef PIN_TDI_OFF_INIT = {
		PIN_TDI,
		(GPIOSpeed_TypeDef)0,
		GPIO_Mode_IPU
	};
	const GPIO_InitTypeDef PIN_TDO_OFF_INIT = {
		PIN_TDO,
		(GPIOSpeed_TypeDef)0,
		GPIO_Mode_IPU
	};
	const GPIO_InitTypeDef PIN_nRESET_OFF_INIT = {
		PIN_nRESET,
		(GPIOSpeed_TypeDef)0,
		GPIO_Mode_IPU
	};

	GPIO_INIT(PIN_SWCLK_TCK_PORT, PIN_SWCLK_TCK_OFF_INIT);
	GPIO_INIT(PIN_SWDIO_TMS_PORT, PIN_SWDIO_TMS_OFF_INIT);
	GPIO_INIT(PIN_TDO_PORT, PIN_TDO_OFF_INIT);
	GPIO_INIT(PIN_TDI_PORT, PIN_TDI_OFF_INIT);
	GPIO_INIT(PIN_nRESET_PORT, PIN_nRESET_OFF_INIT);
}

#if (USBD_CDC_ACM_ENABLE == 1)

/* Check if status has changed and if so, send notify to USB Host on Int EP   */
void NotifyOnStatusChange (void)
{
	static int32_t old_notify = -1;
	int32_t status, notify = 0;

	status = UART_GetCommunicationErrorStatus();

	if (status & UART_OVERRUN_ERROR_Msk)
		notify |= CDC_SERIAL_STATE_OVERRUN;
	if (status & UART_PARITY_ERROR_Msk )
		notify |= CDC_SERIAL_STATE_OVERRUN;
	if (status & UART_FRAMING_ERROR_Msk)
		notify |= CDC_SERIAL_STATE_FRAMING;

	status = UART_GetStatusLineState();

	if (status & UART_STATUS_LINE_RI_Msk )
		notify |= CDC_SERIAL_STATE_RING;
	if (status & UART_STATUS_LINE_DSR_Msk)
		notify |= CDC_SERIAL_STATE_TX_CARRIER;
	if (status & UART_STATUS_LINE_DCD_Msk)
		notify |= CDC_SERIAL_STATE_RX_CARRIER;

	if (UART_GetBreak())
		notify |= CDC_SERIAL_STATE_BREAK;

	if (notify ^ old_notify)				// If notify changed
	{
		if (USBD_CDC_ACM_Notify (notify))   // Send new notification
		{
			old_notify = notify;
		}
	}
}

#endif

void BoardInit(void)
{
	const GPIO_InitTypeDef INIT_PINS_A = {
		(	GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
			GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |
			GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
	//		GPIO_Pin_11 | GPIO_Pin_12 | 	// USB pins
	//		GPIO_Pin_13 | GPIO_Pin_14 | 	// SWD pins
			GPIO_Pin_15
		),
		GPIO_Speed_2MHz,
		GPIO_Mode_IN_FLOATING
	};
	const GPIO_InitTypeDef INIT_PINS_B = {
		GPIO_Pin_All,
		GPIO_Speed_2MHz,
		GPIO_Mode_IN_FLOATING
	};
	const GPIO_InitTypeDef INIT_PINS_C = {
		(GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15),
		GPIO_Speed_2MHz,
		GPIO_Mode_IN_FLOATING
	};

	// Enable GPIOA-GPIOC
	RCC->APB2ENR |= (RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN);

#if defined (UIDISK_V1)
	// Full SWJ Disabled
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
#else
	// Enable SWJ only
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
#endif

	GPIO_INIT(GPIOA, INIT_PINS_A);
	GPIO_INIT(GPIOB, INIT_PINS_B);
	GPIO_INIT(GPIOC, INIT_PINS_C);

	PORT_USB_CONNECT_SETUP();
	LEDS_SETUP();
}

void USBD_Error_Event(void)
{
	LedConnectedOn();
	LedRunningOn();

	usbd_connect(__FALSE);
	usbd_reset_core();

	HardFault_Handler();
}


/** nRESET I/O pin: Set Output.
\param bit target device hardware reset pin status:
           - 0: issue a device hardware reset.
           - 1: release device hardware reset.
*/
void PIN_nRESET_OUT(uint8_t bit)
{
 	if (bit & 1)
	{
		PIN_nRESET_HIGH();
	}
	else
	{
		PIN_nRESET_LOW();
	}
}

