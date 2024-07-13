;
; cronometro.asm
;
; Created: 30/05/2024 14:44:02
; Francisco Bley Ruthes

// Configuração do display

rjmp config

.org		0x20	// Local da Interrupção
	push	R16		// Salva R6 na pilha
		in		R16, SREG
		push	R16		// Salva SREG na pilha
			
			ldi		R16,131		// Valor de recarga para interrupções de 2ms
			out		TCNT0,R16	

			dec R19
			cpi R19, 0
			breq interrupcao10ms	
			rjmp	saida

		interrupcao10ms:

			ldi R19, 5		// Recarga R19 para 5 interrupções de 2ms

			inc		R23
			cpi		R23, 10
			brlo	saida
			ldi		R23, 0
			inc		R22
			cpi		R22, 10
			brlo	saida
			ldi		R22, 0
			inc		R21
			cpi		R21, 10
			brlo	saida
			ldi		R21, 0
			inc		R20
			cpi		R20, 6
			brlo	saida
			ldi		R20, 0

		saida:
		pop		R16
		out		SREG,R16
	pop		R16

	reti

.include "lib4.inc"

config:
	// Configura entradas e saídas referentes ao display e contadores
	call configDisplay
	// Configura parcialmente o contador (sem clock)
	call configContador
	
checaBotaoPausa:
	
	sbis PINB, 5
	jmp checaBotaoInicia
	call pausaContagem

checaBotaoInicia:

	sbis PINB, 4
	jmp loopDisplay
	call comecaContagem

loopDisplay:

	// Display 4
    sbi PORTB, 3
    escolheDigito R23, R17
    out PORTD, R17
	sbi PORTD, 7
    call pausa_4ms

	cbi	PORTB, 3
	cbi	PORTD, 7

	// Display 3
    sbi PORTB, 2
    escolheDigito R22, R17
    out PORTD, R17
    call pausa_4ms

	cbi	PORTB, 2

	// Display 2
    sbi PORTB, 1
    escolheDigito R21, R17
    out PORTD, R17
	sbi PORTD, 7
    call pausa_4ms

	cbi	PORTB, 1
	cbi	PORTD, 7

    // Display 1
	sbi PORTB, 0
	escolheDigito R20, R17
    out PORTD, R17
    call pausa_4ms

	cbi	PORTB, 0

    jmp checaBotaoPausa