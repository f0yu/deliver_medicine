#include "stm32_u8g2.h"
//#include "stm32f10x_i2c.h"
#include "i2c.h"

#ifdef HARDWARE_I2C
//uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
//    uint8_t* data = (uint8_t*) arg_ptr;
//    switch(msg) {
//        case U8X8_MSG_BYTE_SEND:
//            while( arg_int-- > 0 ) {
//                I2C_SendData(I2C2, *data++);
//                while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) 
//                    continue;
//            }
//            break;
//        case U8X8_MSG_BYTE_INIT:
//        /* add your custom code to init i2c subsystem */
//            RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
//            I2C_InitTypeDef I2C_InitStructure = {
//                .I2C_Mode = I2C_Mode_I2C,
//                .I2C_DutyCycle = I2C_DutyCycle_2,
//                .I2C_OwnAddress1 = 0x10,
//                .I2C_Ack = I2C_Ack_Enable,
//                .I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit,
//                .I2C_ClockSpeed = 400000
//            };
//            I2C_Init(I2C2, &I2C_InitStructure);
//            I2C_Cmd(I2C2, ENABLE);  
//            break;
//        case U8X8_MSG_BYTE_SET_DC:
//        /* ignored for i2c */
//            break;
//        case U8X8_MSG_BYTE_START_TRANSFER:
//            while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
//            I2C_GenerateSTART(I2C2, ENABLE);
//            while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
//                continue;
//            I2C_Send7bitAddress(I2C2, 0x78, I2C_Direction_Transmitter);
//            while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
//                continue;
//            break;
//        case U8X8_MSG_BYTE_END_TRANSFER:
//            I2C_GenerateSTOP(I2C2, ENABLE);
//            break;
//        default:
//            return 0;
//    }
//    return 1;
//}

uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    /* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
    static uint8_t buffer[128];
    static uint8_t buf_idx;
    uint8_t *data;

    switch (msg)
    {
    case U8X8_MSG_BYTE_INIT:
    {
        /* add your custom code to init i2c subsystem */
//        MX_I2C1_Init(); //I2C1��ʼ��
//			 MX_I2C2_Init(); //I2C2��ʼ��
    }
    break;

    case U8X8_MSG_BYTE_START_TRANSFER:
    {
        buf_idx = 0;
    }
    break;

    case U8X8_MSG_BYTE_SEND:
    {
        data = (uint8_t *)arg_ptr;

        while (arg_int > 0)
        {
            buffer[buf_idx++] = *data;
            data++;
            arg_int--;
        }
    }
    break;

    case U8X8_MSG_BYTE_END_TRANSFER:
    {
        if (HAL_I2C_Master_Transmit(&hi2c1, (OLED_ADDRESS), buffer, buf_idx, 1000) != HAL_OK)
            return 0;
    }
    break;

    case U8X8_MSG_BYTE_SET_DC:
        break;

    default:
        return 0;
    }

    return 1;
}

uint8_t u8x8_gpio_and_delay_hw(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
            break;
        case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
            break;
        case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
//            HAL_Delay(1);
            break;
        case U8X8_MSG_DELAY_I2C: // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
            break;                    // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
        case U8X8_MSG_GPIO_I2C_CLOCK: // arg_int=0: Output low at I2C clock pin
            break;                    // arg_int=1: Input dir with pullup high for I2C clock pin
        case U8X8_MSG_GPIO_I2C_DATA:  // arg_int=0: Output low at I2C data pin
            break;                    // arg_int=1: Input dir with pullup high for I2C data pin
        case U8X8_MSG_GPIO_MENU_SELECT:
            u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_NEXT:
            u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_PREV:
            u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_HOME:
            u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
            break;
        default:
            u8x8_SetGPIOResult(u8x8, 1); // default return value
            break;
    }
    return 1;
}

void HardWare_I2C2_GPIOInit(void)
{
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

//    GPIO_InitTypeDef GPIO_InitStructure;
//    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
//    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//    GPIO_Init(GPIOB,&GPIO_InitStructure);
}

#endif 

#ifdef SOFTWARE_I2C
uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
        __NOP();
        break;
    case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
        for (uint16_t n = 0; n < 320; n++)
        {
            __NOP();
        }
        break;
    case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
//        HAL_Delay(1);
        break;
    case U8X8_MSG_DELAY_I2C: // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
//		Tims_delay_us(5);
        break;                    // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    case U8X8_MSG_GPIO_I2C_CLOCK: // arg_int=0: Output low at I2C clock pin
        if(arg_int==1)
        {
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
        }
        else if (arg_int==0)
        {
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
        }
        break;                    // arg_int=1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:  // arg_int=0: Output low at I2C data pin
        if(arg_int==1)
        {
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
        }
        else if (arg_int==0)
        {
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
        }
        break;                    // arg_int=1: Input dir with pullup high for I2C data pin
    case U8X8_MSG_GPIO_MENU_SELECT:
        u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_NEXT:
        u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_PREV:
        u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_HOME:
        u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
        break;
    default:
        u8x8_SetGPIOResult(u8x8, 1); // default return value
        break;
    }
    return 1;
}
#endif 
//��u8g2���г�ʼ��   ��Ӳ��iic�ڵĳ�����б�д
void u8g2Init(u8g2_t *u8g2)
{
    #ifdef SOFTWARE_I2C
//    MyI2C_Init();
	OLED_Init();
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay); 
    #endif 

	#ifdef HARDWARE_I2C
    HardWare_I2C2_GPIOInit();
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_hw_i2c, u8x8_gpio_and_delay_hw);
    #endif 
     
	u8g2_InitDisplay(u8g2);                                                                       
	u8g2_SetPowerSave(u8g2, 0);                                                                   
	u8g2_ClearBuffer(u8g2);
}
//void Tims_delay_ms(uint32_t xms)
//{
//	unsigned char xms1 = 1;
//	while(xms--)
//	{
//		xms1--;
//	}
//}

void draw_(u8g2_t *u8g2)
{
	u8g2_ClearBuffer(u8g2); 
	
    u8g2_SetFontMode(u8g2, 1); 
    u8g2_SetFontDirection(u8g2, 0); 
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf); 
    u8g2_DrawStr(u8g2, 0, 20, "U");
    
    u8g2_SetFontDirection(u8g2, 1);
    u8g2_SetFont(u8g2, u8g2_font_inb30_mn);
    u8g2_DrawStr(u8g2, 21,8,"8");
        
    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(u8g2, 51,30,"g");
    u8g2_DrawStr(u8g2, 67,30,"\xb2");
    
    u8g2_DrawHLine(u8g2, 2, 35, 47);
    u8g2_DrawHLine(u8g2, 3, 36, 47);
    u8g2_DrawVLine(u8g2, 45, 32, 12);
    u8g2_DrawVLine(u8g2, 46, 33, 12);
  
    u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
    u8g2_DrawStr(u8g2, 1,54,"github.com/olikraus/u8g2");
		
	u8g2_SendBuffer(u8g2);
//	HAL_Delay(1000);
}


void testDrawPixelToFillScreen(u8g2_t *u8g2)
{
	u8g2_ClearBuffer(u8g2);

  for (int j = 0; j < 64; j++)
  {
    for (int i = 0; i < 128; i++)
    {
      u8g2_DrawPixel(u8g2,i, j);
    }
  }
//  HAL_Delay(1);
  u8g2_SendBuffer(u8g2);
}
