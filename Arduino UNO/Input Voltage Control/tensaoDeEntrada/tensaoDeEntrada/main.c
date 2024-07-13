/*
 * tensaoDeEntrada.c
 *
 * Created: 16/06/2024 12:43:50
 * Author : Francisco Bley Ruthes
 */ 

#define F_CPU 16000000UL
#include <avr/io.h> // IOs
#include <util/delay.h>
#include "lib5.h"
#include <avr/interrupt.h> 
#include <stdint.h> // Tipos de dados "uint16_t"
#include <stdio.h> // Opera��es de sa�da como "sprintf"

// Vari�vel global para receber valores do ADC de 10 bits
volatile uint16_t resultado_adc = 0; // "volatile" indica ao compilador que essa vari�vel pode ser modificada por uma interrup��o

const uint8_t numeros[] = {
	0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

void config_display() {
	DDRD = 0xFF; 
	DDRB = 0x0F; 
}

void config_ADC() {
	ADMUX  = 0x40; // AVcc � a nossa tens�o de refer�ncia
	//ADCSRA = 0b11001010;
	ADCSRA = 0xCF; // O fabricante recomenda que a frequ�ncia do ADC esteja entre 50 kHz e 200 kHz para garantir precis�o (divis�o por 128)	
}

void acionaVisor(int n, int seletorDisplay) {
	// Apaga-se todos os displays
	for (int i = 0; i <= 3; i++) {
		clr_bit(PORTB, i);
	}

	// Seleciona-se o display que ser� acendido
	set_bit(PORTB, seletorDisplay);

	// Atribui-se a combina��o de segmentos do display �s sa�das do registrador DP
	PORTD = numeros[n];

	// Se o display for o mais � esquerda, aciona-se o segmento DP
	if (seletorDisplay == 0) {
		set_bit(PORTD, 7);
	}
}

int main(void) {
	config_display();
	config_ADC();
	sei(); 

	while (1) {
		float tensaoDeEntrada = (resultado_adc / 1023.0) * 5 * 1000;
		
		char stringTensao[5];
		sprintf(stringTensao, "%04d", (int)tensaoDeEntrada); // Formata o valor de tens�o para 4 d�igitos e envia para o vetor string escolhido
		
		int dig[4];
		
		for (int i = 0; i < 4; i++) {
			dig[i] = stringTensao[i] - '0';
			acionaVisor(dig[i], i);
			_delay_ms(1);
		}
	}
	return 0;
}

ISR(ADC_vect) { //Vetor de mem�ria interrup��o referente ao ADC
	resultado_adc = ADC;
	set_bit(ADCSRA, 6); 
}

