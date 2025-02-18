#include "screen.h"
#include "lcd.h"
#include "image.h"
#include "remote.h"
#include "key.h"

u8 Page_Flag = 0;
int16_t show_Pitch;
int16_t show_Roll;
int16_t show_Yaw;

void Page_Num1(void)
{
	LCD_Clear(BLACK);
	LCD_Show_Image(20,10,64,64,yaokong_image);
	LCD_ShowChinese(110,15,"遥控数据",WHITE,BLACK,32,0);
	LCD_Show_Image(270,10,48,48,dianliang_image);
	LCD_Show_Image(270,110,50,50,you_image);
	Page_Flag = 0;
}

void Page_Num2(void)
{
	LCD_Clear(BLACK);
	LCD_Show_Image(20,10,64,64,wurenji_image);
	LCD_ShowChinese(110,15,"飞控数据",WHITE,BLACK,32,0);
	LCD_Show_Image(270,10,48,48,dianliang_image);
	LCD_Show_Image(10,110,50,50,zuo_image);
	Page_Flag = 1;
}

void Screen_Init(void)
{
	LCD_Init(HORIZONTAL);
	LCD_Clear(BLACK);
	Page_Num1();
}

void Screen_Show(void)
{
	if(Page_Flag==0){
		LCD_ShowChinese(155,70,"模式",YELLOW,BLACK,24,0);

		LCD_ShowChinese(155,110,"状态",YELLOW,BLACK,24,0);
		if(!Key_2){
			Mode = 0;
			LCD_ShowChinese(210,110,"飞行",YELLOW,BLACK,24,0);
		}else{
			Mode = 1;
			LCD_ShowChinese(210,110,"悬停",YELLOW,BLACK,24,0);
		}
		show_Pitch = (int16_t)Pitch-50;
		show_Roll = (int16_t)Roll-50;
		show_Yaw = (int16_t)Yaw-50;
		LCD_ShowChinese(30,80,"俯仰",YELLOW,BLACK,24,0);
		LCD_ShowNum(85,80,show_Pitch,3,YELLOW,BLACK,24);
		LCD_ShowChinese(30,120,"横滚",YELLOW,BLACK,24,0);
		LCD_ShowNum(85,120,show_Roll,3,YELLOW,BLACK,24);
		LCD_ShowChinese(30,160,"偏航",YELLOW,BLACK,24,0);
		LCD_ShowNum(85,160,show_Yaw,3,YELLOW,BLACK,24);
		LCD_ShowChinese(30,200,"油门",YELLOW,BLACK,24,0);
		LCD_ShowIntNum(85,200,Speed,4,YELLOW,BLACK,24);
	}else if(Page_Flag == 1){
		LCD_ShowChinese(200,70,"模式",YELLOW,BLACK,24,0);

		LCD_ShowChinese(200,110,"状态",YELLOW,BLACK,24,0);
		if(!Key_2){
			Mode = 0;
			LCD_ShowChinese(260,110,"飞行",YELLOW,BLACK,24,0);
		}else{
			Mode = 1;
			LCD_ShowChinese(260,110,"悬停",YELLOW,BLACK,24,0);
		}
		
		LCD_ShowString(80,80,"Pitch",YELLOW,BLACK,24,0);
		LCD_ShowIntNum(140,80,f_Pitch,3,YELLOW,BLACK,24);
		LCD_ShowString(80,120,"Roll",YELLOW,BLACK,24,0);
		LCD_ShowIntNum(140,120,f_Roll,3,YELLOW,BLACK,24);
		LCD_ShowString(80,160,"Yaw",YELLOW,BLACK,24,0);
		LCD_ShowIntNum(140,160,f_Yaw,3,YELLOW,BLACK,24);
		
		LCD_ShowChinese(160,200,"电量",YELLOW,BLACK,24,0);
		LCD_ShowIntNum(220,200,battery_level,3,YELLOW,BLACK,24);
	}

}

