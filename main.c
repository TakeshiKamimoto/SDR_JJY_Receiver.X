/************************************************
40kHz JJY放送信号受信ソフトウェアラジオ  with dsPIC33FJ64GP802 (2022/2/18) 
                       
A/D変換 → ビート周波数混合 → ローパスフィルタリング → D/A変換

A/Dサンプリング周波数 40MHz/256 = 156.25 kHz

 * MPLAB X IDE v5.50
 * Compiler: XC16 (v1.70)
*************************************************/
#include "p33fj64gp802.h"
#include <stdio.h>
#include <timer.h>
#include "adc802.h"
#include "dac802.h"
#include <dsp.h>    // to use dsp library, "libdsp-elf.a" is to be added under Libraries folder of the project.
#include <xc.h>
#include "filterCoef.h"


// コンフィギュレーション設定
// DSPIC33FJ64GP802 Configuration Bit Settings

// 'C' source line config statements

// FBS
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)
#pragma config RBS = NO_RAM             // Boot Segment RAM Protection (No Boot RAM)

// FSS
#pragma config SWRP = WRPROTECT_OFF     // Secure Segment Program Write Protect (Secure segment may be written)
#pragma config SSS = NO_FLASH           // Secure Segment Program Flash Code Protection (No Secure Segment)
#pragma config RSS = NO_RAM             // Secure Segment Data RAM Protection (No Secure RAM)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF                // General Segment Code Protection (User program memory is not code-protected)

// FOSCSEL
#pragma config FNOSC = PRIPLL           // Oscillator Mode (Primary Oscillator (XT, HS, EC) w/ PLL)
#pragma config IESO = ON                // Internal External Switch Over Mode (Start-up device with FRC, then automatically switch to user-selected oscillator source when ready)

// FOSC
#pragma config POSCMD = HS              // Primary Oscillator Source (HS Oscillator Mode)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function (OSC2 pin has clock out function)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow Only One Re-configuration)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR128           // POR Timer Value (128ms)
#pragma config ALTI2C = ON              // Alternate I2C  pins (I2C mapped to ASDA1/ASCL1 pins)

// FICD
#pragma config ICS = PGD1               // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled)



// FIR用構造体の宣言***
FIRStruct FIR_SDR_LPFilter;

// 遅延素子(delay)を用意 ***
//fractional LPFilterDelay[NUM_TAPS] __attribute__ ((section(".ybss, bss, ymemory"), far, aligned ( 256 ))); // 2byte x 127個 分の領域を確保
fractional _YBSS(256) LPFilterDelay[NUM_TAPS];

// データ格納配列宣言 for Filter
fractional Sig_LP_Out;		// LPフィルタ出力
fractional MixedSignal;

// ビート周波数  の準備
#define bfSIZE  4   // = Sampling freq / Beat freq = 156.253kHz / 39.0625kHz
fractional bf[bfSIZE];
unsigned int bfIndex = 0;

void initBeatFreq(void);

// 音量ボリューム設定
#define Volume  64		// 	

//サンプリング周期設定
#define ADsamp_156kHz   256 	// タイマ3の周期設定　(40MHz/256 = 156.25kHz, (Fcy = Fosc/2 = 40MHz) )

// 復調関数(ADC1Interrrupt)　A/D変換終了時に割り込みがかかり，この関数が起動される．
void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt (void)
{
/*long tmp1, tmp2, product;

    //ビート発生用信号混合
    tmp1 = ADCBUF0;
    tmp2 = bf[bfIndex++];
    bfIndex %= bfSIZE;
    
    product = tmp1 * tmp2;   //long(32ビット)変数に入れてから乗算する。
    product = product >> 14; //Q15フォーマット同士の乗算はQ30になるのでQ15に戻すために15ビット右シフト処理するが、結果を２倍にしたいので14右シフトする。
    MixedSignal = (int)product;//int(16ビット)変数にキャストする。
*/    
    /// ビート発生用信号ミキシング演算（ベクトル乗算dsp関数）
    MixedSignal = VectorDotProduct(1, &ADCBUF0, &bf[bfIndex]);
    bfIndex++;
    bfIndex %= bfSIZE;

    /// ローパスフィルタリング
	FIR(1, &Sig_LP_Out, &MixedSignal,  &FIR_SDR_LPFilter);	
	DAC1RDAT = Sig_LP_Out*Volume;       // D/A変換モジュールへの出力 (RIGHT DATA REGISTERへ)

 	IFS0bits.AD1IF = 0;     //Clear the ADC1Interrupt Flag
}


// タイマ3モジュール　初期設定　　A/D変換モジュールはTimer3により起動される．
void init_Timer(void)
{
	unsigned int TM3Config = T3_ON & T3_GATE_OFF & T3_PS_1_1&  T3_SOURCE_INT;

	OpenTimer3(TM3Config, ADsamp_156kHz - 1);       //Timer3初期設定
}


//ディジタルフィルタ初期化 ***
void init_Filter(void)
{
    //  Initialize FIR filter structure ***
    FIRStructInit(&FIR_SDR_LPFilter, NUM_TAPS, (fractional *)__builtin_psvoffset(lpfCoeffs),
            (int) __builtin_psvpage(lpfCoeffs), LPFilterDelay);
    
	FIRDelayInit(&FIR_SDR_LPFilter);
}


// A/D変換モジュール初期設定，割り込み設定
void init_ADC(void)
{
		// 12bits A/Dコンバータ用パラメータ宣言定義
		// AN0入力設定
		// Timer3 起動

		//  Tcy = 1/(クロック周波数:40 MHz)  
		// データシートのTable 30-44  によると ADC clock period は　min.  117.6 ns
		//  117.6 ns < TAD =  5Tcy  = 5 x (1/40MHz) = 5 x 25 ns = 125 ns  (ADC Conversion Clock Selectにより 5Tcy を設定する)
		// 以下はA/D変換モジュールの変換速度の計算 
		// Tsamp(Sample Time)  = 3 TAD
		// FCNV(Conversion Time) = 14 TADとあるので， A/D変換の所要速度は　3TAD + 14TAD = 17TAD * 125ns = 2.125 us  →　470ksps と求められる．

		AD1CON1	= ADC_MODULE_ON 
				& ADC_IDLE_CONTINUE
				& ADC_AD12B_12BIT 
				& ADC_FORMAT_SIGN_FRACT 
				& ADC_CLK_TMR 
				& ADC_AUTO_SAMPLING_ON
				& ADC_SAMP_OFF;
        
		AD1CON2	= ADC_VREF_AVDD_AVSS 
				& ADC_SCAN_OFF 
				& ADC_SELECT_CHAN_0
				& ADC_DMA_ADD_INC_1 
				& ADC_ALT_BUF_OFF 
				& ADC_ALT_INPUT_OFF;
         
		AD1CON3	= ADC_SAMPLE_TIME_3 
				& ADC_CONV_CLK_SYSTEM 
				& ADC_CONV_CLK_5Tcy;
               
		AD1CON4 = 0;
        
		AD1PCFGL = ENABLE_AN0_ANA; /*Enable AN0 in analog mode */
        
		AD1CSSL = 0;
        
        
        //A/D変換モジュールチャンネル，入力ピン設定        
		AD1CHS0 = ADC_CH0_POS_SAMPLEA_AN0 
                & ADC_CH0_NEG_SAMPLEA_VREFN; /* A/D Chan 0 pos i/p sel for SAMPLE A is AN0 */ 

        //A/D変換モジュール割り込み設定
        unsigned int ADCIntConfig =  ADC_INT_PRI_4 & ADC_INT_ENABLE;
        
        IFS0bits.AD1IF = 0;
  
        /* Setting Priority */
        IPC3bits.AD1IP = ADCIntConfig & 0x07;
  
        /* Setting the Interrupt enable bit */
        IEC0bits.AD1IE = (ADCIntConfig & 0x08)>>3;
}


// D/A変換モジュール初期設定
void init_DAC(void)
{
		// PLL out = AUX_CLK  = 80MHz,  DAC clock = (AUX_CLK_DIV_by_4)*(DAC_CLK_DIV_by_1)
		//   = 20MHz (which must not exceed 25.6 MHz)
		ACLKCON	= FRC_PLL_FOR_AUX_CLK_DIV 
				& AUX_CLK_DIV_by_4;
        
		DAC1CON	= DAC_MODULE_ON 
				& DAC_IDLE_STOP 
				& DAC_SLEEP_DISABLED 
				& DAC_FORMAT_SIGN_INT 
				& DAC_CLK_DIV_by_1;
        
		DAC1STAT = RIGHT_CH_OUT_ENABLE;// 右CHへ出力
		DAC1DFLT = DAC_DEFAULT_VALUE_ZERO;
}


void initBeatFreq(void)
{
    /*
     ビート発生用信号生成用数値テーブル
     int i;
     for(i = 0; i < bfSIZE; i++){
        bf}[i] = (fractional)Q15(sin(2 * PI * i / bfSIZE);
     }
     */

    bf[0] = (fractional)Q15(0.0);
    bf[1] = (fractional)Q15(1.0);
    bf[2] = (fractional)Q15(0.0);
    bf[3] = (fractional)Q15(-1.0);
}


/// メイン関数
void main(void)
{
	// Fosc= Fin * M / (N1 * N2), Fcy = Fosc / 2
	// Fosc= 10 MHz * 32 / (2 * 2) = 80MHz, Fcy = Fosc/2 = 40MHz
	PLLFBD = 30;                	// M=32 (for Fosc = 80MHz)
	CLKDIVbits.PLLPOST = 0;     	// N1=2
	CLKDIVbits.PLLPRE = 0;      	// N2=2

	RCONbits.SWDTEN = 0;

	// PLLロック待ち
	while(OSCCONbits.LOCK !=1 );

	// ポート初期設定
	TRISAbits.TRISA0 = 	1;		// RA0( = AN0 A/D変換入力端子） 入力設定，Aポートの他のピンは全て出力設定          
	TRISB = 0x0;				// Bポートは全て出力設定

	// 周辺モジュール設定
	init_Timer();
	init_ADC();
	init_DAC();

	// フィルタ設定
	init_Filter();
    
	// ビート発生用信号数値テーブル生成
	initBeatFreq();

	while(1) {
	}
}
