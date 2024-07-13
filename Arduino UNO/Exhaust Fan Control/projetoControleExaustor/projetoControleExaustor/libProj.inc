/*
 * IncFile1.inc
 *
 *  Created: 15/05/2024 18:50:59
 *   Author: Francisco Bley Ruthes
 */ 

 // Guarda valores nos registradores USART
.macro store
	push R16
	ldi R16,@1
	sts @0,R16
	pop R16
.endmacro

.macro		tx_register
	push	R17
	mov		R17, @0
	call	tx_R17
	pop		R17
.endm

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


// Função que caminha na string transmitindo caracter por caracter a cada loop
tx_string:
	push	R16
	push	R17
	ldi		R16,0

	tx_string_loop:

		le_tabela_R16 R17

		cpi		R17, 0
		breq	tx_string_fim
		call	tx_R17			// Transmissão pelo R17
		inc		R16

		jmp		tx_string_loop
		 
	tx_string_fim:
		pop		R17
		pop		R16
		ret

.macro txs 
	push R30
	push R31
		ldi R31, HIGH(@0*2)
		ldi R30, LOW(@0*2)
		call tx_string
	pop R31
	pop R30
.endmacro


// Configuração dos registradores USART
config_serial:
	// BAUDRATE: 38400bps
	store	UBRR0L,0x33
	store	UBRR0H,0x00
	//Configurações dasportas A, B e C:
	store	UCSR0A,0x22
	store	UCSR0B,0x18
	store	UCSR0C,0x0E
	ret

config_ADC:
	store	ADMUX,	0xE3
	store	ADCSRA, 0x87
	ret

// Transmissão de R17
tx_R17:
	push	R16
tx_R17_volta:
	lds		R16, UCSR0A		// Se o buffer está limpo a execução continua
	sbrs	R16, 5			
	rjmp	tx_R17_volta
	sts		UDR0, R17		
	pop		R16
	ret

// Confere se o UCSR0A recebeu o sinal (input)

rx_R17:
	push	R16
rx_R17_volta:
	lds		R16, UCSR0A
	sbrs	R16, 7		// Se o bit 7 do UCSR0A estiver set = "Receive Complete"
	rjmp	rx_R17_volta
	lds		R17, UDR0   // R17 recebe o que o usuário digitou
	pop		R16
	ret
