
#ifndef _GT31L16M1Y80_H_
#define _GT31L16M1Y80_H_

extern unsigned char r_dat_bat(unsigned long address,unsigned long byte_long,unsigned char *p_arr);

#define ASCII_5X7              1
#define ASCII_7X8              2
#define ASCII_7X10_P           3
#define ASCII_7X12_P           4
#define ASCII_8X16             5
#define ASCII_8X16_F           6
#define ASCII_16_A             7
#define ASCII_16X32            8
#define ASCII_16X32_F          9

unsigned char  ASCII_GetData(unsigned char  ASCIICode,unsigned long  ascii_kind,unsigned char *DZ_Data);
unsigned long  gt_16_GetData (unsigned char  c1, unsigned char  c2, unsigned char  c3, unsigned char  c4,unsigned char *DZ_Data);
unsigned long  U2GB(unsigned int  UCODE );
unsigned long  BIG5_G(unsigned int  B5code) ;
	


	
#endif 
