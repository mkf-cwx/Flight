#include "lcd.h"
#include "spi.h"
#include "dma.h"
#include "fonts.h"

LCD_Attribute lcd;

					    
//д�����
//reg:�Ĵ���ֵ
void LCD_WR_Cmd(u8 reg)
{
	LCD_CS_L;		
	LCD_DC_L;//RS=0 ����	
	
	SPI_SendByte(reg);

	LCD_CS_H;				
}
//д���ݺ���
//data:�Ĵ���ֵ
void LCD_WR_DATA_8Bit(u8 data)
{
	LCD_CS_L;		
	LCD_DC_H;//RS=1 ����
	
	SPI_SendByte(data);
	LCD_CS_H;			
}

void LCD_WR_DATA_16Bit(u16 data)
{
	LCD_CS_L;		
	LCD_DC_H;//RS=1 ����
	
	SPI_SendByte((data>>8)&0xff);
	SPI_SendByte(data&0xff);
	LCD_CS_H;
}

//д�Ĵ���
//LCD_Reg:�Ĵ������
//LCD_RegValue:Ҫд���ֵ
void LCD_WriteReg(u8 LCD_Reg,u16 LCD_RegValue)
{	
	LCD_WR_Cmd(LCD_Reg);  
	LCD_WR_DATA_16Bit(LCD_RegValue);	    		 
}

void LCD_Set_Attributes(uint8_t Scan_dir)
{
	lcd.scan_dir = Scan_dir;
	
	// ����:0x00-��ת0�ȣ� 0xc0-��ת180�ȣ� ����:0x70-��ת90�ȣ� 0xA0-��ת270��
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
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ�� GPIOA ʱ��;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ�� GPIOB ʱ��;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ�� GPIOC ʱ��;
	
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
	LCD_LED_H;	//	��������
}


//��ʼ��lcd
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
	// ����
	if(lcd.scan_dir == VERTICAL){
		LCD_WR_Cmd(0x2a);	// 0x2a �е�ַ����
		LCD_WR_DATA_16Bit(Xstart);	// ��ʼ��ַ
		LCD_WR_DATA_16Bit(Xend-1);	// ������ַ
		
		LCD_WR_Cmd(0x2b);	// 0x2b �е�ַ����
		LCD_WR_DATA_16Bit(Ystart);	// ��ʼ��ַ
		LCD_WR_DATA_16Bit(Yend-1);	// ������ַ
	}// ����
	else{
		LCD_WR_Cmd(0x2a);	// 0x2a �е�ַ����
		LCD_WR_DATA_16Bit(Xstart);	// ��ʼ��ַ
		LCD_WR_DATA_16Bit(Xend-1);	// ������ַ
		
		LCD_WR_Cmd(0x2b);	// 0x2b �е�ַ����
		LCD_WR_DATA_16Bit(Ystart);	// ��ʼ��ַ
		LCD_WR_DATA_16Bit(Yend-1);	// ������ַ
	}
	
	LCD_WR_Cmd(0x2c);	// д���ڴ�
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
//	uint32_t size = lcd.width * lcd.height;	// ��Ļ��С
//    uint32_t remainingSize = size;      // ʣ��������
//    uint16_t chunkSize;                      // ÿ�δ����������
//	
//	LCD_SetWindows(0,0,lcd.width,lcd.height);


//    LCD_CS_L;
//	LCD_DC_H;
//	SPI2_DMA_Init((uint32_t)(&color),DMA_MemoryInc_Disable,DMA_MemoryDataSize_HalfWord);
//	SPI2->CR1|=1<<11;//����SPI16λ����ģʽ
//    // �ֶδ���
//    while (remainingSize > 0) {
//		
//        chunkSize = (remainingSize > 65535) ? 65535 : remainingSize;

//        DMA_Cmd(DMA1_Stream4, DISABLE); // �Ƚ���DMA��
//        DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4 | DMA_FLAG_HTIF4 | DMA_FLAG_TEIF4 | DMA_FLAG_DMEIF4 | DMA_FLAG_FEIF4); // �����־λ
//       
//        SPI2_DMAEnable(chunkSize); // ʹ��DMA��
//	
//        // �ȴ�DMA�������
//        while (DMA_GetFlagStatus(DMA1_Stream4, DMA_FLAG_TCIF4) == RESET);

//        // ����ʣ��������
//        remainingSize -= chunkSize;
//    }

//    LCD_CS_H; // ����LCDƬѡ

//	SPI2->CR1=~SPI2->CR1;
//	SPI2->CR1|=1<<11;
//	SPI2->CR1=~SPI2->CR1;//����SPI8λ����ģʽ
//	SPI_Cmd(SPI2, ENABLE);//ʹ��SPI
	
}


void LCD_DrawPoint(uint16_t X,uint16_t Y,uint16_t Color)
{
	LCD_SetWindows(X,Y,X,Y);//���ù��λ�� 
	LCD_WR_DATA_16Bit(Color);
} 

/*
*Parameter:X,
*					��x
*Parameter:Y,
*					��y
*Parameter:Str,
*					12*12��ʽ���ַ���
*Parameter:Word_Color,
*					����ɫ
*Parameter:Back_Color,
*					������ɫ
*Parameter:Size,
*					�ֵ��ͺ�
*Parameter:Mode,
*					0�ǵ���ģʽ  1����ģʽ
*Role     :
*         ��ʾ�ø�ʽ�ַ�
*/
void LCD_ShowChinese12x12(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//������Ŀ
	uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint16_t x0=X;
	TypefaceNum=(Size/8+((Size%8)?1:0))*Size;
	                         
	HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if((tfont12[k].Index[0]==*(Str))&&(tfont12[k].Index[1]==*(Str+1)))
		{ 	
			LCD_SetWindows(X,Y, X+Size, Y+Size);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!Mode)//�ǵ��ӷ�ʽ
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
					else//���ӷ�ʽ
					{
						if(tfont12[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(X,Y,Word_Color);//��һ����
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
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 

/*
*Parameter:X,
*					��x
*Parameter:Y,
*					��y
*Parameter:Str,
*					16*16��ʽ���ַ���
*Parameter:Word_Color,
*					����ɫ
*Parameter:Back_Color,
*					������ɫ
*Parameter:Size,
*					�ֵ��ͺ�
*Parameter:Mode,
*					0�ǵ���ģʽ  1����ģʽ
*Role     :
*         ��ʾ�ø�ʽ�ַ�
*/
void LCD_ShowChinese16x16(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//������Ŀ
	uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint16_t x0=X;
	TypefaceNum=(Size/8+((Size%8)?1:0))*Size;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(Str))&&(tfont16[k].Index[1]==*(Str+1)))
		{ 	
			LCD_SetWindows(X,Y,X+Size,Y+Size);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!Mode)//�ǵ��ӷ�ʽ
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
					else//���ӷ�ʽ
					{
						if(tfont16[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(X,Y,Word_Color);//��һ����
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
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 

/*
*Parameter:X,
*					��x
*Parameter:Y,
*					��y
*Parameter:Str,
*					24*24��ʽ���ַ���
*Parameter:Word_Color,
*					����ɫ
*Parameter:Back_Color,
*					������ɫ
*Parameter:Size,
*					�ֵ��ͺ�
*Parameter:Mode,
*					0�ǵ���ģʽ  1����ģʽ
*Role     :
*         ��ʾ�ø�ʽ�ַ�
*/
void LCD_ShowChinese24x24(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//������Ŀ
	uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint16_t x0=X;
	TypefaceNum=(Size/8+((Size%8)?1:0))*Size;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont24[k].Index[0]==*(Str))&&(tfont24[k].Index[1]==*(Str+1)))
		{ 	
			LCD_SetWindows(X,Y,X+Size,Y+Size);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!Mode)//�ǵ��ӷ�ʽ
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
					else//���ӷ�ʽ
					{
						if(tfont24[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(X,Y,Word_Color);//��һ����
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
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
}

/*
*Parameter:X,
*					��x
*Parameter:Y,
*					��y
*Parameter:Str,
*					32*32��ʽ���ַ���
*Parameter:Word_Color,
*					����ɫ
*Parameter:Back_Color,
*					������ɫ
*Parameter:Size,
*					�ֵ��ͺ�
*Parameter:Mode,
*					0�ǵ���ģʽ  1����ģʽ
*Role     :
*         ��ʾ�ø�ʽ�ַ�
*/
void LCD_ShowChinese32x32(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//������Ŀ
	uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint16_t x0=X;
	TypefaceNum=(Size/8+((Size%8)?1:0))*Size;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont32[k].Index[0]==*(Str))&&(tfont32[k].Index[1]==*(Str+1)))
		{ 	
			LCD_SetWindows(X,Y,X+Size,Y+Size);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!Mode)//�ǵ��ӷ�ʽ
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
					else//���ӷ�ʽ
					{
						if(tfont32[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(X,Y,Word_Color);//��һ����
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
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
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
	uint16_t i,TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint16_t x0=X;
	sizex=Size/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*Size;
	Char=Char-' ';    //�õ�ƫ�ƺ��ֵ
	LCD_SetWindows(X,Y,X+sizex,Y+Size);  //���ù��λ�� 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(Size==12)temp=ascii_1206[Char][i];		       //����6x12����
		else if(Size==16)temp=ascii_1608[Char][i];		 //����8x16����
		else if(Size==24)temp=ascii_2412[Char][i];		 //����12x24����
		else if(Size==32)temp=ascii_3216[Char][i];		 //����16x32����
		else return;
		for(t=0;t<8;t++)
		{
			if(!Mode)//�ǵ���ģʽ
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
			else//����ģʽ
			{
				if(temp&(0x01<<t))LCD_DrawPoint(X,Y,Word_Color);//��һ����
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
	
	uint32_t imageSize = width * height * 2; // ͼ�����ݴ�С
    uint32_t remainingSize = imageSize;      // ʣ��������
    uint16_t chunkSize;                      // ÿ�δ����������

    LCD_CS_L;
	LCD_DC_H;
	SPI2_DMA_Init((uint32_t)image,DMA_MemoryInc_Enable,DMA_MemoryDataSize_Byte);
    // �ֶδ���
    while (remainingSize > 0) {
        chunkSize = (remainingSize > 65535) ? 65535 : remainingSize;

        DMA_Cmd(DMA1_Stream4, DISABLE);
        DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4 | DMA_FLAG_HTIF4 | DMA_FLAG_TEIF4 | DMA_FLAG_DMEIF4 | DMA_FLAG_FEIF4); // �����־λ
       
		DMA1_Stream4->M0AR = (uint32_t)image;
        SPI2_DMAEnable(chunkSize);
	
        while (DMA_GetFlagStatus(DMA1_Stream4, DMA_FLAG_TCIF4) == RESET);

        remainingSize -= chunkSize;
        image += chunkSize; 
    }

    LCD_CS_H;
	
}



