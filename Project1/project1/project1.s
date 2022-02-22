/*
 * asm.s
 *
 * author: Emre Boran Karabey
 *
 * description: Added the necessary stuff for turning on the green LED on the 
 *   G031K8 Nucleo board. Mostly for teaching.
 */

.syntax unified
.cpu cortex-m0plus
.fpu softvfp
.thumb


/* make linker see this */
.global Reset_Handler

/* get these from linker script */
.word _sdata
.word _edata
.word _sbss
.word _ebss


/* define peripheral addresses from RM0444 page 57, Tables 3-4 */
.equ RCC_BASE,         (0x40021000)          // RCC base address
.equ RCC_IOPENR,       (RCC_BASE   + (0x34)) // RCC IOPENR register offset

.equ GPIOA_BASE,       (0x50000000)          // GPIOA base address
.equ GPIOA_MODER,      (GPIOA_BASE + (0x00)) // GPIOA MODER register offset
.equ GPIOA_ODR,        (GPIOA_BASE + (0x14)) // GPIOA ODR register offset
.equ GPIOA_IDR,		   (GPIOA_BASE + (0x10)) // GPIOA IDR register offset

.equ GPIOB_BASE,       (0x50000400)          // GPIOB base address
.equ GPIOB_MODER,      (GPIOB_BASE + (0x00)) // GPIOB MODER register offset
.equ GPIOB_ODR,        (GPIOB_BASE + (0x14)) // GPIOB ODR register offset

.equ delay,       (1000)
/* vector table, +1 thumb mode */
.section .vectors
vector_table:
	.word _estack             /*     Stack pointer */
	.word Reset_Handler +1    /*     Reset handler */
	.word Default_Handler +1  /*       NMI handler */
	.word Default_Handler +1  /* HardFault handler */
	/* add rest of them here if needed */


/* reset handler */
.section .text
Reset_Handler:
	/* set stack pointer */
	ldr r0, =_estack
	mov sp, r0

	/* initialize data and bss 
	 * not necessary for rom only code 
	 * */
	bl init_data
	/* call main */
	bl main
	/* trap if returned */
	b .


/* initialize data and bss sections */
.section .text
init_data:

	/* copy rom to ram */
	ldr r0, =_sdata
	ldr r1, =_edata
	ldr r2, =_sidata
	movs r3, #0
	b LoopCopyDataInit

	CopyDataInit:
		ldr r4, [r2, r3]
		str r4, [r0, r3]
		adds r3, r3, #4

	LoopCopyDataInit:
		adds r4, r0, r3
		cmp r4, r1
		bcc CopyDataInit

	/* zero bss */
	ldr r2, =_sbss
	ldr r4, =_ebss
	movs r3, #0
	b LoopFillZerobss

	FillZerobss:
		str  r3, [r2]
		adds r2, r2, #4

	LoopFillZerobss:
		cmp r2, r4
		bcc FillZerobss

	bx lr


/* default handler */
.section .text
Default_Handler:
	b Default_Handler


/* main function */
.section .text
main:
	/* enable GPIOC clock, bit2 on IOPENR */
	ldr r2, =RCC_IOPENR
	ldr r3, [r2]
	movs r4, 0x3
	orrs r3, r3, r4
	str r3, [r2]

// PB PORT OUTPUT MODE - PB0 PB1 PB2 PB3 PB4 PB5 PB6
	ldr r2, =GPIOB_MODER
	ldr r3, [r2]

	ldr r4, =0x1555
	ands r3, r3, r4
	str r3, [r2]

// PA PORT OUTPUT MODE - PA9 PA10 PA11 PA12
	ldr r2, =GPIOA_MODER
	ldr r3, [r2]

	ldr r4, =0xFF
	lsls r4, r4, #18
	bics r3, r3, r4
	ldr r4, =0x55
	lsls r4, r4, #18
	orrs r3, r3, r4
	str r3, [r2]

// BUTTON INPUT MODE - PA8

	ldr r2, =GPIOA_MODER
	ldr r3, [r2]
	ldr r4, =0x3
	lsls r4, r4, #16
	bics r3, r3, r4
	str r3, [r2]

// LED OUTPUT MODE - PA5

	ldr r2, =GPIOA_MODER
	ldr r3, [r2]
	ldr r4, =0x3
	lsls r4, r4, #10
	bics r3, r3, r4
	movs r1, #0x1
	lsls r1, r1, #10
	orrs r3, r3, r1
	str r3, [r2]


// DIGIT FUNCTION AS WRITING THE IDLE STATE

digits:

//  FOR THE FIRST DIGIT as 2

	ldr r2, =GPIOA_ODR
	ldr r3, [r2]
	movs r4, 0x1
	lsls r4, r4, #9				// shift 9 bits for PA9
	orrs r3, r3, r4				// orr to make 1
	str r3, [r2]

	ldr r2, =GPIOB_ODR
	ldr r3, [r2]
	movs r4, 0x24				// to have 2 on first segment's display
	ands r3, r3, r4				// and with it
	str r3, [r2]

    ldr r1, =delay

delay1:

	subs r1, r1, #1
	bne delay1

	ldr r2, =GPIOA_ODR
	ldr r3, [r2]
	movs r4, 0x1
	lsls r4, r4, #9
	bics r3, r3, r4
	str r3, [r2]

    ldr r2, =GPIOB_ODR
	ldr r3, [r2]
	movs r4, 0x7F				// make them 1 to unlight
	orrs r3, r3, r4
	str r3, [r2]

//  FOR THE SECOND DIGIT as 0

	ldr r2, =GPIOA_ODR
	ldr r3, [r2]
	movs r4, 0x1
	lsls r4, r4, #10			// shift 10 bits for PA10
	orrs r3, r3, r4				// orr to make 1
	str r3, [r2]

	ldr r2, =GPIOB_ODR
	ldr r3, [r2]
	movs r4, 0x40				// to have 0 on the second segment's display
	ands r3, r3, r4				// and with it
	str r3, [r2]

    ldr r1, =delay

delay2:

	subs r1, r1, #1
	bne delay2

	ldr r2, =GPIOA_ODR
	ldr r3, [r2]
	movs r4, 0x1
	lsls r4, r4, #10
	bics r3, r3, r4
	str r3, [r2]

    ldr r2, =GPIOB_ODR
	ldr r3, [r2]
	movs r4, 0x7F				// make them 1 to unlight
	orrs r3, r3, r4
	str r3, [r2]

// FOR THE THIRD DIGIT as 1

   	ldr r2, =GPIOA_ODR
	ldr r3, [r2]
	movs r4, 0x1
	lsls r4, r4, #11			// shift 11 bits for PA11
	orrs r3, r3, r4				// orr to make 1
	str r3, [r2]

	ldr r2, =GPIOB_ODR
	ldr r3, [r2]
	movs r4, 0x79				// to have 1 on third segment's display
	ands r3, r3, r4				// and with it
	str r3, [r2]

    ldr r1, =delay

delay3:

	subs r1, r1, #1
	bne delay3

	ldr r2, =GPIOA_ODR
	ldr r3, [r2]
	movs r4, 0x1
	lsls r4, r4, #11
	bics r3, r3, r4
	str r3, [r2]

    ldr r2, =GPIOB_ODR
	ldr r3, [r2]
	movs r4, 0x7F				// make them 1 to unlight
	orrs r3, r3, r4
	str r3, [r2]

// FOR THE FOURTH DIGIT as 1

	ldr r2, =GPIOA_ODR
	ldr r3, [r2]
	movs r4, 0x1
	lsls r4, r4, #12			// shift 12 bits for PA12
	orrs r3, r3, r4				// orr to make 1
	str r3, [r2]

	ldr r2, =GPIOB_ODR
	ldr r3, [r2]
	movs r4, 0x79				// to have 1 on third segment's display
	ands r3, r3, r4				// and with it
	str r3, [r2]

    ldr r1, =delay

delay4:

	subs r1, r1, #1
	bne delay4

	ldr r2, =GPIOA_ODR
	ldr r3, [r2]
	movs r4, 0x1
	lsls r4, r4, #12
	bics r3, r3, r4
	str r3, [r2]

    ldr r2, =GPIOB_ODR
	ldr r3, [r2]
	movs r4, 0x7F				// make them 1 to unlight
	orrs r3, r3, r4
	str r3, [r2]

// BUTTON FUNCTION
button:

    ldr r2, =GPIOA_IDR
    ldr r3, [r2]
    lsrs r3, r3, #8
    movs r4, #0x1
    ands r3, r3, r4

	cmp r3, #0x1
	beq ledon
	bne ledoff


	b digits

// TURN ON LED
ledon:

    ldr r2, =GPIOA_ODR
    ldr r3, [r2]
    movs r4, 0x10
    orrs r3, r3, r4
    str r3, [r2]

    b digits

// TURN OFF LED
ledoff:

    ldr r2, =GPIOA_ODR
    ldr r3, [r2]
    movs r4, 0x10
    mvns r4, r4
    adds r3, r3, r4
    str r3, [r2]

    b digits


	b .

	nop
