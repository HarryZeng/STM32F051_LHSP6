/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __differentialDC_H
#define __differentialDC_H
#ifdef __cplusplus
 extern "C" {
#endif
#include  "stm32f0xx.h"

/* Private define ------------------------------------------------------------*/
//#define D4_Pin 										GPIO_Pin_10
//#define D4_GPIO_Port 							GPIOA
//#define D3_Pin 										GPIO_Pin_11
//#define D3_GPIO_Port 							GPIOA
//#define D2_Pin 										GPIO_Pin_12
//#define D2_GPIO_Port 							GPIOA
//#define D1_Pin 										GPIO_Pin_15
//#define D1_GPIO_Port 							GPIOA
//#define C_Pin 										GPIO_Pin_3
//#define C_GPIO_Port 							GPIOB
//#define G_Pin 										GPIO_Pin_4
//#define G_GPIO_Port 							GPIOB
//#define F_Pin 										GPIO_Pin_5
//#define F_GPIO_Port 							GPIOB
//#define A_Pin 										GPIO_Pin_6
//#define A_GPIO_Port 							GPIOB
//#define E_Pin 										GPIO_Pin_7
//#define E_GPIO_Port 							GPIOB
//#define B_Pin 										GPIO_Pin_8
//#define B_GPIO_Port 							GPIOB
//#define D_Pin 										GPIO_Pin_9
//#define D_GPIO_Port 							GPIOB
//#define D5_Pin 										GPIO_Pin_13
//#define D5_GPIO_Port 							GPIOC
//#define D6_Pin 										GPIO_Pin_14
//#define D6_GPIO_Port 							GPIOC
//#define D7_Pin 										GPIO_Pin_15
//#define D7_GPIO_Port 							GPIOC
//#define D9_Pin 										GPIO_Pin_0
//#define D9_GPIO_Port 							GPIOF
//#define D8_Pin 										GPIO_Pin_1
//#define D8_GPIO_Port 							GPIOF

#define D5_Pin 										GPIO_Pin_13
#define D5_GPIO_Port 							GPIOC
#define D6_Pin 										GPIO_Pin_14
#define D6_GPIO_Port 							GPIOC
#define D7_Pin 										GPIO_Pin_15
#define D7_GPIO_Port 							GPIOC
#define D9_Pin 										GPIO_Pin_0
#define D9_GPIO_Port 							GPIOF
#define D8_Pin 										GPIO_Pin_1
#define D8_GPIO_Port 							GPIOF
#define D_Pin 										GPIO_Pin_9
#define D_GPIO_Port 							GPIOB
#define B_Pin 										GPIO_Pin_8
#define B_GPIO_Port 							GPIOB
#define E_Pin 										GPIO_Pin_7
#define E_GPIO_Port 							GPIOB
#define A_Pin 										GPIO_Pin_6
#define A_GPIO_Port 							GPIOB
#define F_Pin 										GPIO_Pin_5
#define F_GPIO_Port 							GPIOB
#define G_Pin 										GPIO_Pin_4
#define G_GPIO_Port 							GPIOB
#define D4_Pin 										GPIO_Pin_12
#define D4_GPIO_Port 							GPIOA
#define D3_Pin 										GPIO_Pin_6
#define D3_GPIO_Port 							GPIOF
#define D2_Pin 										GPIO_Pin_7
#define D2_GPIO_Port 							GPIOF
#define D1_Pin 										GPIO_Pin_15
#define D1_GPIO_Port 							GPIOA
#define C_Pin 										GPIO_Pin_3
#define C_GPIO_Port 							GPIOB


#define ADCIN_Pin 								GPIO_Pin_3
#define ADCIN_GPIO_Port 					GPIOA
#define DACOUT_Pin 								GPIO_Pin_4
#define DACOUT_GPIO_Port 					GPIOA
#define SDA_Pin 									GPIO_Pin_6
#define SDA_GPIO_Port 						GPIOA
#define SCL_Pin 									GPIO_Pin_7
#define SCL_GPIO_Port 						GPIOA
#define BUTTON_SET_Pin 						GPIO_Pin_2
#define BUTTON_SET_GPIO_Port 			GPIOB
#define SC_Pin 										GPIO_Pin_11
#define SC_GPIO_Port 							GPIOB
#define BUTTON_MODE_Pin 					GPIO_Pin_12
#define BUTTON_MODE_GPIO_Port 		GPIOB
#define BUTTON_DOWN_Pin 					GPIO_Pin_13
#define BUTTON_DOWN_GPIO_Port 		GPIOB
#define BUTTON_UP_Pin 						GPIO_Pin_14
#define BUTTON_UP_GPIO_Port 			GPIOB
#define OUT2_Pin 									GPIO_Pin_11
#define OUT2_GPIO_Port 						GPIOA
#define FB_SC_Pin 									GPIO_Pin_8
#define FB_SC_GPIO_Port 						GPIOA	 
#define ATT_Pin 									GPIO_Pin_2
#define ATT_GPIO_Port 						GPIOA	 
#define PWM_Pin 									GPIO_Pin_0
#define PWM_GPIO_Port 						GPIOA	 
#define PWM1_Pin 									GPIO_Pin_1
#define PWM1_GPIO_Port 						GPIOA	 
#define PWM2_Pin 									GPIO_Pin_9
#define PWM2_GPIO_Port 						GPIOA	 

#define COMPOUT_Pin 							GPIO_Pin_0
#define COMPOUT_GPIO_Port 				GPIOB	 	 
	 
#define ADVtimeFlag   		0x01
#define ButtontimeFlag   	0x02 
#define D4toD1timeFlag   	0x04
#define D9toD5timeFlag   	0x08
#define Blink500msFlag   	0x10
#define Blink100msFlag   	0x20
#define EndDispalyFlag   	0x40
	 

//HAL_StatusTypeDef delay_2us(uint32_t num);//��ʱ��3��ʱ
	 
typedef enum
{
		ULOC = 51U,
		LOC
}Button_STATUS;	 
	 
	 
extern int16_t Threshold;
extern uint8_t 	RegisterA;
extern uint32_t ADCValue;
extern uint8_t  EventFlag; 
extern int32_t 	DACOUT;
extern uint32_t 	ShortCircuitCounter;
	 
void differenttialDC(void);	 
void GetADCValue(uint32_t *meanValue);
void GetEEPROM(void);
 void Test_Delay(uint32_t ms);
void SetOUT1Status(void);
	#ifdef __cplusplus
	}
	#endif
#endif /*__ differentialDC_H */


