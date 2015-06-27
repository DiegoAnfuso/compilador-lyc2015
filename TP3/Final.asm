include macros2.asm		 ;incluye macros
include number.asm		 ;incluye el asm para impresion de numeros
.MODEL LARGE ; tipo del modelo de memoria usado.
.386
.STACK 200h ; bytes en el stack
.DATA ; comienzo de la zona de datos
	MAXTEXTSIZE equ 50
	__aux1__ dd (?) ; Variable auxiliar para calculos
	__aux2__ dd (?) ; Variable auxiliar para calculos
	__cond1__ db (?) ; Variable auxiliar para condiciones
	__cond2__ db (?) ; Variable auxiliar para condiciones
	s3 db MAXTEXTSIZE dup (?), 0
	i dd (?)
	j dd (?)
	k dd (?)
	w dd (?)
	x dd (?)
	y dd (?)
	z dd (?)
	b dd (?)
	c dd (?)
	d dd (?)
	s1 db "prueba", 0
	_CTE_STRING_1 db "prueba", 0
	s2 db "ciclo", 0
	_CTE_STRING_2 db "ciclo", 0
	_CTE_ENT_1 dd 5
	_CTE_ENT_2 dd 10
	_CTE_ENT_3 dd 3
	_CTE_ENT_4 dd 2
	_CTE_ENT_5 dd 1
	_CTE_ENT_6 dd 0
	a dd 2.5
	_CTE_REAL_1 dd 2.5
	_CTE_STRING_3 db "se cumple 1er condicion !", 0
	_CTE_REAL_2 dd 0.50
	_CTE_STRING_4 db "se cumplen 2da condicion !", 0
	_CTE_ENT_7 dd 4
	_CTE_STRING_5 db "NO se cumple que i=j", 0
	_CTE_REAL_3 dd 5.5
	_CTE_REAL_4 dd .0
	_CTE_STRING_6 db "MAYOR", 0
	_CTE_STRING_7 db "MENOR", 0
	_CTE_REAL_5 dd 0.25
	_CTE_REAL_6 dd 0.5
	_CTE_STRING_8 db "TODO SALIO BIEN!", 0
