/*
 描述：本程序采用DMP库的方法直接得出mpu9250与mpu6050的四元素与欧拉角，可在200HZ的频率内采集，速度快，准确，简单。
 程序使用说明：
 	 程序中包含SPI与IIC两种方式，
IIC：  如果需要切换成IIC，在主函数中 注释mpu_SPI_INIT();，而I2CInit(0x68);则不需要注释
 	 然后在mpu_DMP.h文件中将#define use_spi注释掉
SPI：  如果需要切换成SPI，在主函数中 注释I2CInit(0x68);，而mpu_SPI_INIT();则不需要注释
	然后在mpu_DMP.h文件中不要将#define use_spi注释掉
两种模式接口如下：
 mpu9250spi接口：P1.0---NCS
   	   	   	   P1.5---SCL
   	   	   	   P1.6---ADO
   	   	   	   P1.7---SDA
 mpu9250与mpu6050接口
 	 	 	   GND---ADO
				P1.7---SDA
				P1.6---SCL
 串口接口P1.1与P1.2波特率9600
 上位机采用“虚拟串口”软件，波特率设为9600
开发平台：msp430g2553开发板

 by 252669569@qq.com
 */

#include  <msp430g2553.h>
#include "mpu_DMP.h"
#include  "outputdata.h"
#include  "I2C.h"
#define CPU_F ((double)8000000)
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

int abc=0;
void UART_INIT(void)
{
	P1SEL |= BIT1 + BIT2;
	 P1SEL2 |= BIT1 + BIT2;
    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_2 + UCSWRST;
/******************9600*****************************/
    UCA0MCTL = UCBRF_0 + UCBRS_2;
    UCA0BR0 = 65;
    UCA0BR1 = 3;
/***********************************************/
    UCA0CTL1 &= ~UCSWRST;
}

void main(void) {
	 WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
	BCSCTL1 = CALBC1_8MHZ; 				// Set range
		DCOCTL = CALDCO_8MHZ;				// Set DCO step + modulation
		BCSCTL3 |= LFXT1S_2;				// Set LFXT1
		IFG1 &= ~OFIFG;        				// Clear OSCFault flag
		BCSCTL2 |=SELM_1 + DIVM_0; 		// Set MCLK

	 //   mpu_SPI_INIT();//注意SPI与I2C只能用一个，且如果用SPI则mpu_DMP.h中#define use_spi不用注释，如果用I2C则需要注释
	    I2CInit(0x68);//0x68：mpu6050 mpu9250的地址

	    delay_ms(50);
	    Init_MPU6050();
	    UART_INIT();
	while(1){

		//delay_ms(5);	//这里的延时5ms注释掉是因为循环中的时间已经超过5ms,如果提高spi速度，串口速度和主频侧需要增加延时

		  readdmp();
		   getquaternion();//四元素
		   getgyro();//陀螺仪
		   getaccel();//加速度计
		   getyawpitchroll();//根据四元素算出欧拉角
		   OutData[0] = yprf[0];//欧拉角
		   OutData[1] =gyrof[0]; //陀螺仪
		   OutData[2] =  accelf[1]/200;//加速度计
		   OutData[3] =  2000;  //用来检测输出是否正常
		   OutPut_Data();
	}


}
