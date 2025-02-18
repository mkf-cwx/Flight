#include "touch.h"
#include "systick.h"
#include "lcd.h"
#include "ctp.h"
#include "screen.h"

void Icon1_Click(void);
void Icon2_Click(void);

Icon icons[] = {
    {10, 110, 60, 180, Icon1_Click}, // �ҽ���
    {270, 110, 320, 180, Icon2_Click}, // �����
};


void Touch_Init(void)
{	
	FT6336_Init();												 
}

/********************************************
* @brief: IsPointInIcon ��ⴥ�����꺯��
* @param: 	u16 x x������
			u16 y y������	
			Icon* icon ͼ�����ͽṹ��ָ��
* @return: 	0:��		1:��
 ********************************************
 */
u8 IsPointInIcon(u16 x, u16 y, Icon* icon) {
    return (x >= icon->x1 && x <= icon->x2 && y >= icon->y1 && y <= icon->y2);
}

/********************************************
* @brief: Touch_Process ���������꺯��
* @param:	u16 x x������
			u16 y y������	
* @return:	
 ********************************************
 */
void Touch_Process(u16 x, u16 y)
{
	 for (int i = 0; i < sizeof(icons) / sizeof(Icon); i++) {
        if (IsPointInIcon(x, y, &icons[i])) {
            icons[i].OnClick(); // ��������¼�
            break;
        }
    }
}
/********************************************
* @brief: Touch_ISR �����ж��е��øú���
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
* @brief: Icon1_Click ͼ��һ����ص�����
* @param: 		
* @return: 	
 ********************************************
 */
void Icon1_Click(void)
{
	Page_Num2();
}
/********************************************
* @brief: Icon2_Click ͼ�������ص�����
* @param:		
* @return:	
 ********************************************
 */
void Icon2_Click(void)
{
	Page_Num1();
}


