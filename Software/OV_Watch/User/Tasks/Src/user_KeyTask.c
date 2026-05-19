/* Private includes -----------------------------------------------------------*/
//includes
#include "user_TasksInit.h"
#include "ui_HomePage.h"
#include "main.h"
#include "key.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


/**
  * @brief  Key press check task
  * @param  argument: Not used
  * @retval None
  */
void KeyTask(void *argument)
{
	uint8_t keystr=0;
	uint8_t Stopstr=0;
	uint8_t IdleBreakstr=0;
	uint8_t key2_long_handled = 0;
	while(1)
	{
		uint8_t key = KeyScan(0);

		// KEY2 long press detection (on home page only)
		if(KeyScan_GetCurrentKey() == 2 && PageStack.top == 1)
		{
			if(KeyScan_GetHoldTime() >= KEY_LONG_PRESS_MS && !key2_long_handled)
			{
				key2_long_handled = 1;
				keystr = 3;
				osMessageQueuePut(Key_MessageQueue, &keystr, 0, 1);
				osMessageQueuePut(IdleBreak_MessageQueue, &IdleBreakstr, 0, 1);
			}
		}

		switch(key)
		{
			case 1:
				keystr = 1;
				osMessageQueuePut(Key_MessageQueue, &keystr, 0, 1);
				osMessageQueuePut(IdleBreak_MessageQueue, &IdleBreakstr, 0, 1);
				break;

			case 2:
				if(!key2_long_handled)
				{
					if(PageStack.top == 1)
					{
						osMessageQueuePut(Stop_MessageQueue, &Stopstr, 0, 1);
					}
					else
					{
						keystr = 2;
						osMessageQueuePut(Key_MessageQueue, &keystr, 0, 1);
						osMessageQueuePut(IdleBreak_MessageQueue, &IdleBreakstr, 0, 1);
					}
				}
				key2_long_handled = 0;
				break;
		}
		osDelay(1);
	}
}
