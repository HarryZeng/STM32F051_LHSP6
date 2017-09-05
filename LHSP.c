
///**
//  ********************************  STM32F0x1  *********************************
//  * @�ļ���     �� differentialDC.c
//  * @����       �� HarryZeng
//  * @��汾     �� V1.5.0
//  * @�ļ��汾   �� V1.0.0
//  * @����       �� 2017��05��11��
//  * @ժҪ       �� ���ݴ���
//  ******************************************************************************/
///*----------------------------------------------------------------------------
//  ������־:
//  2017-05-11 V1.0.0:��ʼ�汾
//  ----------------------------------------------------------------------------*/
///* ������ͷ�ļ� --------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "LHSP.h"
#include "key.h"
#include "display.h"
#include "SelfStudy.h"
#include "menu.h"
#include "bsp_eeprom_24xx.h"

/*DSP��궨�壺ARM_MATH_CM0*/


extern uint8_t sample_finish;
uint32_t DealyBaseTime=23;
int16_t Threshold=1000;
uint8_t RegisterA=0;
uint8_t RegisterB=1;
uint8_t OUT1=0;
uint8_t OUT2=0;
int16_t OUT2_TimerCounter=0;
uint16_t OUT2_Timer=0;
uint32_t ADCValue=0;
uint8_t ADCValueFlag=0;
uint32_t ADC_ROW_Value[25];
uint32_t ADC_Display_Value[258];
uint32_t ADC_Display=0;
uint16_t ADC_Display_Index=0;
int32_t DACOUT = 500;
uint32_t CPV = 0;
Button_STATUS KEY=ULOC;
uint8_t 	ConfirmShortCircuit=0;
uint32_t   ShortCircuitCounter = 0;
uint32_t   ShortCircuitLastTime = 0;
uint8_t 	KeyMappingFlag=0;
uint8_t  	EventFlag=0x00; 
uint8_t 	ShortCircuit=0;
uint32_t 	ShortCircuitTimer=0;

void GetADCValue(uint32_t *meanValue);
void DisplayMODE(void);
void DisplayModeONE(void);
void DisplayModeTWO(void);
void DisplayModeTHIRD(void);
void ShortCircuitProtection(void);
void SetOUT1Status(void);
void SetOUT2Status(void);
void ButtonMapping(void);
/*----------------------------------�궨��-------------------------------------*/
extern uint32_t OUTADCValue;

/*���������Ĵ�*/
void GetADCValue(uint32_t *meanValue)
{
//	uint32_t TempValue=0;
//	uint8_t i=0;
//	
//	if(sample_finish)
//	{
//		sample_finish = 0;
//		for(i=0;i<4;i++)
//			TempValue += ADC_ROW_Value[i];
//	}
	
	*meanValue = OUTADCValue;
}

/*����OUT1*/
/********************
*
*�жϳ�RegisterA״̬
*
**********************/
uint8_t GetRegisterAState(uint32_t ADCValue)
{
	uint8_t A;
		
	if(ADCValue>=Threshold)
		A = 1;
	else if(ADCValue<=Threshold-50)
		A = 0;
	return A;
}



void differenttialDC(void)
{
	GetEEPROM();
	while(1)
	{
//			/*������ʾ*/
//			if(EventFlag&ADVtimeFlag) //�ж��¼�
//			{
//				EventFlag = EventFlag &(~ADVtimeFlag);  //�����־λ
				GetADCValue(&ADC_Display_Value[ADC_Display_Index]);		//��ʱADC����
				ADC_Display_Value[256]+=ADC_Display_Value[ADC_Display_Index++];//�ۼ�
				if(ADC_Display_Index>127)
				{
					ADC_Display_Index = 0;
					ADCValueFlag = 1;
				}
				if(ADCValueFlag)
				{
					ADCValueFlag = 0;
					ADC_Display = ADC_Display_Value[256]/128;
					//ADC_Display++;
					ADC_Display_Value[256] = 0;
				}
		//	}			
				//		ADC_Display++;
				/*��·����*/
				ShortCircuitProtection();

				while(ConfirmShortCircuit)
				{
						//GPIO_WriteBit(OUT1_GPIO_Port,OUT1_Pin,Bit_RESET);
						GPIO_WriteBit(FB_SC_GPIO_Port,FB_SC_Pin,Bit_RESET);
						//GPIO_WriteBit(OUT2_GPIO_Port,OUT2_Pin,Bit_RESET);/*��������OUT*/ /*���ֶ�·����OUT��������*/
						if((ShortCircuitLastTime - ShortCircuitTimer)>=10000)
						{
								ConfirmShortCircuit = 0;
								ShortCircuitCounter = 0;
								ShortCircuit=0;
						}
				}
				
				/*����OUT2*/
				//GPIOA->ODR ^= GPIO_Pin_9;
				SetOUT2Status();
				/*��ʾOUT1��OUT2��״̬*/
				//SMG_DisplayOUT_STATUS(OUT1,OUT2);
				/*������ʾģʽ*/
				DisplayMODE();
			
				/*��������*/
				ButtonMapping();
				
				if(KEY==ULOC)/*�жϰ����Ƿ�����*/
				{
				/*SET��ѧϰģʽ*/
					selfstudy();
				/*Mode�˵�ģʽ*/
					//menu();
				}
	}
}


/*******************************
*
*��ʾģʽ�л�
*
*******************************/
uint8_t DisplayModeNo=0;

void DisplayMODE(void)
{
	if(KEY==ULOC)
	{
		if(ModeButton.PressCounter==0)
		{
				DisplayModeNo = 0;
		}
		else if(ModeButton.Effect==PressShort && ModeButton.PressCounter==1&&DownButton.Status==Release)
		{
			DisplayModeNo = 1;
		}
		else if(ModeButton.Effect==PressShort && ModeButton.PressCounter==2&&DownButton.Status==Release)
		{
				DisplayModeNo = 2;
		}
		if(ModeButton.Effect == PressShort && ModeButton.PressCounter==3 &&DownButton.Status==Release)
		{
				ModeButton.PressCounter = 0;
				DisplayModeNo = 0;
		}
	}
	else
	{
		while(ModeButton.Status==Press||SetButton.Status==Press||DownButton.Status==Press||UpButton.Status==Press)
		{
			if(ModeButton.Effect==PressLong &&DownButton.Effect ==PressLong&&DownButton.Status ==Press)
			{
				if(KEY == ULOC)
						KEY = LOC;
				else
				{
					KEY = ULOC;
					KeyMappingFlag = 0;
				}
				ModeButton.PressCounter = 0;
				while((ReadButtonStatus(&ModeButton)) == Press&&(ReadButtonStatus(&DownButton) == Press))
				{
					ButtonMappingDisplay(1);
				}
				ee_WriteBytes((uint8_t*)&KEY, 60, 1);
				ModeButton.Effect=PressNOEffect;
				ModeButton.PressCounter = 0;
				DownButton.PressCounter = 0;
				DownButton.Effect=PressNOEffect;
				//ee_WriteBytes((uint8_t*)&KEY, 60, 4);
				//I2C_WriteByte(KEY,60,EE_DEV_ADDR);
			}
			else
				ButtonMappingDisplay(1);
		}
	}
	/*��ʾ*/
	if(DisplayModeNo==0)
	{
		DisplayModeONE();
	}
	else if(DisplayModeNo==1)
	{
		DisplayModeTWO();
	}
	else if(DisplayModeNo==2)
	{
		DisplayModeTHIRD();
	}
}

/*******************************
*
*��ʾģʽ1
*
*******************************/
void DisplayModeONE(void)
{
	static uint8_t lastCounter;
	static int16_t LastThreshold;
		/*�������ʾ*/
		SMG_DisplayModeONE(Threshold,ADC_Display);
	
	if(ModeButton.Status==Release && KeyMappingFlag==0 && KEY==ULOC)
	{
		/*Up Button*/
		LastThreshold = Threshold;
		if(UpButton.PressCounter !=lastCounter && UpButton.Effect==PressShort)
		{
			lastCounter = UpButton.PressCounter;
			UpButton.PressCounter = 0;
			Threshold = Threshold+1;
		}
		else 	if(UpButton.Status==Press&&(UpButton.Effect==PressLong))
		{				/*�����Ű���������ʱ�䳬������ʱ��*/
			UpButton.PressCounter = 0;
			if(UpButton.PressTimer<=KEY_LEVEL_1)
			{
				if(UpButton.PressTimer%KEY_LEVEL_1_SET==0)
					Threshold = Threshold+1;
			}
			else if(UpButton.PressTimer>KEY_LEVEL_1&&UpButton.PressTimer<=KEY_LEVEL_2)
			{
				if(UpButton.PressTimer%KEY_LEVEL_2_SET==0)
					Threshold = Threshold+1;
			}
			else 
			{
				if(UpButton.PressTimer%KEY_LEVEL_3_SET==0)
					Threshold = Threshold+1;
			}
		}	
		else
		{
			UpButton.Effect = PressShort;
		}
			/*Down Button*/
		if(DownButton.PressCounter !=lastCounter && DownButton.Effect==PressShort)
		{
			DownButton.PressCounter = 0;
			Threshold = Threshold-1;
		}
		else if(DownButton.Status==Press&&(DownButton.Effect==PressLong))
		{
			DownButton.PressCounter = 0;
			if(DownButton.PressTimer<KEY_LEVEL_1)
			{
				if(DownButton.PressTimer%KEY_LEVEL_1_SET==0)
					Threshold = Threshold-1;
			}
			else if(DownButton.PressTimer>KEY_LEVEL_1&&DownButton.PressTimer<KEY_LEVEL_2)
			{
				if(DownButton.PressTimer%KEY_LEVEL_2_SET==0)
					Threshold = Threshold-1;
			}
			else 
			{
				if(DownButton.PressTimer%KEY_LEVEL_3_SET==0)
					Threshold = Threshold-1;
			}
		}
		else
		{
			DownButton.Effect = PressShort;
		}
		if(LastThreshold!=Threshold && DownButton.Status==Release && UpButton.Status==Release)
		{
			ee_WriteBytes((uint8_t*)&Threshold, 40, 4);
		}
	}
		if(Threshold>=4095)
				Threshold = 4095;
		else if(Threshold<=20)
				Threshold = 20;
}



/*******************************
*
*��ʾģʽ2
*
*******************************/
uint8_t ATT_MODE=0;
uint8_t ATTUpAndDownCounter=0;
void DisplayModeTWO(void)
{
		/*�������ʾ*/
		SMG_DisplayModeTWO(ATT_MODE);
	
		/*Up Button*/
			if(UpButton.PressCounter !=ATTUpAndDownCounter && UpButton.Effect==PressShort)
			{
				ATTUpAndDownCounter = UpButton.PressCounter;
				UpButton.PressCounter = 0;
				if(ATT_MODE==0)
					ATT_MODE = 1;
				else 
					ATT_MODE=0;
				ee_WriteBytes((uint8_t*)&ATT_MODE, 20, 1);
				GPIO_WriteBit(ATT_GPIO_Port, ATT_Pin, (BitAction)ATT_MODE);
			}

		/*Down Button*/
		if(DownButton.PressCounter !=ATTUpAndDownCounter && DownButton.Effect==PressShort)
		{
			DownButton.PressCounter = 0;
			if(ATT_MODE==0)
				ATT_MODE = 1;
			else 
				ATT_MODE=0;
			ee_WriteBytes((uint8_t*)&ATT_MODE, 20, 1);
			GPIO_WriteBit(ATT_GPIO_Port, ATT_Pin, (BitAction)ATT_MODE);
		}
}


/*******************************
*
*��ʾģʽ3
*
*******************************/
uint8_t TOFF_MODE=0;
uint8_t TOFFUpAndDownCounter=0;
void DisplayModeTHIRD(void)
{
		/*�������ʾ*/
		SMG_DisplayModeTHIRD(TOFF_MODE);
			/*Up Button*/
		if(UpButton.PressCounter !=TOFFUpAndDownCounter && UpButton.Effect==PressShort)
		{
			TOFFUpAndDownCounter = UpButton.PressCounter;
			UpButton.PressCounter = 0;
			if(TOFF_MODE==0)
				TOFF_MODE = 1;
			else 
				TOFF_MODE=0;
			ee_WriteBytes((uint8_t*)&TOFF_MODE, 10, 1);
		}

		/*Down Button*/
		if(DownButton.PressCounter !=TOFFUpAndDownCounter && DownButton.Effect==PressShort)
		{
			DownButton.PressCounter = 0;
			if(TOFF_MODE==0)
				TOFF_MODE = 1;
			else 
				TOFF_MODE=0;
			ee_WriteBytes((uint8_t*)&TOFF_MODE, 10, 1);
		}
}


///*******************************
//*
//*�ж�OUT1�����״̬
//*
//*******************************/
//uint8_t SHOTflag=0;
//void SetOUT1Status(void)
//{
//	if(ShortCircuit!=1)/*���Ƕ�·����������²��ж�OUT1�����*/
//	{
//		/*ͬ������*/
//		OUT1=!(RegisterB^RegisterA);
//		if(OUT1_Mode.DelayMode==TOFF)
//		{
//			//GPIOA->ODR ^= GPIO_Pin_9;
//			if(OUT1)
//			{
//				GPIO_WriteBit(OUT1_GPIO_Port, OUT1_Pin, Bit_SET);
//				OUT1_Mode.DelayCounter = 0;
//			}
//			else
//			{
//				GPIO_WriteBit(OUT1_GPIO_Port, OUT1_Pin, Bit_RESET);
//				OUT1_Mode.DelayCounter=0;
//			}
//		}
//		/*OFFD*/
//		else if(OUT1_Mode.DelayMode==OFFD)
//		{
//			if(OUT1)
//			{
//				GPIO_WriteBit(OUT1_GPIO_Port, OUT1_Pin, Bit_SET);
//				OUT1_Mode.DelayCounter = 0;
//			}
//			else
//			{
//				if(OUT1_Mode.DelayCounter>(OUT1_Mode.DelayValue*DealyBaseTime))
//				{
//					GPIO_WriteBit(OUT1_GPIO_Port, OUT1_Pin, Bit_RESET);
//				}
//			}
//		}
//		/*ON_D*/
//		else if(OUT1_Mode.DelayMode==ON_D)
//		{
//			if(OUT1)
//			{
//				if(OUT1_Mode.DelayCounter>(OUT1_Mode.DelayValue*DealyBaseTime))
//				{
//					GPIO_WriteBit(OUT1_GPIO_Port, OUT1_Pin, Bit_SET);
//				}
//			}
//			else
//			{
//				GPIO_WriteBit(OUT1_GPIO_Port, OUT1_Pin, Bit_RESET);
//				OUT1_Mode.DelayCounter = 0;
//			}
//		}
//		/*SHOT*/
//		else if(OUT1_Mode.DelayMode==SHOT)
//		{
//			if(OUT1 || SHOTflag == 1)
//			{
//				if(OUT1_Mode.DelayCounter<(OUT1_Mode.DelayValue*DealyBaseTime))
//				{
//					GPIO_WriteBit(OUT1_GPIO_Port, OUT1_Pin, Bit_SET);
//					SHOTflag = 1;
//				}
//				else
//				{
//					GPIO_WriteBit(OUT1_GPIO_Port, OUT1_Pin, Bit_RESET);
//					SHOTflag = 0;
//				}
//			}
//			else
//			{
//				OUT1_Mode.DelayCounter = 0;
//				GPIO_WriteBit(OUT1_GPIO_Port, OUT1_Pin, Bit_RESET);
//			}
//		}
//	}
//}
/*******************************
*
*�ж�OUT2�����״̬
*
*******************************/
uint8_t COMPOUT_STATUS;
uint8_t LAST_COMPOUT_STATUS;
void SetOUT2Status(void)
{
	if(ShortCircuit!=1)/*���Ƕ�·����������²��ж�OUT2�����*/
	{
		if(TOFF_MODE==0)
		{
			GPIO_WriteBit(OUT2_GPIO_Port,OUT2_Pin,Bit_RESET);/*����*/
		}
		else if(TOFF_MODE==1)
		{
			if(OUT2==0)
				COMPOUT_STATUS = GPIO_ReadOutputDataBit(COMPOUT_GPIO_Port,COMPOUT_Pin);
			if(COMPOUT_STATUS==1 && LAST_COMPOUT_STATUS==0 && OUT2==0)
			{
				OUT2=1;
				OUT2_TimerCounter = 0;
				GPIO_WriteBit(OUT2_GPIO_Port,OUT2_Pin,Bit_SET);
			}
			if(OUT2==0)
				LAST_COMPOUT_STATUS = COMPOUT_STATUS;
			if(OUT2_TimerCounter >=250)
			{
				OUT2 = 0;
				OUT2_TimerCounter = 0;  /*��ȡ��ǰʱ��*/
				//GPIO_WriteBit(OUT1_GPIO_Port,OUT1_Pin,Bit_RESET);
				GPIO_WriteBit(OUT2_GPIO_Port,OUT2_Pin,Bit_RESET);/*80ms������*/
			}
		}
	}
}



/*******************************
*
*��·����
*
*******************************/
void ShortCircuitProtection(void)
{
	uint8_t SCState;
	GPIO_InitTypeDef gpio_init_structure;  

	/*��ȡSC���ŵ�״̬*/
	if(ShortCircuit!=1)
	{
		SCState = GPIO_ReadInputDataBit(SC_GPIO_Port ,SC_Pin);
		if((BitAction)SCState == Bit_RESET)
		{
			/*����FB_SC*/
			ShortCircuit= 1;
		}
		else
		{
			ShortCircuit = 0;
			ConfirmShortCircuit = 0;
			gpio_init_structure.GPIO_Pin = FB_SC_Pin;  
			gpio_init_structure.GPIO_Mode = GPIO_Mode_IN;  
			gpio_init_structure.GPIO_PuPd= GPIO_PuPd_NOPULL;
			GPIO_Init(FB_SC_GPIO_Port, &gpio_init_structure);
		}
	}
	if(ShortCircuit && ShortCircuitCounter>=100)
	{
		ConfirmShortCircuit=1;			
		gpio_init_structure.GPIO_Pin = FB_SC_Pin;  
		gpio_init_structure.GPIO_Mode = GPIO_Mode_OUT;  
		gpio_init_structure.GPIO_PuPd= GPIO_PuPd_DOWN;
		GPIO_Init(FB_SC_GPIO_Port, &gpio_init_structure);

		GPIO_WriteBit(FB_SC_GPIO_Port,FB_SC_Pin,Bit_RESET);
		//GPIO_WriteBit(OUT2_GPIO_Port,OUT2_Pin,Bit_RESET);/*��������OUT*/
		ShortCircuitTimer = ShortCircuitLastTime;
	}	
}

/*******************************
*
*��������
*
*******************************/
void ButtonMapping(void)
{
	/*��������*/
	if(ModeButton.Effect==PressLong &&DownButton.Effect ==PressLong&&DownButton.Status ==Press)
	{
		if(KEY == ULOC)
				KEY = LOC;
		else
			KEY = ULOC;
		ModeButton.PressCounter = 0;
		while((ReadButtonStatus(&ModeButton)) == Press&&(ReadButtonStatus(&DownButton) == Press))
		{
			ButtonMappingDisplay(1);
			KeyMappingFlag = 1;
		}
		ee_WriteBytes((uint8_t*)&KEY, 60, 1);
		ModeButton.Effect=PressNOEffect;
		ModeButton.PressCounter = 0;
		DownButton.PressCounter = 0;
		DownButton.Effect=PressNOEffect;
		//ee_WriteBytes((uint8_t*)&KEY, 60, 4);
		//I2C_WriteByte(KEY,60,EE_DEV_ADDR);
	}
	/*�����ʼ��*/
	if(ModeButton.Effect==PressLong &&SetButton.Effect ==PressLong&&SetButton.Status ==Press)
	{
		CSV = 1000;
		Threshold = 1000;
		KEY = ULOC;
		OUT1_Mode.DelayMode = TOFF;
		OUT1_Mode.DelayValue = 10;
		DACOUT = 500;
		while((ReadButtonStatus(&ModeButton)) == Press&&(ReadButtonStatus(&SetButton) == Press))
		{
			ButtonMappingDisplay(2);
			KeyMappingFlag = 1;
		}
		ee_WriteBytes((uint8_t*)&OUT1_Mode.DelayMode, 10, 4);
		ee_WriteBytes((uint8_t*)&OUT1_Mode.DelayValue, 20, 4);
		ee_WriteBytes((uint8_t*)&CSV, 30, 4);
		ee_WriteBytes((uint8_t*)&Threshold, 40, 4);
		ee_WriteBytes((uint8_t*)&DACOUT, 50, 4);
		ee_WriteBytes((uint8_t*)&KEY, 60, 4);	
		
		ModeButton.Effect=PressNOEffect;
		ModeButton.PressTimer = 0;
		ModeButton.PressCounter = 0;
		SetButton.Effect=PressNOEffect;
		SetButton.PressCounter = 0;
		
		DAC_SetChannel1Data(DAC_Align_12b_R,(uint16_t)DACOUT);
		DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);

	}
	/*����������*/
	if(ModeButton.Effect==PressLong &&UpButton.Effect ==PressLong&&UpButton.Status ==Press)
	{
		CPV = 0;
		while((ReadButtonStatus(&ModeButton)) == Press&&(ReadButtonStatus(&UpButton) == Press))
		{
			ButtonMappingDisplay(3);
			KeyMappingFlag=1;
		}
		ModeButton.Effect=PressNOEffect;
		ModeButton.PressTimer = 0;
		ModeButton.PressCounter = 0;
		UpButton.PressCounter = 0;
		UpButton.Effect=PressNOEffect;
	}	
}

 void Test_Delay(uint32_t ms)
{
	uint32_t i;

	/*��
		CPU��Ƶ168MHzʱ�����ڲ�Flash����, MDK���̲��Ż�����̨ʽʾ�����۲Ⲩ�Ρ�
		ѭ������Ϊ5ʱ��SCLƵ�� = 1.78MHz (����ʱ: 92ms, ��д������������ʾ����̽ͷ���ϾͶ�дʧ�ܡ�ʱ��ӽ��ٽ�)
		ѭ������Ϊ10ʱ��SCLƵ�� = 1.1MHz (����ʱ: 138ms, ���ٶ�: 118724B/s)
		ѭ������Ϊ30ʱ��SCLƵ�� = 440KHz�� SCL�ߵ�ƽʱ��1.0us��SCL�͵�ƽʱ��1.2us

		��������ѡ��2.2Kŷʱ��SCL������ʱ��Լ0.5us�����ѡ4.7Kŷ����������Լ1us

		ʵ��Ӧ��ѡ��400KHz���ҵ����ʼ���
	*/
	for (i = 0; i < ms*10; i++);
}
/*******************************
*
*��ȡEEPROM����
*
*******************************/

	uint32_t _test_send_buf[30];
	uint32_t _test_recv_buf[30];
	short _test_size = 30;

	char statusW=2;
	char statusR=2;

void GetEEPROM(void)
{
	uint8_t tempKEY=0;
	
	while(ee_CheckOk()==0)//��ⲻ��24c02
	{
	}
//	if(tempDelayMode==31U)
//	{
//		OUT1_Mode.DelayMode = TOFF;
//	}
//	else if(tempDelayMode==32U)
//		OUT1_Mode.DelayMode = OFFD;
//	else if(tempDelayMode==33U)
//		OUT1_Mode.DelayMode = ON_D;
//	else if(tempDelayMode==34U)
//		OUT1_Mode.DelayMode = SHOT;
	
//		for(i=0; i<_test_size; i++)
//		{
//			_test_send_buf[i] = 0;
//			_test_recv_buf[i] = 0xff;
//		}
//		//write
//		while((statusW = ee_WriteBytes((uint8_t*)_test_send_buf, 0, _test_size))==0);
//		Test_Delay(600000);
//		//read
//		while((statusR=ee_ReadBytes((uint8_t*)_test_recv_buf, 0, _test_size))==0);
//		//check		
		///////write 0~255	
//			for(i=0; i<_test_size; i++)
//			{
//				_test_send_buf[i] = i;
//				_test_recv_buf[i] = 0xff;
//			}
			//write
			//ee_WriteBytes((uint8_t*)_test_send_buf, 0, _test_size*4);
			Test_Delay(6000);
			//read
//			ee_ReadBytes((uint8_t*)_test_recv_buf, 0, _test_size*4);
//			//check
//			for(i=0; i<_test_size; i++)
//			{
//				if(_test_recv_buf[i] == _test_send_buf[i])
//				{
//					statusW = 1;
//				}
//				else
//				{
//					statusW = 0;
//				}	
//			}		
		while(ee_ReadBytes((uint8_t*)&TOFF_MODE,10,1)==0);
		while(ee_ReadBytes((uint8_t*)&ATT_MODE,20,1)==0);
		while(ee_ReadBytes((uint8_t*)&CSV,30,4)==0);
		while(ee_ReadBytes((uint8_t*)&Threshold,40,4)==0);
		while(ee_ReadBytes((uint8_t*)&DACOUT,50,4)==0);
		while(ee_ReadBytes((uint8_t*)&KEY,60,1)==0);
		//while(ee_ReadBytes((uint8_t*)&RegisterB,70,1)==0);
//		if(tempKEY == 51U)
//				KEY = ULOC;
//		else if(tempKEY == 52U)
//				KEY = LOC;
		/*��ȡ��֮������DAC��ֵ*/
		GPIO_WriteBit(ATT_GPIO_Port, ATT_Pin, (BitAction)ATT_MODE);
		DAC_SetChannel1Data(DAC_Align_12b_R,(uint16_t)DACOUT);
		DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
		
}


