#define NUM_TAPS 40

//フィルタ　  40タップ　FIR　バンドパスフィルタ*************************************************
/*
 Remez Algorithm BPF
Sampling Frequency =   195300.0
cutoff1 = 8000.0000000
cutoff2 = 10000.0000000
cutoff3 = 50000.0000000
cutoff4 = 54000.0000000
Tap Count = 40
attenuate =   -80.00
ripple factor =    0.0100000
Quantized by 16 [bits]
 * 
 * */
const fractional bpfCoeffs[NUM_TAPS] __attribute__ ((space(auto_psv), aligned (256))) ={ 
       -2645,       -1675,        1342,        3382,
        2018,          56,         577,        1591,
         304,       -1232,        -346,         468,
       -1549,       -3138,       -1182,        -159,
       -4080,       -6202,        1454,       12266,
       12266,        1454,       -6202,       -4080,
        -159,       -1182,       -3138,       -1549,
         468,        -346,       -1232,         304,
        1591,         577,          56,        2018,
        3382,        1342,       -1675,       -2645
};
 

//フィルタ　　8.5kHz    40タップ　FIR  ローパスフィルタ********************************************
/*
const fractional lpfCoeffs[NUM_TAPS] __attribute__ ((space(auto_psv), aligned (256))) ={
    0x0000,	0xFFFE,	0xFFF2,	0xFFDD,	0xFFC1,	0xFFA5,	0xFF95,	0xFFA0,	0xFFD4,
    0x0040,	0x00EE,	0x01E1,	0x0315,	0x047C,	0x0601,	0x078A,	0x08F7,	0x0A29,
    0x0B07,	0x0B7B,	0x0B7B,	0x0B07,	0x0A29,	0x08F7,	0x078A,	0x0601,	0x047C,
    0x0315,	0x01E1,	0x00EE,	0x0040,	0xFFD4,	0xFFA0,	0xFF95,	0xFFA5,	0xFFC1,
	0xFFDD,	0xFFF2,	0xFFFE,	0x0000
};


Remez Algorithm LPF
Sampling Frequency =   195300.0
cutoff1 = 8000.0000000
cutoff2 = 10000.0000000
Tap Count = 40
attenuate =  -100.00
ripple factor =    0.1000000
Quantized by 16 [bits]
        
const fractional lpfCoeffs[NUM_TAPS] __attribute__ ((space(auto_psv), aligned (256))) ={        
          45,          45,          67,          93,
         124,         159,         200,         245,
         293,         343,         395,         448,
         499,         548,         593,         633,
         667,         693,         711,         720,
         720,         711,         693,         667,
         633,         593,         548,         499,
         448,         395,         343,         293,
         245,         200,         159,         124,
          93,          67,          45,          45
};*/

/*
 Remez Algorithm LPF
Sampling Frequency =   195300.0
cutoff1 = 12000.0000000
cutoff2 = 15000.0000000
Tap Count = 40
attenuate =   -80.00
ripple factor =    0.1000000
Quantized by 16 [bits]
*/
const fractional lpfCoeffs[NUM_TAPS] __attribute__ ((space(auto_psv), aligned (256))) ={
        -331,        -415,        -629,        -870,
       -1116,       -1338,       -1502,       -1576,
       -1529,       -1336,        -985,        -480,
         162,         907,        1708,        2508,
        3245,        3860,        4302,        4533,
        4533,        4302,        3860,        3245,
        2508,        1708,         907,         162,
        -480,        -985,       -1336,       -1529,
       -1576,       -1502,       -1338,       -1116,
        -870,        -629,        -415,        -331
};