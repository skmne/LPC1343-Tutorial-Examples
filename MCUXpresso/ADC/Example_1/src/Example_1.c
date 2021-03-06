/*(C) Umang Gajera - www.ocfreaks.com
LPC1343 ADC Example 1 Source Code
http://www.ocfreaks.com/lpc1343-adc-programming-tutorial/
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
License : GPL.*/

#include <lpc13xx.h>
#include <stdio.h>
#include "ocf_lpc134x_lib.h" //contains code for UART, Timer & to retarget printf().

#define VREF       3.3 //Reference Voltage at VREFP pin, given VREFN = 0V(GND)
#define ADC_CLK_EN (1<<13) //Enable ADC clock
#define SEL_AD0    (1<<0) //Select Channel AD0
#define CLKDIV     15 // ADC clock-divider (ADC_CLOCK=PCLK/CLKDIV+1), yields 4.5Mhz ADC clock
#define ADC_PWRUP  (~(1<<4)) //setting it to 0 will power it up
#define START_CNV  (1<<24) //001 for starting the conversion immediately
#define ADC_DONE   (1U<<31) //define it as unsigned value or compiler will throw warning
#define ADCR_SETUP_SCM (CLKDIV<<8)

int main(void)
{
	//SystemInit(); //Gets called by Startup code, sets CCLK=100Mhz, PCLK=25Mhz
	initUART(); //Initialize UART for printf()
	initTimer0(); //For delayMS()

	LPC_SYSCON->PDRUNCFG &= ADC_PWRUP; //Power-up ADC Block
	LPC_SYSCON->SYSAHBCLKCTRL |= ADC_CLK_EN; //Enable ADC clock
	LPC_ADC->CR =  ADCR_SETUP_SCM | SEL_AD0; //Setup ADC Block

	/* Now select AD0 function and set ADMODE=0 for PIO0_11(P0.11) */
	LPC_IOCON->R_PIO0_11 = 0x2; //Use this for KEIL and LPCXpresso/MCUXpresso - check tutorial for more
	//LPC_IOCON->JTAG_TDI_PIO0_11 = 0x2; //Older version of CMSIS uses this. Uncomment this for CoIDE.

	unsigned int result = 0;

	printf("OCFreaks.com LPC134x ADC Tutorial Example 1.\nSoftware Controlled ADC Mode on AD0 Channel.\n");
	while(1)
	{
		LPC_ADC->CR |= START_CNV; //Start new Conversion
		while((LPC_ADC->DR0 & ADC_DONE) == 0); //Wait until conversion is finished
		result = (LPC_ADC->DR0>>6) & 0x3FF; //10 bit Mask to extract result
		printf("AD0 = %dmV\n" , (int)( result*VREF ));
		delayMS(500); //Slowing down Updates to 2 Updates per second
	}

	//return 0; //This won't execute
}
