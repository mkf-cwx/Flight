#ifndef __LCD_H__
#define __LCD_H__

#include "system.h"	 
 
typedef struct{
    uint16_t width;
    uint16_t height;
    uint8_t scan_dir;
}LCD_Attribute;

#define LCD_HEIGHT 320
#define LCD_WIDTH 240

#define HORIZONTAL 0
#define VERTICAL   1

//LCD参数
extern LCD_Attribute lcd;


////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义----------------// 

#define LCD_CS_PORT				GPIOB
#define LCD_CS_PIN				GPIO_Pin_12

#define LCD_RST_PORT			GPIOC                                                                                                                            
#define LCD_RST_PIN				GPIO_Pin_7

#define LCD_DC_PORT				GPIOC
#define LCD_DC_PIN				GPIO_Pin_6

#define LCD_BLK_PORT			GPIOA
#define LCD_BLK_PIN				GPIO_Pin_11

// 片选端口
#define LCD_CS_H		GPIO_SetBits(GPIOB, GPIO_Pin_12);	
#define LCD_CS_L		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
// 复位															  	
#define LCD_RST_H 		GPIO_SetBits(GPIOC, GPIO_Pin_7);
#define LCD_RST_L 		GPIO_ResetBits(GPIOC, GPIO_Pin_7);
// 数据/命令
#define LCD_DC_H		GPIO_SetBits(GPIOC, GPIO_Pin_6);
#define LCD_DC_L		GPIO_ResetBits(GPIOC, GPIO_Pin_6);
// 背光 开关 
#define	LCD_LED_H		GPIO_SetBits(GPIOA, GPIO_Pin_11);
#define	LCD_LED_L		GPIO_ResetBits(GPIOA, GPIO_Pin_11);


//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色 
#define LGRAY 					 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
void LCD_Init(uint8_t Scan_dir);														  
void LCD_Clear(uint16_t color);
void LCD_ShowChinese(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode);
void LCD_ShowChar(uint16_t X,uint16_t Y,uint8_t Char,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode);
void LCD_ShowString(uint16_t X,uint16_t Y,const uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode);
void LCD_ShowIntNum(uint16_t X,uint16_t Y,uint16_t Number,uint8_t Len,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size);
void LCD_ShowNum(uint16_t X,uint16_t Y,int16_t Number,uint8_t Len,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size);
void LCD_ShowFloatNum(uint16_t X,uint16_t Y,float Number,uint8_t Len,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size);
void LCD_Show_Image(u16 x, u16 y, u16 width, u16 height, const u8 *image);

#endif	// __LCD_H__

