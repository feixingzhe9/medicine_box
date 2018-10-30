/******************************************************************************

 *重要说明！
 在.h文件中，#define Immediately时是立即显示当前画面
 而如果#define Delay，则只有在执行了lcd_wr_reg(0x0007,0x0173);
 之后才会显示，执行一次lcd_wr_reg(0x0007,0x0173)后，所有写入数
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
 * 函数名：lcd_fsmc_config
 * 描述  ：LCD  FSMC 模式配置
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */

static void lcd_fsmc_config(void)
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

static void lcd_rst(void)
{
    Clr_Rst;
    Delay(10000);
    Set_Rst;
    Delay(10000);
}

static void lcd_write_cmd(u16 CMD)
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
void lcd_init(void)
{
    //	int a;
    lcd_gpio_config();
    lcd_fsmc_config();
    lcd_rst();

    //================= Command page 0 =================
    lcd_wr_reg(0xFF,0x00); // Command page 0
    Delay(15);
    lcd_wr_reg(0x1A,0x04);//VGH VGL VCL  DDVDH
    lcd_wr_reg(0x1B,0x1C);
    // Power Settng
    lcd_wr_reg(0x23,0x94); // Set VCOM offset, VMF=0x52
    lcd_wr_reg(0x24,0x69); // Set VCOMH voltage, VHH=0x64
    lcd_wr_reg(0x25,0x63); // Set VCOML voltage, VML=0x71
    lcd_wr_reg(0x19,0x01);
    Delay(10);
    lcd_wr_reg(0x1A,0x00);//VGH VGL VCL  DDVDH
    lcd_wr_reg(0x1F,0x8A);//
    lcd_wr_reg(0x01,0x00);//
    lcd_wr_reg(0x1C,0x05);//
    lcd_wr_reg(0x1F,0x82);//
    Delay(10);

    lcd_wr_reg(0x1F,0x92);//
    Delay(10);
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

    lcd_wr_reg(0x16,0x88);//set my mx mv bgr...
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
    Delay(80);
    lcd_wr_reg(0x28,0x3C);// GON=0, DTE=0, D[1:0]=01

    lcd_wr_reg(0x80,0x00);
    lcd_wr_reg(0x81,0x00);
    lcd_wr_reg(0x82,0x00);
    lcd_wr_reg(0x83,0x00);

    Delay(100);


    *(__IO u16 *) (Bank1_LCD_C) = (0x22);


    Delay(10);

    Lcd_Light_ON;
    lcd_color_box(0,0,320,480,Yellow);
    //printf("lcd_read_pixel=%04x\r\n",lcd_read_pixel(8, 8));
    lcd_draw_pixel(10, 10, 0xaaaa);
    //printf("lcd_read_pixel=%04x\r\n",lcd_read_pixel(10, 10));
    lcd_draw_pixel(10, 11, 0XFFFF);
    //printf("lcd_read_pixel=%04x\r\n",lcd_read_pixel(10, 11));
    lcd_draw_pixel(10, 12, 0X00);
    //printf("lcd_read_pixel=%04x\r\n",lcd_read_pixel(10, 12));

}
/******************************************
  函数名：Lcd写命令函数
  功能：向Lcd指定位置写入应有命令或数据
  入口参数：Index 要寻址的寄存器地址
  ConfigTemp 写入的数据或命令值
 ******************************************/
void lcd_wr_reg(u16 Index,u16 CongfigTemp)
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
void lcd_block_write(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend)
{
    //HX8357-A

    lcd_wr_reg(0x80, Ystart >> 8); // Set CAC=0x0000
    lcd_wr_reg(0x81, Ystart);
    lcd_wr_reg(0x82, Xstart >> 8); // Set RAC=0x0000
    lcd_wr_reg(0x83, Xstart);

    lcd_wr_reg(0x02, Xstart >> 8);
    lcd_wr_reg(0x03, Xstart);     //Column Start
    lcd_wr_reg(0x04, Xend >> 8);
    lcd_wr_reg(0x05, Xend);     //Column End

    lcd_wr_reg(0x06, Ystart >> 8);
    lcd_wr_reg(0x07, Ystart);     //Row Start
    lcd_wr_reg(0x08, Yend >> 8);
    lcd_wr_reg(0x09, Yend);     //Row End

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
void lcd_color_box(u16 xStart, u16 yStart, u16 xLong, u16 yLong, u16 Color)
{
    u32 temp;

    lcd_block_write(xStart, xStart + xLong - 1, yStart, yStart + yLong - 1);
    for (temp = 0; temp < xLong * yLong; temp++)
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
void lcd_fill_pic(u16 x, u16 y, u16 pic_H, u16 pic_V, const unsigned char* pic)
{
    unsigned long i;
    unsigned int j;

    // 	lcd_write_cmd(0x16); //Set_address_mode
    // 	WriteData(0x20); //横屏，从左下角开始，从左到右，从下到上
    // 	lcd_block_write(x,x+pic_H-1,y,y+pic_V-1);
    for (i = 0; i < pic_H * pic_V * 2; i += 2)
    {
        j = pic[i];
        j = j << 8;
        j = j + pic[i + 1];
        *(__IO u16 *) (Bank1_LCD_D) = j;
    }
    // 	lcd_write_cmd(0x36); //Set_address_mode
    // 	WriteData(0xaa);
}

void lcd_draw_pixel(u16 x, u16 y, u16 Color)
{
    lcd_wr_reg(0x02, 0);
    lcd_wr_reg(0x03, 0);     //Column Start
    lcd_wr_reg(0x04, 479 >> 8);
    lcd_wr_reg(0x05, 479 & 0xff);     //Column End

    lcd_wr_reg(0x06, 0);
    lcd_wr_reg(0x07, 0);     //Row Start
    lcd_wr_reg(0x08, 319 >> 8);
    lcd_wr_reg(0x09, 319);     //Row End

    //这里故意交换X,Y以实现横屏，也可以写两个这样的函数，一个竖的，一个横的
    lcd_wr_reg(0x80, x >> 8); // Set CAC=0x0000
    lcd_wr_reg(0x81, x);
    lcd_wr_reg(0x82, y >> 8); // Set RAC=0x0000
    lcd_wr_reg(0x83, y);
    lcd_write_cmd(0x22);
    *(__IO u16 *) (Bank1_LCD_D) = Color;
}

u16 lcd_read_pixel(u16 x,u8 y)
{
    u16 dat;
    lcd_wr_reg(0x80, x >> 8); // Set CAC=0x0000
    lcd_wr_reg(0x81, x);
    lcd_wr_reg(0x82, y >> 8); // Set RAC=0x0000
    lcd_wr_reg(0x83, y);
    lcd_write_cmd(0x22);

    dat = *(__IO u16 *) (Bank1_LCD_D);
    dat = *(__IO u16 *) (Bank1_LCD_D);
    dat = (dat & 0xf800) | ((dat & 0x00fc) << 3) | ((*(__IO u16 *) (Bank1_LCD_D)) >> 11);
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
//		lcd_write_cmd(0x36); //Set_address_mode
//		WriteData(0x20); //横屏，从左下角开始，从左到右，从下到上
//		if(bmp.pic_w_l<480||bmp.pic_h_l<320)
//			{
//				lcd_color_box(0,0,480,320,0x0000);
//				lcd_block_write((480-bmp.pic_w_l)/2,(480-bmp.pic_w_l)/2+bmp.pic_w_l-1,(320-bmp.pic_h_l)/2,(320-bmp.pic_h_l)/2+bmp.pic_h_l-1);
//			}
//			else 	lcd_block_write(0,479,0,319);
//		}
//		else
//		{
//			lcd_write_cmd(0x36); //Set_address_mode
//			WriteData(0x80); //竖屏，从左下角开始，从左到右，从下到上
//			if(bmp.pic_w_l<320||bmp.pic_h_l<480)
//				{
//					lcd_color_box(0,0,320,480,0x0000);
//					lcd_block_write((320-bmp.pic_w_l)/2,(320-bmp.pic_w_l)/2+bmp.pic_w_l-1,(480-bmp.pic_h_l)/2,(480-bmp.pic_h_l)/2+bmp.pic_h_l-1);
//				}
//				else lcd_block_write(0,319,0,479);
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
// 	lcd_write_cmd(0x36); //Set_address_mode
// 	WriteData(0x20); //横屏，从左下角开始，从左到右，从下到上
//	if(bmp.pic_w_l<480||bmp.pic_h_l<320)
//		{
// 			lcd_color_box(0,0,480,320,0x0000);
// 			lcd_block_write((480-bmp.pic_w_l)/2,(480-bmp.pic_w_l)/2+bmp.pic_w_l-1,(320-bmp.pic_h_l)/2,(320-bmp.pic_h_l)/2+bmp.pic_h_l-1);
//		}
//		else 	lcd_block_write(0,479,0,319);
//	}
//	else
//	{
//		lcd_write_cmd(0x36); //Set_address_mode
//		WriteData(0x80); //竖屏，从左下角开始，从左到右，从下到上
//		if(bmp.pic_w_l<320||bmp.pic_h_l<480)
//			{
//				lcd_color_box(0,0,320,480,0x0000);
//				lcd_block_write((320-bmp.pic_w_l)/2,(320-bmp.pic_w_l)/2+bmp.pic_w_l-1,(480-bmp.pic_h_l)/2,(480-bmp.pic_h_l)/2+bmp.pic_h_l-1);
//			}
//			else lcd_block_write(0,319,0,479);
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
//// 	lcd_write_cmd(0x36); //Set_address_mode
////  	WriteData(0x08); //竖屏，从左上角开始，从左到右，从上到下
//  return 1;
//}
