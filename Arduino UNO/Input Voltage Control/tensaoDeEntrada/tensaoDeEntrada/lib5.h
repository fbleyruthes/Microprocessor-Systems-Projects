/*
 * IncFile1.h
 *
 * Created: 16/06/2024 13:15:20
 *  Author: Francisco Bley Ruthes
 */ 

 
#ifndef INCFILE1_H_
#define INCFILE1_H_

#define set_bit(porta,bit) porta = porta | (1<<bit)  // deslocado "bit" vezes o 1 para esquerda

#define clr_bit(porta,bit) porta = porta & ~(1<<bit) // nega o 1 depois de deslocado "bit" vezes (maneira de mover o ZERO) 

#endif /* INCFILE1_H_ */