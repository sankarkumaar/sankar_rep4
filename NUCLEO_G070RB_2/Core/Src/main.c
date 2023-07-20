/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */ //**09 May 2023 08:59:16 GMT
/*
        hour = (hour + 5) % 24
        minute = minute + 30
        if minute >= 60 then
            hour = hour + 1
            minute = minute - 60
        end
        second = second or "00"
        */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdarg.h>

#define TWO_RELAYS              2
#define FOUR_RELAYS             4

#define NO_OF_RELAYS            FOUR_RELAYS

#define Ready_to_send           2
static const unsigned char uniqueid[]="SS001";
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */
unsigned char Hour=0,Min=0,Sec=0;
unsigned char Date=0,Month=0,Year=0;
/* Private function prototypes -----------------------------------------------*/
void update_time(char *rx_time);
void Settime_RTC();
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM1_Init(void);
void vprint(const char *fmt, va_list argp);
void Read_Sensor(void);
//void create_datapocket(char data);
//void create_datapocket(char data, char configflag,int configcnt);
void create_datapocket(char conn,char data, char configflag1,unsigned int configcnt1,char configflag2,unsigned int configcnt2);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
char time[15];
char date[15];
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
//=====================================
unsigned char B_data[201][100]={0};
unsigned int B_row=0,B_col=0,sent_cnt=0;
unsigned char timer_flag=0,data_flag=0;
unsigned char rx_arr[1000];
unsigned char time_arr[100];
unsigned char config_arr1[100],config_arr2[100];
unsigned int timer_cnt=0,rx_cnt=0,i=0,time_cnt=0,config_cnt1=0,config_cnt2=0;
unsigned char rx_data[1];


unsigned char set_time=0,set_config=0;
unsigned char event=0,starter=0,starter1=0,starter2=0;

unsigned char postdata[100];
unsigned char sensor1=0,sensor2=0,sensor1_backup=0,sensor2_backup=0;
unsigned char sensordetected=0;
unsigned char starterflag=0,machine1=0,machine2=0;
unsigned int startertimercnt=0,currentval1=0,inputval1=0,currentval2=0,inputval2=0;
char firstdata =1,connection=1,backup_bit=0;
char c_flag1=0,c_flag2=0;
char B_timer_flag=0,B_timer_send=0;
int B_timer_cnt=0;
int postflag=0,postcnt=0;


//D0 Relay0->, D1 Relay1->, D2->Relay3, D3 Relay4
//C6 sensor0,C7 sensor1
//void create_datapocket(char data, char configflag1,int configcnt1,char configflag2,int configcnt2);

int main(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();


  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, 1);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 1);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 1);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 1);

  MX_USART2_UART_Init();
 // MX_RTC_Init();
  MX_TIM1_Init();

  HAL_TIM_Base_Start_IT(&htim1);

  HAL_UART_Receive_IT(&huart2, rx_data, 1);
  ei_printf("Project starts");
  //D0 Relay0->, D1 Relay1->, D2->Relay3, D3 Relay4
  //C6 sensor0,C7 sensor1

  __HAL_RCC_GPIOD_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, 1);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 1);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 1);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 1);
  ////set_time=3;////
  while(1)
  {
//	  while(1)
//	  {
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, 0);
//		  HAL_Delay(1000);
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 1);
//		  HAL_Delay(1000);
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 1);
//		  HAL_Delay(1000);
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 1);
//		  HAL_Delay(1000);
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, 0);
//		  HAL_Delay(1000);
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 0);
//		  HAL_Delay(1000);
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 0);
//		  HAL_Delay(1000);
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 0);
//		  HAL_Delay(1000);
//	  }
	  if(backup_bit==1 && connection == 1)  // transmit backup data
	  {
		  B_timer_flag=1;
		  if(B_timer_send==1)
		  {
			  ei_printf("%s", B_data[sent_cnt]);
			  sent_cnt++;
			  B_timer_flag=1;
			  if(B_row==sent_cnt)
			  {
				  B_row=0;
				  sent_cnt=0;
				  backup_bit=0;
				  B_timer_flag=0;
			  }
			  B_timer_send=0;
		  }
	  }
	  if(data_flag==1)
	  {
        // HAL_UART_Transmit(&huart2, rx_arr, rx_cnt, rx_cnt);
         rx_cnt=0;
         data_flag=0;
	  }

	  if(set_time==Ready_to_send)// update time
	  {
		  //update_time("2023-05-10 15:59:10");//*2023-05-10 15:39:10,https://api.sourcesol.co.in/dac/data,000,000,000,000,000#

		  //ei_printf("%s",time_arr);
		  update_time(time_arr);
		  Settime_RTC();
		  set_time=3;

		  time_cnt=0;
		  sensordetected=1;// to send data soon after time is taken
	  }
	  if(set_time==3)
	     Read_Sensor();
	  if(starterflag == Ready_to_send) // turn HIGH all relay pins afr specific pin turned low
	  {
		  //ei_printf(" ----- came2----- ");
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, 1);
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 1);
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 1);
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 1);
		  starterflag=0;
	  }
	  if((event==Ready_to_send || sensordetected == 1) && (set_time==3 || set_config ==1))
	  {

		  if(event==Ready_to_send || set_config == 1)
		  {
			  //ei_printf(" ----- came----- ");
			  if(NO_OF_RELAYS == TWO_RELAYS)
			  {
                  if(starter==1 || machine1==1)
                	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, 0); // ON
                  else if(starter==2)
                	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, 1); // OFF
                  else if(starter==3|| machine2==1)
                	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 0); //
                  else if(starter==4)
                	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 1);

			  }
			  else if(NO_OF_RELAYS == FOUR_RELAYS)
			  {
				    if(starter==1 || machine1==1)
					  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, 0);// starter1 on
					else if(starter==2)
					  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 0);// starter1 off
					else if(starter==3 || machine2==1)
					  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 0);// starter2 on
					else if(starter==4)
					  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 0);// starter1 off

				    starterflag=1;
				    startertimercnt=0;

			  }
			  event=0;
		  }
		  if(set_config==1)
		  {
			  if(machine1==1)
			  {
				  for(i=0;i<20;i++)   //remove '*'
				  {
					  if(config_arr1[i]=='*')
					  {
						  config_arr1[i]=='\0';
						  break;
					  }
				  }
				  inputval1 = atoi(config_arr1);
				  set_config=0;
				  config_cnt1=0;
				  c_flag1=1;
				  for(i=0;i<20;i++)
					  config_arr1[i]=0;
			  }
			  else if(machine2==1)
			  {
				 for(i=0;i<20;i++)   //remove '*'
				 {
					   if(config_arr2[i]=='*')
					   {
						  config_arr2[i]=='\0';
						  break;
					   }
				 }
				 inputval2 = atoi(config_arr2);
				 set_config=0;
				 config_cnt2=0;
				 c_flag2=1;

				 for(i=0;i<20;i++)
					config_arr2[i]=0;
			  }
			  machine1=0;
			  machine2=0;
		  }

		 //ei_printf("%s",starter1+48,starter2+48);
		  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		  HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);


		  sprintf(date,"%02d%02d%02d ",sDate.Date,sDate.Month,sDate.Year);
		  sprintf(time,"%02d%02d%02d ",sTime.Hours,sTime.Minutes,sTime.Seconds);
		  //
		  for(i=0;i<50;i++)
			postdata[i]=0;

		  create_datapocket(connection,firstdata,c_flag1,currentval1/2,c_flag2,currentval2/2);

		  if(connection==1)
		  {

		     if(firstdata==1)
		     {
		        HAL_Delay(5000);
		     }
		     if(postflag==1)
		     {
		        ei_printf("%s",postdata);
		        postflag=0;
		     }
		     //HAL_Delay(100);
		     firstdata=0;
		  }

		  sensordetected=0;
	  }

  }

}

void Read_Sensor(void)
{
	  if((HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)==1))
		  sensor1=0;
	  else
	  {
		  sensor1=1;

	  }

	  if((HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==1))
		  sensor2=0;
	  else
	  {
		  sensor2=1;

	  }

	  if(sensor1_backup!=sensor1 || sensor2_backup!=sensor2)
	  {
		  sensordetected=1;
		  if(sensor1_backup!=sensor1)
		  {
			 if(c_flag1 == 1)
			     currentval1++;
		     if(c_flag1 == 1 && currentval1/2== inputval1)
		     {
				 c_flag1=0;
				 currentval1=0;
				 inputval1=0;
				 if(NO_OF_RELAYS == FOUR_RELAYS)
				 {

				    starter=2;
				    event=Ready_to_send;
				 }
				 else if(NO_OF_RELAYS == TWO_RELAYS)
				 {
					  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, 1); // OFF
				 }
				 starter1=0;  // turn off starter1 flag
		     }
		  }
		  if(sensor2_backup!=sensor2)
		  {
			 if(c_flag2 == 1)
			      currentval2++;
			 if(c_flag2 == 1 && currentval2/2== inputval2)
			 {

				 c_flag2=0;
				 currentval2=0;
				 inputval2=0;
				 if(NO_OF_RELAYS == FOUR_RELAYS)
				 {

				    starter=4;
				    event=Ready_to_send;
				 }
				 else if(NO_OF_RELAYS == TWO_RELAYS)
				 {
					  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 1); // OFF
				 }
				 starter2=0;   // turn off starter2 flag
			 }
		  }
		  //ei_printf("%d%d",sensor1+48,sensor2+48);
	  }
	  sensor1_backup = sensor1;
	  sensor2_backup = sensor2;
}
void create_datapocket(char conn,char data, char configflag1,unsigned int configcnt1,char configflag2,unsigned int configcnt2)
{
	  int i=0,j=0;
	  int postdata_cnt=0;
	  char countdata1[20]={0};
	  char countdata2[20]={0};


	  for(i=0;uniqueid[i]!='\0';i++)
	  {
		  postdata[postdata_cnt++]=uniqueid[i];
	  }
	  postdata[postdata_cnt++] = ',';

	  if(conn==1)
	     postdata[postdata_cnt++] = 'L';
	  else if(conn==0)
		 postdata[postdata_cnt++] = 'B';

	  postdata[postdata_cnt++] = ',';
	  postdata[postdata_cnt++] = (data)+48;
	  postdata[postdata_cnt++] = ',';
	  postdata[postdata_cnt++] = (sensor1)+48;
	  postdata[postdata_cnt++] = (sensor2)+48;
	  postdata[postdata_cnt++] =  starter1+48;
	  postdata[postdata_cnt++] =  starter2+48;
	  postdata[postdata_cnt++] =  ',';
	  for(i=0;i<6;i++)
	  {
		  postdata[postdata_cnt++]=date[i];
	  }
	  for(i=0;i<6;i++)
	  {
		  postdata[postdata_cnt++]=time[i];
	  }
	  postdata[postdata_cnt++] =  ',';
	  postdata[postdata_cnt++] =  configflag1+48;
	  postdata[postdata_cnt++] =  ',';
	  sprintf(countdata1,"%d",(configcnt1));
	  for(i=0;countdata1[i]!='\0';i++)
		  postdata[postdata_cnt++] = countdata1[i];

	  postdata[postdata_cnt++] =  ',';
	  postdata[postdata_cnt++] =  configflag2+48;
	  postdata[postdata_cnt++] =  ',';
	  sprintf(countdata2,"%d",(configcnt2));
	  for(i=0;countdata2[i]!='\0';i++)
		  postdata[postdata_cnt++] = countdata2[i];
	  //postdata[postdata_cnt++] =  ',';
	  postdata[postdata_cnt++]=0x0A;

	  if(conn==0 && B_row!=200)
	  {
		  backup_bit=1;
		  B_col=0;
		  for(j=0;j<postdata_cnt;j++)
		  {
			  B_data[B_row][B_col] = postdata[j];
			  B_col++;
		  }
		  B_row++;

	  }

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

    HAL_UART_Receive_IT(&huart2, rx_data, 1);
    rx_arr[rx_cnt]=rx_data[0];
    if(set_time==1 && rx_arr[rx_cnt]!='*')
        time_arr[time_cnt++]=rx_arr[rx_cnt];


    if(set_config==1 && rx_arr[1]== '1' && rx_arr[rx_cnt]!=','  )
    {
    	if(rx_cnt!=1)
    	   config_arr1[config_cnt1++] = rx_arr[rx_cnt];
    	machine1=1;
    	starter1=1;

    }
    if(set_config==1 && rx_arr[1]== '2' && rx_arr[rx_cnt]!=','  )
    {
    	if(rx_cnt!=1)
    	   config_arr2[config_cnt2++] = rx_arr[rx_cnt];
    	machine2=1;
    	starter2=1;
    }
    if(set_time==0 && rx_arr[rx_cnt]=='*'&& rx_arr[rx_cnt-1]=='*')
    {
    	set_time=1;
    }
    if(set_config==0 && rx_arr[rx_cnt]==')'&& rx_arr[rx_cnt-1]==')')
    {
    	set_config=1;
    	event=1;
    	rx_cnt=0;
    }
    if(event==0 && rx_arr[rx_cnt]=='+'&& rx_arr[rx_cnt-1]=='+')
    {
    	event = 1,starter=1;
    	starter1=1;
    }
    if(event==0 && rx_arr[rx_cnt]=='-'&& rx_arr[rx_cnt-1]=='-')
    {
    	event = 1,starter=2;
    	starter1=0;
    }
    if(event==0 && rx_arr[rx_cnt]=='('&& rx_arr[rx_cnt-1]=='(')
    {
    	event = 1,starter=3;
    	starter2=1;
    }
    if(event==0 && rx_arr[rx_cnt]=='#'&& rx_arr[rx_cnt-1]=='#')
    {
    	event = 1,starter=4;
    	starter2=0;
    }
    if(rx_arr[rx_cnt]=='&'&& rx_arr[rx_cnt-1]=='&')
    {
         connection=0;
    }
    if(rx_arr[rx_cnt]=='@'&& rx_arr[rx_cnt-1]=='@')
    {
         connection=1;
    }
    if(set_config==1 && rx_arr[rx_cnt]=='*')
        rx_cnt =0;
    rx_cnt++;
    timer_flag=1;

}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	  //ei_printf("T1");

	  if(starterflag==1 && NO_OF_RELAYS == FOUR_RELAYS)
	  {
		 startertimercnt++;
		 if(startertimercnt == 5)
		 {
			 starterflag = Ready_to_send;
			 startertimercnt=0;
		 }
	  }
	  //=========================
	  if(postflag==0)
	     postcnt=1;
	  if(postcnt==1)
		  postcnt=0,postflag=1;
	  //========================
      if(B_timer_flag)
    	  B_timer_cnt++;
      if(B_timer_cnt==2)
    	  B_timer_send=1,B_timer_cnt=0,B_timer_flag=0;
	  if(timer_flag)
		  timer_cnt++;
	  if(timer_cnt==3)
	  {
		  data_flag=1,timer_flag=0,timer_cnt=0;
		  if(set_time==1)
			 set_time=2,rx_arr[rx_cnt]!='\0';
		  if(set_config==1)
			  sensordetected=1,rx_arr[rx_cnt]!='\0'; // dont go inside starter on/off,so sen_detected=1
		  if(event==1 && set_config!=1)              // dont go inside starter on/off,so sen_detected=1
			  event=Ready_to_send;
	  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  //hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
 // hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x10;
  sTime.Minutes = 0x10;
  sTime.Seconds = 0x10;
  sTime.SubSeconds = 0x10;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = 0x01;//RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x10;
  sDate.Year = 0x23;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the reference Clock input
  */
  if (HAL_RTCEx_SetRefClock(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

void Settime_RTC()
{
	  RTC_TimeTypeDef sTime = {0};
	  RTC_DateTypeDef sDate = {0};

	  /* USER CODE BEGIN RTC_Init 1 */

	  /* USER CODE END RTC_Init 1 */

	  /** Initialize RTC Only
	  */
	  hrtc.Instance = RTC;
	  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	  hrtc.Init.AsynchPrediv = 127;
	  hrtc.Init.SynchPrediv = 255;
	  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	  //hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
	  if (HAL_RTC_Init(&hrtc) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sTime.Hours = Hour;
	  sTime.Minutes = Min;
	  sTime.Seconds = Sec;
	  //sTime.SubSeconds = 0x0;
	  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
	  sDate.Month = Month;
	  sDate.Date = Date;
	  sDate.Year = Year;

	  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Enable the reference Clock input
	  */
	  if (HAL_RTCEx_SetRefClock(&hrtc) != HAL_OK)
	  {
	    Error_Handler();
	  }
}
/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 1000;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 7200;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_GREEN_Pin */
  GPIO_InitStruct.Pin = LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_GREEN_GPIO_Port, &GPIO_InitStruct);

}
void vprint(const char *fmt, va_list argp)
{
    char string[200];
    if(0 < vsprintf(string, fmt, argp)) // build string
    {
        HAL_UART_Transmit(&huart2, (uint8_t*)string, strlen(string), 0xffffff); // send message via UART
    }
}

void ei_printf(const char *format, ...) {
    va_list myargs;
    va_start(myargs, format);
    vprint(format, myargs);
    va_end(myargs);
}
/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

