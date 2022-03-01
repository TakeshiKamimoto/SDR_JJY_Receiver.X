/********************************************************************/
/*              Header for 16 bit DAC module library functions      */
/*                      for 33F/ 24H devices only                   */
/********************************************************************/
#include <p33Fxxxx.h>

/* List of SFRs for DAC */
/* This list contains the SFRs with default (POR) values to be used for configuring DAC */
/* The user can modify this based on the requirement */

/* DAC1CON Configuration Bit Definitions */

#define DAC_MODULE_ON               0xFFFF 	/* D/A Converter on */
#define DAC_MODULE_OFF             0x7FFF 	/* D/A Converter off */

#define DAC_IDLE_CONTINUE        0xDFFF 	/* D/A Operate in Idle mode */
#define DAC_IDLE_STOP               	0xFFFF 	/* D/A Stop in Idle mode */

#define DAC_SLEEP_ENABLED             0xFFFF /* Analog Output Amplifier is enabled during Sleep Mode/Stop-in Idle mode */
#define DAC_SLEEP_DISABLED             0xEFFF /* Analog Output Amplifier is disabled during Sleep Mode/Stop-in Idle mode. All channels are reset*/

#define DAC_FORMAT_SIGN_INT         	0xFFFF /* D/A data format signed integer */
#define DAC_FORMAT_UNSIGN_INTG 0xFEFF /* D/A data format unsigned integer */

#define DAC_CLK_DIV_by_4		0xFF83 /* Divide input clock by 4 */
#define DAC_CLK_DIV_by_3		0xFF82 /* Divide input clock by 3 */
#define DAC_CLK_DIV_by_2		0xFF81 /* Divide input clock by 2 */
#define DAC_CLK_DIV_by_1		0xFF80 /* Divide input clock by 1 */

/* defines for the DAC1STAT register*/
#define LEFT_CH_OUT_ENABLE	0xFFFF  	/*Positive and negative DAC outputs are enabled*/
#define LEFT_CH_OUT_DISABLE	0x7FFF  	/*DAC outputs are disabled*/

#define LEFT_CH_MIDPOINT_OUT_ENABLE	0xFFFF 	/* Midpoint DAC output is enabled*/
#define LEFT_CH_MIDPOINT_OUT_DISABLE	0xDFFF 	/* Midpoint output is disabled*/

#define LEFT_CH_INT_FIFOempty		0xFFFF		/* Interrupt if FIFO is empty*/
#define LEFT_CH_INT_FIFOnotfull		0xFBFF		/* Interrupt if FIFO is not full*/

#define RIGHT_CH_OUT_ENABLE	0xFFFF  	/*Positive and negative DAC outputs are enabled*/
#define RIGHT_CH_OUT_DISABLE	0xFF7F  	/*DAC outputs are disabled*/

#define RIGHT_CH_MIDPOINT_OUT_ENABLE	0xFFFF 	/* Midpoint DAC output is enabled*/
#define RIGHT_CH_MIDPOINT_OUT_DISABLE	0xFFDF 	/* Midpoint output is disabled*/

#define RIGHT_CH_INT_FIFOempty		0xFFFF		/* Interrupt if FIFO is empty*/
#define RIGHT_CH_INT_FIFOnotfull	0xFFFB		/* Interrupt if FIFO is not full*/


/* defines for the DAC1DFLT register */

#define DAC_DEFAULT_VALUE_ZERO      	0x0000 /* DAC Default Value = 0 */
#define DAC_DEFAULT_VALUE_MID          	0x8000 /* DAC Default Value = middle */
#define DAC_DEFAULT_VALUE_FULL     	0xFFFF/* DAC Default Value = FFFF */


/*defines for the ACLKCON register*/

#define AUX_OSC_FOR_AUX_CLK_DIV	0xFFFF		/*Auxiliary Oscillators provides the source clock for Auxiliary Clock Divider*/
#define FRC_PLL_FOR_AUX_CLK_DIV		0xDFFF	/* FRC with PLL provides the source clock for Auxiliary Clock Divider*/

#define EC_MODE_SEL	0xFFFF 	/* EC External Clock Mode Select*/
#define XT_MODE_SEL 0xEFFF		/* XT Oscillator Mode Select*/
#define HS_MODE_SEL 0xF7FF		/* HS Oscillator Mode Select*/
#define AUX_OSC_DISABLE	0xE7FF	/*Auxiliary Oscillator Disabled*/

#define AUX_CLK_DIV_by_1	0xFFFF		/* divided by 1*/
#define AUX_CLK_DIV_by_2	0xFEFF		/* divided by 2*/
#define AUX_CLK_DIV_by_4	0xFDFF	/* divided by 4*/
#define AUX_CLK_DIV_by_8	0xFCFF		/* divided by 8*/
#define AUX_CLK_DIV_by_16	0xFBFF		/* divided by 16*/
#define AUX_CLK_DIV_by_32	0xFAFF		/* divided by 32*/
#define AUX_CLK_DIV_by_64	0xF9FF		/* divided by 64*/
#define AUX_CLK_DIV_by_256	0xF8FF		/* divided by 256*/

#define PRM_OSC_CLK_SOURCE	0xFFFF 	/*Primary Oscillator is the Clock Source*/
#define AUX_OSC_CLK_SOURCE	0x0FF7		/*Auxiliary Oscillator is the Clock Source*/