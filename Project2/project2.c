/*
 * main.c
 *
 * author: Emre Boran Karabey
 *
 *	Project 2 // Calculator
 *
 */

#include "stm32g0xx.h"
#include <stdio.h>
#include <math.h>

#define High    5500000
#define Low     150

static volatile  int a;
static volatile int b;
static volatile  int m;
static volatile int n;
static volatile int i = 0;

void delay(volatile uint32_t);

void ClearSSD (void);
int SetSSD (int);

void ClearKeyRows (void);
void SetKeyRows (void);

void loop(void);
void IdleStateHandler(void);

void ClearAllDigits(void);

void ClearDigitOne(void);
void ClearDigitTwo(void);
void ClearDigitThree(void);
void ClearDigitFour(void);

void SetDigitOne (void);
void SetDigitTwo (void);
void SetDigitThree (void);
void SetDigitFour (void);

int FirstStateHandler(void);
int SecondStateHandler(void);
int ResultStateHandler(void);

void EXTI0_1_IRQHandler(){

	 ClearAllDigits();
	 ClearKeyRows ();
	 SetDigitFour();

	 GPIOA -> ODR ^= (1U << 8); 		//PA8

	 if((GPIOB -> IDR >> 0) & 1){

		 SetSSD(14); 					//SHARP
	 }

	 GPIOA -> ODR ^= (1U << 8); 		//PA8

/////////////////////////////////////////////

	 GPIOB -> ODR ^= (1U << 9); 		//PB9

	 if((GPIOB -> IDR >> 0) & 1){

		 SetSSD(9);
	 }

	 GPIOB -> ODR ^= (1U << 9); 		//PB9

//////////////////////////////////////////////

	 GPIOB -> ODR ^= (1U << 5); 		//PB5

	 if((GPIOB -> IDR >> 0) & 1){

		 SetSSD(6);
	 }

	 GPIOB -> ODR ^= (1U << 5); 		//PB5

//////////////////////////////////////////////

	 GPIOB -> ODR ^= (1U << 4); 		//PB4

	 if((GPIOB -> IDR >> 0) & 1){

	 SetSSD(3);
	 }

	 GPIOB -> ODR ^= (1U << 4); 		//PB4

	 EXTI -> RPR1 |= (1U << 0);

	 SetKeyRows();
}


void EXTI2_3_IRQHandler(){  			//PB2 INTERRUPT

	 ClearAllDigits();
	 ClearKeyRows();
	 SetDigitFour();

	 GPIOA -> ODR ^= (1U << 8); 		//PA8

	 if((GPIOB -> IDR >> 2) & 1){

		 SetSSD(0); 					//SHARP
	 }

	 GPIOA -> ODR ^= (1U << 8); 		//PA8

/////////////////////////////////////////////

	 GPIOB -> ODR ^= (1U << 9); 		//PB9

	 if((GPIOB -> IDR >> 2) & 1){

		 SetSSD(8);
	 }

	 GPIOB -> ODR ^= (1U << 9); 		//PB9

//////////////////////////////////////////////

	 GPIOB -> ODR ^= (1U << 5); 		//PB5

	 if((GPIOB -> IDR >> 2) & 1){

		 SetSSD(5);
	 }

	 GPIOB -> ODR ^= (1U << 5); 		//PB5

//////////////////////////////////////////////

	 GPIOB -> ODR ^= (1U << 4); 		//PB4

	 if((GPIOB -> IDR >> 2) & 1){

	 SetSSD(2);
	 }

	 GPIOB -> ODR ^= (1U << 4); 		//PB4

	 EXTI -> RPR1 |= (1U << 0);

	 SetKeyRows();
}


void EXTI4_15_IRQHandler(){
										//PB8
	 ClearAllDigits();
	 ClearKeyRows();
	 SetDigitFour();


	 GPIOB -> ODR ^= (1U << 9); 		//PB9

	 if((GPIOB -> IDR >> 8) & 1){

		 SetSSD(7);
	 }

	 GPIOB -> ODR ^= (1U << 9); 		//PB9

//////////////////////////////////////////////

	 GPIOB -> ODR ^= (1U << 5); 		//PB5

	 if((GPIOB -> IDR >> 8) & 1){

		 SetSSD(4);
	 }

	 GPIOB -> ODR ^= (1U << 5); 		//PB5

//////////////////////////////////////////////

	 GPIOB -> ODR ^= (1U << 4); 		//PB4

	 if((GPIOB -> IDR >> 8) & 1){

	 SetSSD(1);
	 }

	 GPIOB -> ODR ^= (1U << 4); 		//PB4

	 	 	 	 	 	 	 	 	 	//PA9
	 ClearKeyRows();
	 SetDigitFour();

	 GPIOA -> ODR ^= (1U << 8); 		//PA8

	 if((GPIOA -> IDR >> 9) & 1){

		 SetSSD(13); 					//SHARP
	 }

	 GPIOA -> ODR ^= (1U << 8); 		//PA8

/////////////////////////////////////////////

	 GPIOB -> ODR ^= (1U << 9); 		//PB9

	 if((GPIOA -> IDR >> 9) & 1){

		 SetSSD(12);
	 }

	 GPIOB -> ODR ^= (1U << 9); 		//PB9

//////////////////////////////////////////////

	 GPIOB -> ODR ^= (1U << 5); 		//PB5

	 if((GPIOA -> IDR >> 9) & 1){

		 SetSSD(11);
	 }

	 GPIOB -> ODR ^= (1U << 5); 		//PB5

//////////////////////////////////////////////

	 GPIOB -> ODR ^= (1U << 4); 		//PB4

	 if((GPIOA -> IDR >> 9) & 1){

	 SetSSD(10);
	 }

	 GPIOB -> ODR ^= (1U << 4); 		//PB4

	 EXTI -> RPR1 |= (1U << 0);

	 SetKeyRows();
}

typedef enum{
	IdleState,
	FirstNumberState,
	SecondNumberState,
	ResultState
} state_;

typedef enum{
	IdleStateEvent,
	FirstNumberStateEvent,
	SecondNumberStateEvent,
	ResultStateEvent
} event_;

state_ state = IdleState;
state_ nextstate;

event_ event;

void Stateloop(void){

	while(1) {

		if(state == IdleState){
	//IdleStateHandler();
			if(event == IdleStateEvent){
				 nextstate = FirstStateHandler();
				 event = FirstNumberStateEvent;
				 delay(Low);
			}
		}

		else if (state  == m) {
			if(event == FirstNumberStateEvent){
				 nextstate = SecondStateHandler();
				 event = SecondNumberStateEvent;
				 delay(Low);
			}
		}

		else if (state  == n) {
			if(event == SecondNumberStateEvent){
				 nextstate = ResultStateHandler();
				 event = ResultStateEvent;
				 delay(High);
			}
		}

		else if (state  == a) {
			if(event == ResultStateEvent){
				 IdleStateHandler();
			}
		}
		state = nextstate;
	}
}


int main(void) {

    /* Enable GPIOA clock */
    RCC->IOPENR |= (1U << 0);

    /* Enable GPIOB clock */
    RCC->IOPENR |= (1U << 1);

    //Setup PA8, PB4, PB5, PB9 as OUTPUT pins FOR ROWS OF KEYPAD

    GPIOA->MODER &= ~(3U << 2 * 8); 		// PA8
    GPIOA->MODER |= (1U << 2 * 8);

    GPIOB->MODER &= ~(3U << 2 * 4); 		// PB4
    GPIOB->MODER |= (1U << 2 * 4);

    GPIOB->MODER &= ~(3U << 2 * 5); 		// PB5
    GPIOB->MODER |= (1U << 2 * 5);

    GPIOB->MODER &= ~(3U << 2 * 9); 		// PB9
    GPIOB->MODER |= (1U << 2 * 9);

    //Setup PB1, 3, 6, 7 OUTPUT FOR DIGITS of SSD

    GPIOB->MODER &= ~(3U << 2 * 1); 		// PB1
    GPIOB->MODER |= (1U << 2 * 1);

    GPIOB->MODER &= ~(3U << 2 * 3);		    // PB3
    GPIOB->MODER |= (1U << 2 * 3);

    GPIOB->MODER &= ~(3U << 2 * 6); 		// PB6
    GPIOB->MODER |= (1U << 2 * 6);

    GPIOB->MODER &= ~(3U << 2 * 7); 		// PB7
    GPIOB->MODER |= (1U << 2 * 7);

    //Setup PA9, PB0, PB2, PB8 as INPUT pins FOR COLUMNS OF KEYPAD

    GPIOA->MODER &= ~(3U << 2 * 9);			// PA9
    GPIOA->PUPDR |= (2U << 2 * 9);  		//Pull-down prevention

    GPIOB->MODER &= ~(3U << 2 * 0); 		// PB0
    GPIOB->PUPDR |= (2U << 2 * 0);  		//Pull-down prevention

    GPIOB->MODER &= ~(3U << 2 * 2); 		// PB2
    GPIOB->PUPDR |= (2U << 2 * 2);  		//Pull-down prevention

    GPIOB->MODER &= ~(3U << 2 * 8); 		// PB8
    GPIOB->PUPDR |= (2U << 2 * 8);  		//Pull-down prevention

    //EXTI CR CONFIG FOR INPUT PA9, PB0, PB2, PB8

    EXTI->EXTICR[2] |= (0U << 8 * 1); 		// PA9
    EXTI->EXTICR[0] |= (1U << 8 * 0); 		// PB0
    EXTI->EXTICR[0] |= (1U << 8 * 2); 		// PB2
    EXTI->EXTICR[2] |= (1U << 8 * 0); 		// PB8

    //RISING EDGE CONFIG FOR INPUT PA9, PB0, PB2, PB8

    EXTI->RTSR1 |= (1U << 9); 		// PA9
    EXTI->RTSR1 |= (1U << 0); 		// PB0
    EXTI->RTSR1 |= (1U << 2); 		// PB2
    EXTI->RTSR1 |= (1U << 8); 		// PB8

    //INTERRUPT MASK CONFIG FOR INPUT PA9, PB0, PB2, PB8

    EXTI->IMR1 |= (1U << 9); 		// PA9
    EXTI->IMR1 |= (1U << 0); 		// PB0
    EXTI->IMR1 |= (1U << 2); 		// PB2
    EXTI->IMR1 |= (1U << 8); 		// PB8

    //NVIC

    NVIC_SetPriority(EXTI0_1_IRQn, 0);
    NVIC_EnableIRQ(EXTI0_1_IRQn);

    NVIC_SetPriority(EXTI2_3_IRQn, 0);
    NVIC_EnableIRQ(EXTI2_3_IRQn);

    NVIC_SetPriority(EXTI4_15_IRQn, 0);
    NVIC_EnableIRQ(EXTI4_15_IRQn);

    //SET PA0, PA1, PA4, PA5, PA6, PA11, PA12 as OUTPUT pins FOR SSD

    GPIOA->MODER &= ~(3U << 2 * 0); 		// PA0
    GPIOA->MODER |= (1U << 2 * 0);

    GPIOA->MODER &= ~(3U << 2 * 1); 		// PA1
    GPIOA->MODER |= (1U << 2 * 1);

    GPIOA->MODER &= ~(3U << 2 * 4); 		// PA4
    GPIOA->MODER |= (1U << 2 * 4);

    GPIOA->MODER &= ~(3U << 2 * 5); 		// PA5
    GPIOA->MODER |= (1U << 2 * 5);

    GPIOA->MODER &= ~(3U << 2 * 6); 		// PA6
    GPIOA->MODER |= (1U << 2 * 6);

    GPIOA->MODER &= ~(3U << 2 * 11); 		// PA11
    GPIOA->MODER |= (1U << 2 * 11);

    GPIOA->MODER &= ~(3U << 2 * 12); 		// PA12
    GPIOA->MODER |= (1U << 2 * 12);

    //Set
    GPIOA->ODR |= (1U << 8); 		// PA8
    GPIOB->ODR |= (1U << 9); 		// PB9
    GPIOB->ODR |= (1U << 5); 		// PB5
    GPIOB->ODR |= (1U << 4); 		// PB4

    ClearSSD();
    //IdleStateHandler();

    while(1) {
    }

    return 0;
}


void ClearKeyRows (void){

    GPIOA->ODR &= ~(1U << 8);		// PA8
    GPIOB->ODR &= ~(1U << 9); 		// PB9
    GPIOB->ODR &= ~(1U << 5); 		// PB5
    GPIOB->ODR &= ~(1U << 4); 		// PB4
}

void SetKeyRows (void){

    GPIOA->ODR |= (1U << 8); 		// PA8
    GPIOB->ODR |= (1U << 9); 		// PB9
    GPIOB->ODR |= (1U << 5); 		// PB5
    GPIOB->ODR |= (1U << 4); 		// PB4
}

void SetDigitOne (void){
    GPIOB->ODR |= (1U << 6); 		// PB6
}

void SetDigitTwo (void){
    GPIOB->ODR |= (1U << 1); 		// PB1
}

void SetDigitThree (void){
    GPIOB->ODR |= (1U << 7); 		// PB7
}

void SetDigitFour (void){
    GPIOB->ODR |= (1U << 3); 		// PB3
}

void ClearSSD(void){

	GPIOA->ODR |= (1U << 0); 		//PA0
	GPIOA->ODR |= (1U << 1); 		//PA1
	GPIOA->ODR |= (1U << 4); 		//PA4
	GPIOA->ODR |= (1U << 5); 		//PA5
	GPIOA->ODR |= (1U << 6); 		//PA6
	GPIOA->ODR |= (1U << 11); 		//PA11
	GPIOA->ODR |= (1U << 12); 		//PA12
}

int SetSSD (int b){

	ClearSSD();

	switch(b)
	{

	case 0:
		//Lights up 0
		a = 0;
		GPIOA->ODR &= ~(1U << 0); 		//PA0
		GPIOA->ODR &= ~(1U << 1); 		//PA1
		GPIOA->ODR &= ~(1U << 4); 		//PA4
		GPIOA->ODR &= ~(1U << 5); 		//PA5
		GPIOA->ODR &= ~(1U << 11); 		//PA11
		GPIOA->ODR &= ~(1U << 12); 		//PA12
		return 0;
		break;
	case 1:
		//Lights up 1
		a = 1;
		GPIOA->ODR &= ~(1U << 1); 		//PA1
		GPIOA->ODR &= ~(1U << 4); 		//PA4
		return 1;
		break;

	case 2:
		//Lights up 2
		a = 2;
		GPIOA->ODR &= ~(1U << 0); 		//PA0
		GPIOA->ODR &= ~(1U << 1); 		//PA1
		GPIOA->ODR &= ~(1U << 5); 		//PA5
		GPIOA->ODR &= ~(1U << 6); 		//PA6
		GPIOA->ODR &= ~(1U << 12); 		//PA12
		return 2;
		break;

	case 3:
		//Lights up 3
		a = 3;
		GPIOA->ODR &= ~(1U << 0); 		//PA0
		GPIOA->ODR &= ~(1U << 1); 		//PA1
		GPIOA->ODR &= ~(1U << 4); 		//PA4
		GPIOA->ODR &= ~(1U << 5); 		//PA5
		GPIOA->ODR &= ~(1U << 6); 		//PA6
		return 3;
		break;

	case 4:
		//Lights up 4
		a = 4;
		GPIOA->ODR &= ~(1U << 1); 		//PA1 - B
		GPIOA->ODR &= ~(1U << 4); 		//PA4 - C
		GPIOA->ODR &= ~(1U << 6); 		//PA6 - G
		GPIOA->ODR &= ~(1U << 11); 		//PA11 - F
		return 4;
		break;

	case 5:
		//Lights up 5
		a = 5;
		GPIOA->ODR &= ~(1U << 0); 		//PA0 - A
		GPIOA->ODR &= ~(1U << 4); 		//PA4 - C
		GPIOA->ODR &= ~(1U << 5); 		//PA5 - D
		GPIOA->ODR &= ~(1U << 6); 		//PA6 - G
		GPIOA->ODR &= ~(1U << 11); 		//PA11 - F
		return 5;
		break;

	case 6:
		//Lights up 6
		a = 6;
		GPIOA->ODR &= ~(1U << 0); 		//PA0 - A
		GPIOA->ODR &= ~(1U << 4); 		//PA4 - C
		GPIOA->ODR &= ~(1U << 5); 		//PA5 - D
		GPIOA->ODR &= ~(1U << 6); 		//PA6 - G
		GPIOA->ODR &= ~(1U << 11);		//PA11 - F
		GPIOA->ODR &= ~(1U << 12); 		//PA12 - E
		return 6;
		break;

	case 7:
		//Lights up 7
		a = 7;
		GPIOA->ODR &= ~(1U << 0); 		//PA0 - A
		GPIOA->ODR &= ~(1U << 1); 		//PA1 - B
		GPIOA->ODR &= ~(1U << 4); 		//PA4 - C
		return 7;
		break;

	case 8:
		//Lights up 8
		a = 8;
		GPIOA->ODR &= ~(1U << 0); 		//PA0 - A
		GPIOA->ODR &= ~(1U << 1); 		//PA1 - B
		GPIOA->ODR &= ~(1U << 4); 		//PA4 - C
		GPIOA->ODR &= ~(1U << 5); 		//PA5 - D
		GPIOA->ODR &= ~(1U << 6); 		//PA6 - G
		GPIOA->ODR &= ~(1U << 11); 		//PA11 - F
		GPIOA->ODR &= ~(1U << 12); 		//PA12 - E
		return 8;
		break;

	case 9:
		//Lights up 9
		a = 9;
		GPIOA->ODR &= ~(1U << 0); 		//PA0 - A
		GPIOA->ODR &= ~(1U << 1); 		//PA1 - B
		GPIOA->ODR &= ~(1U << 4); 		//PA4 - C
		GPIOA->ODR &= ~(1U << 5); 		//PA5 - D
		GPIOA->ODR &= ~(1U << 6); 		//PA6 - G
		GPIOA->ODR &= ~(1U << 11); 		//PA11 - F
		return 9;
		break;

	case 10:
		//Lights up A
		a = 10;
		GPIOA->ODR &= ~(1U << 0); 		//PA0 - A
		GPIOA->ODR &= ~(1U << 1); 		//PA1 - B
		GPIOA->ODR &= ~(1U << 4); 		//PA4 - C
		GPIOA->ODR &= ~(1U << 5); 		//PA5 - D
		GPIOA->ODR &= ~(1U << 6); 		//PA6 - G
		GPIOA->ODR &= ~(1U << 12);		//PA12 - E
		return 10;
		break;

	case 11:
		//Lights up B
		a = 11;
		GPIOA->ODR &= ~(1U << 4); 		//PA4 - C
		GPIOA->ODR &= ~(1U << 5); 		//PA5 - D
		GPIOA->ODR &= ~(1U << 6); 		//PA6 - G
		GPIOA->ODR &= ~(1U << 11); 		//PA11 - F
		GPIOA->ODR &= ~(1U << 12); 		//PA12 - E
		return 11;
		break;

	case 12:
		//Lights up C
		a = 12;
		GPIOA->ODR &= ~(1U << 0); 		//PA0 - A
		GPIOA->ODR &= ~(1U << 5); 		//PA5 - D
		GPIOA->ODR &= ~(1U << 11); 		//PA11 - F
		GPIOA->ODR &= ~(1U << 12); 		//PA12 - E
		return 12;
		break;

	case 13:
		//Lights up D
		a = 13;
		GPIOA->ODR &= ~(1U << 1); 		//PA1 - B
		GPIOA->ODR &= ~(1U << 4); 		//PA4 - C
		GPIOA->ODR &= ~(1U << 5); 		//PA5 - D
		GPIOA->ODR &= ~(1U << 6); 		//PA6 - G
		GPIOA->ODR &= ~(1U << 12);		//PA12 - E
		return 13;
		break;

	case 14:
		//Prints - (pressed #)
		a = 14;
		GPIOA->ODR &= ~(1U << 6); //PA6 -
		return 14;
		break;

	}
}

void delay(volatile uint32_t s) {
    for(; s > 0; s--);
}

void ClearDigitOne(void){
    GPIOB->ODR &= ~(1U << 6); 		// PB6
}
void ClearDigitTwo(void){
    GPIOB->ODR &= ~(1U << 1); 		// PB1
}

void ClearDigitThree(void){
    GPIOB->ODR &= ~(1U << 7); 		// PB7
}
void ClearDigitFour(void){
    GPIOB->ODR &= ~(1U << 3); 		// PB3
}

void ClearAllDigits(void){
    GPIOB->ODR &= ~(1U << 6); 		// PB6
    GPIOB->ODR &= ~(1U << 1); 		// PB1
    GPIOB->ODR &= ~(1U << 7); 		// PB7
    GPIOB->ODR &= ~(1U << 3); 		// PB3
}

void IdleStateHandler(void){

	for(i; i < 5500000 ; i++){

    SetDigitOne();
    SetSSD(1);
    delay(Low);
    GPIOB->ODR &= ~(1U << 6); 		// PB6


    SetDigitTwo();
    SetSSD(8);
    delay(Low);
    GPIOB->ODR &= ~(1U << 7); 		// PB7


    SetDigitThree();
    SetSSD(1);
    delay(Low);
    GPIOB->ODR &= ~(1U << 1); 		// PB1


    SetDigitFour();
    SetSSD(1);
    delay(Low);
    GPIOB->ODR &= ~(1U << 3); 		// PB3
	}
}

int FirstStateHandler(void){

	EXTI0_1_IRQHandler();
	EXTI2_3_IRQHandler();
	EXTI4_15_IRQHandler();

	m = a;
	return m;
}

int SecondStateHandler(void){

	EXTI0_1_IRQHandler();
	EXTI2_3_IRQHandler();
	EXTI4_15_IRQHandler();

	n = a;
	return n;
}

int ResultStateHandler(void){

	if (b == 10){
		a = m + n;
		SetSSD(a);
		return a;
	}
	else if (b == 11){
		a  = m - n;
		SetSSD(a);
		return a;
	}
	else if(b == 12){
		a = m / n;
		SetSSD(a);
		return a;
	}
	else if (b == 13){
		a = m * n;
		SetSSD(a);
		return a;
	}
}
