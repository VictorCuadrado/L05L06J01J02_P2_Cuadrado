static GPIO_InitTypeDef GPIO_InitStruct;

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    HAL_Init();

    /* Configure the system clock to 168 MHz */
    SystemClock_Config();
    SystemCoreClockUpdate();

    /* Add your application code here */

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Infinite loop */
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        HAL_Delay(100);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
        HAL_Delay(100);
    }
}