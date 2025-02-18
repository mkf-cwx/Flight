#include "lcd.h"
#include "spi.h"
#include "dma.h"
#include "fonts.h"

LCD_Attribute lcd;

					    
//写命令函数
//reg:寄存器值
void LCD_WR_Cmd(u8 reg)
{
	LCD_CS_L;		
	LCD_DC_L;//RS=0 命令	
	
	SPI_SendByte(reg);

	LCD_CS_H;				
}
//写数据函数
//data:寄存器值
void LCD_WR_DATA_8Bit(u8 data)
{
	LCD_CS_L;		
	LCD_DC_H;//RS=1 数据
	
	SPI_SendByte(data);
	LCD_CS_H;			
}

void LCD_WR_DATA_16Bit(u16 data)
{
	LCD_CS_L;		
	LCD_DC_H;//RS=1 数据
	
	SPI_SendByte((data>>8)&0xff);
	SPI_SendByte(data&0xff);
	LCD_CS_H;
}

//写寄存器
//LCD_Reg:寄存器编号
//LCD_RegValue:要写入的值
void LCD_WriteReg(u8 LCD_Reg,u16 LCD_RegValue)
{	
	LCD_WR_Cmd(LCD_Reg);  
	LCD_WR_DATA_16Bit(LCD_RegValue);	    		 
}

void LCD_Set_Attributes(uint8_t Scan_dir)
{
	lcd.scan_dir = Scan_dir;
	
	// 竖屏:0x00-旋转0度， 0xc0-旋转180度， 横屏:0x70-旋转90度， 0xA0-旋转270度
	if(Scan_dir == HORIZONTAL){
		lcd.height = LCD_WIDTH;
		lcd.width = LCD_HEIGHT;
		LCD_WR_Cmd(0x36);
		LCD_WR_DATA_8Bit(0xA0);
	}else{
		lcd.height = LCD_HEIGHT;
		lcd.width = LCD_WIDTH;
		LCD_WR_Cmd(0x36);
		LCD_WR_DATA_8Bit(0x00);
	}
}


void ST7789v_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能 GPIOA 时钟;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能 GPIOB 时钟;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能 GPIOC 时钟;
	
	GPIO_InitStructure.GPIO_Pin = LCD_BLK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_BLK_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_CS_PIN;
	GPIO_Init(LCD_CS_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_RST_PIN | LCD_DC_PIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	LCD_CS_H;
	LCD_RST_H;
	LCD_DC_H;
	LCD_LED_H;	//	开启背光
}


//初始化lcd
void LCD_Init(uint8_t Scan_dir)
{	
	ST7789v_GPIOInit();
	SPI2_Config_Init();
	
	LCD_RST_H;
	delay_ms(1);
	LCD_RST_L;
	delay_ms(10);
	LCD_RST_H;
	delay_ms(120);  


	//************* Start Initial Sequence **********//
	delay_ms(120);                //delay_ms 120ms
	LCD_WR_Cmd(0x11);     
	delay_ms(120);                //delay_ms 120ms
	
	LCD_Set_Attributes(Scan_dir);
	
	LCD_WR_Cmd(0x3A);     
	LCD_WR_DATA_8Bit(0x55);   
	LCD_WR_Cmd(0xB2);     
	LCD_WR_DATA_8Bit(0x0C);   
	LCD_WR_DATA_8Bit(0x0C);   
	LCD_WR_DATA_8Bit(0x00);   
	LCD_WR_DATA_8Bit(0x33);   
	LCD_WR_DATA_8Bit(0x33);   
	LCD_WR_Cmd(0xB7);     
	LCD_WR_DATA_8Bit(0x75);   
	LCD_WR_Cmd(0xBB);     
	LCD_WR_DATA_8Bit(0x15);   
	LCD_WR_Cmd(0xC0);     
	LCD_WR_DATA_8Bit(0x2C);   
	LCD_WR_Cmd(0xC2);     
	LCD_WR_DATA_8Bit(0x01);   
	LCD_WR_Cmd(0xC3);     
	LCD_WR_DATA_8Bit(0x13);    
	LCD_WR_Cmd(0xC4);     
	LCD_WR_DATA_8Bit(0x23);   
	LCD_WR_Cmd(0xC6);     
	LCD_WR_DATA_8Bit(0x0F);   
	LCD_WR_Cmd(0xD0);     
	LCD_WR_DATA_8Bit(0xA4);   
	LCD_WR_DATA_8Bit(0xA1);   
	LCD_WR_Cmd(0xD6);     
	LCD_WR_DATA_8Bit(0xA1);
	LCD_WR_Cmd(0x21);     
	LCD_WR_Cmd(0xE0);
	LCD_WR_DATA_8Bit(0xD0);
	LCD_WR_DATA_8Bit(0x08);
	LCD_WR_DATA_8Bit(0x10);
	LCD_WR_DATA_8Bit(0x0D);
	LCD_WR_DATA_8Bit(0x0C);
	LCD_WR_DATA_8Bit(0x07);
	LCD_WR_DATA_8Bit(0x37);
	LCD_WR_DATA_8Bit(0x53);
	LCD_WR_DATA_8Bit(0x4C);
	LCD_WR_DATA_8Bit(0x39);
	LCD_WR_DATA_8Bit(0x15);
	LCD_WR_DATA_8Bit(0x15);
	LCD_WR_DATA_8Bit(0x2A);
	LCD_WR_DATA_8Bit(0x2D);
	LCD_WR_Cmd(0xE1);
	LCD_WR_DATA_8Bit(0xD0);
	LCD_WR_DATA_8Bit(0x0D);
	LCD_WR_DATA_8Bit(0x12);
	LCD_WR_DATA_8Bit(0x08);
	LCD_WR_DATA_8Bit(0x08);
	LCD_WR_DATA_8Bit(0x15);
	LCD_WR_DATA_8Bit(0x34);
	LCD_WR_DATA_8Bit(0x34);
	LCD_WR_DATA_8Bit(0x4A);
	LCD_WR_DATA_8Bit(0x36);
	LCD_WR_DATA_8Bit(0x12);
	LCD_WR_DATA_8Bit(0x13);
	LCD_WR_DATA_8Bit(0x2B);
	LCD_WR_DATA_8Bit(0x2F);   
	LCD_WR_Cmd(0x29); // Display on
	
} 

void LCD_SetWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{	
	// 竖屏
	if(lcd.scan_dir == VERTICAL){
		LCD_WR_Cmd(0x2a);	// 0x2a 列地址设置
		LCD_WR_DATA_16Bit(Xstart);	// 起始地址
		LCD_WR_DATA_16Bit(Xend-1);	// 结束地址
		
		LCD_WR_Cmd(0x2b);	// 0x2b 行地址设置
		LCD_WR_DATA_16Bit(Ystart);	// 起始地址
		LCD_WR_DATA_16Bit(Yend-1);	// 结束地址
	}// 横屏
	else{
		LCD_WR_Cmd(0x2a);	// 0x2a 列地址设置
		LCD_WR_DATA_16Bit(Xstart);	// 起始地址
		LCD_WR_DATA_16Bit(Xend-1);	// 结束地址
		
		LCD_WR_Cmd(0x2b);	// 0x2b 行地址设置
		LCD_WR_DATA_16Bit(Ystart);	// 起始地址
		LCD_WR_DATA_16Bit(Yend-1);	// 结束地址
	}
	
	LCD_WR_Cmd(0x2c);	// 写入内存
}

void LCD_Clear(uint16_t color)
{
	LCD_SetWindows(0,0,lcd.width,lcd.height);
	
	uint16_t i,j;
	
	for(i=0;i<lcd.width;i++){
		for(j=0;j<lcd.height;j++){
			LCD_WR_DATA_16Bit(color);
		}
	}
	
//	
//	uint32_t size = lcd.width * lcd.height;	// 屏幕大小
//    uint32_t remainingSize = size;      // 剩余像素数
//    uint16_t chunkSize;                      // 每次传输的像素数
//	
//	LCD_SetWindows(0,0,lcd.width,lcd.height);


//    LCD_CS_L;
//	LCD_DC_H;
//	SPI2_DMA_Init((uint32_t)(&color),DMA_MemoryInc_Disable,DMA_MemoryDataSize_HalfWord);
//	SPI2->CR1|=1<<11;//设置SPI16位传输模式
//    // 分段传输
//    while (remainingSize > 0) {
//		
//        chunkSize = (remainingSize > 65535) ? 65535 : remainingSize;

//        DMA_Cmd(DMA1_Stream4, DISABLE); // 先禁用DMA流
//        DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4 | DMA_FLAG_HTIF4 | DMA_FLAG_TEIF4 | DMA_FLAG_DMEIF4 | DMA_FLAG_FEIF4); // 清除标志位
//       
//        SPI2_DMAEnable(chunkSize); // 使能DMA流
//	
//        // 等待DMA传输完成
//        while (DMA_GetFlagStatus(DMA1_Stream4, DMA_FLAG_TCIF4) == RESET);

//        // 更新剩余像素数
//        remainingSize -= chunkSize;
//    }

//    LCD_CS_H; // 禁用LCD片选

//	SPI2->CR1=~SPI2->CR1;
//	SPI2->CR1|=1<<11;
//	SPI2->CR1=~SPI2->CR1;//设置SPI8位传输模式
//	SPI_Cmd(SPI2, ENABLE);//使能SPI
	
}


void LCD_DrawPoint(uint16_t X,uint16_t Y,uint16_t Color)
{
	LCD_SetWindows(X,Y,X,Y);//设置光标位置 
	LCD_WR_DATA_16Bit(Color);
} 

/*
*Parameter:X,
*					点x
*Parameter:Y,
*					点y
*Parameter:Str,
*					12*12格式，字符串
*Parameter:Word_Color,
*					字颜色
*Parameter:Back_Color,
*					背景颜色
*Parameter:Size,
*					字的型号
*Parameter:Mode,
*					0非叠加模式  1叠加模式
*Role     :
*         显示该格式字符
*/
void LCD_ShowChinese12x12(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=X;
	TypefaceNum=(Size/8+((Size%8)?1:0))*Size;
	                         
	HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if((tfont12[k].Index[0]==*(Str))&&(tfont12[k].Index[1]==*(Str+1)))
		{ 	
			LCD_SetWindows(X,Y, X+Size, Y+Size);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!Mode)//非叠加方式
					{
						if(tfont12[k].Msk[i]&(0x01<<j))LCD_WR_DATA_16Bit(Word_Color);
						else LCD_WR_DATA_16Bit(Back_Color);
						m++;
						if(m%Size==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont12[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(X,Y,Word_Color);//画一个点
						X++;
						if((X-x0)==Size)
						{
							X=x0;
							Y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 

/*
*Parameter:X,
*					点x
*Parameter:Y,
*					点y
*Parameter:Str,
*					16*16格式，字符串
*Parameter:Word_Color,
*					字颜色
*Parameter:Back_Color,
*					背景颜色
*Parameter:Size,
*					字的型号
*Parameter:Mode,
*					0非叠加模式  1叠加模式
*Role     :
*         显示该格式字符
*/
void LCD_ShowChinese16x16(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=X;
	TypefaceNum=(Size/8+((Size%8)?1:0))*Size;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(Str))&&(tfont16[k].Index[1]==*(Str+1)))
		{ 	
			LCD_SetWindows(X,Y,X+Size,Y+Size);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!Mode)//非叠加方式
					{
						if(tfont16[k].Msk[i]&(0x01<<j))LCD_WR_DATA_16Bit(Word_Color);
						else LCD_WR_DATA_16Bit(Back_Color);
						m++;
						if(m%Size==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont16[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(X,Y,Word_Color);//画一个点
						X++;
						if((X-x0)==Size)
						{
							X=x0;
							Y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 

/*
*Parameter:X,
*					点x
*Parameter:Y,
*					点y
*Parameter:Str,
*					24*24格式，字符串
*Parameter:Word_Color,
*					字颜色
*Parameter:Back_Color,
*					背景颜色
*Parameter:Size,
*					字的型号
*Parameter:Mode,
*					0非叠加模式  1叠加模式
*Role     :
*         显示该格式字符
*/
void LCD_ShowChinese24x24(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=X;
	TypefaceNum=(Size/8+((Size%8)?1:0))*Size;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont24[k].Index[0]==*(Str))&&(tfont24[k].Index[1]==*(Str+1)))
		{ 	
			LCD_SetWindows(X,Y,X+Size,Y+Size);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!Mode)//非叠加方式
					{
						if(tfont24[k].Msk[i]&(0x01<<j))LCD_WR_DATA_16Bit(Word_Color);
						else LCD_WR_DATA_16Bit(Back_Color);
						m++;
						if(m%Size==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont24[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(X,Y,Word_Color);//画一个点
						X++;
						if((X-x0)==Size)
						{
							X=x0;
							Y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
}

/*
*Parameter:X,
*					点x
*Parameter:Y,
*					点y
*Parameter:Str,
*					32*32格式，字符串
*Parameter:Word_Color,
*					字颜色
*Parameter:Back_Color,
*					背景颜色
*Parameter:Size,
*					字的型号
*Parameter:Mode,
*					0非叠加模式  1叠加模式
*Role     :
*         显示该格式字符
*/
void LCD_ShowChinese32x32(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=X;
	TypefaceNum=(Size/8+((Size%8)?1:0))*Size;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont32[k].Index[0]==*(Str))&&(tfont32[k].Index[1]==*(Str+1)))
		{ 	
			LCD_SetWindows(X,Y,X+Size,Y+Size);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!Mode)//非叠加方式
					{
						if(tfont32[k].Msk[i]&(0x01<<j))LCD_WR_DATA_16Bit(Word_Color);
						else LCD_WR_DATA_16Bit(Back_Color);
						m++;
						if(m%Size==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont32[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(X,Y,Word_Color);//画一个点
						X++;
						if((X-x0)==Size)
						{
							X=x0;
							Y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
}

void LCD_ShowChinese(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode)
{
	while(*Str!=0)
	{
		if(Size==12) LCD_ShowChinese12x12(X,Y,Str,Word_Color,Back_Color,Size,Mode);
		else if(Size==16) LCD_ShowChinese16x16(X,Y,Str,Word_Color,Back_Color,Size,Mode);
		else if(Size==24) LCD_ShowChinese24x24(X,Y,Str,Word_Color,Back_Color,Size,Mode);
		else if(Size==32) LCD_ShowChinese32x32(X,Y,Str,Word_Color,Back_Color,Size,Mode);
		else return;
		Str+=2;
		X+=Size;
	}
}

void LCD_ShowChar(uint16_t X,uint16_t Y,uint8_t Char,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode)
{
	uint8_t temp,sizex,t,m=0;
	uint16_t i,TypefaceNum;//一个字符所占字节大小
	uint16_t x0=X;
	sizex=Size/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*Size;
	Char=Char-' ';    //得到偏移后的值
	LCD_SetWindows(X,Y,X+sizex,Y+Size);  //设置光标位置 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(Size==12)temp=ascii_1206[Char][i];		       //调用6x12字体
		else if(Size==16)temp=ascii_1608[Char][i];		 //调用8x16字体
		else if(Size==24)temp=ascii_2412[Char][i];		 //调用12x24字体
		else if(Size==32)temp=ascii_3216[Char][i];		 //调用16x32字体
		else return;
		for(t=0;t<8;t++)
		{
			if(!Mode)//非叠加模式
			{
				if(temp&(0x01<<t))LCD_WR_DATA_16Bit(Word_Color);
				else LCD_WR_DATA_16Bit(Back_Color);
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else//叠加模式
			{
				if(temp&(0x01<<t))LCD_DrawPoint(X,Y,Word_Color);//画一个点
				X++;
				if((X-x0)==sizex)
				{
					X=x0;
					Y++;
					break;
				}
			}
		}
	}   	 	  
}

void LCD_ShowString(uint16_t X,uint16_t Y,const uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode)
{         
	while(*Str!='\0')
	{       
		LCD_ShowChar(X,Y,*Str,Word_Color,Back_Color,Size,Mode);
		X+=Size/2;
		Str++;
	}  
}

uint32_t Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;
	return result;
}

void LCD_ShowIntNum(uint16_t X,uint16_t Y,uint16_t Number,uint8_t Len,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size)
{         	
	uint8_t t,temp;
	
	uint8_t enshow=0;
	uint8_t sizex=Size/2;
	for(t=0;t<Len;t++)
	{
		temp=(Number/Pow(10,Len-t-1))%10;
		if(enshow==0&&t<(Len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(X+t*sizex,Y,' ',Word_Color,Back_Color,Size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(X+t*sizex,Y,temp+48,Word_Color,Back_Color,Size,0);
	}
} 

void LCD_ShowNum(uint16_t X,uint16_t Y,int16_t Number,uint8_t Len,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;
	u8 negative = 0;
	if(Number<0){
		Number = -Number;
		negative = 1;
	}
	uint8_t sizex=Size/2;
	for(t=0;t<Len;t++)
	{
		temp=(Number/Pow(10,Len-t-1))%10;
		if(enshow==0&&t<(Len-1))
		{
			if(temp==0)
			{
				if(negative){
					LCD_ShowChar(X+t*sizex,Y,'-',Word_Color,Back_Color,Size,0);
					negative = 0;
				}else	LCD_ShowChar(X+t*sizex,Y,' ',Word_Color,Back_Color,Size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(X+t*sizex,Y,temp+48,Word_Color,Back_Color,Size,0);
	}
} 

void LCD_ShowFloatNum(uint16_t X,uint16_t Y,float Number,uint8_t Len,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size)
{         	
	uint8_t t,temp,sizex;
	uint16_t num1;
	sizex=Size/2;
	num1=Number*100;
	for(t=0;t<Len;t++)
	{
		temp=(num1/Pow(10,Len-t-1))%10;
		if(t==(Len-2))
		{
			LCD_ShowChar(X+(Len-2)*sizex,Y,'.',Word_Color,Back_Color,Size,0);
			t++;
			Len+=1;
		}
	 	LCD_ShowChar(X+t*sizex,Y,temp+48,Word_Color,Back_Color,Size,0);
	}
}

void LCD_Show_Image(u16 x, u16 y, u16 width, u16 height, const u8 *image)
{
	LCD_SetWindows(x,y,x+width,y+height);
	
	uint32_t imageSize = width * height * 2; // 图像数据大小
    uint32_t remainingSize = imageSize;      // 剩余像素数
    uint16_t chunkSize;                      // 每次传输的像素数

    LCD_CS_L;
	LCD_DC_H;
	SPI2_DMA_Init((uint32_t)image,DMA_MemoryInc_Enable,DMA_MemoryDataSize_Byte);
    // 分段传输
    while (remainingSize > 0) {
        chunkSize = (remainingSize > 65535) ? 65535 : remainingSize;

        DMA_Cmd(DMA1_Stream4, DISABLE);
        DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4 | DMA_FLAG_HTIF4 | DMA_FLAG_TEIF4 | DMA_FLAG_DMEIF4 | DMA_FLAG_FEIF4); // 清除标志位
       
		DMA1_Stream4->M0AR = (uint32_t)image;
        SPI2_DMAEnable(chunkSize);
	
        while (DMA_GetFlagStatus(DMA1_Stream4, DMA_FLAG_TCIF4) == RESET);

        remainingSize -= chunkSize;
        image += chunkSize; 
    }

    LCD_CS_H;
	
}



