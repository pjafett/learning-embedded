#include "stm32f446xx.h"

#define GPIOAEN (1U<<0)
#define PIN5 (1U<<5)
#define LED_PIN PIN5

int main()
{
	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	while(1)
	{
		GPIOA->BSRR = (1U<<21);
		for(int i = 0; i<100000;i++);

		GPIOA->BSRR = (1U<<5);
		for(int i = 0; i<100000;i++);
	}
}
