#include "touch.h"
#include "systick.h"
#include "lcd.h"
#include "ctp.h"
#include "screen.h"

void Icon1_Click(void);
void Icon2_Click(void);

Icon icons[] = {
    {10, 110, 60, 180, Icon1_Click}, // 右界面
    {270, 110, 320, 180, Icon2_Click}, // 左界面
};


void Touch_Init(void)
{	
	FT6336_Init();												 
}

/********************************************
* @brief: IsPointInIcon 检测触摸坐标函数
* @param: 	u16 x x轴坐标
			u16 y y轴坐标	
			Icon* icon 图标类型结构体指针
* @return: 	0:假		1:真
 ********************************************
 */
u8 IsPointInIcon(u16 x, u16 y, Icon* icon) {
    return (x >= icon->x1 && x <= icon->x2 && y >= icon->y1 && y <= icon->y2);
}

/********************************************
* @brief: Touch_Process 处理触摸坐标函数
* @param:	u16 x x轴坐标
			u16 y y轴坐标	
* @return:	
 ********************************************
 */
void Touch_Process(u16 x, u16 y)
{
	 for (int i = 0; i < sizeof(icons) / sizeof(Icon); i++) {
        if (IsPointInIcon(x, y, &icons[i])) {
            icons[i].OnClick(); // 触发点击事件
            break;
        }
    }
}
/********************************************
* @brief: Touch_ISR 触摸中断中调用该函数
* @param:		
* @return:	
 ********************************************
 */
void Touch_ISR(void)
{
	u16 x, y;
	if(FT6336_GetCoordinates(&x, &y)){
		Touch_Process(x, y);
	}
}

/********************************************
* @brief: Icon1_Click 图标一点击回调函数
* @param: 		
* @return: 	
 ********************************************
 */
void Icon1_Click(void)
{
	Page_Num2();
}
/********************************************
* @brief: Icon2_Click 图标二点击回调函数
* @param:		
* @return:	
 ********************************************
 */
void Icon2_Click(void)
{
	Page_Num1();
}


