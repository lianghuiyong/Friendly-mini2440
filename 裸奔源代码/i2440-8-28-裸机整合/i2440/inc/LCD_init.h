/****************************************************************
 NAME: LCD_init.h
 DESC: 
 HISTORY:
 Mar.29.2002:purnnamu: created first
 ****************************************************************/
 
#ifndef __LCD_init_h__
#define __LCD_init_h__

void Pait_Bmp(int x0,int y0,int h,int l,const unsigned char *bmp);
void Lcd_ClearScr( U16 c);
void Lcd_Port_Init( void );
void LCD_Init(void);
void Lcd_EnvidOnOff(int onoff);
void Lcd_PowerEnable(int invpwren,int pwren);

#endif /*__LCD_init_h__*/