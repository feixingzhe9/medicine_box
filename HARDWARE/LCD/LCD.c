/******************************************************************************

*重要说明！
在.h文件中，#define Immediately时是立即显示当前画面
而如果#define Delay，则只有在执行了LCD_WR_REG(0x0007,0x0173);
之后才会显示，执行一次LCD_WR_REG(0x0007,0x0173)后，所有写入数
据都立即显示。
#define Delay一般用在开机画面的显示，防止显示出全屏图像的刷新过程
******************************************************************************/
#include "stm32f10x.h"
#include "LCD.h"
//#include "ff.h"	
#include <stdio.h>
#include <stm32f10x_fsmc.h>
// #include "MM_T035.h"
/*
 * 函数名：LCD_GPIO_Config
 * 描述  ：根据FSMC配置LCD的I/O
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用        
 */
 void LCD_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Enable the FSMC Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    
    /* config lcd gpio clock base on FSMC */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    
    /* config tft rst gpio */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    //LED 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;		
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    /* config tft back_light gpio base on the PT4101 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; 	 
    GPIO_Init(GPIOA, &GPIO_InitStructure);  		   
    
    /* config tft data lines base on FSMC
	 * data lines,FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10
	 */	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | 
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                  GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    
    /* config tft control lines base on FSMC
	 * PD4-FSMC_NOE  :LCD-RD
   * PD5-FSMC_NWE  :LCD-WR
	 * PD7-FSMC_NE1  :LCD-CS
   * PD11-FSMC_A16 :LCD-DC
	 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
    /* tft control gpio init */	 

		GPIO_SetBits(GPIOD, GPIO_Pin_13);		 // RST = 1 
    GPIO_SetBits(GPIOD, GPIO_Pin_4);		 // RD = 1  
    GPIO_SetBits(GPIOD, GPIO_Pin_5);		 // WR = 1 
    GPIO_SetBits(GPIOD, GPIO_Pin_7);		 //	CS = 1 

}

/*
 * 函数名：LCD_FSMC_Config
 * 描述  ：LCD  FSMC 模式配置
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用        
 */

static void LCD_FSMC_Config(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p; 
    
    
    p.FSMC_AddressSetupTime = 0x02;	 //地址建立时间
    p.FSMC_AddressHoldTime = 0x00;	 //地址保持时间
    p.FSMC_DataSetupTime = 0x05;		 //数据建立时间
    p.FSMC_BusTurnAroundDuration = 0x00;
    p.FSMC_CLKDivision = 0x00;
    p.FSMC_DataLatency = 0x00;

    p.FSMC_AccessMode = FSMC_AccessMode_B;	 // 一般使用模式B来控制LCD
    
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p; 
    
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
    
    /* Enable FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
}

volatile static void Delay(__IO u32 nCount)
{	
	volatile int i;
	for(i=0;i<0XFFFF;i++)
    for(; nCount != 0; nCount--);
}  
  
static void LCD_Rst(void)
{			
    Clr_Rst;
    Delay(10000);					   
    Set_Rst;		 	 
    Delay(10000);	
}
static void WriteComm(u16 CMD)
{			
	*(__IO u16 *) (Bank1_LCD_C) = CMD;
}
static void WriteData(u16 tem_data)
{			
	*(__IO u16 *) (Bank1_LCD_D) = tem_data;
}

/**********************************************
Lcd初始化函数
***********************************************/
void Lcd_Initialize(void)
{	
	int a;
LCD_GPIO_Config();
LCD_FSMC_Config();
LCD_Rst();

// 	WriteComm(0xBF);
// 	a = *(__IO u16 *) (Bank1_LCD_D); 
// 	printf("ID=%04x\r\n",a);
// 	a = *(__IO u16 *) (Bank1_LCD_D); 
// 	printf("ID=%04x\r\n",a);
// 	a = *(__IO u16 *) (Bank1_LCD_D); 
// 	printf("ID=%04x\r\n",a);
// 	a = *(__IO u16 *) (Bank1_LCD_D); 
// 	printf("ID=%04x\r\n",a);
// 	a = *(__IO u16 *) (Bank1_LCD_D); 
// 	printf("ID=%04x\r\n",a);
// 	a = *(__IO u16 *) (Bank1_LCD_D); 
// 	printf("ID=%04x\r\n",a);	
	
// // LCD_WR_REG(0xFF,0x00); // Command page 0
// // LCD_WR_REG(0x72,0xF6); // SUB_SEL=0xF6
// // // Power saving for HX8357-A
// // LCD_WR_REG(0xFF,0x00); // Command page 0
// // LCD_WR_REG(0xF2,0x00); // GENON=0x00
// // LCD_WR_REG(0xE4,0x00); // EQVCI_M1=0x00
// // LCD_WR_REG(0xE5,0x1C); // EQGND_M1=0x1C
// // LCD_WR_REG(0xE6,0x00); // EQVCI_M0=0x1C
// // LCD_WR_REG(0xE7,0x1C); // EQGND_M0=0x1C
// // LCD_WR_REG(0xEE,0x42); // For GRAM read/write speed
// // LCD_WR_REG(0xEF,0xDB); // For GRAM read/write speed
// // LCD_WR_REG(0x2E,0x98); //For Gate timing, prevent the display abnormal in RGB I/F
// // // Gamma
// // LCD_WR_REG(0x40,0x01); 
// //   LCD_WR_REG(0x41,0x12); 
// //   LCD_WR_REG(0x42,0x19); 
// //   LCD_WR_REG(0x43,0x26); 
// //   LCD_WR_REG(0x44,0x2C); 
// //   LCD_WR_REG(0x45,0x3C); 
// //   LCD_WR_REG(0x46,0x0B); 
// //   LCD_WR_REG(0x47,0x5F); 
// //   LCD_WR_REG(0x48,0x00); 
// //   LCD_WR_REG(0x49,0x08); 
// //   LCD_WR_REG(0x4A,0x0B); 
// //   LCD_WR_REG(0x4B,0x10); 
// //   LCD_WR_REG(0x4C,0x16); 
// //   LCD_WR_REG(0x50,0x01); 
// //   LCD_WR_REG(0x51,0x1D); 
// //   LCD_WR_REG(0x52,0x21); 
// //   LCD_WR_REG(0x53,0x34); 
// //   LCD_WR_REG(0x54,0x37); 
// //   LCD_WR_REG(0x55,0x3F); 
// //   LCD_WR_REG(0x56,0x29); 
// //   LCD_WR_REG(0x57,0x7F); 
// //   LCD_WR_REG(0x58,0x01); 
// //   LCD_WR_REG(0x59,0x12); 
// //   LCD_WR_REG(0x5A,0x1B); 
// //   LCD_WR_REG(0x5B,0x1E); 
// //   LCD_WR_REG(0x5C,0x1A); 
// //   LCD_WR_REG(0x5D,0x55); 
// // // LCD_WR_REG(0x40,0x00);
// // // LCD_WR_REG(0x41,0x2E);
// // // LCD_WR_REG(0x42,0x2D);
// // // LCD_WR_REG(0x43,0x3F);
// // // LCD_WR_REG(0x44,0x3F);
// // // LCD_WR_REG(0x45,0x3F);
// // // LCD_WR_REG(0x46,0x2C);
// // // LCD_WR_REG(0x47,0x7F);
// // // LCD_WR_REG(0x48,0x07);
// // // LCD_WR_REG(0x49,0x05);
// // // LCD_WR_REG(0x4A,0x08);
// // // LCD_WR_REG(0x4B,0x13);
// // // LCD_WR_REG(0x4C,0x1E);
// // // LCD_WR_REG(0x50,0x00);
// // // LCD_WR_REG(0x51,0x00);
// // // LCD_WR_REG(0x52,0x00);
// // // LCD_WR_REG(0x53,0x12);
// // // LCD_WR_REG(0x54,0x11);
// // // LCD_WR_REG(0x55,0x3F);
// // // LCD_WR_REG(0x56,0x00);
// // // LCD_WR_REG(0x57,0x53);
// // // LCD_WR_REG(0x58,0x01);
// // // LCD_WR_REG(0x59,0x0C);
// // // LCD_WR_REG(0x5A,0x17);
// // // LCD_WR_REG(0x5B,0x1A);
// // // LCD_WR_REG(0x5C,0x18);
// // // LCD_WR_REG(0x5D,0xC3);

// // // Set GRAM area 320x240
// // LCD_WR_REG(0x02,0x00);
// // LCD_WR_REG(0x03,0x00);
// // LCD_WR_REG(0x04,0x01);
// // LCD_WR_REG(0x05,0x3F);
// // LCD_WR_REG(0x06,0x00);
// // LCD_WR_REG(0x07,0x00);
// // LCD_WR_REG(0x08,0x00);
// // LCD_WR_REG(0x09,0xEF);

// // LCD_WR_REG(0x24,0x22); // Set VCOMH voltage, VHH=0x64
// // LCD_WR_REG(0x25,0x64); // Set VCOML voltage, VML=0x71
// // LCD_WR_REG(0x23,0x90); // Set VCOM offset, VMF=0x52
// // LCD_WR_REG(0x1B,0x0E); // Set VERG1 voltage, VRH[5:0]=0x1E
// // LCD_WR_REG(0x1D,0x11); // FS0[1:0]=01, Set the operating frequency of the step-up circuit 1
// // // Power on Setting
// // LCD_WR_REG(0x19,0x01); // OSC_EN=1, Start to Oscillate
// // LCD_WR_REG(0x1C,0x03); // AP=011
// // LCD_WR_REG(0x01,0x00); // Normal display(Exit Deep standby mode)
// // LCD_WR_REG(0x1F,0x80); // Exit standby mode and Step-up circuit 1 enable
// // // GAS_EN=1, VCOMG=0, PON=0, DK=0, XDK=0, DDVDH_TRI=0, STB=0
// // Delay(5);
// // LCD_WR_REG(0x1F,0x90); // Step-up circuit 2 enable
// // // GAS_EN=1, VCOMG=0, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0
// // Delay(5);
// // LCD_WR_REG(0x1F,0xD4);
// // // GAS_EN=1, VCOMG=1, PON=1, DK=0, XDK=1, DDVDH_TRI=0, STB=0
// // Delay(5);
// // // Display ON Setting
// // LCD_WR_REG(0x28,0x08); // GON=0, DTE=0, D[1:0]=01
// // Delay(40);
// // LCD_WR_REG(0x28,0x38); // GON=1, DTE=1, D[1:0]=10
// // Delay(40);
// // LCD_WR_REG(0x28,0x3C); // GON=1, DTE=1, D[1:0]=11


// // LCD_WR_REG(0x17,0x05); // 16-bit/pixel
// // LCD_WR_REG(0x36,0x03);
// // // LCD_WR_REG(0x16,0x20);
// // WriteComm(0x0029);

//================= Command page 0 =================
	LCD_WR_REG(0xFF,0x00); // Command page 0
	Delay(15); 
	LCD_WR_REG(0x1A,0x04);//VGH VGL VCL  DDVDH
	LCD_WR_REG(0x1B,0x1C);
	// Power Settng
	LCD_WR_REG(0x23,0x94); // Set VCOM offset, VMF=0x52	
	LCD_WR_REG(0x24,0x69); // Set VCOMH voltage, VHH=0x64
	LCD_WR_REG(0x25,0x63); // Set VCOML voltage, VML=0x71
	LCD_WR_REG(0x19,0x01); 
    Delay(10); 
	LCD_WR_REG(0x1A,0x00);//VGH VGL VCL  DDVDH
	LCD_WR_REG(0x1F,0x8A);//
	LCD_WR_REG(0x01,0x00);//
	LCD_WR_REG(0x1C,0x05);//	
	LCD_WR_REG(0x1F,0x82);//
	Delay(10); 
	
  LCD_WR_REG(0x1F,0x92);//
	Delay(10); 
  LCD_WR_REG(0x1F,0xD4);//
		// Set GRAM aea 320x480
	LCD_WR_REG(0x02,0x00);
	LCD_WR_REG(0x03,0x00);
	LCD_WR_REG(0x04,0x01);
	LCD_WR_REG(0x05,0x3F);
	LCD_WR_REG(0x06,0x00);
	LCD_WR_REG(0x07,0x00);
	LCD_WR_REG(0x08,0x01);
	LCD_WR_REG(0x09,0xDF);
	
  LCD_WR_REG(0x16,0x88);//set my mx mv bgr...
	LCD_WR_REG(0x17,0x55);//262-bit/pixel 0X60 ;262-bit/pixel 0X50		
	LCD_WR_REG(0x18,0x21);	//Fosc=130%*5.2MHZ   21
	LCD_WR_REG(0x1D,0x00); // FS0[1:0]=01, Set the operating frequency of the step-up circuit 1		
	LCD_WR_REG(0x1E,0x00);	
	
	LCD_WR_REG(0x26,0x33);
	LCD_WR_REG(0x27,0x01);
	LCD_WR_REG(0x29,0x00);
	LCD_WR_REG(0x2A,0x00);
	LCD_WR_REG(0x2B,0x01);//0A
	LCD_WR_REG(0x2C,0x0A);
	LCD_WR_REG(0x2D,0x20);
	LCD_WR_REG(0x2E,0xA3);
	LCD_WR_REG(0x2F,0x00);//00 100416
	
//   LCD_WR_REG(0x31,0x00);
// 	LCD_WR_REG(0x32,0x00);
// 	LCD_WR_REG(0x33,0x08);
//   LCD_WR_REG(0x34,0x02);
    
	LCD_WR_REG(0x31,0x02);//RGB MODE1  0X03=RGB MODE2 
	LCD_WR_REG(0x32,0x00);
	LCD_WR_REG(0x33,0x08);
	LCD_WR_REG(0x34,0x08);
	LCD_WR_REG(0x36,0x02);//REV

		// Gamma     
	LCD_WR_REG(0x40,0x01);
	LCD_WR_REG(0x41,0x0F);
	LCD_WR_REG(0x42,0x0F);
	LCD_WR_REG(0x43,0x26);
	LCD_WR_REG(0x44,0x2C);
	LCD_WR_REG(0x45,0x3C);
	LCD_WR_REG(0x46,0x0B);
	LCD_WR_REG(0x47,0x5F);
	LCD_WR_REG(0x48,0x00);
	LCD_WR_REG(0x49,0x06);
	LCD_WR_REG(0x4A,0x09);
	LCD_WR_REG(0x4B,0x0E);
	LCD_WR_REG(0x4C,0x16);
	
	LCD_WR_REG(0x50,0x01);
	LCD_WR_REG(0x51,0x1D);
	LCD_WR_REG(0x52,0x21);
	LCD_WR_REG(0x53,0x3A);
	LCD_WR_REG(0x54,0x39);
	LCD_WR_REG(0x55,0x3F);
	LCD_WR_REG(0x56,0x2D);
	LCD_WR_REG(0x57,0x7F);
	LCD_WR_REG(0x58,0x02);
	LCD_WR_REG(0x59,0x15);
	LCD_WR_REG(0x5A,0x1B);
	LCD_WR_REG(0x5B,0x1B);
	LCD_WR_REG(0x5C,0x1A);
	LCD_WR_REG(0x5D,0x55);
//================= Command page 1 =================
	    LCD_WR_REG(0xff,0x01);
     	LCD_WR_REG(0x00,0x01);
	    LCD_WR_REG(0x01,0x00);
        LCD_WR_REG(0x02,0x01);
        LCD_WR_REG(0x03,0x03);
        LCD_WR_REG(0x04,0x05);
        LCD_WR_REG(0x05,0x06);
        LCD_WR_REG(0x06,0x08);
        LCD_WR_REG(0x07,0x0C);
        LCD_WR_REG(0x08,0x0E);
        LCD_WR_REG(0x09,0x11);
        LCD_WR_REG(0x0A,0x12);
        LCD_WR_REG(0x0B,0x14);
        LCD_WR_REG(0x0C,0x1B);
        LCD_WR_REG(0x0D,0x23);
        LCD_WR_REG(0x0E,0x29);
        LCD_WR_REG(0x0F,0x2F);
        LCD_WR_REG(0x10,0x34);
        LCD_WR_REG(0x11,0x39);
        LCD_WR_REG(0x12,0x3E);
        LCD_WR_REG(0x13,0x43);
        LCD_WR_REG(0x14,0x48);
        LCD_WR_REG(0x15,0x4C);
        LCD_WR_REG(0x16,0x51);
        LCD_WR_REG(0x17,0x55);
        LCD_WR_REG(0x18,0x59);
        LCD_WR_REG(0x19,0x5D);
        LCD_WR_REG(0x1A,0x60);
        LCD_WR_REG(0x1B,0x64);
        LCD_WR_REG(0x1C,0x68);
        LCD_WR_REG(0x1D,0x6C);
        LCD_WR_REG(0x1E,0x70);
        LCD_WR_REG(0x1F,0x73);
        LCD_WR_REG(0x20,0x77);
        LCD_WR_REG(0x21,0x7B);
        LCD_WR_REG(0x22,0x7F);
        LCD_WR_REG(0x23,0x83);
        LCD_WR_REG(0x24,0x87);
        LCD_WR_REG(0x25,0x8A);
        LCD_WR_REG(0x26,0x8E);
        LCD_WR_REG(0x27,0x92);
        LCD_WR_REG(0x28,0x96);
        LCD_WR_REG(0x29,0x9A);
        LCD_WR_REG(0x2A,0x9F);
        LCD_WR_REG(0x2B,0xA3);
        LCD_WR_REG(0x2C,0xA7);
        LCD_WR_REG(0x2D,0xAC);
        LCD_WR_REG(0x2E,0xAF);
        LCD_WR_REG(0x2F,0xB3);
        LCD_WR_REG(0x30,0xB7);
        LCD_WR_REG(0x31,0xBA);
        LCD_WR_REG(0x32,0xBE);
        LCD_WR_REG(0x33,0xC3);
        LCD_WR_REG(0x34,0xC7);
        LCD_WR_REG(0x35,0xCC);
        LCD_WR_REG(0x36,0xD1);
        LCD_WR_REG(0x37,0xD7);
        LCD_WR_REG(0x38,0xDD);
        LCD_WR_REG(0x39,0xE3);
        LCD_WR_REG(0x3A,0xE8);
        LCD_WR_REG(0x3B,0xEA);
        LCD_WR_REG(0x3C,0xEC);
        LCD_WR_REG(0x3D,0xEF);
        LCD_WR_REG(0x3E,0xF3);
        LCD_WR_REG(0x3F,0xF6);
				
        LCD_WR_REG(0x40,0xFE);
        LCD_WR_REG(0x41,0x00);
        LCD_WR_REG(0x42,0x01);
        LCD_WR_REG(0x43,0x03);
        LCD_WR_REG(0x44,0x05);
        LCD_WR_REG(0x45,0x06);
        LCD_WR_REG(0x46,0x08);
        LCD_WR_REG(0x47,0x0C);
        LCD_WR_REG(0x48,0x0E);
        LCD_WR_REG(0x49,0x11);
        LCD_WR_REG(0x4A,0x12);
        LCD_WR_REG(0x4B,0x14);
        LCD_WR_REG(0x4C,0x1B);
        LCD_WR_REG(0x4D,0x23);
        LCD_WR_REG(0x4E,0x29);
        LCD_WR_REG(0x4F,0x2F);
        LCD_WR_REG(0x50,0x34);
        LCD_WR_REG(0x51,0x39);
        LCD_WR_REG(0x52,0x3E);
        LCD_WR_REG(0x53,0x43);
        LCD_WR_REG(0x54,0x48);
        LCD_WR_REG(0x55,0x4C);
        LCD_WR_REG(0x56,0x51);
        LCD_WR_REG(0x57,0x55);
        LCD_WR_REG(0x58,0x59);
        LCD_WR_REG(0x59,0x5D);
        LCD_WR_REG(0x5A,0x60);
        LCD_WR_REG(0x5B,0x64);
        LCD_WR_REG(0x5C,0x68);
        LCD_WR_REG(0x5D,0x6C);
				
        LCD_WR_REG(0x5E,0x70);
        LCD_WR_REG(0x5F,0x73);
        LCD_WR_REG(0x60,0x77);
        LCD_WR_REG(0x61,0x7B);
        LCD_WR_REG(0x62,0x7F);
        LCD_WR_REG(0x63,0x83);
        LCD_WR_REG(0x64,0x87);
        LCD_WR_REG(0x65,0x8A);
        LCD_WR_REG(0x66,0x8E);
        LCD_WR_REG(0x67,0x92);
        LCD_WR_REG(0x68,0x96);
        LCD_WR_REG(0x69,0x9A);
        LCD_WR_REG(0x6A,0x9F);
        LCD_WR_REG(0x6B,0xA3);
        LCD_WR_REG(0x6C,0xA7);
        LCD_WR_REG(0x6D,0xAC);
        LCD_WR_REG(0x6E,0xAF);
        LCD_WR_REG(0x6F,0xB3);
        LCD_WR_REG(0x70,0xB7);
        LCD_WR_REG(0x71,0xBA);
        LCD_WR_REG(0x72,0xBE);
        LCD_WR_REG(0x73,0xC3);
        LCD_WR_REG(0x74,0xC7);
        LCD_WR_REG(0x75,0xCC);
        LCD_WR_REG(0x76,0xD1);
        LCD_WR_REG(0x77,0xD7);
        LCD_WR_REG(0x78,0xDD);
        LCD_WR_REG(0x79,0xE3);
        LCD_WR_REG(0x7A,0xE8);
        LCD_WR_REG(0x7B,0xEA);
        LCD_WR_REG(0x7C,0xEC);
        LCD_WR_REG(0x7D,0xEF);
        LCD_WR_REG(0x7E,0xF3);
        LCD_WR_REG(0x7F,0xF6);
        LCD_WR_REG(0x80,0xFE);
        LCD_WR_REG(0x81,0x00);
        LCD_WR_REG(0x82,0x01);
        LCD_WR_REG(0x83,0x03);
        LCD_WR_REG(0x84,0x05);
        LCD_WR_REG(0x85,0x06);
        LCD_WR_REG(0x86,0x08);
        LCD_WR_REG(0x87,0x0C);
        LCD_WR_REG(0x88,0x0E);
        LCD_WR_REG(0x89,0x11);
        LCD_WR_REG(0x8A,0x12);
        LCD_WR_REG(0x8B,0x14);
        LCD_WR_REG(0x8C,0x1B);
        LCD_WR_REG(0x8D,0x23);
        LCD_WR_REG(0x8E,0x29);
        LCD_WR_REG(0x8F,0x2F);
        LCD_WR_REG(0x90,0x34);
        LCD_WR_REG(0x91,0x39);
        LCD_WR_REG(0x92,0x3E);
        LCD_WR_REG(0x93,0x43);
        LCD_WR_REG(0x94,0x48);
        LCD_WR_REG(0x95,0x4C);
        LCD_WR_REG(0x96,0x51);
        LCD_WR_REG(0x97,0x55);
        LCD_WR_REG(0x98,0x59);
        LCD_WR_REG(0x99,0x5D);
        LCD_WR_REG(0x9A,0x60);
        LCD_WR_REG(0x9B,0x64);
        LCD_WR_REG(0x9C,0x68);
        LCD_WR_REG(0x9D,0x6C);
        LCD_WR_REG(0x9E,0x70);
        LCD_WR_REG(0x9F,0x73);
        LCD_WR_REG(0xA0,0x77);
        LCD_WR_REG(0xA1,0x7B);
        LCD_WR_REG(0xA2,0x7F);
        LCD_WR_REG(0xA3,0x83);
        LCD_WR_REG(0xA4,0x87);
        LCD_WR_REG(0xA5,0x8A);
        LCD_WR_REG(0xA6,0x8E);
        LCD_WR_REG(0xA7,0x92);
        LCD_WR_REG(0xA8,0x96);
        LCD_WR_REG(0xA9,0x9A);
        LCD_WR_REG(0xAA,0x9F);
        LCD_WR_REG(0xAB,0xA3);
        LCD_WR_REG(0xAC,0xA7);
        LCD_WR_REG(0xAD,0xAC);
        LCD_WR_REG(0xAE,0xAF);
        LCD_WR_REG(0xAF,0xB3);
        LCD_WR_REG(0xB0,0xB7);
        LCD_WR_REG(0xB1,0xBA);
        LCD_WR_REG(0xB2,0xBE);
        LCD_WR_REG(0xB3,0xC3);
        LCD_WR_REG(0xB4,0xC7);
        LCD_WR_REG(0xB5,0xCC);
        LCD_WR_REG(0xB6,0xD1);
        LCD_WR_REG(0xB7,0xD7);
        LCD_WR_REG(0xB8,0xDD);
        LCD_WR_REG(0xB9,0xE3);
        LCD_WR_REG(0xBA,0xE8);
        LCD_WR_REG(0xBB,0xEA);
        LCD_WR_REG(0xBC,0xEC);
        LCD_WR_REG(0xBD,0xEF);
        LCD_WR_REG(0xBE,0xF3);
        LCD_WR_REG(0xBF,0xF6);
        LCD_WR_REG(0xC0,0xFE);
				
//================= Command page 0 =================
	LCD_WR_REG(0xff,0x00);
	LCD_WR_REG(0x60,0x08);
	LCD_WR_REG(0xF2,0x00);
	LCD_WR_REG(0xE4,0x1F); //EQVCI_M1=0x00
	LCD_WR_REG(0xE5,0x1F); //EQGND_M1=0x1C
	LCD_WR_REG(0xE6,0x20); //EQVCI_M0=0x1C
	LCD_WR_REG(0xE7,0x00); //EQGND_M0=0x1C
	LCD_WR_REG(0xE8,0xD1);
	LCD_WR_REG(0xE9,0xC0);
	LCD_WR_REG(0x28,0x38);
    Delay(80);
	LCD_WR_REG(0x28,0x3C);// GON=0, DTE=0, D[1:0]=01	
		
	LCD_WR_REG(0x80,0x00);
	LCD_WR_REG(0x81,0x00);
	LCD_WR_REG(0x82,0x00);
	LCD_WR_REG(0x83,0x00);
	
	Delay(100);
	
	
	*(__IO u16 *) (Bank1_LCD_C) = (0x22);
	
	
Delay(10);

Lcd_Light_ON;
Lcd_ColorBox(0,0,320,480,Yellow);
//printf("ReadPixel=%04x\r\n",ReadPixel(8, 8));
DrawPixel(10, 10, 0xaaaa);
//printf("ReadPixel=%04x\r\n",ReadPixel(10, 10));
DrawPixel(10, 11, 0XFFFF);
//printf("ReadPixel=%04x\r\n",ReadPixel(10, 11));
DrawPixel(10, 12, 0X00);
//printf("ReadPixel=%04x\r\n",ReadPixel(10, 12));

// // while(1)
// // 	{
// // // 		Lcd_ColorBox(0,0,480,320,Yellow);
// // // 		Lcd_ColorBox(0,0,480,320,Blue);
// // // 		Lcd_ColorBox(0,0,480,320,Red);
// // // 		Lcd_ColorBox(0,0,480,320,Green);
// // // 		Lcd_ColorBox(0,0,480,320,Magenta);
// // // 		Lcd_ColorBox(0,0,480,320,Cyan);
// // 		
// // // 		Lcd_ColorBox(0,0,320,480,Yellow);Delay(5000000);
// // 		Lcd_ColorBox(0,0,320,480,Blue);Delay(5000000);
// // 		Lcd_ColorBox(0,0,320,480,Red);Delay(5000000);
// // 		Lcd_ColorBox(0,0,320,480,Green);Delay(5000000);
// // // 		Lcd_ColorBox(0,0,320,480,Magenta);Delay(5000000);
// // 		Lcd_ColorBox(0,0,320,480,Black);Delay(5000000);
// // 		Lcd_ColorBox(0,0,320,480,White);Delay(5000000);
// // // 		Lcd_ColorBox(100,100,100,100,Green);Delay(5000000);
// // 		LCD_Fill_Pic(0,0,320,480, gImage_MM_T035);Delay(50000000);
// // 	}
}
/******************************************
函数名：Lcd写命令函数
功能：向Lcd指定位置写入应有命令或数据
入口参数：Index 要寻址的寄存器地址
          ConfigTemp 写入的数据或命令值
******************************************/
void LCD_WR_REG(u16 Index,u16 CongfigTemp)
{
	*(__IO u16 *) (Bank1_LCD_C) = Index;	
	*(__IO u16 *) (Bank1_LCD_D) = CongfigTemp;
}
/************************************************
函数名：Lcd写开始函数
功能：控制Lcd控制引脚 执行写操作
************************************************/
void Lcd_WR_Start(void)
{
*(__IO u16 *) (Bank1_LCD_C) = 0x2C;
}
/**********************************************
函数名：Lcd块选函数
功能：选定Lcd上指定的矩形区域

注意：xStart、yStart、Xend、Yend随着屏幕的旋转而改变，位置是矩形框的四个角

入口参数：xStart x方向的起始点
          ySrart y方向的起始点
          Xend   y方向的终止点
          Yend   y方向的终止点
返回值：无
***********************************************/
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) 
{
//HX8357-A

	LCD_WR_REG(0x80,Ystart>>8); // Set CAC=0x0000 
	LCD_WR_REG(0x81,Ystart);   
	LCD_WR_REG(0x82,Xstart>>8); // Set RAC=0x0000 
	LCD_WR_REG(0x83,Xstart);
	
	LCD_WR_REG(0x02,Xstart>>8); 
	LCD_WR_REG(0x03,Xstart);     //Column Start 
	LCD_WR_REG(0x04,Xend>>8); 
	LCD_WR_REG(0x05,Xend);     //Column End 
	 
	LCD_WR_REG(0x06,Ystart>>8); 
	LCD_WR_REG(0x07,Ystart);     //Row Start 
	LCD_WR_REG(0x08,Yend>>8); 
	LCD_WR_REG(0x09,Yend);     //Row End 

	WriteComm(0x22);
}
/**********************************************
函数名：Lcd块选函数
功能：选定Lcd上指定的矩形区域

注意：xStart和 yStart随着屏幕的旋转而改变，位置是矩形框的四个角

入口参数：xStart x方向的起始点
          ySrart y方向的终止点
          xLong 要选定矩形的x方向长度
          yLong  要选定矩形的y方向长度
返回值：无
***********************************************/
void Lcd_ColorBox(u16 xStart,u16 yStart,u16 xLong,u16 yLong,u16 Color)
{
	u32 temp;

	BlockWrite(xStart,xStart+xLong-1,yStart,yStart+yLong-1);
	for (temp=0; temp<xLong*yLong; temp++)
	{
		*(__IO u16 *) (Bank1_LCD_D) = Color;
	}
}

/******************************************
函数名：Lcd图像填充100*100
功能：向Lcd指定位置填充图像
入口参数：Index 要寻址的寄存器地址
          ConfigTemp 写入的数据或命令值
******************************************/
void LCD_Fill_Pic(u16 x, u16 y,u16 pic_H, u16 pic_V, const unsigned char* pic)
{
  unsigned long i;
	unsigned int j;

// 	WriteComm(0x16); //Set_address_mode
// 	WriteData(0x20); //横屏，从左下角开始，从左到右，从下到上
// 	BlockWrite(x,x+pic_H-1,y,y+pic_V-1);
	for (i = 0; i < pic_H*pic_V*2; i+=2)
	{
		j=pic[i];
		j=j<<8;
		j=j+pic[i+1];
		*(__IO u16 *) (Bank1_LCD_D) = j;
	}
// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0xaa);
}

void DrawPixel(u16 x, u16 y, u16 Color)
{
	LCD_WR_REG(0x02,0); 
	LCD_WR_REG(0x03,0);     //Column Start 
	LCD_WR_REG(0x04,479>>8); 
	LCD_WR_REG(0x05,479&0xff);     //Column End 
	 
	LCD_WR_REG(0x06,0); 
	LCD_WR_REG(0x07,0);     //Row Start 
	LCD_WR_REG(0x08,319>>8); 
	LCD_WR_REG(0x09,319);     //Row End 

	//这里故意交换X,Y以实现横屏，也可以写两个这样的函数，一个竖的，一个横的
	LCD_WR_REG(0x80,x>>8); // Set CAC=0x0000 
	LCD_WR_REG(0x81,x);   
	LCD_WR_REG(0x82,y>>8); // Set RAC=0x0000 
	LCD_WR_REG(0x83,y);
	WriteComm(0x22);
	*(__IO u16 *) (Bank1_LCD_D) = Color;
}

u16 ReadPixel(u16 x,u8 y)
{
	u16 dat;
 	LCD_WR_REG(0x80,x>>8); // Set CAC=0x0000 
	LCD_WR_REG(0x81,x);   
	LCD_WR_REG(0x82,y>>8); // Set RAC=0x0000 
	LCD_WR_REG(0x83,y);
	WriteComm(0x22);

	dat = *(__IO u16 *) (Bank1_LCD_D);
	dat = *(__IO u16 *) (Bank1_LCD_D);
	dat = (dat&0xf800)|((dat&0x00fc)<<3)|((*(__IO u16 *) (Bank1_LCD_D))>>11);
	return dat;
}

typedef __packed struct
{
	u8  pic_head[2];				//1
	u16 pic_size_l;			    //2
	u16 pic_size_h;			    //3
	u16 pic_nc1;				    //4
	u16 pic_nc2;				    //5
	u16 pic_data_address_l;	    //6
	u16 pic_data_address_h;		//7	
	u16 pic_message_head_len_l;	//8
	u16 pic_message_head_len_h;	//9
	u16 pic_w_l;					//10
	u16 pic_w_h;				    //11
	u16 pic_h_l;				    //12
	u16 pic_h_h;				    //13	
	u16 pic_bit;				    //14
	u16 pic_dip;				    //15
	u16 pic_zip_l;			    //16
	u16 pic_zip_h;			    //17
	u16 pic_data_size_l;		    //18
	u16 pic_data_size_h;		    //19
	u16 pic_dipx_l;			    //20
	u16 pic_dipx_h;			    //21	
	u16 pic_dipy_l;			    //22
	u16 pic_dipy_h;			    //23
	u16 pic_color_index_l;	    //24
	u16 pic_color_index_h;	    //25
	u16 pic_other_l;			    //26
	u16 pic_other_h;			    //27
	u16 pic_color_p01;		    //28
	u16 pic_color_p02;		    //29
	u16 pic_color_p03;		    //30
	u16 pic_color_p04;		    //31
	u16 pic_color_p05;		    //32
	u16 pic_color_p06;		    //33
	u16 pic_color_p07;		    //34
	u16 pic_color_p08;			//35			
}BMP_HEAD;

BMP_HEAD bmp;

 


//任意屏大小范围内显示图片
/*
x，y像素起点坐标
*/
//char display_picture(char *filename)
//{
//	u16 ReadValue;
//	FATFS fs;            // Work area (file system object) for logical drive
//	FIL fsrc;      			// file objects
//	u8 buffer[2048]; 		// file copy buffer
//	FRESULT res;         // FatFs function common result code
//	UINT br;         		// File R/W count
//	u16 r_data,g_data,b_data;	
//	u32	 tx,ty,temp;
//	
//	
//  f_mount(0, &fs);

//  res = f_open(&fsrc, filename, FA_OPEN_EXISTING | FA_READ);	 //打上图片文件名
//  if(res==FR_NO_FILE||res==FR_INVALID_NAME){
//     f_mount(0, NULL);
//	 return 0;
//  }

//  if(res!=FR_OK){
//     f_mount(0, NULL);
//	 SD_Init();//重新初始化SD卡 
//	 return 0;
//  }

//  res = f_read(&fsrc, &bmp, sizeof(bmp), &br);

//  if(br!=sizeof(bmp))
//	{
//		f_close(&fsrc);
//		f_mount(0, NULL);
//	 	return 0;
//  }

//  if((bmp.pic_head[0]=='B')&&(bmp.pic_head[1]=='M'))
//  {
//	res = f_lseek(&fsrc, ((bmp.pic_data_address_h<<16)|bmp.pic_data_address_l));
//	if(res!=FR_OK){
//     f_close(&fsrc);
//     f_mount(0, NULL);
//	 return 0;
//    }
//	if(bmp.pic_dip==16)
//	{
//		temp=bmp.pic_w_l*2;	
//		if(bmp.pic_w_l>bmp.pic_h_l)
//		{
//		WriteComm(0x36); //Set_address_mode
//		WriteData(0x20); //横屏，从左下角开始，从左到右，从下到上
//		if(bmp.pic_w_l<480||bmp.pic_h_l<320)
//			{
//				Lcd_ColorBox(0,0,480,320,0x0000);
//				BlockWrite((480-bmp.pic_w_l)/2,(480-bmp.pic_w_l)/2+bmp.pic_w_l-1,(320-bmp.pic_h_l)/2,(320-bmp.pic_h_l)/2+bmp.pic_h_l-1);
//			}
//			else 	BlockWrite(0,479,0,319);
//		}
//		else
//		{
//			WriteComm(0x36); //Set_address_mode
//			WriteData(0x80); //竖屏，从左下角开始，从左到右，从下到上
//			if(bmp.pic_w_l<320||bmp.pic_h_l<480)
//				{
//					Lcd_ColorBox(0,0,320,480,0x0000);
//					BlockWrite((320-bmp.pic_w_l)/2,(320-bmp.pic_w_l)/2+bmp.pic_w_l-1,(480-bmp.pic_h_l)/2,(480-bmp.pic_h_l)/2+bmp.pic_h_l-1);
//				}
//				else BlockWrite(0,319,0,479);	
//		}
//		
//		for (tx = 0; tx < bmp.pic_h_l; tx++)
//		{
//		f_read(&fsrc, buffer, (bmp.pic_w_l)*2, &br);
//		for(ty=0;ty<temp;ty+=2)
//			{
//// 				r_data = *(ty +2+buffer);
//				g_data = *(ty +1+buffer);
//				b_data = *(ty +  buffer);			
//				ReadValue= (g_data<<8) | b_data;
//				g_data = ReadValue & 0x1f;  //保留低5位
//				b_data = ReadValue & 0xffe0;    //保留高11位
//				if(b_data & 0x20 == 0x20)
//				 {
//						 b_data <<= 1;
//				 }
//				 else
//				 {
//						 b_data <<= 1;
//						 b_data |= 0x20;
//				 }		
//				*(__IO u16 *) (Bank1_LCD_D) = g_data | b_data; 	
//			}
//		}	
//			f_close(&fsrc);
//			f_mount(0, NULL);
//		return 1;
//	}
//		
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
//	temp=bmp.pic_w_l*3;	
//	if(bmp.pic_w_l>bmp.pic_h_l)
//	{
// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0x20); //横屏，从左下角开始，从左到右，从下到上
//	if(bmp.pic_w_l<480||bmp.pic_h_l<320)
//		{
// 			Lcd_ColorBox(0,0,480,320,0x0000);
// 			BlockWrite((480-bmp.pic_w_l)/2,(480-bmp.pic_w_l)/2+bmp.pic_w_l-1,(320-bmp.pic_h_l)/2,(320-bmp.pic_h_l)/2+bmp.pic_h_l-1);
//		}
//		else 	BlockWrite(0,479,0,319);
//	}
//	else
//	{
//		WriteComm(0x36); //Set_address_mode
//		WriteData(0x80); //竖屏，从左下角开始，从左到右，从下到上
//		if(bmp.pic_w_l<320||bmp.pic_h_l<480)
//			{
//				Lcd_ColorBox(0,0,320,480,0x0000);
//				BlockWrite((320-bmp.pic_w_l)/2,(320-bmp.pic_w_l)/2+bmp.pic_w_l-1,(480-bmp.pic_h_l)/2,(480-bmp.pic_h_l)/2+bmp.pic_h_l-1);
//			}
//			else BlockWrite(0,319,0,479);	
//	}
//	
//	for (tx = 0; tx < bmp.pic_h_l; tx++)
//	{
//	f_read(&fsrc, buffer, (bmp.pic_w_l)*3, &br);
//	for(ty=0;ty<temp;ty+=3)
//		{
//			r_data = *(ty +2+buffer);
//			g_data = *(ty +1+buffer);
//			b_data = *(ty +  buffer);			
//			ReadValue=(r_data & 0xF8) << 8 | (g_data & 0xFC) << 3 | b_data >> 3;
//			*(__IO u16 *) (Bank1_LCD_D) = ReadValue; 	
//			
//			
//// 			*(__IO u16 *) (Bank1_LCD_D) = ((*(ty +2+buffer)<<8))|(*(ty +1+buffer)); 
//// 			*(__IO u16 *) (Bank1_LCD_D) = ((*(ty +0+buffer)<<8))|(*(ty +5+buffer));  
//// 			*(__IO u16 *) (Bank1_LCD_D) = ((*(ty +4+buffer)<<8))|(*(ty +3+buffer)); 
//		}
//	}	
//		f_close(&fsrc);
//   }
//  f_mount(0, NULL);
//// 	WriteComm(0x36); //Set_address_mode
////  	WriteData(0x08); //竖屏，从左上角开始，从左到右，从上到下
//  return 1;
//}