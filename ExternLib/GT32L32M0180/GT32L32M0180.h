
#ifndef _GT32L32M0180_H_
#define _GT32L32M0180_H_
 
extern unsigned char r_dat_bat(unsigned long address,unsigned long byte_long,unsigned char *p_arr);

#define ASCII_5X7              1
#define ASCII_7X8              2
#define ASCII_7X8_F            3
#define ASCII_6X12             4
#define ASCII_8X16             5
#define ASCII_8X16_F           6
#define ASCII_12X24            7 
#define ASCII_16X32            9	
#define ASCII_16X32_F         10	
#define ASCII_12_A            11		
#define ASCII_12_T            12		
#define ASCII_16_A            13		
#define ASCII_16_T            14			
#define ASCII_24_A            15		
#define ASCII_24_T            16		
#define ASCII_32_A            17		
#define ASCII_32_T            18	

#define NUB_14X28							 1
#define NUB_20X40							 2
#define NUB_28X28							 3
#define NUB_40X40							 4

unsigned char	 ASCII_GetData(unsigned char  ASCIICode,unsigned long  ascii_kind,unsigned char *DZ_Data) ;
unsigned char	 Dig_Ch_GetData(unsigned char  Sequence,unsigned long  NUB_kind,unsigned char *DZ_Data) ;
unsigned long  gt_12_GetData(unsigned char  c1, unsigned char  c2, unsigned char  c3, unsigned char  c4,unsigned char *DZ_Data) ;
unsigned long  gt_16_GetData(unsigned char  c1, unsigned char  c2, unsigned char  c3, unsigned char  c4,unsigned char *DZ_Data);
unsigned long  gt_24_GetData(unsigned char  c1, unsigned char  c2, unsigned char  c3, unsigned char  c4,unsigned char *DZ_Data);
unsigned long  gt_32_GetData (unsigned char  c1, unsigned char  c2, unsigned char  c3, unsigned char  c4,unsigned char *DZ_Data);
unsigned long  BAR_CODE13(unsigned char  * BAR_NUM,unsigned char *BAR_PIC_ADDR);
unsigned long  BAR_CODE128(unsigned char  *BAR_NUM,unsigned char  flag,unsigned char *BAR_PIC_ADDR);
unsigned long  Antenna_CODE_12X12_GetData(unsigned char  NUM,unsigned char *DZ_Data);
unsigned long  Battery_CODE_12X12_GetData(unsigned char   NUM,unsigned char *DZ_Data);
unsigned long  U2G(unsigned int  unicode );
unsigned int   U2G_13(unsigned int  Unicode);
unsigned long  BIG5_G(unsigned int  B5code);

#endif




