/******************************************************************************

 *重要说明！
 在.h文件中，#define Immediately时是立即显示当前画面
 而如果#define lcd_delay，则只有在执行了lcd_wr_reg(0x0007,0x0173);
 之后才会显示，执行一次lcd_wr_reg(0x0007,0x0173)后，所有写入数
 据都立即显示。
#define lcd_delay一般用在开机画面的显示，防止显示出全屏图像的刷新过程
 ******************************************************************************/
#include "stm32f10x.h"
#include "lcd.h"
#include <stdio.h>
#include <stm32f10x_fsmc.h>
#include "delay.h"

/*
 * 函数名：lcd_gpio_config
 * 描述  ：根据FSMC配置LCD的I/O
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
void lcd_gpio_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable the FSMC Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
//    delay_us_ex(100 * 1000);
    /* config lcd gpio clock base on FSMC */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    /* config tft rst gpio */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    //backlight
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_0);

    /* config tft back_light gpio base on the PT4101 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* config tft data lines base on FSMC
     * data lines,FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10
     */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;    //GPIO_Speed_50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |\
        GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |\
        GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* config tft control lines base on FSMC
     * PD4-FSMC_NOE  :LCD-RD
     * PD5-FSMC_NWE  :LCD-WR
     * PD7-FSMC_NE1  :LCD-CS
     * PD11-FSMC_A16 :LCD-DC
     */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;// RD
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;// WR
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOG, &GPIO_InitStructure);  //CS

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;//DC
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* tft control gpio init */


    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOG, &GPIO_InitStructure);


    GPIO_SetBits(GPIOG, GPIO_Pin_8);		 // RST = 1
    GPIO_SetBits(GPIOD, GPIO_Pin_4);		 // RD = 1
    GPIO_SetBits(GPIOD, GPIO_Pin_5);		 // WR = 1
    GPIO_SetBits(GPIOG, GPIO_Pin_10);		 //	CS = 1


//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//    GPIO_Init(GPIOG, &GPIO_InitStructure);

//    GPIO_ResetBits(GPIOG, GPIO_Pin_6);		 //	DE = 0
}


static void lcd_fsmc_config(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p;

    FSMC_NORSRAMDeInit(FSMC_Bank1_NORSRAM3);
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);

    p.FSMC_AddressSetupTime = 0x02;	 //地址建立时间
    p.FSMC_AddressHoldTime = 0x00;	 //地址保持时间
    p.FSMC_DataSetupTime = 0x05;		 //数据建立时间
    p.FSMC_BusTurnAroundDuration = 0x00;
    p.FSMC_CLKDivision = 0x00;
    p.FSMC_DataLatency = 0x00;

    p.FSMC_AccessMode = FSMC_AccessMode_B;	 // 一般使用模式B来控制LCD

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
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
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);
}

static void lcd_delay(__IO u32 count)
{
    volatile int i = 0;
    for(i = 0;i < 0XFFFF; i++)
        for(; count != 0; count--);
}

static void lcd_rst(void)
{
    LCD_CLR_RST;
    lcd_delay(90000);
    LCD_SET_RST;
    lcd_delay(90000);
}

static void lcd_write_cmd(u16 CMD)
{
    *(__IO u16 *) (BANK_1_LCD_CMD) = CMD;
}

//static void lcd_write_data(u16 tem_data)
//{
//    *(__IO u16 *) (BANK_1_LCD_DATA) = tem_data;
//}


void lcd_init(void)
{
    //	int a;
    lcd_gpio_config();
    lcd_fsmc_config();
    lcd_rst();

    //================= Command page 0 =================
    lcd_wr_reg(0xFF,0x00); // Command page 0
    lcd_delay(15);
    lcd_wr_reg(0x1A,0x04);//VGH VGL VCL  DDVDH
    lcd_wr_reg(0x1B,0x1C);
    // Power Settng
    lcd_wr_reg(0x23,0x94); // Set VCOM offset, VMF=0x52
    lcd_wr_reg(0x24,0x69); // Set VCOMH voltage, VHH=0x64
    lcd_wr_reg(0x25,0x63); // Set VCOML voltage, VML=0x71
    lcd_wr_reg(0x19,0x01);
    lcd_delay(10);
    lcd_wr_reg(0x1A,0x00);//VGH VGL VCL  DDVDH
    lcd_wr_reg(0x1F,0x8A);//
    lcd_wr_reg(0x01,0x00);//
    lcd_wr_reg(0x1C,0x05);//
    lcd_wr_reg(0x1F,0x82);//
    lcd_delay(10);

    lcd_wr_reg(0x1F,0x92);//
    lcd_delay(10);
    lcd_wr_reg(0x1F,0xD4);//
    // Set GRAM aea 320x480
    lcd_wr_reg(0x02,0x00);
    lcd_wr_reg(0x03,0x00);
    lcd_wr_reg(0x04,0x01);
    lcd_wr_reg(0x05,0x3F);
    lcd_wr_reg(0x06,0x00);
    lcd_wr_reg(0x07,0x00);
    lcd_wr_reg(0x08,0x01);
    lcd_wr_reg(0x09,0xDF);

#if (DIS_DIRECTION & DIRECTION_VERTICAL)
    #if (DIS_DIRECTION & DIRECTION_TYPE_A)

    lcd_wr_reg(0x016,0x58);//set my mx mv bgr...    HX8357-A芯片手册datasheet.pdf  page 89 and page 153

    #elif (DIS_DIRECTION & DIRECTION_TYPE_B)

    lcd_wr_reg(0x016,0x98);//set my mx mv bgr...    HX8357-A芯片手册datasheet.pdf  page 89 and page 153

    #endif

#else
    #if (DIS_DIRECTION & DIRECTION_TYPE_A)

    lcd_wr_reg(0x16,0x38);//set my mx mv bgr...     HX8357-A芯片手册datasheet.pdf  page 89 and page 153

    #elif (DIS_DIRECTION & DIRECTION_TYPE_B)

    lcd_wr_reg(0x16,0xe8);//set my mx mv bgr...     HX8357-A芯片手册datasheet.pdf  page 89 and page 153

    #endif

#endif

    lcd_wr_reg(0x17,0x55);//262-bit/pixel 0X60 ;262-bit/pixel 0X50
    lcd_wr_reg(0x18,0x21);	//Fosc=130%*5.2MHZ   21
    lcd_wr_reg(0x1D,0x00); // FS0[1:0]=01, Set the operating frequency of the step-up circuit 1
    lcd_wr_reg(0x1E,0x00);

    lcd_wr_reg(0x26,0x33);
    lcd_wr_reg(0x27,0x01);
    lcd_wr_reg(0x29,0x00);
    lcd_wr_reg(0x2A,0x00);
    lcd_wr_reg(0x2B,0x01);//0A
    lcd_wr_reg(0x2C,0x0A);
    lcd_wr_reg(0x2D,0x20);
    lcd_wr_reg(0x2E,0xA3);
    lcd_wr_reg(0x2F,0x00);//00 100416

    //   lcd_wr_reg(0x31,0x00);
    // 	lcd_wr_reg(0x32,0x00);
    // 	lcd_wr_reg(0x33,0x08);
    //   lcd_wr_reg(0x34,0x02);

    lcd_wr_reg(0x31,0x02);//RGB MODE1  0X03=RGB MODE2
    lcd_wr_reg(0x32,0x00);
    lcd_wr_reg(0x33,0x08);
    lcd_wr_reg(0x34,0x08);
    lcd_wr_reg(0x36,0x02);//REV

    // Gamma
    lcd_wr_reg(0x40,0x01);
    lcd_wr_reg(0x41,0x0F);
    lcd_wr_reg(0x42,0x0F);
    lcd_wr_reg(0x43,0x26);
    lcd_wr_reg(0x44,0x2C);
    lcd_wr_reg(0x45,0x3C);
    lcd_wr_reg(0x46,0x0B);
    lcd_wr_reg(0x47,0x5F);
    lcd_wr_reg(0x48,0x00);
    lcd_wr_reg(0x49,0x06);
    lcd_wr_reg(0x4A,0x09);
    lcd_wr_reg(0x4B,0x0E);
    lcd_wr_reg(0x4C,0x16);

    lcd_wr_reg(0x50,0x01);
    lcd_wr_reg(0x51,0x1D);
    lcd_wr_reg(0x52,0x21);
    lcd_wr_reg(0x53,0x3A);
    lcd_wr_reg(0x54,0x39);
    lcd_wr_reg(0x55,0x3F);
    lcd_wr_reg(0x56,0x2D);
    lcd_wr_reg(0x57,0x7F);
    lcd_wr_reg(0x58,0x02);
    lcd_wr_reg(0x59,0x15);
    lcd_wr_reg(0x5A,0x1B);
    lcd_wr_reg(0x5B,0x1B);
    lcd_wr_reg(0x5C,0x1A);
    lcd_wr_reg(0x5D,0x55);
    //================= Command page 1 =================
    lcd_wr_reg(0xff,0x01);
    lcd_wr_reg(0x00,0x01);
    lcd_wr_reg(0x01,0x00);
    lcd_wr_reg(0x02,0x01);
    lcd_wr_reg(0x03,0x03);
    lcd_wr_reg(0x04,0x05);
    lcd_wr_reg(0x05,0x06);
    lcd_wr_reg(0x06,0x08);
    lcd_wr_reg(0x07,0x0C);
    lcd_wr_reg(0x08,0x0E);
    lcd_wr_reg(0x09,0x11);
    lcd_wr_reg(0x0A,0x12);
    lcd_wr_reg(0x0B,0x14);
    lcd_wr_reg(0x0C,0x1B);
    lcd_wr_reg(0x0D,0x23);
    lcd_wr_reg(0x0E,0x29);
    lcd_wr_reg(0x0F,0x2F);
    lcd_wr_reg(0x10,0x34);
    lcd_wr_reg(0x11,0x39);
    lcd_wr_reg(0x12,0x3E);
    lcd_wr_reg(0x13,0x43);
    lcd_wr_reg(0x14,0x48);
    lcd_wr_reg(0x15,0x4C);
    lcd_wr_reg(0x16,0x51);
    lcd_wr_reg(0x17,0x55);
    lcd_wr_reg(0x18,0x59);
    lcd_wr_reg(0x19,0x5D);
    lcd_wr_reg(0x1A,0x60);
    lcd_wr_reg(0x1B,0x64);
    lcd_wr_reg(0x1C,0x68);
    lcd_wr_reg(0x1D,0x6C);
    lcd_wr_reg(0x1E,0x70);
    lcd_wr_reg(0x1F,0x73);
    lcd_wr_reg(0x20,0x77);
    lcd_wr_reg(0x21,0x7B);
    lcd_wr_reg(0x22,0x7F);
    lcd_wr_reg(0x23,0x83);
    lcd_wr_reg(0x24,0x87);
    lcd_wr_reg(0x25,0x8A);
    lcd_wr_reg(0x26,0x8E);
    lcd_wr_reg(0x27,0x92);
    lcd_wr_reg(0x28,0x96);
    lcd_wr_reg(0x29,0x9A);
    lcd_wr_reg(0x2A,0x9F);
    lcd_wr_reg(0x2B,0xA3);
    lcd_wr_reg(0x2C,0xA7);
    lcd_wr_reg(0x2D,0xAC);
    lcd_wr_reg(0x2E,0xAF);
    lcd_wr_reg(0x2F,0xB3);
    lcd_wr_reg(0x30,0xB7);
    lcd_wr_reg(0x31,0xBA);
    lcd_wr_reg(0x32,0xBE);
    lcd_wr_reg(0x33,0xC3);
    lcd_wr_reg(0x34,0xC7);
    lcd_wr_reg(0x35,0xCC);
    lcd_wr_reg(0x36,0xD1);
    lcd_wr_reg(0x37,0xD7);
    lcd_wr_reg(0x38,0xDD);
    lcd_wr_reg(0x39,0xE3);
    lcd_wr_reg(0x3A,0xE8);
    lcd_wr_reg(0x3B,0xEA);
    lcd_wr_reg(0x3C,0xEC);
    lcd_wr_reg(0x3D,0xEF);
    lcd_wr_reg(0x3E,0xF3);
    lcd_wr_reg(0x3F,0xF6);

    lcd_wr_reg(0x40,0xFE);
    lcd_wr_reg(0x41,0x00);
    lcd_wr_reg(0x42,0x01);
    lcd_wr_reg(0x43,0x03);
    lcd_wr_reg(0x44,0x05);
    lcd_wr_reg(0x45,0x06);
    lcd_wr_reg(0x46,0x08);
    lcd_wr_reg(0x47,0x0C);
    lcd_wr_reg(0x48,0x0E);
    lcd_wr_reg(0x49,0x11);
    lcd_wr_reg(0x4A,0x12);
    lcd_wr_reg(0x4B,0x14);
    lcd_wr_reg(0x4C,0x1B);
    lcd_wr_reg(0x4D,0x23);
    lcd_wr_reg(0x4E,0x29);
    lcd_wr_reg(0x4F,0x2F);
    lcd_wr_reg(0x50,0x34);
    lcd_wr_reg(0x51,0x39);
    lcd_wr_reg(0x52,0x3E);
    lcd_wr_reg(0x53,0x43);
    lcd_wr_reg(0x54,0x48);
    lcd_wr_reg(0x55,0x4C);
    lcd_wr_reg(0x56,0x51);
    lcd_wr_reg(0x57,0x55);
    lcd_wr_reg(0x58,0x59);
    lcd_wr_reg(0x59,0x5D);
    lcd_wr_reg(0x5A,0x60);
    lcd_wr_reg(0x5B,0x64);
    lcd_wr_reg(0x5C,0x68);
    lcd_wr_reg(0x5D,0x6C);

    lcd_wr_reg(0x5E,0x70);
    lcd_wr_reg(0x5F,0x73);
    lcd_wr_reg(0x60,0x77);
    lcd_wr_reg(0x61,0x7B);
    lcd_wr_reg(0x62,0x7F);
    lcd_wr_reg(0x63,0x83);
    lcd_wr_reg(0x64,0x87);
    lcd_wr_reg(0x65,0x8A);
    lcd_wr_reg(0x66,0x8E);
    lcd_wr_reg(0x67,0x92);
    lcd_wr_reg(0x68,0x96);
    lcd_wr_reg(0x69,0x9A);
    lcd_wr_reg(0x6A,0x9F);
    lcd_wr_reg(0x6B,0xA3);
    lcd_wr_reg(0x6C,0xA7);
    lcd_wr_reg(0x6D,0xAC);
    lcd_wr_reg(0x6E,0xAF);
    lcd_wr_reg(0x6F,0xB3);
    lcd_wr_reg(0x70,0xB7);
    lcd_wr_reg(0x71,0xBA);
    lcd_wr_reg(0x72,0xBE);
    lcd_wr_reg(0x73,0xC3);
    lcd_wr_reg(0x74,0xC7);
    lcd_wr_reg(0x75,0xCC);
    lcd_wr_reg(0x76,0xD1);
    lcd_wr_reg(0x77,0xD7);
    lcd_wr_reg(0x78,0xDD);
    lcd_wr_reg(0x79,0xE3);
    lcd_wr_reg(0x7A,0xE8);
    lcd_wr_reg(0x7B,0xEA);
    lcd_wr_reg(0x7C,0xEC);
    lcd_wr_reg(0x7D,0xEF);
    lcd_wr_reg(0x7E,0xF3);
    lcd_wr_reg(0x7F,0xF6);
    lcd_wr_reg(0x80,0xFE);
    lcd_wr_reg(0x81,0x00);
    lcd_wr_reg(0x82,0x01);
    lcd_wr_reg(0x83,0x03);
    lcd_wr_reg(0x84,0x05);
    lcd_wr_reg(0x85,0x06);
    lcd_wr_reg(0x86,0x08);
    lcd_wr_reg(0x87,0x0C);
    lcd_wr_reg(0x88,0x0E);
    lcd_wr_reg(0x89,0x11);
    lcd_wr_reg(0x8A,0x12);
    lcd_wr_reg(0x8B,0x14);
    lcd_wr_reg(0x8C,0x1B);
    lcd_wr_reg(0x8D,0x23);
    lcd_wr_reg(0x8E,0x29);
    lcd_wr_reg(0x8F,0x2F);
    lcd_wr_reg(0x90,0x34);
    lcd_wr_reg(0x91,0x39);
    lcd_wr_reg(0x92,0x3E);
    lcd_wr_reg(0x93,0x43);
    lcd_wr_reg(0x94,0x48);
    lcd_wr_reg(0x95,0x4C);
    lcd_wr_reg(0x96,0x51);
    lcd_wr_reg(0x97,0x55);
    lcd_wr_reg(0x98,0x59);
    lcd_wr_reg(0x99,0x5D);
    lcd_wr_reg(0x9A,0x60);
    lcd_wr_reg(0x9B,0x64);
    lcd_wr_reg(0x9C,0x68);
    lcd_wr_reg(0x9D,0x6C);
    lcd_wr_reg(0x9E,0x70);
    lcd_wr_reg(0x9F,0x73);
    lcd_wr_reg(0xA0,0x77);
    lcd_wr_reg(0xA1,0x7B);
    lcd_wr_reg(0xA2,0x7F);
    lcd_wr_reg(0xA3,0x83);
    lcd_wr_reg(0xA4,0x87);
    lcd_wr_reg(0xA5,0x8A);
    lcd_wr_reg(0xA6,0x8E);
    lcd_wr_reg(0xA7,0x92);
    lcd_wr_reg(0xA8,0x96);
    lcd_wr_reg(0xA9,0x9A);
    lcd_wr_reg(0xAA,0x9F);
    lcd_wr_reg(0xAB,0xA3);
    lcd_wr_reg(0xAC,0xA7);
    lcd_wr_reg(0xAD,0xAC);
    lcd_wr_reg(0xAE,0xAF);
    lcd_wr_reg(0xAF,0xB3);
    lcd_wr_reg(0xB0,0xB7);
    lcd_wr_reg(0xB1,0xBA);
    lcd_wr_reg(0xB2,0xBE);
    lcd_wr_reg(0xB3,0xC3);
    lcd_wr_reg(0xB4,0xC7);
    lcd_wr_reg(0xB5,0xCC);
    lcd_wr_reg(0xB6,0xD1);
    lcd_wr_reg(0xB7,0xD7);
    lcd_wr_reg(0xB8,0xDD);
    lcd_wr_reg(0xB9,0xE3);
    lcd_wr_reg(0xBA,0xE8);
    lcd_wr_reg(0xBB,0xEA);
    lcd_wr_reg(0xBC,0xEC);
    lcd_wr_reg(0xBD,0xEF);
    lcd_wr_reg(0xBE,0xF3);
    lcd_wr_reg(0xBF,0xF6);
    lcd_wr_reg(0xC0,0xFE);

    //================= Command page 0 =================
    lcd_wr_reg(0xff,0x00);
    lcd_wr_reg(0x60,0x08);
    lcd_wr_reg(0xF2,0x00);
    lcd_wr_reg(0xE4,0x1F); //EQVCI_M1=0x00
    lcd_wr_reg(0xE5,0x1F); //EQGND_M1=0x1C
    lcd_wr_reg(0xE6,0x20); //EQVCI_M0=0x1C
    lcd_wr_reg(0xE7,0x00); //EQGND_M0=0x1C
    lcd_wr_reg(0xE8,0xD1);
    lcd_wr_reg(0xE9,0xC0);
    lcd_wr_reg(0x28,0x38);
    lcd_delay(80);
    lcd_wr_reg(0x28,0x3C);// GON=0, DTE=0, D[1:0]=01

    lcd_wr_reg(0x80,0x00);
    lcd_wr_reg(0x81,0x00);
    lcd_wr_reg(0x82,0x00);
    lcd_wr_reg(0x83,0x00);

    lcd_delay(100);


    *(__IO u16 *) (BANK_1_LCD_CMD) = (0x22);


    lcd_delay(10);

    LCD_BACK_LIGHT_ON;
//    lcd_color_box(0, 0, 320, 480, Yellow);
//    lcd_draw_pixel(10, 10, 0xaaaa);
//    lcd_draw_pixel(10, 11, 0XFFFF);
//    lcd_draw_pixel(10, 12, 0X00);

}


//void change_dis_direction()
/******************************************
  函数名：Lcd写命令函数
  功能：向Lcd指定位置写入应有命令或数据
  入口参数：Index 要寻址的寄存器地址
  ConfigTemp 写入的数据或命令值
 ******************************************/
void lcd_wr_reg(u16 index,u16 config)
{
    *(__IO u16 *) (BANK_1_LCD_CMD) = index;
    *(__IO u16 *) (BANK_1_LCD_DATA) = config;
}
/************************************************
  函数名：Lcd写开始函数
  功能：控制Lcd控制引脚 执行写操作
 ************************************************/
void lcd_wr_start(void)
{
    *(__IO u16 *) (BANK_1_LCD_CMD) = 0x2C;
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
void lcd_block_write(u16 x_start,u16 x_end,u16 y_start, u16 y_end)
{
    //HX8357-A
    lcd_wr_reg(0x80, x_start >> 8); // Set CAC=0x0000
    lcd_wr_reg(0x81, x_start & 0xff);
    lcd_wr_reg(0x82, y_start >> 8); // Set RAC=0x0000
    lcd_wr_reg(0x83, y_start & 0xff);

    lcd_wr_reg(0x02, x_start >> 8);
    lcd_wr_reg(0x03, x_start & 0xff);     //Column Start
    lcd_wr_reg(0x04, x_end >> 8);
    lcd_wr_reg(0x05, x_end & 0xff);     //Column End

    lcd_wr_reg(0x06, y_start >> 8);
    lcd_wr_reg(0x07, y_start & 0xff);     //Row Start
    lcd_wr_reg(0x08, y_end >> 8);
    lcd_wr_reg(0x09, y_end & 0xff);     //Row End


    lcd_write_cmd(0x22);
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
void lcd_color_box(uint16_t x_start, uint16_t y_start, uint16_t x_long, uint16_t y_long, uint16_t color)
{
    uint32_t i = 0;

    lcd_block_write(x_start, x_start + x_long - 1, y_start, y_start + y_long - 1);
    for (i = 0; i < x_long * y_long; i++)
    {
        *(__IO u16 *) (BANK_1_LCD_DATA) = color;
    }
}

/******************************************
  函数名：Lcd图像填充
  功能：向Lcd指定位置填充图像
  入口参数：Index 要寻址的寄存器地址
  ConfigTemp 写入的数据或命令值
 ******************************************/
void lcd_fill_pic(u16 x, u16 y,u16 pic_h, u16 pic_v, const unsigned char* pic)
{
    unsigned long i;
    unsigned int j;

//     	lcd_write_cmd(0x16); //Set_address_mode
//     	lcd_write_data(0x20); //横屏，从左下角开始，从左到右，从下到上
//     	lcd_block_write(x,x+pic_H-1,y,y+pic_V-1);
    for (i = 0; i < pic_h * pic_v * 2; i += 2)
    {
        j = pic[i];
        j = j << 8;
        j = j + pic[i + 1];
        *(__IO u16 *) (BANK_1_LCD_DATA) = j;
    }
//     	lcd_write_cmd(0x36); //Set_address_mode
//     	lcd_write_data(0xaa);
}

void lcd_draw_pixel(u16 x, u16 y, u16 color)
{
#if (DIS_DIRECTION & DIRECTION_HORIZONTAL)
    lcd_wr_reg(0x02, 0);
    lcd_wr_reg(0x03, 0);     //Column Start
    lcd_wr_reg(0x04, (LCD_X_MAX - 1) >> 8);
    lcd_wr_reg(0x05, (LCD_X_MAX - 1) & 0xff);     //Column End

    lcd_wr_reg(0x06, 0);
    lcd_wr_reg(0x07, 0);     //Row Start
    lcd_wr_reg(0x08, (LCD_Y_MAX - 1) >> 8);
    lcd_wr_reg(0x09, (LCD_Y_MAX - 1) & 0xff);     //Row End

    //这里故意交换X,Y以实现横屏，也可以写两个这样的函数，一个竖的，一个横的
    lcd_wr_reg(0x80, y >> 8); // Set CAC=0x0000
    lcd_wr_reg(0x81, y & 0xff);
    lcd_wr_reg(0x82, x >> 8); // Set RAC=0x0000
    lcd_wr_reg(0x83, x & 0xff);
#else
    lcd_wr_reg(0x02, 0);
    lcd_wr_reg(0x03, 0);     //Column Start
    lcd_wr_reg(0x04, (LCD_X_MAX - 1) >> 8);
    lcd_wr_reg(0x05, (LCD_X_MAX - 1) & 0xff);     //Column End

    lcd_wr_reg(0x06, 0);
    lcd_wr_reg(0x07, 0);     //Row Start
    lcd_wr_reg(0x08, (LCD_Y_MAX - 1) >> 8);
    lcd_wr_reg(0x09, (LCD_Y_MAX - 1) & 0xff);     //Row End

    //这里故意交换X,Y以实现横屏，也可以写两个这样的函数，一个竖的，一个横的
    lcd_wr_reg(0x80, x >> 8); // Set CAC=0x0000
    lcd_wr_reg(0x81, x & 0xff);
    lcd_wr_reg(0x82, y >> 8); // Set RAC=0x0000
    lcd_wr_reg(0x83, y & 0xff);
#endif
    lcd_write_cmd(0x22);
    *(__IO u16 *) (BANK_1_LCD_DATA) = color;
}

u16 lcd_read_pixel(u16 x, u8 y)
{
    u16 dat;
    lcd_wr_reg(0x80, x >> 8); // Set CAC=0x0000
    lcd_wr_reg(0x81, x);
    lcd_wr_reg(0x82, y >> 8); // Set RAC=0x0000
    lcd_wr_reg(0x83, y);
    lcd_write_cmd(0x22);

    dat = *(__IO u16 *) (BANK_1_LCD_DATA);
    dat = *(__IO u16 *) (BANK_1_LCD_DATA);
    dat = (dat & 0xf800) | ((dat & 0x00fc) << 3) | ((*(__IO u16 *) (BANK_1_LCD_DATA)) >> 11);
    return dat;
}



