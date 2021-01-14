
#ifndef __FDC2214_INTERFACE_H__
#define __FDC2214_INTERFACE_H__

typedef enum
{
    CHANNEL_0,
    CHANNEL_1,
    CHANNEL_2,
    CHANNEL_3,

    CHANNEL_COUNT
}e_channel;

typedef struct
{
    uint32_t v_ch0;
    uint32_t v_ch1;
    uint32_t v_ch2;
    uint32_t v_ch3;
}t_channel_data;

typedef struct
{
    double v_ch0;
    double v_ch1;
    double v_ch2;
    double v_ch3;
}t_channel_capatiance;
/*
 * ADDR  <--->  P1.2
 * SD    <--->  P1.3
 * INTB  <--->  P1.4
 */
#define FDC_GPIO_DIR            P1DIR
#define FDC_GPIO_REN            P1REN
#define FDC_GPIO_OUT            P1OUT
#define FDC_GPIO_IES            P1IES
#define FDC_GPIO_IFG            P1IFG
#define FDC_GPIO_IE             P1IE

#define FDC_GPIO_IN             P1IN
#define FDC_ADDR_PIN            BIT2
#define FDC_SD_PIN              BIT3
#define FDC_INTB_PIN            BIT4

#define FDC_ADDRESS_BASE        0x2A

#define FDC_FREF_X              (40)                //40MHz
#define FDC_PARALLEL_INDUCTOR   (18)                //18uH
#define FDC_PARALLEL_CAPACITOR  (33)                //33pF

extern void fdc2214_init(void);
extern void fdc2214_configure_work_mode(void);

extern uint16_t fdc2214_get_manufacturer_id(void);
extern uint16_t fdc2214_get_device_id(void);
extern bool fdc2214_check(void);
double fdc2214_calc_capatiance(uint32_t data);

extern uint32_t fdc2214_get_channel_data(e_channel channel);
extern t_channel_data fdc2214_get_data(void);
extern t_channel_capatiance fdc2214_get_capatiance(t_channel_data *p_data);
extern double fdc2214_get_waterLevel(t_channel_capatiance *p_data);

#endif /* __FDC2214_INTERFACE_H__ */
