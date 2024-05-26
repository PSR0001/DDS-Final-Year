#include <driver/dac.h>

// Timer0 Configuration Pointer (Handle)
hw_timer_t *Timer0_Cfg = NULL;

// Sine LookUpTable & Index Variable
uint8_t SampleIdx = 0;
const uint8_t sineLookupTable[] = {
128, 136, 143, 151, 159, 167, 174, 182,
189, 196, 202, 209, 215, 220, 226, 231,
235, 239, 243, 246, 249, 251, 253, 254,
255, 255, 255, 254, 253, 251, 249, 246,
243, 239, 235, 231, 226, 220, 215, 209,
202, 196, 189, 182, 174, 167, 159, 151,
143, 136, 128, 119, 112, 104, 96, 88,
81, 73, 66, 59, 53, 46, 40, 35,
29, 24, 20, 16, 12, 9, 6, 4,
2, 1, 0, 0, 0, 1, 2, 4,
6, 9, 12, 16, 20, 24, 29, 35,
40, 46, 53, 59, 66, 73, 81, 88,
96, 104, 112, 119};

// The Timer0 ISR Function (Executes Every Timer0 Interrupt Interval)
void IRAM_ATTR Timer0_ISR()
{
 // Send SineTable Values To DAC One By One
 dac_output_voltage(DAC_CHANNEL_1, sineLookupTable[SampleIdx++]);
 if(SampleIdx == 50)
 {
   SampleIdx = 0;
 }
}

void setup()
{
 // Configure Timer0 Interrupt
 Timer0_Cfg = timerBegin(0, 80, true);
 timerAttachInterrupt(Timer0_Cfg, &Timer0_ISR, true);
 timerAlarmWrite(Timer0_Cfg, 10, true);
 timerAlarmEnable(Timer0_Cfg);
 // Enable DAC1 Channel's Output
 dac_output_enable(DAC_CHANNEL_1);
}

void loop()
{
 // DO NOTHING
}
