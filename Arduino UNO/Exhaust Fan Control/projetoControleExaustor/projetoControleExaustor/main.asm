 
;
; proj3.asm
;
; Created: 15/05/2024 18:32:43
; Author : Francisco Bley Ruthes
;

jmp config
.include "lib.inc"	//Biblioteca
 
config:
// Define as portas de saída
	sbi		DDRD,6			// LED 1
	sbi		DDRD,7			// LED 2
// Configuração dos registradores USART e ADC
	call	config_serial		
	call	config_ADC			
	nop
	nop
	nop
	
desligado:
	cbi		PORTD,6			
	cbi		PORTD,7
esperaLigar:
	lds		R16, UCSR0A
	sbrs	R16, 7	
	jmp		esperaLigar
	lds		R16, UDR0		
	cpi		R16, '+'		//Verifica se o input é igual ao caracter '+'
	breq	monitorando
	jmp		esperaLigar

monitorando:
	sbi		PORTD,6
	cbi		PORTD,7
cicloMonitoramento:
	lds		R16, UCSR0A
	sbrs	R16, 7	
	jmp		monitoraTemp
	lds		R16, UDR0		
	cpi		R16, '-'		//Verifica se o input é igual ao caracter '-'
	breq	desligado
monitoraTemp:
	call	ativar_ADC
	lds		R17,ADCSRA		//Verifica se concluiu a conversao
	sbrc	R17,6
	jmp		monitoraTemp
	lds		R17, ADCH		//Atribuí o valor valor convertido ao R17
	call	tx_R17			//Chama a função de transmissão
	cpi		R17, 0x74		//Valor conertido pelo ADC referente a 50 graus celcius
	brsh	exaustorAtivado
	jmp		cicloMonitoramento	

exaustorAtivado:
	sbi		PORTD,7
cicloInputExaustor: 
	lds		R16, UCSR0A 
	sbrs	R16, 7	
	jmp		cicloExaustorAtivado
	lds		R16, UDR0		
	cpi		R16, '-'		//Verifica se o input é igual ao caracter '-'
	breq	desligado
cicloExaustorAtivado:
	call	ativar_ADC
	lds		R17,ADCSRA		//Verifica se concluiu a conversão
	sbrc	R17,6
	jmp		cicloExaustorAtivado
	lds		R17, ADCH		//Atribuí o valor valor convertido ao R17
	call	tx_R17			//Chama a função de transmissão
	cpi		R17, 0x5D		//Valor convertido pelo ADC referente a 40 graus celcius
	brlo	monitorando
	breq	monitorando
	jmp		cicloInputExaustor

