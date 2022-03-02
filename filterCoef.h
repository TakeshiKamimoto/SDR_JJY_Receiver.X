#define NUM_TAPS 40

//フィルタ　　40タップ　FIR  ローパスフィルタ********************************************
/*
Remez Algorithm LPF
Sampling Frequency =   195300.0
cutoff1 = 2000.0000000
cutoff2 = 5000.0000000
Tap Count = 40
attenuate =   -40.00
ripple factor =    1.0000000
Quantized by 16 [bits]
 */
const fractional lpfCoeffs[NUM_TAPS] __attribute__ ((space(auto_psv), aligned (256))) ={
        1296,         326,         364,         402,
         441,         479,         518,         555,
         591,         625,         659,         690,
         719,         742,         766,         784,
         799,         811,         819,         822,
         822,         819,         811,         799,
         784,         766,         742,         719,
         690,         659,         625,         591,
         555,         518,         479,         441,
         402,         364,         326,        1296
};
