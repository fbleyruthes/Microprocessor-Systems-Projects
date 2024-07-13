;
; expOneComp.asm
;
; Created: 16/03/2024 11:19:15
; Author : Francisco Bey Ruthes
;
; Experiment 1

	cbi DDRD,2
	cbi DDRD,3
	cbi DDRD,4
	sbi DDRD,6
	sbi DDRD,7
loop:
	sbic PIND,2
	jmp path_S1_1
	sbis PIND,3
	jmp path_S2_0
	sbi PORTD,6
	sbic PIND,4
	jmp onLed2
	jmp offLed2
path_S1_1:
	cbi PORTD,6
	jmp onLed2
path_S2_0:
	cbi PORTD,6
	sbis PIND,4
	jmp onLed2
	jmp offLed2
onLed2:
	sbi PORTD,7
	jmp loop
offLed2:
	cbi PORTD,7
	jmp loop