;
; proj2.asm
;
; Craido: 21/04/2024 12:19:01
; Autor: Francisco Bley Ruthes
;

//jmp inputUsuario
jmp config
.include "tx_serial.inc"	//Biblioteca
 
config:
	call config_serial		//Configura os registradores USART
	nop
	nop
	nop

inicioDoGame:
	txs mensagemInicio		//Transmite a mensagem de início caracter por caracter
	push	R16

esperaInput:
	ldi		R18, 0			
ciclo0a9_loop:
	lds		R16, UCSR0A
	sbrs	R16, 7	
	jmp		ciclo0a9
	lds		R16, UDR0		
	cpi		R16, '+'		//Verifica se o input é igual ao caracter '+'
	breq	digitouOmais
	//breq	impR18			
ciclo0a9:
	inc		R18				
	cpi		R18, 10
	breq	esperaInput
	jmp		ciclo0a9_loop
	
impR18:
	tx_register R18
	jmp esperaInput

digitouOmais:
	pop		R16
	txs mensagemJogoComecou
	push	R17

inputUsuario:
	call	rx_R17			//Recebe input do usuário
	//call tx_R17
	cpi		R17, '0'
	brlo	inputUsuario
	cpi		R17, '9'
	brlo	comparacaoComR18
	breq	comparacaoComR18
	jmp		inputUsuario

comparacaoComR18:
	subi	R17, 0x30		
	cp		R17, R18		
	breq	ganhou
	brlt	pedeMaior
	jmp		pedeMenor


pedeMaior:
	txs mensagemSeMenor
	jmp inputUsuario
pedeMenor:
	txs	mensagemSeMaior
	jmp inputUsuario
ganhou:
	txs mensagemVenceu
	pop	R17
	jmp	inicioDoGame

	fim:
		rjmp fim

		
//Tabelas

// Mensagem de início

mensagemInicio:
	.db '\n', "Digite + para iniciar o jogo",'\n', 0, 0
mensagemJogoComecou:
	.db "Jogo iniciado! Tente adivinhar o número que foi sorteado de 0 a 9.", '\n', 0
mensagemSeMenor:
	.db "Tente um número maior!", '\n', 0
mensagemSeMaior:
	.db "Tente um número menor!", '\n', 0
mensagemVenceu:
	.db "Parabéns! Você acertou.", '\n', 0, 0

						
