#ifndef __OLED_APP_H
#define __OLED_APP_H

/* 改成自己的姓名和学号 */
#define STUDENT_NAME "刘佳烜"
#define STUDENT_ID   "632407030115"

void OLED_ShowID(void);
void OLED_ShowInfo(void);
void OLED_ScrollTextHorizontal(void);
void OLED_ScrollTextVertical(void);
void OLED_U8g2Demo(void);
void OLED_AvatarAnimation(void);

#endif
