/*
 * IncFile1.inc
 *
 *  Created: 15/05/2024 18:50:59
 *   Author: Francisco Bley Ruthes
 */ 

 // Guarda valores nos registradores
.macro store
	push R16
	ldi R16,@1
	sts @0,R16
	pop R16
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

// Configuração dos registradores ADC
config_ADC:
	store	ADMUX,	0xE3
	store	ADCSRA, 0x87
	ret
// Ativação do conversor ADC
ativar_ADC:
	store	ADCSRA, 0b11000111	// Set bit 6
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