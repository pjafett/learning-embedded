#include <stdint.h>
#include "stm32f446xx.h"

int main(void)
{
    //Enable clock GPIOA
    RCC->AHB1ENR |= 1U<<0;

    //Set PA9 to input mode
    GPIOA->MODER &= ~(3U<<18);

    //Enable pull-down on PA9
    GPIOA->PUPDR &= ~(3U<<18);
    GPIOA->PUPDR |= 1U<<19;

    //Set PA8 to output mode
    GPIOA->MODER &= ~(3U<<16);
    GPIOA->MODER |= 1U<<16;

    //Set PA8 output to follow PA9 input
    while(1) {
        uint32_t button = (GPIOA->IDR >> 9) & 1U;

        if (button) {
            GPIOA->ODR |= 1U<<8;
        } else {
            GPIOA->ODR &= ~(1U<<8);
        }
    }
}
