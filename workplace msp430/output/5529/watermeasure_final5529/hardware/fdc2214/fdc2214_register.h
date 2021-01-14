

#ifndef __FDC2214_REGISTER_H__
#define __FDC2214_REGISTER_H__

//----------------------------register address---------------------------//
#define REG_DATA_CH0                        0x00                    //channel 0 msb conversion result and status
#define REG_DATA_LSB_CH0                    0x01                    //channel 0 lsb conversion result and status
#define REG_DATA_CH1                        0x02
#define REG_DATA_LSB_CH1                    0x03
#define REG_DATA_CH2                        0x04
#define REG_DATA_LSB_CH2                    0x05
#define REG_DATA_CH3                        0x06
#define REG_DATA_LSB_CH3                    0x07

#define REG_RCOUNT_CH0                      0x08                    //reference count setting for channel 0
#define REG_RCOUNT_CH1                      0x09
#define REG_RCOUNT_CH2                      0x0A
#define REG_RCOUNT_CH3                      0x0B

#define REG_OFFSET_CH0                      0x0C                    //offset value for channel 0
#define REG_OFFSET_CH1                      0x0D
#define REG_OFFSET_CH2                      0x0E
#define REG_OFFSET_CH3                      0x0F

#define REG_SETTLECOUNT_CH0                 0x10                    //channel 0 setting reference count
#define REG_SETTLECOUNT_CH1                 0x11
#define REG_SETTLECOUNT_CH2                 0x12
#define REG_SETTLECOUNT_CH3                 0x13

#define REG_CLOCK_DIVIDERS_CH0              0x14                    //reference divider settings for channel 0
#define REG_CLOCK_DIVIDERS_CH1              0x15
#define REG_CLOCK_DIVIDERS_CH2              0x16
#define REG_CLOCK_DIVIDERS_CH3              0x17

#define REG_STATUS                          0x18                    //device status reporting
#define REG_STATUS_CONFIG                   0x19                    //device status reporting configuration
#define REG_CONFIG                          0x1A                    //conversion configuration
#define REG_MUX_CONFIG                      0x1B                    //channel multiplexing configuration
#define REG_RESET_DEV                       0x1C                    //reset device

#define REG_DRIVE_CURRENT_CH0               0x1E                    //channel 0 sensor current drive configuration
#define REG_DRIVE_CURRENT_CH1               0x1F
#define REG_DRIVE_CURRENT_CH2               0x20
#define REG_DRIVE_CURRENT_CH3               0x21

#define REG_MANUFACTURER_ID                 0x7E                    //manufacturer id = 0x5449
#define REG_DEVICE_ID                       0x7F                    //device id = 0x3055


//--------------------------register value--------------------//
#define VALUE_RCOUNT_CH0                    0x186A                  //reference count from timing requirements(100SPS) and resolution requirements
#define VALUE_RCOUNT_CH1                    0x186A
#define VALUE_RCOUNT_CH2                    0x186A
#define VALUE_RCOUNT_CH3                    0x186A

#define VALUE_SETTLECOUNT_CH0               0x000A                  //minimum setting time for chosen sensor
#define VALUE_SETTLECOUNT_CH1               0x000A
#define VALUE_SETTLECOUNT_CH2               0x000A
#define VALUE_SETTLECOUNT_CH3               0x000A

#define VALUE_CLOCK_DIVIDER_CH0             0x2002                  //CH0_FIN_DIVIDER = 1, CH0_FREF_DIVIDER = 2
#define VALUE_CLOCK_DIVIDER_CH1             0x2002
#define VALUE_CLOCK_DIVIDER_CH2             0x2002
#define VALUE_CLOCK_DIVIDER_CH3             0x2002

#define VALUE_ERROR_CONFIG                  0x0000                  //default,reg=0x19 REG_STATUS_CONFIG

#define VALUE_MUX_CONFIG                    0xC20D                  //enable ch0, ch1, ch2, ch3 (sequential mode 0B10),set input deglitch bandwidth to 10MHz

#define VALUE_DRIVE_CURRENT_CH0             0x7C00                  //sets sensor drive current on ch0
#define VALUE_DRIVE_CURRENT_CH1             0x7C00
#define VALUE_DRIVE_CURRENT_CH2             0x7C00
#define VALUE_DRIVE_CURRENT_CH3             0x7C00

#define VALUE_CONFIG                        0x1601                  //enable full current drive during sensor activation,
                                                                    //select external clock source. wake up device to start conversion,
                                                                    //This register write must occur last because device configuration
                                                                    //is not permitted while the FDC is in active mode

#define VALUE_MANUFACTURER_ID               0x5449
#define VALUE_DEVICE_ID                     0x3055

#endif /* __FDC2214_REGISTER_H__ */
