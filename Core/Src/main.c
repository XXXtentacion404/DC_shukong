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
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sys.h"
#include "delay.h"
#include "lcd.h"
#include "touch.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t __io_putchar(int ch)
{
    uint8_t temp[1]={ch};
    HAL_UART_Transmit(&huart1,temp,1,2);
    return(ch);
}

uint8_t _write(int file, char *ptr,int len)
{
    int DataIdx;
    for(DataIdx = 0; DataIdx < len; DataIdx++)
    {
        __io_putchar(*ptr++);
    }
    return len;
}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int a=0;
int DAC_value=0;
char xianshi[20];		//显示部分 数组
int q=0;
int b=0;
int s=0;
int g=0;
float  input=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t adc_value=0;




void deplay() {
    for (int i = 0; i < 5; ++i) {
        lcd_draw_bline(100 * i+50, 70, 100 * i+50, 470, 2, RED);//竖
        lcd_draw_bline(50,  100 * i+70, 450,  100 * i+70, 2, RED);//横
    }

}


void Dac_value(float input)
{
    if(input<=3.3)
    {
        float dac_value = (input / 3.3) * 4096;
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_value);
        HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
        lcd_show_string(120, 560, 100, 100, 16, "          ", RED);
        lcd_show_float(120, 560, 3*input, 16, RED);
        lcd_show_float(120, 530, dac_value, 16, RED);
    }
    else
        {
            lcd_show_string(120, 530, 100, 100, 16, "         ", RED);
            lcd_show_string(120, 530, 100, 100, 16, "WRONG", RED);
        }
}


void adcgetvalue(){
    HAL_ADC_Start(&hadc1);	                 //启动ADC单次转换
    HAL_ADC_PollForConversion(&hadc1, 50);	//等待ADC转换完成
    adc_value = HAL_ADC_GetValue(&hadc1); 	//读取ADC转换数据
    double vol = ((double) adc_value / 4095) * 3.3;
    lcd_show_float(120,630,adc_value,16,RED);
    lcd_show_float(120,590,vol,16,RED);
}
void ctp_test(void)
{
    //初始化定义变量
    int biaozhi=1; 	 //小数点标志位
    int biaozhi2=1;
    int n=1;
    uint8_t t=0;
    uint8_t i=0;
    uint16_t  lastpos[5][2];
//    char xianshi[20];		//显示部分 数组
    xianshi[0]=' ';     //初始第一位
    input = 0;
    Dac_value(input);
    while(1)
    {
//        adcgetvalue();
        //键盘显示部分
        lcd_show_string(20,500,100,100,24,"Input",RED);
        lcd_show_string(20,530,100,100,24,"Output:",RED);
        lcd_show_string(20,560,100,100,24,"Value:",RED);
        lcd_show_string(20,590,100,100,24,"ADC :",RED);
        lcd_draw_rectangle(95,500,400,525,RED);

        lcd_show_num(100,100,1,1,24,RED);
        lcd_show_num(200,100,2,1,24,RED);
        lcd_show_num(300,100,3,1,24,RED);
        lcd_show_string(400,100,200,24,32,"+",RED);
        lcd_show_num(100,200,4,1,24,RED);
        lcd_show_num(200,200,5,1,24,RED);
        lcd_show_num(300,200,6,1,24,RED);
        lcd_show_string(400,200,200,24,32,"-",RED);
        lcd_show_num(100,300,7,1,24,RED);
        lcd_show_num(200,300,8,1,24,RED);
        lcd_show_num(300,300,9,1,24,RED);
        lcd_show_string(400,300,200,24,32,"C",RED);
        lcd_show_num(100,400,0,1,24,RED);
        lcd_show_char(200,400,'.',24,1,BLACK);
        lcd_show_string(300,400,200,24,24,"OK",RED);
        lcd_show_string(400,400,200,24,24,"NO",RED);
        lcd_show_string(400,720,200,200,24,"GET",RED);

        //坐标信息
        tp_dev.scan(0);
        for(t=0;t<1;t++)
        {
            if((tp_dev.sta)&(1<<t))
            {
                if(tp_dev.x[t]<lcddev.width&&tp_dev.y[t]<lcddev.height)
                {
                    if(lastpos[t][0]==0XFFFF)
                    {
                        lastpos[t][0] = tp_dev.x[t];
                        lastpos[t][1] = tp_dev.y[t];
                    }
//					lcd_draw_bline(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],2,POINT_COLOR_TBL[t]);//画线
                    lastpos[t][0]=tp_dev.x[t];
                    lastpos[t][1]=tp_dev.y[t];

                    if(tp_dev.x[t]>50&&tp_dev.x[t]<150&&tp_dev.y[t]>50&&tp_dev.y[t]<150)  //如果按下1
                    {
                        while(tp_dev.x[t]>50&&tp_dev.x[t]<150&&tp_dev.y[t]>50&&tp_dev.y[t]<150)
                        {
                            tp_dev.scan(0);
                        }
                        xianshi[n]='1';
                        n++;
                    }
                    if(tp_dev.x[t]>150&&tp_dev.x[t]<250&&tp_dev.y[t]>50&&tp_dev.y[t]<150)  //如果按下2
                    {
                        while(tp_dev.x[t]>150&&tp_dev.x[t]<250&&tp_dev.y[t]>50&&tp_dev.y[t]<150)
                        {
                            tp_dev.scan(0);
                        }
                        xianshi[n]='2';
                        n++;
                    }
                    if(tp_dev.x[t]>250&&tp_dev.x[t]<350&&tp_dev.y[t]>50&&tp_dev.y[t]<150)  //如果按下3
                    {
                        while(tp_dev.x[t]>250&&tp_dev.x[t]<350&&tp_dev.y[t]>50&&tp_dev.y[t]<150)
                        {
                            tp_dev.scan(0);
                        }
                        xianshi[n]='3';
                        n++;
                    }
                    if (tp_dev.x[t] > 350 && tp_dev.x[t] < 450 && tp_dev.y[t] > 50 && tp_dev.y[t] < 150)  // 如果按下+
                    {
                        while (tp_dev.x[t] > 350 && tp_dev.x[t] < 450 && tp_dev.y[t] > 50 && tp_dev.y[t] < 150)
                        {
                            tp_dev.scan(0);
                        }
                        input = atof(xianshi);  // 字符串转化为数字
                        input = input + 0.0034
                                ;
                        snprintf(xianshi, 20, "%f", input);  // 数字转化为字符串
                        Dac_value(input);
                    }


                    if(tp_dev.x[t]>50&&tp_dev.x[t]<150&&tp_dev.y[t]>150&&tp_dev.y[t]<250)  //如果按下4
                    {
                        while(tp_dev.x[t]>50&&tp_dev.x[t]<150&&tp_dev.y[t]>150&&tp_dev.y[t]<250)
                        {
                            tp_dev.scan(0);
                        }
                        xianshi[n]='4';
                        n++;
                    }
                    if(tp_dev.x[t]>150&&tp_dev.x[t]<250&&tp_dev.y[t]>150&&tp_dev.y[t]<250)  //如果按下5
                    {
                        while(tp_dev.x[t]>150&&tp_dev.x[t]<250&&tp_dev.y[t]>150&&tp_dev.y[t]<250)
                        {
                            tp_dev.scan(0);
                        }
                        xianshi[n]='5';
                        n++;
                    }
                    if(tp_dev.x[t]>250&&tp_dev.x[t]<350&&tp_dev.y[t]>150&&tp_dev.y[t]<250)  //如果按下6
                    {
                        while(tp_dev.x[t]>250&&tp_dev.x[t]<350&&tp_dev.y[t]>150&&tp_dev.y[t]<250)
                        {
                            tp_dev.scan(0);
                        }
                        xianshi[n]='6';
                        n++;
                    }
                    if (tp_dev.x[t] > 350 && tp_dev.x[t] < 450 && tp_dev.y[t] > 150 && tp_dev.y[t] < 250)  // 如果按下-
                    {
                        while (tp_dev.x[t] > 350 && tp_dev.x[t] < 450 && tp_dev.y[t] > 150 && tp_dev.y[t] < 250)
                        {
                            tp_dev.scan(0);
                        }
                        input = atof(xianshi);  // 字符串转化为数字
                        input = input - 0.0034;
                        snprintf(xianshi, 20, "%f", input);  // 数字转化为字符串
                        Dac_value(input);
                    }
                    if(tp_dev.x[t]>50&&tp_dev.x[t]<150&&tp_dev.y[t]>250&&tp_dev.y[t]<350)  //如果按下7
                    {
                        while(tp_dev.x[t]>50&&tp_dev.x[t]<150&&tp_dev.y[t]>250&&tp_dev.y[t]<350)
                        {
                            tp_dev.scan(0);
                        }
                        xianshi[n]='7';
                        n++;
                    }
                    if(tp_dev.x[t]>150&&tp_dev.x[t]<250&&tp_dev.y[t]>250&&tp_dev.y[t]<350)  //如果按下8
                    {
                        while(tp_dev.x[t]>150&&tp_dev.x[t]<250&&tp_dev.y[t]>250&&tp_dev.y[t]<350)
                        {
                            tp_dev.scan(0);
                        }
                        xianshi[n]='8';
                        n++;
                    }
                    if(tp_dev.x[t]>250&&tp_dev.x[t]<350&&tp_dev.y[t]>250&&tp_dev.y[t]<350)  //如果按下9
                    {
                        while(tp_dev.x[t]>250&&tp_dev.x[t]<350&&tp_dev.y[t]>250&&tp_dev.y[t]<350)
                        {
                            tp_dev.scan(0);
                        }
                        xianshi[n]='9';
                        n++;
                    }
                    if(tp_dev.x[t]>350&&tp_dev.x[t]<450&&tp_dev.y[t]>250&&tp_dev.y[t]<350)  //如果按下C
                    {
                        while(tp_dev.x[t]>350&&tp_dev.x[t]<450&&tp_dev.y[t]>250&&tp_dev.y[t]<350)
                        {
                            tp_dev.scan(0);
                        }
                       biaozhi=1;
                        for(i=0;i<20;i++)
                        {
                            xianshi[i]=' ';
                        }
                        n=1;
                        xianshi[0]=' ';
                            lcd_show_string(100,500,240,24,24,xianshi,RED);
                            lcd_fill(95,500,400,525,WHITE);    //清屏部分，防止乱码
                    }
                    if(tp_dev.x[t]>50&&tp_dev.x[t]<150&&tp_dev.y[t]>350&&tp_dev.y[t]<450)  //如果按下0
                    {
                        while(tp_dev.x[t]>50&&tp_dev.x[t]<150&&tp_dev.y[t]>350&&tp_dev.y[t]<450)
                        {
                            tp_dev.scan(0);
                        }
                        xianshi[n]='0';
                        n++;
                    }
                    if(tp_dev.x[t]>150&&tp_dev.x[t]<250&&tp_dev.y[t]>350&&tp_dev.y[t]<450)  //如果按下.
                    {
                        while(tp_dev.x[t]>150&&tp_dev.x[t]<250&&tp_dev.y[t]>350&&tp_dev.y[t]<450)
                        {
                            tp_dev.scan(0);
                        }
                        if(biaozhi==1)               //小数点防重复程序
                        {xianshi[n]='.';
                            n++;
                            biaozhi++;
                        }
                    }
                    if(tp_dev.x[t]>250&&tp_dev.x[t]<350&&tp_dev.y[t]>350&&tp_dev.y[t]<450)  //如果按下OK
                    {
                        while(tp_dev.x[t]>250&&tp_dev.x[t]<350&&tp_dev.y[t]>350&&tp_dev.y[t]<450)
                        {
                            tp_dev.scan(0);
                        }
                        float i=atof(xianshi);
                        Dac_value( i);
                    }
                    if(tp_dev.x[t]>400&&tp_dev.x[t]<480&&tp_dev.y[t]>700&&tp_dev.y[t]<800)  //如果按下get
                    {
                        while(tp_dev.x[t]>250&&tp_dev.x[t]<350&&tp_dev.y[t]>350&&tp_dev.y[t]<450)
                        {
                            tp_dev.scan(0);
                        }
                       adcgetvalue();
                    }
                    if(tp_dev.x[t]>350&&tp_dev.x[t]<450&&tp_dev.y[t]>350&&tp_dev.y[t]<450)  //如果按下NO  删除部分
                    {
                        while(tp_dev.x[t]>350&&tp_dev.x[t]<450&&tp_dev.y[t]>350&&tp_dev.y[t]<450)
                        {
                            tp_dev.scan(0);
                        }
                        if(xianshi[n-1]=='.')      //小数点删除特殊情况，还原标志位
                        {
                            biaozhi=1;
                        }
                        xianshi[n-1]=' ';
                        n--;
                        if(n==0)
                            n=1;
                    }
//如果输入超过长度程序部分
                    if(n>9)
                    {
                        for(i=0;i<9;i++)        //清空数组
                        {
                            xianshi[i]=' ';
                            n=1;
                        }
                        lcd_show_string(100,500,240,24,24,xianshi,RED);
                        lcd_fill(95,500,400,525,WHITE);    //清屏部分，防止乱码
                    }
//显示部分
                    lcd_fill(95,500,400,525,WHITE);
                    lcd_show_string(100,500,240,24,24,xianshi,RED);
                }
            }else lastpos[t][0]=0XFFFF;
        }
    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_DAC_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
    lcd_init();
    lcd_clear(WHITE);
    deplay();
    tp_dev.init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//        deplay();
//        draw();
//        abc();
//
        ctp_test();
        }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
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
