
; Autor: Francisco Bley Ruthes

// Lê tabela .db criada
.macro le_tabela_R16

	push R31
	push R30
	push R16

		add R30, R16
		ldi R16, 0
		adc R31, R16
		lpm @0, Z

	pop R16
	pop R30
	pop R31

.endmacro

.macro escolheDigito 

	push R30
	push R31
	push R16

		ldi R31, HIGH(numeros*2)
		ldi R30, LOW(numeros*2)
		mov R16, @0
		le_tabela_R16 @1

	pop R16
	pop R31
	pop R30

.endmacro

// Realiza uma pausa de R16 us
.macro pausa_us
	ldi		R16,@0
	call	pausaR16us
.endmacro

configDisplay:
	
	sbi DDRD,0 // A
	sbi DDRD,1 // B
	sbi DDRD,2 // C
	sbi DDRD,3 // D
	sbi DDRD,4 // E
	sbi DDRD,5 // F
	sbi DDRD,6 // G
	sbi DDRD,7 // DP

// Seletores de Display
	sbi DDRB,0 // 1
	sbi DDRB,1 // 2
	sbi DDRB,2 // 3
	sbi DDRB,3 // 4

//	Botões
	cbi DDRB,4 //S1
	cbi DDRB,5 //S2

//	Interrupção de 10ms
	ldi R19, 5

//	Dígitos
	ldi R23, 0
	ldi R22, 0
	ldi R21, 0
	ldi R20, 0

	ret

configContador:

	ldi		R16,0b00000000 // Configuração normal
	out		TCCR0A,R16

	ldi		R16,131		// Inicia com valor 131 para que apos
	out		TCNT0,R16	// 125 contagens ele estoure (interrupção de 2ms)

	ldi		R16,1		// Habilita Interrupcao por overflow
	sts		TIMSK0,R16

	ret

comecaContagem:

	ldi		R16,0b00000100	// Prescaler f/256 = 62,5kHz
	out		TCCR0B,R16		// Inicia a contagem
	sei
	ret

pausaContagem:
	ldi		R16,0b00000000	// Prescaler f/256 = 62,5kHz
	out		TCCR0B,R16		// Inicia a contagem

	// Pausa de 4ms para a obtenção dos 60 FPS nos displays
pausa_4ms:
	push R17
	ldi R17, 1 // Repito 16 os 250us para chegar a 4ms, aproximadamente
pausa_loop:
	cpi R17, 0
	breq fim_pausa
	dec R17
	pausa_us 250
	rjmp pausa_loop
fim_pausa:
	pop R17
	ret

// Realiza uma pausa de R16 us
pausaR16us:
	push R16 // Gasta 2 ciclos 
		nop
pausaR16us_loop:
	cpi		R16,1 // Gasta 62,5ns (1 ciclo de clock)
	breq	pausaR16us_fim // Gasta 2 ciclos se verdadeiro e 1 se falso
	dec		R16
		nop		// Gasta 62,5ns (1 ciclo de clock)
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
	rjmp	pausaR16us_loop // Gasta 2 ciclos 
pausaR16us_fim:
	pop R16 // Gasta 2 ciclos 
	ret

// Tabela dos números de 0 a 9 para o display de 7 segmentos
numeros:
	.db 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
