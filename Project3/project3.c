/*
 * project3.c
 *
 * author: Emre Boran Karabey / 1801022011
 *
 *
 */

#include <stdint.h>
#include "stm32g0xx.h"

#define high    5500000
#define low     1500

void Digit_One (void);
void Digit_Two (void);
void Digit_Three (void);
void Digit_Four (void);
void ClearingSSD (void);
void SettingSSD (int);

volatile int i;
static volatile int a;

void delay(volatile uint32_t);
void Start_State(void);
void Idle_State(void);

const uint8_t songarr[] = {
#include "voice.txt"
};

uint32_t song_N = sizeof(songarr);
uint32_t song_i = 0;
uint32_t temp_ = 0;

void TIM2_IRQ();

int main(void)
{
	uint32_t tmpa = RCC->CR;
	uint32_t tmpb = RCC->CFGR;
	uint32_t tmpc = RCC->PLLCFGR;


	RCC->PLLCFGR &= ~0xFF3F7F73;
	RCC->PLLCFGR |= 0x70000802;
	tmpc = RCC->PLLCFGR;
	tmpa = RCC->CR;
	RCC->CR |= 0x100;
	RCC->CR |= 0x1000000;
	RCC->CFGR |= 0x2;

    /* Enable GPIOA clock */
    RCC->IOPENR |= (1U << 0);

    /* Enable GPIOB clock */
    RCC->IOPENR |= (1U << 1);

	RCC->APBENR2 |= 0x800;
	RCC->APBENR1 |= 0x1;
	RCC->APBENR1 |= 0x200000;
	RCC->CCIPR |= 0x2000;

	// SSD DIGITS
    GPIOB->MODER &= ~(3U << 2 * 1); // PB1
    GPIOB->MODER |= (1U << 2 * 1);

    GPIOB->MODER &= ~(3U << 2 * 3); // PB3
    GPIOB->MODER |= (1U << 2 * 3);

    GPIOB->MODER &= ~(3U << 2 * 6); // PB6
    GPIOB->MODER |= (1U << 2 * 6);

    GPIOB->MODER &= ~(3U << 2 * 7); // PB7
    GPIOB->MODER |= (1U << 2 * 7);

    //Setup PA0, PA1, PA4, PA5, PA6, PA11, PA12 as OUTPUT pins FOR SSD
    GPIOA->MODER &= ~(3U << 2 * 0); // PA0
    GPIOA->MODER |= (1U << 2 * 0);

    GPIOA->MODER &= ~(3U << 2 * 1); // PA1
    GPIOA->MODER |= (1U << 2 * 1);

    GPIOA->MODER &= ~(3U << 2 * 4); // PA4
    GPIOA->MODER |= (1U << 2 * 4);

    GPIOA->MODER &= ~(3U << 2 * 5); // PA5
    GPIOA->MODER |= (1U << 2 * 5);

    GPIOA->MODER &= ~(3U << 2 * 6); // PA6
    GPIOA->MODER |= (1U << 2 * 6);

    GPIOA->MODER &= ~(3U << 2 * 11); // PA11
    GPIOA->MODER |= (1U << 2 * 11);

    GPIOA->MODER &= ~(3U << 2 * 12); // PA12
    GPIOA->MODER |= (1U << 2 * 12);

	GPIOA->MODER &= ~ (0x10000);
	GPIOA->AFR[2] |= (0x2);

	TIM1->CCMR1 |= (0x68);
	TIM1->CR1 |= (0x80);
	TIM1->BDTR |= (0x8C00);
	TIM1->CCER |= (0x1);
	TIM1->ARR = (0xFF);
	TIM1->CCR1 = (0x0);
	TIM1->EGR = (0x1);
	TIM1->CR1 |= (0x1);

	TIM2->CCMR1 |= (0x68);
	TIM2->CR1 |= (0x80);
	TIM2->ARR = (0xFFFF);
	TIM2->CCER |= (0x1);
	TIM2->DIER |= (0x2);
	TIM2->CCR1 = (0xF00);
	TIM2->EGR = (0x1);
	TIM2->CR1 |= (0x1);

	GPIOA->MODER &= ~(0x140000);
	GPIOA->AFR[2] |= (0x440);

	I2C1->TIMINGR |= (0x30420F13);
	I2C1->CR1 |= (0x57);
	I2C1->CR2 |= (0xA0);
	I2C1->CR2 |= (0x10000);
	I2C1->TXDR = (0xA);
	I2C1->CR2 |= (0x2000);


    NVIC_SetPriority(TIM2_IRQn, 0);
    NVIC_EnableIRQ(TIM2_IRQn);

    NVIC_SetPriority(I2C1_IRQn, 0);
    NVIC_EnableIRQ(I2C1_IRQn);


	while(1){

		Start_State();

	}

	tmpa = 0;
}

void TIM2_IRQ(){
	temp_ = TIM2->CNT;

	if(song_i >= song_N){
		song_i = 0;
	}
	TIM1->CCR1 = songarr[song_i];
	if((song_i%2) == 1){
		GPIOA->ODR |= (0x80);
	} else {
		GPIOA->ODR &= ~(0x80);
	}
	song_i++;
	TIM2->SR &= ~(0x3);
	TIM2->CNT = (0);
}

void I2C1_IRQ(){
	int tttt = 0;
	tttt = 0x4;
}

void delay(volatile uint32_t s) {
    for(; s > 0; s--);
}

void Digit_One (void){
    GPIOB->ODR |= (1U << 6); // PB6
}

void Digit_Two (void){
    GPIOB->ODR |= (1U << 1); // PB1
}

void Digit_Three (void){
    GPIOB->ODR |= (1U << 7); // PB7
}

void Digit_Four (void){
    GPIOB->ODR |= (1U << 3); // PB3
}

void ClearingSSD(void){

	GPIOA->ODR |= (1U << 0); //PA0
	GPIOA->ODR |= (1U << 1); //PA1
	GPIOA->ODR |= (1U << 4); //PA4
	GPIOA->ODR |= (1U << 5); //PA5
	GPIOA->ODR |= (1U << 6); //PA6
	GPIOA->ODR |= (1U << 11); //PA11
	GPIOA->ODR |= (1U << 12); //PA12
}

void SettingSSD (int a){

	ClearingSSD();

	switch(a)
	{

	case 0:

		GPIOA->ODR &= ~(1U << 0); //PA0
		GPIOA->ODR &= ~(1U << 1); //PA1
		GPIOA->ODR &= ~(1U << 4); //PA4
		GPIOA->ODR &= ~(1U << 5); //PA5
		GPIOA->ODR &= ~(1U << 11); //PA11
		GPIOA->ODR &= ~(1U << 12); //PA12

		break;

	case 1:

		GPIOA->ODR &= ~(1U << 1); //PA1
		GPIOA->ODR &= ~(1U << 4); //PA4

		break;

	case 2:

		GPIOA->ODR &= ~(1U << 0); //PA0
		GPIOA->ODR &= ~(1U << 1); //PA1
		GPIOA->ODR &= ~(1U << 5); //PA5
		GPIOA->ODR &= ~(1U << 6); //PA6
		GPIOA->ODR &= ~(1U << 12); //PA12

		break;

	case 3:

		GPIOA->ODR &= ~(1U << 0); //PA0
		GPIOA->ODR &= ~(1U << 1); //PA1
		GPIOA->ODR &= ~(1U << 4); //PA4
		GPIOA->ODR &= ~(1U << 5); //PA5
		GPIOA->ODR &= ~(1U << 6); //PA6

		break;

	case 4:

		GPIOA->ODR &= ~(1U << 1); //PA1 - B
		GPIOA->ODR &= ~(1U << 4); //PA4 - C
		GPIOA->ODR &= ~(1U << 6); //PA6 - G
		GPIOA->ODR &= ~(1U << 11); //PA11 - F

		break;

	case 5:

		GPIOA->ODR &= ~(1U << 0); //PA0 - A
		GPIOA->ODR &= ~(1U << 4); //PA4 - C
		GPIOA->ODR &= ~(1U << 5); //PA5 - D
		GPIOA->ODR &= ~(1U << 6); //PA6 - G
		GPIOA->ODR &= ~(1U << 11); //PA11 - F
		break;

	case 6:

		GPIOA->ODR &= ~(1U << 0); //PA0 - A
		GPIOA->ODR &= ~(1U << 4); //PA4 - C
		GPIOA->ODR &= ~(1U << 5); //PA5 - D
		GPIOA->ODR &= ~(1U << 6); //PA6 - G
		GPIOA->ODR &= ~(1U << 11); //PA11 - F
		GPIOA->ODR &= ~(1U << 12); //PA12 - E

		break;

	case 7:

		GPIOA->ODR &= ~(1U << 0); //PA0 - A
		GPIOA->ODR &= ~(1U << 1); //PA1 - B
		GPIOA->ODR &= ~(1U << 4); //PA4 - C

		break;

	case 8:

		GPIOA->ODR &= ~(1U << 0); //PA0 - A
		GPIOA->ODR &= ~(1U << 1); //PA1 - B
		GPIOA->ODR &= ~(1U << 4); //PA4 - C
		GPIOA->ODR &= ~(1U << 5); //PA5 - D
		GPIOA->ODR &= ~(1U << 6); //PA6 - G
		GPIOA->ODR &= ~(1U << 11); //PA11 - F
		GPIOA->ODR &= ~(1U << 12); //PA12 - E

		break;

	case 9:

		GPIOA->ODR &= ~(1U << 0); //PA0 - A
		GPIOA->ODR &= ~(1U << 1); //PA1 - B
		GPIOA->ODR &= ~(1U << 4); //PA4 - C
		GPIOA->ODR &= ~(1U << 5); //PA5 - D
		GPIOA->ODR &= ~(1U << 6); //PA6 - G
		GPIOA->ODR &= ~(1U << 11); //PA11 - F

		break;

	case 10:

		GPIOA->ODR &= ~(1U << 0); //PA0 - A
		GPIOA->ODR &= ~(1U << 4); //PA4 - C
		GPIOA->ODR &= ~(1U << 5); //PA5 - D
		GPIOA->ODR &= ~(1U << 6); //PA6 - G
		GPIOA->ODR &= ~(1U << 11); //PA11 - F
		GPIOA->ODR &= ~(1U << 12); //PA12 - E

		break;

	case 11:

		GPIOA->ODR &= ~(1U << 1); //PA1
		GPIOA->ODR &= ~(1U << 4); //PA4

		break;

	case 12:

		GPIOA->ODR &= ~(1U << 0); //PA0 - A
		GPIOA->ODR &= ~(1U << 1); //PA1 - B
		GPIOA->ODR &= ~(1U << 4); //PA4 - C
		GPIOA->ODR &= ~(1U << 5); //PA5 - D
		GPIOA->ODR &= ~(1U << 6); //PA6 - G
		GPIOA->ODR &= ~(1U << 11); //PA11 - F
		GPIOA->ODR &= ~(1U << 12); //PA12 - E

		break;

	case 13:

		GPIOA->ODR &= ~(1U << 0); //PA0 - A
		GPIOA->ODR &= ~(1U << 1); //PA1 - B
		GPIOA->ODR &= ~(1U << 4); //PA4 - C
		GPIOA->ODR &= ~(1U << 5); //PA5 - D
		GPIOA->ODR &= ~(1U << 6); //PA6 - G
		GPIOA->ODR &= ~(1U << 11); //PA11 - F

		break;
	}

}

void Start_State(void){

	for(i; i < 5500000 ; i++){

    Digit_One();
    SettingSSD(1);
    delay(low);
    GPIOB->ODR &= ~(1U << 6); // PB6


    Digit_Two();
    SettingSSD(8);
    delay(low);
    GPIOB->ODR &= ~(1U << 1); // PB1


    Digit_Three();
    SettingSSD(1);
    delay(low);
    GPIOB->ODR &= ~(1U << 7); // PB7


    Digit_Four();
    SettingSSD(1);
    delay(low);
    GPIOB->ODR &= ~(1U << 3); // PB3
	}
}


void Idle_State(void){

	for(i; i < 5500000 ; i++){

    Digit_One();
    SettingSSD(10);
    delay(low);
    GPIOB->ODR &= ~(1U << 6); // PB6


    Digit_Two();
    SettingSSD(11);
    delay(low);
    GPIOB->ODR &= ~(1U << 1); // PB1


    Digit_Three();
    SettingSSD(12);
    delay(low);
    GPIOB->ODR &= ~(1U << 7); // PB7


    Digit_Four();
    SettingSSD(13);
    delay(low);
    GPIOB->ODR &= ~(1U << 3); // PB3
	}
}
