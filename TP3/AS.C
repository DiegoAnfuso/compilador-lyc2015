
# line 9 "AS.y"
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MI_DEBUG 1


int esCONST = 0;
int enDECLARE = 0;
int esLETDEFAULT = 0;
int yyerror(char *s);

#define PR_DECLARE 257
#define PR_ENDDECLARE 258
#define PR_INT 259
#define PR_REAL 260
#define PR_STRING 261
#define PR_CONST 262
#define PR_IF 263
#define PR_ELSE 264
#define PR_ENDIF 265
#define PR_BEGIN 266
#define PR_END 267
#define PR_GET 268
#define PR_PUT 269
#define PR_WHILE 270
#define PR_LET 271
#define PR_QEQUAL 272
#define PR_DEFAULT 273
#define PR_THEN 274
#define OP_ASIG 275
#define OP_SUMA 276
#define OP_RESTA 277
#define OP_MULTIPLI 278
#define OP_DIVISION 279
#define OP_CONCAT 280
#define PR_OR 281
#define PR_AND 282
#define OP_IGUAL 283
#define OP_MENOR 284
#define OP_MENORIGUAL 285
#define OP_MAYOR 286
#define OP_MAYORIGUAL 287
#define PR_NOT 288
#define ID 289
#define CTE_ENT 290
#define CTE_REAL 291
#define CTE_STRING 292
#define OP_PABRE 293
#define OP_PCIERRA 294
#define OP_CABRE 295
#define OP_CCIERRA 296
#define OP_LLABRE 297
#define OP_LLCIERRA 298
#define OP_TIPO 299
#define OP_GUION 300
#define SEP_DOSP 301
#define SEP_DECIMAL 302
#define SEP_SENT 303
#define SEP_LISTA 304
#define OP_DISTINTO 305
#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE yylval, yyval;
#define YYERRCODE 256

# line 428 "AS.y"


/* -------------------------------------------------------------------------- */
/*                             VARIABLES GLOBALES                             */
/* -------------------------------------------------------------------------- */

FILE *input;          // ARCHIVO ENTRADA.

//Ficheros Temporales
FILE *tokens;         //Archivo de Tokens
FILE *tos;            //Archivo de TOS

char caracter;	      // CARACTER LEIDO.
int NroToken=-1;	      // Valor que devuelve el yylex.
int TamToken=0;	      // Se utiliza para validar los tama?os de los ID y CteSTR.
char token[1000];     // Almacena el token armado.
int cteStrAbierta;    // Para validar que las ctes string esten bien formadas. Por ahora no la estamos usando.
int comAbierto;       // Para validar que los comentarios son cerrados.
int error = 0;        // para identificar si hubo o no error.
int nroLinea=1;		  // Para identificar en que l?nea se produjo el error.
int ptoInicio=0;	  // Para completar el nro segun corresponda EJ:.36 => 0.36
int TOStop = 0;		  // ?ndice de la TOS


/* -------------------------------------------------------------------------- */
/*                           DEFINCION CONSTANTES                             */
/* -------------------------------------------------------------------------- */
#define ESTADO_FINAL 20
#define MAXLONG 30
#define TAMMAX 1000
#define TAMTOKEN 1000
#define CANTPR 21
#define NV -1  //ESTADO DE CARACTER NO V?LIDO

#define INT_MAX     65535
#define FLT_MAX     3.40282347e+38F
#define FLT_MIN     1.4e-45F
//#define FLT_PREC    18
//DEFINES TEMPORALES
#define COMENTARIO -2


/* -------------------------------------------------------------------------- */
/*                           DECLARACION PROTOTIPOS                           */
/* -------------------------------------------------------------------------- */

// FUNCIONES GENERALES
void abrirCodigoFuente();
void cerrarCodigoFuente();

void leerCaracter();
void mostrarToken();
int  leerColumna(char);
void limpiarToken();

char * buscarEnTOS(int);
int insertarTOS(int , const char *);
void mostrarTOS();

int esPalabraReservada();
void agregarPalabrasReservadas();
void Validaciones();


// FUNCIONES LEXICO
void Agregar_Com();
void Agregar_Cte();
void Agregar_CteStr();
void Agregar_ID();
void Caract_No_Val();
void Fin_Com();
void Fin_Com_N2();
void Inciar_CteStr();
void Inc_NroLinea();
void Inf_Asignacion();
void Inf_Cte();
void Inf_CteStr();
void Inf_ID();
void Inf_Igual();
void Inf_Mayor();
void Inf_MayorIgual();
void Inf_Menor();
void Inf_MenorIgual();
void Inf_OpConcat();
void Inf_OpCorchAbre();
void Inf_OpCorchCierra();
void Inf_OpDivision();
void Inf_OpLLAbre();
void Inf_OpLLCierra();
void Inf_OpParAbre();
void Inf_OpParCierra();
void Inf_OpProducto();
void Inf_OpResta();
void Inf_OpSuma();
void Inf_Sep();
void Inf_SepLista();
void Inf_SepDosP();
void Iniciar_Asignacion();
void Iniciar_Com();
void Iniciar_Com_N2();
void Iniciar_concat();
void Iniciar_Cte();
void Iniciar_Cte_Float();
void Iniciar_ID();
void Iniciar_Igual();
void Iniciar_MayorIgual();
void Iniciar_MenorIgual();
void Nada();


/* -------------------------------------------------------------------------- */
/*                            DEFINICION FUNCIONES                            */
/* -------------------------------------------------------------------------- */


void abrirCodigoFuente(const char * fuente)
{
    if ((input = fopen (fuente, "r"))== NULL)
    {
        printf("No se puede abrir el archivo de entrada");
        getch();
        exit(1);
    }
}

void cerrarCodigoFuente()
{
    if(fclose(input)!=0)
    {
        printf("No se puede CERRAR el archivo de entrada");
        //getch();
        //exit(1);
    }
}

void Inc_NroLinea()
{
    nroLinea++;
}

/* IDENTIFICADORES ---------------------------------------------------------- */
void Iniciar_ID ()
{
    limpiarToken();
    token[TamToken]=caracter;
}

void Agregar_ID()
{
    if (strlen(token)<MAXLONG)
    {
        token[++TamToken]=caracter;
    }
    else
    {
        printf("\n ERROR: La cantidad Maxima de caracteres para un IDENTIFICADOR es de %d. \n",MAXLONG);
        printf("\n - Analisis Lexico INTERRUMPIDO4 - \n");
        exit(1);
    }
}

void Inf_ID ()
{
    if (error == 0)
    {
		NroToken = esPalabraReservada();

		if (NroToken == ID)
            yylval = insertarTOS(NroToken, token); //TOS
		else
            yylval=NroToken;

 		ungetc((int)caracter, input);
   }
   else
   {
        NroToken=0;
   }
}

/* CONSTANTE NUMERICAS ------------------------------------------------------ */
void Iniciar_Cte()
{
    limpiarToken();
    token[TamToken] = caracter;    	//guardamos el primer caracter del ID en la cadena auxiliar
    TamToken++;
    //Posible CTE
    NroToken=CTE_ENT;
}

void Iniciar_Cte_Float()
{
    if (NroToken!=CTE_ENT)
    {
        limpiarToken();
    }

    token[TamToken] = caracter;    	//guardamos el primer caracter del ID en la cadena auxiliar
    TamToken++;
    //Posible CTE
    NroToken=CTE_REAL;
}

void Agregar_Cte()
{
    token[TamToken] = caracter;
    TamToken++;
}

void Inf_Cte()
{
    if (NroToken==CTE_ENT)
    {
        long cte = atol(token);
        if (cte > INT_MAX)
        {
            printf("\n ERROR: # Se excede el rango para un ENTERO. \n");
            printf("\n - Analisis Lexico INTERRUMPIDO - \n");
            printf("\n - leido: %s - actual: %c", token, caracter);
            exit(1);
        }
    }

    if (NroToken==CTE_REAL)
    {
        if (strcmp(token,".")==0)
        {
            printf("\n - ERROR: Constante REAL incompleta, se esperana un digito \n");
            printf("\n - Analisis Lexico INTERRUMPIDO1 - \n");
            printf("\n - leido: %s - actual: %c", token, caracter);
            exit(1);
        }

        int i=0, decimales=-1;

        for(i=0;i < strlen(token); i++)
        {
            if (token[i]=='.' || decimales >-1)
                decimales++;
        }

        //RANGO
        /*if (decimales > FLT_PREC )
        {
            printf("\n ERROR: # Se excede el rango de presicion para un REAL. \n");
            printf("\n - Analisis Lexico INTERRUMPIDO - \n");
            exit(1);
        }*/

        double cte = atof(token);

		if(cte != 0.0){
			if (cte > FLT_MAX)
			{
				printf("\n ERROR: # Se excede el rango para un REAL. \n");
				printf("\n - Analisis Lexico INTERRUMPIDO - \n");
				exit(1);
			}
			if (cte < FLT_MIN)
			{
				printf("\n ERROR: # Se excede el rango para un REAL. \n");
				printf("\n - Analisis Lexico INTERRUMPIDO - \n");
				exit(1);
			}
		}
    }

    yylval= insertarTOS(NroToken, token); // TOS
    ungetc((int)caracter, input);
}

/* CONSTANTE STRING --------------------------------------------------------- */
void Inciar_CteStr()
{
    limpiarToken();
    token[TamToken]=caracter;
    cteStrAbierta++;
    //Posible CTE_STRING
    NroToken=CTE_STRING;
}

void Agregar_CteStr()
{
    if (strlen(token)<MAXLONG)
    {
        token[++TamToken]=caracter;
    }
    else
    {
        printf("\n ERROR: La cantidad Maxima de caracteres para una Constante STRING es de 30. \n");
        printf("\n - Analisis Lexico INTERRUMPIDO2 - \n");
        printf("\n - leido: %s - actual: %c", token, caracter);
        exit(1);
    }
}

// ?hay que incluir la comilla doble?
void Inf_CteStr()
{
    token[++TamToken]=caracter;
    if (error == 0)
    {
        NroToken = CTE_STRING;
        yylval = insertarTOS(NroToken, token);  //TOS
        cteStrAbierta--;
    }
    else
    {
        NroToken=0;
        cteStrAbierta=0;
    }
}

/* COMENTARIOS -------------------------------------------------------------- */
void Iniciar_Com()
{
    NroToken = COMENTARIO;
    limpiarToken();
	token[TamToken]=caracter;
	comAbierto++;
}
void Agregar_Com()
{
    Nada();
}
void Iniciar_Com_N2()
{
    comAbierto++;
}
void Fin_Com_N2()
{
    comAbierto--;
}
void Fin_Com()
{
    comAbierto--;
	Nada();
}


/* SEPARADOR ---------------------------------------------------------------- */
void Inf_Sep()
{
	limpiarToken();
    token[TamToken]=caracter;
    NroToken = SEP_SENT;
}

/* NADA --------------------------------------------------------------------- */
void Nada()
{
    TamToken++;
    token[TamToken]=caracter;
    //limpiarToken();
	if(caracter == '\n')
		Inc_NroLinea();
}

/* CARACTER NO VALIDO ------------------------------------------------------- */
void Caract_No_Val () //REVISAR MENSAJES DE ERROR
{
    printf("\n - ERROR: Caracter no válido \n");

    if(strcmp(token,":")==0)
        printf("\n - ERROR: Se esperaba = \n");

    if(strcmp(token,".")==0)
        printf("\n - ERROR: Se esperaba un digito \n");

    printf("\n - Analisis Lexico INTERRUMPIDO3 - \n");
    printf("\n - leido: %s - actual: %c", token, caracter);
    exit(0);
}

/* ASIGNACION --------------------------------------------------------------- */
void Iniciar_Asignacion()
{
    limpiarToken();
	token[TamToken]=caracter;
	//Posible ASIG
	NroToken = OP_ASIG;
}
void Inf_Asignacion()
{
    TamToken++;
    token[TamToken]=caracter;
    NroToken = OP_ASIG;
}


/* SUMA --------------------------------------------------------------------- */
void Inf_OpSuma()
{
    NroToken = OP_SUMA;
    ungetc((int)caracter, input);
}

/* RESTA -------------------------------------------------------------------- */
void Inf_OpResta()
{
    NroToken = OP_RESTA;
    comAbierto--;
    ungetc((int)caracter, input);
}

/* RESTA 2-------------------------------------------------------------------- */
void Inf_OpResta_2()
{
    NroToken = OP_RESTA;
    comAbierto--;
    ungetc((int)caracter, input); //devuelve el caracter que no es / porque no inicio comentario
    ungetc((int)token[TamToken], input); //devuelve el segundo menos
    token[TamToken] = '\0'; //borra el segundo menos del token asi queda un -
    TamToken--;
}

/* PRODUCTO ----------------------------------------------------------------- */
void Inf_OpProducto()
{
    limpiarToken();
	token[TamToken]=caracter;
    NroToken = OP_MULTIPLI;
}

/* DIVISION ----------------------------------------------------------------- */
void Inf_OpDivision()
{
    limpiarToken();
	token[TamToken]=caracter;
    NroToken = OP_DIVISION;
    comAbierto=0;
}

/* CONCATENA ---------------------------------------------------------------- */
void Iniciar_concat()
{
    limpiarToken();

    TamToken=0;
	token[TamToken]=caracter;

	//Posible SUMA
	NroToken = OP_SUMA;
}
void Inf_OpConcat()
{
    TamToken++;
    token[TamToken]=caracter;
    NroToken = OP_CONCAT;
     //ungetc((int)caracter, input);
}

/* IGUAL -------------------------------------------------------------------- */
void Iniciar_Igual()
{
	limpiarToken();
	token[TamToken]=caracter;
	//Posible ASIG
	NroToken = OP_ASIG;
}
void Inf_Igual()
{
	TamToken++;
	token[TamToken]=caracter;
    NroToken = OP_IGUAL;
}

/* MENOR -------------------------------------------------------------------- */
void Inf_Menor()
{
    NroToken = OP_MENOR;
    ungetc((int)caracter, input);  //Segun Automata
}

/* MAYOR -------------------------------------------------------------------- */
void Inf_MayorIgual()
{
    TamToken++;
    token[TamToken]=caracter;
    NroToken = OP_MAYORIGUAL;
}

/* MAYORoIGUAL -------------------------------------------------------------- */
void Iniciar_MayorIgual()
{
    limpiarToken();
    TamToken=0;
    token[TamToken]=caracter;
    NroToken = OP_MAYOR;
}
void Inf_Mayor()
{
    NroToken = OP_MAYOR;
    ungetc((int)caracter, input); //segun automata
}

/* MENORoIGUAL -------------------------------------------------------------- */
void Iniciar_MenorIgual()
{
    limpiarToken();
    TamToken=0;
    token[TamToken]=caracter;
    NroToken = OP_MENOR;
}
void Inf_MenorIgual()
{
    TamToken++;
    token[TamToken]=caracter;
    NroToken = OP_MENORIGUAL;
}

/* PARENTESIS --------------------------------------------------------------- */
void Inf_OpParAbre()
{
    limpiarToken();
    TamToken=0;
    token[TamToken]=caracter;
    NroToken = OP_PABRE;
}
void Inf_OpParCierra()
{
    limpiarToken();
    TamToken=0;
    token[TamToken]=caracter;
    NroToken = OP_PCIERRA;
}

/* LLAVE -------------------------------------------------------------------- */
void Inf_OpLLAbre()
{
    limpiarToken();
    TamToken=0;
    token[TamToken]=caracter;
    NroToken = OP_LLABRE;
}
void Inf_OpLLCierra()
{
    limpiarToken();
    TamToken=0;
    token[TamToken]=caracter;
    NroToken = OP_LLCIERRA;
}

/* CORCHETES ---------------------------------------------------------------- */
void Inf_OpCorchAbre()
{
    limpiarToken();
    TamToken=0;
    token[TamToken]=caracter;
    NroToken = OP_CABRE;
}
void Inf_OpCorchCierra()
{
    limpiarToken();
    TamToken=0;
    token[TamToken]=caracter;
    NroToken = OP_CCIERRA;
}

/* OPERADOR DECLARACION DE TIPO DE VARIABLE --------------------------------- */
/*
void Inf_OpTipo()
{
    limpiarToken();
    TamToken=0;
    token[TamToken]=caracter;
    NroToken = OP_TIPO;
}
*/
/* SEPARADOR DE UNA LISTA DE VARIALBLES O TIPOS ----------------------------- */
void Inf_SepLista()
{
    limpiarToken();
    TamToken=0;
    token[TamToken]=caracter;

    NroToken = SEP_LISTA;
}

/* SEPARADOR DE LAS DOS PARTES DE UN LET ----------------------------- */
void Inf_SepDosP()
{
    /*limpiarToken();
    TamToken=0;
    token[TamToken]=caracter;*/

    NroToken = SEP_DOSP;
    ungetc((int)caracter, input);
}

/* -------------------------------------------------------------------------- */
/*                           ESTRUCTURAS ESPECIALES                           */
/* -------------------------------------------------------------------------- */

// TABLA SIMBOLOS
int tipo_dato;
int tipo_const;

typedef struct
{
    char nombre[TAMTOKEN];
    int  token;
    char tipo[15];
    int  tipo_dato;
    char valor[TAMTOKEN];
    int longitud;
} nodoTS;

nodoTS TOS[TAMMAX];

//MATRIZ PUNTERO A FUNCION
void (*proceso[][25])() = {
//			0|					1|					2|					3|					4|					5|					6|					7|						8|						9|					10|						11|					12|						13|					14|					15|					16|					17|					18|					19|					20|						21|					22|					23|					24|
//			+|					-|					*|					/|					Let|				Dig|				=|					<|						>|						"|					.|						[|					]|						{|					}|					(|					)|					,|					_|					;|					:|				        tab|				blan|				ent|				OTRO|
/*E0*/ {	Iniciar_concat,	    Iniciar_Com,	    Inf_OpProducto,	    Inf_OpDivision,	    Iniciar_ID,	        Iniciar_Cte,	    Iniciar_Igual,	    Iniciar_MenorIgual,	    Iniciar_MayorIgual,	    Inciar_CteStr,	    Iniciar_Cte_Float,	    Inf_OpCorchAbre,	Inf_OpCorchCierra,	    Inf_OpLLAbre,	    Inf_OpLLCierra,	    Inf_OpParAbre,	    Inf_OpParCierra,	Inf_SepLista,	    Iniciar_ID,	        Inf_Sep,	        Iniciar_Asignacion,	    Nada,	            Nada,	            Inc_NroLinea,	    Caract_No_Val },
/*E1*/ {	Inf_ID,	            Inf_ID,	            Inf_ID,	            Inf_ID,	            Agregar_ID,	        Agregar_ID,	        Inf_ID,	            Inf_ID,	                Inf_ID,	                Inf_ID,	            Agregar_ID,	            Inf_ID,	            Inf_ID,	                Inf_ID,	            Inf_ID,	            Inf_ID,	            Inf_ID,	            Inf_ID,	            Agregar_ID,	        Inf_ID,	            Inf_ID,	                Inf_ID,	            Inf_ID,	            Inf_ID,	            Inf_ID },
/*E2*/ {	Inf_Cte,	        Inf_Cte,	        Inf_Cte,	        Inf_Cte,	        Inf_Cte,	        Agregar_Cte,	    Inf_Cte,	        Inf_Cte,	            Inf_Cte,	            Inf_Cte,	        Iniciar_Cte_Float,	    Inf_Cte,	        Inf_Cte,	            Inf_Cte,	        Inf_Cte,	        Inf_Cte,	        Inf_Cte,	        Inf_Cte,	        Inf_Cte,	        Inf_Cte,	        Inf_Cte,	            Inf_Cte,	        Inf_Cte,	        Inf_Cte,	        Inf_Cte },
/*E3*/ {	Inf_Cte,	        Inf_Cte,	        Inf_Cte,	        Inf_Cte,	        Inf_Cte,	        Agregar_Cte,	    Inf_Cte,	        Inf_Cte,	            Inf_Cte,	            Inf_Cte,	        Caract_No_Val,	        Inf_Cte,	        Inf_Cte,	            Inf_Cte,	        Inf_Cte,	        Inf_Cte,	        Inf_Cte,	        Inf_Cte,	        Inf_Cte,	        Inf_Cte,	        Inf_Cte,	            Inf_Cte,	        Inf_Cte,	        Inf_Cte,	        Inf_Cte },
/*E4*/ {	Agregar_CteStr,	    Agregar_CteStr,	    Agregar_CteStr,	    Agregar_CteStr,	    Agregar_CteStr,	    Agregar_CteStr,	    Agregar_CteStr,	    Agregar_CteStr,	        Agregar_CteStr,	        Inf_CteStr,	        Agregar_CteStr,	        Agregar_CteStr,	    Agregar_CteStr,	        Agregar_CteStr,	    Agregar_CteStr,	    Agregar_CteStr,	    Agregar_CteStr,	    Agregar_CteStr,	    Agregar_CteStr,	    Agregar_CteStr,	    Agregar_CteStr,	        Agregar_CteStr,     Agregar_CteStr,	    Agregar_CteStr,	    Agregar_CteStr },
/*E5*/ {	Inf_OpConcat,	    Inf_OpSuma,	        Inf_OpSuma,	        Inf_OpSuma,	        Inf_OpSuma,	        Inf_OpSuma,	        Inf_OpSuma,	        Inf_OpSuma, 	        Inf_OpSuma,	            Inf_OpSuma,	        Inf_OpSuma,	            Inf_OpSuma,	        Inf_OpSuma,	            Inf_OpSuma,	        Inf_OpSuma,	        Inf_OpSuma,	        Inf_OpSuma,	        Inf_OpSuma,	        Inf_OpSuma,	        Inf_OpSuma,	        Inf_OpSuma,	            Inf_OpSuma,	        Inf_OpSuma,	        Inf_OpSuma,	        Inf_OpSuma },
/*E6*/ {	Inf_SepDosP,	    Inf_SepDosP,	    Inf_SepDosP,	    Inf_SepDosP,	    Inf_SepDosP,	    Inf_SepDosP,	    Inf_Asignacion,	    Inf_SepDosP,	            Inf_SepDosP,	            Inf_SepDosP,	        Inf_SepDosP,	            Inf_SepDosP,	        Inf_SepDosP,	            Inf_SepDosP,	        Inf_SepDosP,	        Inf_SepDosP,	        Inf_SepDosP,	        Inf_SepDosP,	        Inf_SepDosP,	        Inf_SepDosP,	        Inf_SepDosP,	            Inf_SepDosP,	        Inf_SepDosP,	        Inf_SepDosP,	        Inf_SepDosP },
/*E7*/ {	Caract_No_Val,	    Caract_No_Val,	    Caract_No_Val,	    Caract_No_Val,	    Caract_No_Val,	    Caract_No_Val,	    Inf_Igual,	        Caract_No_Val,	        Caract_No_Val,	        Caract_No_Val,	    Caract_No_Val,	        Caract_No_Val,	    Caract_No_Val,	        Caract_No_Val,	    Caract_No_Val,	    Caract_No_Val,	    Caract_No_Val,	    Caract_No_Val,	    Caract_No_Val,	    Caract_No_Val,	    Caract_No_Val,	        Caract_No_Val,	    Caract_No_Val,	    Caract_No_Val,	    Caract_No_Val },
/*E8*/ {	Inf_Mayor,	        Inf_Mayor,	        Inf_Mayor,	        Inf_Mayor,	        Inf_Mayor,	        Inf_Mayor,	        Inf_MayorIgual,	    Inf_Mayor,	            Inf_Mayor,	            Inf_Mayor,	        Inf_Mayor,	            Inf_Mayor,	        Inf_Mayor,	            Inf_Mayor,	        Inf_Mayor,	        Inf_Mayor,	        Inf_Mayor,	        Inf_Mayor,	        Inf_Mayor,	        Inf_Mayor,	        Inf_Mayor,	            Inf_Mayor,	        Inf_Mayor,	        Inf_Mayor,	        Inf_Mayor },
/*E9*/ {	Inf_Menor,	        Inf_Menor,	        Inf_Menor,	        Inf_Menor,	        Inf_Menor,	        Inf_Menor,	        Inf_MenorIgual,	    Inf_Menor,	            Inf_Menor,	            Inf_Menor,	        Inf_Menor,	            Inf_Menor,	        Inf_Menor,	            Inf_Menor,	        Inf_Menor,	        Inf_Menor,	        Inf_Menor,	        Inf_Menor,	        Inf_Menor,	        Inf_Menor,	        Inf_Menor,	            Inf_Menor,	        Inf_Menor,	        Inf_Menor,	        Inf_Menor },
/*E10*/ {	Inf_OpResta,	    Nada,	            Inf_OpResta,	    Inf_OpResta,	    Inf_OpResta,	    Inf_OpResta,	    Inf_OpResta,	    Inf_OpResta,	        Inf_OpResta,	        Inf_OpResta,	    Inf_OpResta,	        Inf_OpResta,	    Inf_OpResta,	        Inf_OpResta,	    Inf_OpResta,	    Inf_OpResta,	    Inf_OpResta,	    Inf_OpResta,	    Inf_OpResta,	    Inf_OpResta,	    Inf_OpResta,	        Inf_OpResta,	    Inf_OpResta,	    Inf_OpResta,	    Inf_OpResta },
/*E11*/ {	Inf_OpResta_2,	    Inf_OpResta_2,	    Inf_OpResta_2,	    Agregar_Com,	    Inf_OpResta_2,	    Inf_OpResta_2,	    Inf_OpResta_2,	    Inf_OpResta_2,	        Inf_OpResta_2,	        Inf_OpResta_2,	    Inf_OpResta_2,	        Inf_OpResta_2,	    Inf_OpResta_2,	        Inf_OpResta_2,	    Inf_OpResta_2,	    Inf_OpResta_2,	    Inf_OpResta_2,	    Inf_OpResta_2,	    Inf_OpResta_2,	    Inf_OpResta_2,	    Inf_OpResta_2,	        Inf_OpResta_2,	    Inf_OpResta_2,	    Inf_OpResta_2,	    Inf_OpResta_2 },
/*E12*/ {	Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	        Agregar_Com,	        Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com },
/*E13*/ {	Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	        Agregar_Com,	        Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com },
/*E14*/ {	Agregar_Com,	    Fin_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	        Agregar_Com,	        Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com },
/*E15*/ {	Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	        Agregar_Com,	        Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com },
/*E16*/ {	Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Iniciar_Com_N2,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	        Agregar_Com,	        Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com },
/*E17*/ {	Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	        Agregar_Com,	        Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com },
/*E18*/ {	Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	        Agregar_Com,	        Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com },
/*E19*/ {	Agregar_Com,	    Fin_Com_N2,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	        Agregar_Com,	        Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com },
/*E20*/ {	Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	        Agregar_Com,	        Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com,	        Agregar_Com,	    Agregar_Com,	    Agregar_Com,	    Agregar_Com }
};



//MATRIZ DE ESTADOS
static int nEstado[21][25] = {
//          0|	1|	2|	3|	4|	5|	6|	7|	8|	9|	10|	11|	12|	13|	14|	15|	16|	17|	18|	19|	20|	21|	22|	23|	24|
//          +|	-|	*|	/| Lt| Dg|  =|	<|	>|	"|	.|	[|	]|	{|	}|	(|	)|	,|	_|	;|	:|	/t|	\b|	\n| OTRO|
/*E0*/ {	5,	10,	20,	20,	1,	2,	7,	9,	8,	4,	3,	20,	20,	20,	20,	20,	20,	20,	1,	20,	6,	0,	0,	0,	NV},
/*E1*/ {	20,	20,	20,	20,	1,	1,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	1,	20,	20,	20,	20,	20,	20},
/*E2*/ {	20,	20,	20,	20,	20,	2,	20,	20,	20,	20,	3,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20},
/*E3*/ {	20,	20,	20,	20,	20,	3,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20},
/*E4*/ {	4,	4,	4,	4,	4,	4,	4,	4,	4,	20,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4},
/*E5*/ {	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20},
/*E6*/ {	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20},
/*E7*/ {	NV,	NV,	NV,	NV,	NV,	NV,	20,	NV,	NV,	NV,	NV,	NV,	NV,	NV,	NV,	NV,	NV,	NV,	NV,	NV,	NV,	NV,	NV,	NV,	NV},
/*E8*/ {	20,	20,	20,	20,	20,	1,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20},
/*E9*/ {	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20},
/*E10*/ {	20,	11,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20},
/*E11*/ {	20,	20,	20,	12,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20},
/*E12*/ {	12,	15,	12,	13,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12},
/*E13*/ {	12,	14,	12,	13,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12},
/*E14*/ {	12,	0,	12,	13,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12},
/*E15*/ {	12,	16,	12,	13,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12},
/*E16*/ {	12,	16,	12,	17,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12},
/*E17*/ {	17,	17,	17,	18,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17},
/*E18*/ {	17,	19,	17,	18,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17},
/*E19*/ {	17,	12,	17,	18,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17,	17},
/*E20*/ {	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21,	21}
};

/* -------------------------------------------------------------------------- */
/* limpiarToken(): Limpia buffer de tokens                                    */
/* -------------------------------------------------------------------------- */
void limpiarToken()
{
	int i;
	for (i = 0; i <= MAXLONG; i++)
   	token[i] = '\0';
   	TamToken = 0;
}

/* -------------------------------------------------------------------------- */
/* leerCaracter(): Lee un caracter del archivo                                */
/* -------------------------------------------------------------------------- */
void leerCaracter()
{
	caracter = fgetc(input);
}

/* -------------------------------------------------------------------------- */
/* mostrarToken(): Muestra el token por pantalla y lo guarda en tabla tokens  */
/* -------------------------------------------------------------------------- */
void mostrarToken()
{
    char linea[100];
    int j;
    int i;
    char auxStr[200];
    switch(NroToken)
    {
        case ID:
            sprintf(linea,"< ID             : %s>\n",token);
            break;
        case PR_DECLARE:
            sprintf(linea,"< PR_DECLARE     : %s>\n",token);
            break;
        case PR_ENDDECLARE:
            sprintf(linea,"< PR_ENDDECLARE  : %s>\n",token);
            break;
        case PR_INT:
            sprintf(linea,"< PR_INT         : %s>\n",token);
            break;
        case PR_REAL:
            sprintf(linea,"< PR_REAL        : %s>\n",token);
            break;
        case PR_STRING:
            sprintf(linea,"< PR_STRING      : %s>\n",token);
            break;
        case PR_CONST:
            sprintf(linea,"< PR_CONST       : %s>\n",token);
            break;
        case PR_WHILE:
            sprintf(linea,"< PR_WHILE      : %s>\n",token);
            break;
        case PR_LET:
            sprintf(linea,"< PR_LET      : %s>\n",token);
            break;
        case PR_QEQUAL:
            sprintf(linea,"< PR_QEQUAL      : %s>\n",token);
            break;
        case PR_IF:
            sprintf(linea,"< PR_IF          : %s>\n",token);
            break;
        case PR_ELSE:
            sprintf(linea,"< PR_ELSE        : %s>\n",token);
            break;
        case PR_ENDIF:
            sprintf(linea,"< PR_ENDIF       : %s>\n",token);
            break;
        case PR_BEGIN:
            sprintf(linea,"< PR_BEGIN       : %s>\n",token);
            break;
        case PR_END:
            sprintf(linea," <PR_END         : %s>\n",token);
            break;
        case PR_AND:
            sprintf(linea," <PR_AND         : %s>\n",token);
            break;
        case PR_OR:
            sprintf(linea," <PR_OR          : %s>\n",token);
            break;
        case PR_GET:
            sprintf(linea," <PR_GET         : %s>\n",token);
            break;
        case PR_PUT:
            sprintf(linea," <PR_PUT         : %s>\n",token);
            break;
        case PR_DEFAULT:
            sprintf(linea," <PR_DEFAULT     : %s>\n",token);
            break;
        case PR_THEN:
            sprintf(linea," <PR_THEN     : %s>\n",token);
            break;
        case CTE_ENT:
             sprintf(linea,"< CTE ENT  : %s >\n", token);
             break;
        case CTE_REAL:
             sprintf(linea,"< CTE REAL : %s >\n", token);
             break;
        case CTE_STRING:
             i=0;
             strcpy(auxStr," ");
             for (j=0;j< strlen(token);j++)
             {
                 auxStr[i]=token[j];
                 if(token[j]=='%')
                 {
                     auxStr[++i]= token[j];
                 }
                 i++;
             }
             auxStr[i]='\0';
             sprintf(linea,"< CTE_STR  : %s >\n", auxStr);
             break;
        case OP_PABRE:
             sprintf(linea,"< ABRE PAR : %s >\n", token);
             break;
        case OP_PCIERRA:
             sprintf(linea,"< CIERR PAR: %s >\n", token);
             break;
        case PR_NOT:
             sprintf(linea,"< PR_NOT   : %s >\n", token);
             break;
        case OP_MENOR:
             sprintf(linea,"< OP_MENOR : %s >\n", token);
             break;
        case OP_MENORIGUAL:
             sprintf(linea,"< OP_MENIGU: %s >\n", token);
             break;
        case OP_MAYOR:
             sprintf(linea,"< OP_MAYOR : %s >\n", token);
             break;
        case OP_MAYORIGUAL:
             sprintf(linea,"< OP_MAYIGU: %s >\n", token);
             break;
        case OP_IGUAL:
             sprintf(linea,"< OP_IGUAL : %s >\n", token);
             break;
        case OP_SUMA:
             sprintf(linea,"< OP_SUMA  : %s >\n", token);
             break;
        case OP_RESTA:
             sprintf(linea,"< OP_RESTA : %s >\n", token);
             break;
        case OP_MULTIPLI:
             sprintf(linea,"< OP_MULTI : %s >\n", token);
             break;
        case OP_DIVISION:
             sprintf(linea,"< OP_DIV   : %s >\n", token);
             break;
        case OP_CABRE:
             sprintf(linea,"< ABRE COR : %s >\n", token);
             break;
        case OP_CCIERRA:
             sprintf(linea,"< CIERR COR: %s >\n", token);
             break;
        case OP_ASIG:
		     sprintf(linea,"< OP_ASIG  : %s >\n", token);
             break;
        case OP_LLABRE:
             sprintf(linea,"< ABRE LLA : %s >\n", token);
             break;
        case OP_LLCIERRA:
             sprintf(linea,"< CIERR LLA: %s >\n", token);
             break;
        case OP_CONCAT:
             sprintf(linea,"< OP_CONCAT: %s >\n", token);
             break;
        case SEP_SENT:
		     sprintf(linea,"< SEPA_SENT: %s >\n", token);
             break;
        case OP_TIPO:
             sprintf(linea,"< OP_TIPO  : %s >\n", token);
             break;
        case SEP_LISTA:
             sprintf(linea,"< SEP_LISTA: %s >\n", token);
             break;
        case OP_GUION:
             sprintf(linea,"< OP_GUION : %s >\n", token);
             break;
        case SEP_DOSP:
             sprintf(linea,"< SEP_DOSP: %s >\n", token);
             break;
     }

     fprintf(tokens, linea);
     printf(linea);
}

/* -------------------------------------------------------------------------- */
/* leerColumna(): Retorna la columna que corresponde al caracter ingresado    */
/* -------------------------------------------------------------------------- */
int leerColumna(char caracter)
{
	//LETRAS
	if(caracter <= 'z' && caracter >= 'a' )
		return 4;
	if (caracter <= 'Z' && caracter >= 'A')
	  	return 4;

	//DIGITOS
	if(caracter >= '0' && caracter <= '9')
	  return 5;

	//OTROS CARACTERES
    switch(caracter)
	{
		case '+':
		    return 0;
		    break;
		case '-':
		    return 1;
		    break;
		case '*':
		    return 2;
		    break;
		case '/':
		    return 3;
		    break;
		case '=':
		    return 6;
		    break;
		case '<':
		    return 7;
		    break;
		case '>':
		    return 8;
		    break;
		case '"':
		    return 9;
		    break;
		case '.':
            return 10;
            break;
		case '[':
			return 11;
            break;
		case ']':
            return 12;
            break;
		case '{':
			return 13;
            break;
		case '}':
            return 14;
            break;
        case '(':
            return 15;
            break;
		case ')':
            return 16;
            break;
		case ',':
            return 17;
            break;
        case '_':
            return 18;
            break;
		case ';':
            return 19;
            break;
        case ':':
            return 20;
            break;
		case '\t':
            return 21;
            break;
		case ' ':
			return 22;
            break;
		/*case '\0':
            return 22;
            break;*/
		case '\n':
			return 23;
            break;
        case EOF:
            return EOF;
            break;
        default:
            return 24;
            break;
	}
}

/* -------------------------------------------------------------------------- */
/* buscarEnTOS(): Busca en la TOS un simbolo por el indice                    */
/* -------------------------------------------------------------------------- */
char * buscarEnTOS(int index)
{
    return TOS[index].nombre;
}

/* -------------------------------------------------------------------------- */
/* insertarTOS(): Inserta en la TOS o si ya existe devuelve la posicion       */
/* -------------------------------------------------------------------------- */

int insertarTOS(int NroToken, const char * lexema)
{
    int i, x = 0, j;
	char aux[100];
	char auxStr[100];
	if (NroToken==CTE_STRING)
    {
        strcpy(auxStr," ");

        for (j=0;j< strlen(lexema);j++)
        {
            if(lexema[j]!='"')
            {
                auxStr[x]= lexema[j];
                x++;
            }
        }

        auxStr[x]='\0';
		for (i=CANTPR; i<TOStop;  i++)
		{
			if (strcmp(TOS[i].valor,auxStr)==0)
				return i;
		}
    } else {
		for (i=CANTPR; i<TOStop;  i++)
		{
			if (strcmp(TOS[i].nombre,lexema)==0){
				if ((esCONST || enDECLARE) && NroToken == ID) {
					yyerror("Variable declarada previamente.");
				} else {
					return i;
				}
			}
		}
	}
    switch (NroToken)
    {
        case ID:
            if (!enDECLARE && !esCONST) {
                yyerror("Variable no declarada.");
            }

            if (!esCONST)
                strcpy(TOS[TOStop].tipo,"ID" );
            else
                strcpy(TOS[TOStop].tipo,"CONST" );

            TOS[TOStop].tipo_dato = tipo_dato;
			strcpy(TOS[TOStop].nombre, lexema);
			strcpy(TOS[TOStop].valor, lexema);
            break;
        case CTE_ENT:
			//strcpy(aux,"_");
            //strcat(aux, lexema);
            strcpy(TOS[TOStop].tipo,"CTE_ENT");
            TOS[TOStop].tipo_dato = PR_INT;
			strcpy(TOS[TOStop].nombre, lexema);
			strcpy(TOS[TOStop].valor, lexema);
            break;
        case CTE_REAL:
			//strcpy(aux,"_");
            //strcat(aux, lexema);
            strcpy(TOS[TOStop].tipo,"CTE_REAL");
            TOS[TOStop].tipo_dato = PR_REAL;
			strcpy(TOS[TOStop].nombre, lexema);
			strcpy(TOS[TOStop].valor, lexema);
            break;
        case CTE_STRING:
			//strcpy(aux,"_");
            //strcat(aux, auxStr);
            strcpy(TOS[TOStop].tipo,"CTE_STRING" );
            TOS[TOStop].tipo_dato = PR_STRING;
            TOS[TOStop].longitud = (strlen(auxStr));
			strcpy(TOS[TOStop].nombre, lexema);
			strcpy(TOS[TOStop].valor, auxStr);
            break;
    }


    TOStop++;

    return TOStop-1;
}

void mostrarTOS()
{
	if ((tos = fopen ("tos.txt","w"))== NULL)
    {
        printf("No se puede crear el archivo de la tabla de TOS");
        getch();
        exit(1);
    }
    int i;

    //printf("\n------------------------------ TABLA DE  SIMBOLOS ------------------------------\n");
    fprintf(tos,"\n------------------------------ TABLA DE  SIMBOLOS ------------------------------\n");

    //printf ("Nro\t | Nombre\t\t\t | Tipo\t | Valor\n");
    fprintf(tos,"Nro\t | Nombre\t\t\t | Tipo\t | Valor\t | Longitud\t | Tipo_Dato \n");
    for (i=0; i<TOStop; i++)
    {
      //  printf ("%d     \t | %s     \t\t\t | %s     \t | %s \n",i,TOS[i].nombre, TOS[i].tipo, TOS[i].valor);
        fprintf(tos,"%d     \t | %s     \t\t\t | %s     \t | %s \t | %d \t | %d \n",i,TOS[i].nombre, TOS[i].tipo, TOS[i].valor, TOS[i].longitud, TOS[i].tipo_dato);
    }
    //printf("\n--------------------------------------------------------------------------------\n");
    fprintf(tos,"\n------------------------------ TABLA DE  SIMBOLOS ------------------------------\n");
}

/* -------------------------------------------------------------------------- */
/* esPalabraReservada(): Varifica si un ID es una palabra reservada           */
/* -------------------------------------------------------------------------- */
int esPalabraReservada()
{
    int i;
    for (i=0; i<CANTPR; i++)
    {
        if (strcmp(TOS[i].nombre,token)==0) // strcmpi lo hacia sin diferenciar Mayus/Minus
        {
            return TOS[i].token;
        }
    }

    return ID;
}

/* -------------------------------------------------------------------------- */
/* agregarPalabrasReservadas(): Agrega las Palabras reservadas a la Tabla de S*/
/* -------------------------------------------------------------------------- */
void agregarPalabrasReservadas()
{
    TOStop=0;
    strcpy(TOS[TOStop].nombre, "DECLARE");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_DECLARE;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "ENDDECLARE");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_ENDDECLARE;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "INT");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_INT;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "REAL");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_REAL;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "STRING");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_STRING;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "CONST");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_CONST;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "WHILE");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_WHILE;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "LET");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_LET;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "IF");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_IF;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "QEqual");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_QEQUAL;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "ELSE");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_ELSE;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "ENDIF");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_ENDIF;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "BEGIN");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_BEGIN;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "END");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_END;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "AND");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_AND;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "OR");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_OR;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "GET");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_GET;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "PUT");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_PUT;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "NOT");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_NOT;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "DEFAULT");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_DEFAULT;
    TOStop++;

    strcpy(TOS[TOStop].nombre, "THEN");
    strcpy(TOS[TOStop].tipo, "PR");
	TOS[TOStop].token = PR_THEN;
    TOStop++;
}

void Validaciones()
{
    if (comAbierto != 0)
    {
        printf("\n ERROR: COMENTARIO no cerrado correctamente (no balancea). \n");
        error=1;
    }

    if (cteStrAbierta !=0)
    {
        printf("\n ERROR: CTE STRING no cerrada correctamente (no balancea). \n");
        error=1;
    }

    if ( (strcmp(token,"&")==0) || (strcmp(token,"|")==0) )
    {
        printf("\n ERROR: Se esperaba %s \n", token);
        exit(1);
    }

    if ( NroToken==CTE_ENT || NroToken==CTE_REAL)
    {
        Inf_Cte();
    }
}

/* -------------------------------------------------------------------------- */
/*                               FUNCION YYLEX                                */
/* -------------------------------------------------------------------------- */
int yylex() //FUNCION  QUE LEE HASTA FINAL DE TOKEN O EOF
{
	int estado=0;
	int columna;

    leerCaracter();

    if (caracter == EOF)
    {
        return EOF;
    }

	while(estado != ESTADO_FINAL && estado != NV)
	{
        columna = leerColumna(caracter);

        //Ejecuto la funcion de la matriz
		(*proceso[estado][columna])();

        //Luego cambio de estado
		estado = nEstado[estado][columna];

		if (estado != ESTADO_FINAL)
		{
		    leerCaracter();

		    if (caracter == EOF)
		    {
                Validaciones();

                if (NroToken == ID && error == 0)
                    Inf_ID();

                if (estado == 0)
                    return EOF;
				//printf("\nToken %d - %s", NroToken, token);
                return NroToken;
		    }
		}
    }
	//printf("\nToken %d - %s", NroToken, token);
    return NroToken;
}


/* -------------------------------------------------------------------------- */
/*                      COMPONENTES DE LA POLACA INVERSA                      */
/* -------------------------------------------------------------------------- */

#define ARCH_POLACA "polaca.txt"

FILE * archPolaca;

char operadorCond[5];
char operadorLogico[5];

typedef struct
{
    int    tipo;
    nodoTS nodo;
} nodoPolaca;

nodoPolaca polacaInversa[TAMMAX];
int nroNodoPolaca = 0;
int pilaSaltos[MAXLONG]; //NO SOPORTA MAS DE 30
int indexPilaSaltos = 0;
int posicionOperadorComparacion;
int esCondicionMultiple = 0;
int posSaltoPrimeraCondicion = 0;
int pilaOperadorLogico[100];
int indexPilaOperadorLogico = 0;

nodoPolaca polacaLetDefault[TAMMAX];
int nroNodoPolacaLetDefault = 0;

nodoPolaca polacaIdLet[TAMMAX];
int nroNodoPolacaIdLet = 0;

void imprimirPolacaInversa();
void insertarNodoEnPolaca(int, const nodoTS);
void insertarValorEnPolaca(int, const char *);
void asignarSalto(int, int);
void invertirOperadorCondicional();

void imprimirPolacaInversa()
{
    if ((archPolaca = fopen (ARCH_POLACA, "w"))== NULL)
    {
        printf("No se puede generar el archivo de polaca");
        getch();
        exit(1);
    }

    int i;

    for (i=0; i<nroNodoPolaca; i++)
    {
        fprintf(archPolaca, "%d - %s\n", i, polacaInversa[i].nodo.valor);
    }

    if(fclose(archPolaca)!=0)
    {
        printf("No se puede CERRAR el archivo de polaca");
        getch();
        exit(1);
    }
}

void insertarNodoEnPolaca(int tipo, const nodoTS nodo)
{
	if(esLETDEFAULT){
		polacaLetDefault[nroNodoPolacaLetDefault].tipo = tipo;
		polacaLetDefault[nroNodoPolacaLetDefault].nodo = nodo;
		nroNodoPolacaLetDefault++;
	} else {
		polacaInversa[nroNodoPolaca].tipo = tipo;
		polacaInversa[nroNodoPolaca].nodo = nodo;
		nroNodoPolaca++;
	}

}

void insertarValorEnPolaca(int tipo, const char * valor)
{
	if(esLETDEFAULT){
		polacaLetDefault[nroNodoPolacaLetDefault].tipo = tipo;
		strcpy(polacaLetDefault[nroNodoPolacaLetDefault].nodo.valor, valor);
		nroNodoPolacaLetDefault++;
	} else {
		polacaInversa[nroNodoPolaca].tipo = tipo;
		strcpy(polacaInversa[nroNodoPolaca].nodo.valor, valor);
		nroNodoPolaca++;
	}

}

void asignarSalto(int posicion, int salto)
{
    sprintf(polacaInversa[posicion].nodo.valor, "%d", salto);
}

void invertirOperadorCondicional()
{
    char res[5];

    if ( strcmp(polacaInversa[posicionOperadorComparacion].nodo.valor, "BEQ") == 0 )
        strcpy(res, "BNE");
    else if ( strcmp(polacaInversa[posicionOperadorComparacion].nodo.valor, "BGE") == 0 )
        strcpy(res, "BLT");
    else if ( strcmp(polacaInversa[posicionOperadorComparacion].nodo.valor, "BGT") == 0 )
        strcpy(res, "BLE");
    else if ( strcmp(polacaInversa[posicionOperadorComparacion].nodo.valor, "BLE") == 0 )
        strcpy(res, "BGT");
    else if ( strcmp(polacaInversa[posicionOperadorComparacion].nodo.valor, "BLT") == 0 )
        strcpy(res, "BGE");
    else
        strcpy(res, "BEQ");

    strcpy(polacaInversa[posicionOperadorComparacion].nodo.valor, res);
}

/* -------------------------------------------------------------------------- */
/*                                      MAIN                                  */
/* -------------------------------------------------------------------------- */


int yyparse(void);
void warning(char *, char *);



/* -------------------------------------------------------------------------- */
/*                                      MAIN                                  */
/* -------------------------------------------------------------------------- */
int main(int argc, char* argv[])
{
    if (argc > 1)
        abrirCodigoFuente(argv[1]);
    else
        abrirCodigoFuente("prueba.txt");

    agregarPalabrasReservadas();

    /*-----------------CICLO QUE LEE HASTA FIN DE ARCHIVO (MAIN)-----------------*/

    yyparse();

    /*---------------------------------------------------------------------------*/

    cerrarCodigoFuente();

    if (error==0){
		printf("\nCOMPILACION EXITOSA! %d\n",indexPilaSaltos);
		mostrarTOS();
		imprimirPolacaInversa();
	} else
        return 1;

    return 0;
}

void warning(char *s, char *t){
	fprintf(stderr, "\n%s", s);
	if (t)
		fprintf(stderr, " %s", t);
	fprintf(stderr, " en la linea %d.\nLexema: %s\nCaracter: %c\n", nroLinea, token, caracter);
}

int yyerror(char *s){
	warning(s, (char *) 0);
	mostrarTOS();
	exit(1);
}
FILE *yytfilep;
char *yytfilen;
int yytflag = 0;
int svdprd[2];
char svdnams[2][2];

int yyexca[] = {
  -1, 1,
  0, -1,
  -2, 0,
  -1, 37,
  301, 63,
  -2, 70,
  -1, 47,
  280, 25,
  -2, 63,
  -1, 48,
  280, 26,
  -2, 50,
  0,
};

#define YYNPROD 73
#define YYLAST 284

int yyact[] = {
      70,      71,      70,      71,     131,      70,      71,      88,
      84,      85,      86,      87,     132,      64,      94,      27,
      25,      24,      23,      95,      65,      46,      20,      52,
      53,      61,      49,     123,      46,      89,     117,     134,
      96,      46,      68,     121,     111,      59,      20,      52,
      53,      61,      49,      82,      63,      20,      52,      53,
      61,      49,      47,      52,      53,      48,      49,      18,
      15,     106,     136,      70,      71,      16,      17,      14,
      19,      18,      15,      78,     125,      75,      54,      16,
      17,      14,      19,      38,      74,     105,      18,      15,
      30,      37,      20,     124,      16,      17,      14,      19,
       3,      18,      15,      20,      20,      18,      15,      16,
      17,      14,      19,      16,      17,      14,      19,     102,
      20,      20,     103,      33,     107,      81,      80,      72,
      73,      70,      71,      39,      20,      28,     130,     122,
      20,     114,     115,     116,      50,      13,      42,      58,
      45,      13,      60,      55,      67,      62,      34,     128,
      26,      44,      50,      43,      51,      31,      32,      29,
      36,       5,      83,     108,      79,       4,      22,      16,
      57,      56,      35,     126,      17,      19,      41,      40,
      12,     106,      11,      10,       9,       8,       7,       6,
       5,     113,      69,      39,      93,      66,      21,       2,
       1,       0,       0,       0,      76,       0,       0,       0,
       0,       0,      77,       0,      36,       0,       0,       0,
       0,      90,       0,       0,      92,      97,      98,       0,
      49,      99,     100,       0,       0,       0,     104,      62,
       0,      55,     109,       0,     101,       0,     110,       0,
       0,       0,      91,       0,       0,      65,       0,      75,
      71,      42,       0,     112,      73,      45,       0,      13,
      13,       0,       0,      83,     120,     108,       0,      44,
       0,     123,       0,      13,      13,      67,      35,      13,
      58,       0,       0,       0,      13,       0,     129,      13,
     118,     119,      13,       0,     132,       0,       0,     135,
      22,      22,      60,       0,     107,     122,       0,       0,
     127,      22,       0,       0,       0,     130,       0,      22,
     133,       0,       0,       4,
};

int yypact[] = {
    -169,   -1000,   -1000,   -1000,    -173,   -1000,    -285,   -1000,
   -1000,    -286,    -287,    -198,    -288,    -158,   -1000,    -213,
    -198,    -185,   -1000,    -208,   -1000,    -218,   -1000,   -1000,
   -1000,   -1000,    -160,   -1000,    -239,    -223,    -251,   -1000,
   -1000,   -1000,    -260,   -1000,    -281,   -1000,    -255,    -244,
    -163,   -1000,    -167,   -1000,    -204,   -1000,    -224,   -1000,
   -1000,    -244,   -1000,   -1000,   -1000,   -1000,    -251,    -227,
   -1000,   -1000,    -172,    -250,    -276,   -1000,    -244,    -208,
   -1000,    -244,    -290,   -1000,    -282,    -271,    -267,    -267,
    -267,    -267,    -186,    -244,    -217,    -237,    -166,   -1000,
   -1000,   -1000,    -244,    -244,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,    -163,   -1000,    -163,    -258,    -255,    -138,
   -1000,    -167,    -167,   -1000,   -1000,   -1000,   -1000,   -1000,
    -274,   -1000,    -173,    -173,    -244,    -259,    -163,    -139,
   -1000,   -1000,   -1000,   -1000,   -1000,    -268,    -184,    -197,
   -1000,   -1000,    -173,    -244,   -1000,   -1000,    -146,    -173,
    -292,    -163,    -173,    -263,    -244,    -207,   -1000,    -163,
   -1000,
};

int yypgo[] = {
       0,     176,     175,     174,     173,     172,     149,     132,
     169,     145,     167,     166,     165,     164,     163,     162,
     124,     130,     160,     158,     137,     131,     155,     153,
     152,     127,     148,     147,     146,     143,     140,     126,
     139,     128,     135,     134,     133,     154,
};

int yyr1[] = {
       0,       1,       3,       5,       2,       2,       4,       4,
       7,       8,       8,       8,       6,       6,       9,       9,
       9,       9,       9,       9,       9,      15,      10,      10,
      19,      20,      20,      12,      22,      12,      21,      21,
      23,      27,      24,      24,      25,      28,      28,      28,
      28,      28,      28,      26,      26,      29,      11,      13,
      14,      14,      30,      30,      30,      17,      17,      17,
      17,      31,      31,      31,      33,      33,      33,      16,
      32,      34,      34,      18,      36,      37,      37,      35,
      35,
};

int yyr2[] = {
       2,       1,       0,       0,       8,       1,       1,       3,
       3,       1,       1,       1,       1,       2,       2,       1,
       1,       2,       2,       5,       2,       1,       3,       3,
       3,       1,       1,       7,       0,      10,       1,       1,
       1,       0,       4,       4,       3,       1,       1,       1,
       1,       1,       1,       1,       1,       0,       7,       2,
       2,       2,       1,       1,       1,       3,       3,       1,
       1,       3,       3,       1,       3,       1,       1,       1,
       8,       1,       3,       4,       1,       3,       1,       1,
       3,
};

int yychk[] = {
   -1000,      -1,      -2,     257,      -6,      -9,     -10,     -11,
     -12,     -13,     -14,     -15,     -18,     -16,     270,     263,
     268,     269,     262,     271,     289,      -3,      -9,     303,
     303,     303,     -16,     303,     275,     -29,     293,     -16,
     -16,     292,     -35,     -37,     -16,     289,     293,     275,
     -17,     -19,     -31,     -32,     -20,     -33,     272,     289,
     292,     293,     -16,     -30,     290,     291,     293,     -21,
     -23,     -24,     -25,     288,     -17,     292,     -36,     304,
     273,     301,      -4,      -7,     289,     -17,     276,     277,
     278,     279,     280,     293,     -17,     -21,     294,     -26,
     282,     281,     293,     -28,     284,     285,     286,     287,
     283,     305,     -17,     -37,     -17,      -5,     304,     301,
     303,     -31,     -31,     -33,     -33,     -20,     289,     292,
     -17,     294,     294,     274,     -27,     -25,     -17,     294,
      -7,      -8,     259,     260,     261,     304,      -6,      -6,
     -25,     294,     258,     295,     267,     265,     -22,      -6,
     -34,     -17,     264,     296,     304,      -6,     294,     -17,
     265,
};

int yydef[] = {
       0,      -2,       1,       2,       5,      12,       0,      15,
      16,       0,       0,       0,       0,       0,      45,       0,
       0,       0,      21,       0,      63,       0,      13,      14,
      17,      18,       0,      20,       0,       0,       0,      47,
      48,      49,       0,      71,       0,      -2,       0,       0,
      22,      23,      55,      56,       0,      59,       0,      -2,
      -2,       0,      61,      62,      51,      52,       0,       0,
      30,      31,      32,       0,       0,      50,       0,       0,
      68,       0,       3,       6,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,      33,
      43,      44,       0,       0,      37,      38,      39,      40,
      41,      42,      67,      72,      69,       0,       0,       0,
      19,      53,      54,      57,      58,      24,      25,      26,
       0,      60,       0,       0,       0,       0,      36,       0,
       7,       8,       9,      10,      11,       0,       0,      28,
      34,      35,       0,       0,      46,      27,       0,       4,
       0,      65,       0,       0,       0,       0,      64,      66,
      29,
};

int *yyxi;


/*****************************************************************/
/* PCYACC LALR parser driver routine -- a table driven procedure */
/* for recognizing sentences of a language defined by the        */
/* grammar that PCYACC analyzes. An LALR parsing table is then   */
/* constructed for the grammar and the skeletal parser uses the  */
/* table when performing syntactical analysis on input source    */
/* programs. The actions associated with grammar rules are       */
/* inserted into a switch statement for execution.               */
/*****************************************************************/


#ifndef YYMAXDEPTH
#define YYMAXDEPTH 200
#endif
#ifndef YYREDMAX
#define YYREDMAX 1000
#endif
#define PCYYFLAG -1000
#define WAS0ERR 0
#define WAS1ERR 1
#define WAS2ERR 2
#define WAS3ERR 3
#define yyclearin pcyytoken = -1
#define yyerrok   pcyyerrfl = 0
YYSTYPE yyv[YYMAXDEPTH];     /* value stack */
int pcyyerrct = 0;           /* error count */
int pcyyerrfl = 0;           /* error flag */
int redseq[YYREDMAX];
int redcnt = 0;
int pcyytoken = -1;          /* input token */


int yyparse()
{
  int statestack[YYMAXDEPTH]; /* state stack */
  int      j, m;              /* working index */
  YYSTYPE *yypvt;
  int      tmpstate, tmptoken, *yyps, n;
  YYSTYPE *yypv;


  tmpstate = 0;
  pcyytoken = -1;
#ifdef YYDEBUG
  tmptoken = -1;
#endif
  pcyyerrct = 0;
  pcyyerrfl = 0;
  yyps = &statestack[-1];
  yypv = &yyv[-1];


  enstack:    /* push stack */
#ifdef YYDEBUG
    printf("at state %d, next token %d\n", tmpstate, tmptoken);
#endif
    if (++yyps - &statestack[YYMAXDEPTH] > 0) {
      yyerror("pcyacc internal stack overflow");
      return(1);
    }
    *yyps = tmpstate;
    ++yypv;
    *yypv = yyval;


  newstate:
    n = yypact[tmpstate];
    if (n <= PCYYFLAG) goto defaultact; /*  a simple state */


    if (pcyytoken < 0) if ((pcyytoken=yylex()) < 0) pcyytoken = 0;
    if ((n += pcyytoken) < 0 || n >= YYLAST) goto defaultact;


    if (yychk[n=yyact[n]] == pcyytoken) { /* a shift */
#ifdef YYDEBUG
      tmptoken  = pcyytoken;
#endif
      pcyytoken = -1;
      yyval = yylval;
      tmpstate = n;
      if (pcyyerrfl > 0) --pcyyerrfl;
      goto enstack;
    }


  defaultact:


    if ((n=yydef[tmpstate]) == -2) {
      if (pcyytoken < 0) if ((pcyytoken=yylex())<0) pcyytoken = 0;
      for (yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=tmpstate); yyxi += 2);
      while (*(yyxi+=2) >= 0) if (*yyxi == pcyytoken) break;
      if ((n=yyxi[1]) < 0) { /* an accept action */
        if (yytflag) {
          int ti; int tj;
          yytfilep = fopen(yytfilen, "w");
          if (yytfilep == NULL) {
            fprintf(stderr, "Can't open t file: %s\n", yytfilen);
            return(0);          }
          for (ti=redcnt-1; ti>=0; ti--) {
            tj = svdprd[redseq[ti]];
            while (strcmp(svdnams[tj], "$EOP"))
              fprintf(yytfilep, "%s ", svdnams[tj++]);
            fprintf(yytfilep, "\n");
          }
          fclose(yytfilep);
        }
        return (0);
      }
    }


    if (n == 0) {        /* error situation */
      switch (pcyyerrfl) {
        case WAS0ERR:          /* an error just occurred */
          yyerror("syntax error");
          yyerrlab:
            ++pcyyerrct;
        case WAS1ERR:
        case WAS2ERR:           /* try again */
          pcyyerrfl = 3;
	   /* find a state for a legal shift action */
          while (yyps >= statestack) {
	     n = yypact[*yyps] + YYERRCODE;
	     if (n >= 0 && n < YYLAST && yychk[yyact[n]] == YYERRCODE) {
	       tmpstate = yyact[n];  /* simulate a shift of "error" */
	       goto enstack;
            }
	     n = yypact[*yyps];


	     /* the current yyps has no shift on "error", pop stack */
#ifdef YYDEBUG
            printf("error: pop state %d, recover state %d\n", *yyps, yyps[-1]);
#endif
	     --yyps;
	     --yypv;
	   }


	   yyabort:
            if (yytflag) {
              int ti; int tj;
              yytfilep = fopen(yytfilen, "w");
              if (yytfilep == NULL) {
                fprintf(stderr, "Can't open t file: %s\n", yytfilen);
                return(1);              }
              for (ti=1; ti<redcnt; ti++) {
                tj = svdprd[redseq[ti]];
                while (strcmp(svdnams[tj], "$EOP"))
                  fprintf(yytfilep, "%s ", svdnams[tj++]);
                fprintf(yytfilep, "\n");
              }
              fclose(yytfilep);
            }
	     return(1);


	 case WAS3ERR:  /* clobber input char */
#ifdef YYDEBUG
          printf("error: discard token %d\n", pcyytoken);
#endif
          if (pcyytoken == 0) goto yyabort; /* quit */
	   pcyytoken = -1;
	   goto newstate;      } /* switch */
    } /* if */


    /* reduction, given a production n */
#ifdef YYDEBUG
    printf("reduce with rule %d\n", n);
#endif
    if (yytflag && redcnt<YYREDMAX) redseq[redcnt++] = n;
    yyps -= yyr2[n];
    yypvt = yypv;
    yypv -= yyr2[n];
    yyval = yypv[1];
    m = n;
    /* find next state from goto table */
    n = yyr1[n];
    j = yypgo[n] + *yyps + 1;
    if (j>=YYLAST || yychk[ tmpstate = yyact[j] ] != -n) tmpstate = yyact[yypgo[n]];
    switch (m) { /* actions associated with grammar rules */

      case 2:
# line 54 "AS.y"
      { enDECLARE = 1;
      						printf("Seteando EnDeclare %d", enDECLARE);} break;
      case 3:
# line 55 "AS.y"
      { enDECLARE = 0;
      						printf("Seteando EnDeclare %d", enDECLARE);} break;
      case 8:
# line 64 "AS.y"
      {
      												#ifdef MI_DEBUG
      													printf("\nDeclaracion de variable\n");
      												#endif
      												TOS[yypvt[-2]].tipo_dato = tipo_dato;
      											} break;
      case 9:
# line 72 "AS.y"
      { tipo_dato = PR_INT; } break;
      case 10:
# line 73 "AS.y"
      { tipo_dato = PR_REAL; } break;
      case 11:
# line 74 "AS.y"
      { tipo_dato = PR_STRING; } break;
      case 14:
# line 81 "AS.y"
      {

      									#ifdef MI_DEBUG
      										printf(":=\n");
      									#endif
      									insertarValorEnPolaca(1, ":=");
      								} break;
      case 19:
# line 92 "AS.y"
      {
      																			esCONST = 0;
      																			printf("\nCONST id %d, exp %d",yypvt[-3], yypvt[-1]);
      																			strcpy(TOS[yypvt[-3]].valor, TOS[yypvt[-1]].valor);
      																			TOS[yypvt[-3]].tipo_dato = TOS[yypvt[-1]].tipo_dato;
      																		} break;
      case 21:
# line 101 "AS.y"
      { esCONST = 1;} break;
      case 22:
# line 104 "AS.y"
      {
      										#ifdef MI_DEBUG
      											printf("\nAsignacion\n");
      											printf("%s\n",buscarEnTOS(yypvt[-2]));
      										#endif
      										if(strcmp(TOS[yypvt[-2]].tipo,"CONST") == 0)
      											yyerror("No se puede realizar una asignacion a una constante con nombre.");
      										if(TOS[yypvt[-2]].tipo_dato != TOS[yypvt[-0]].tipo_dato)
      											yyerror("Tipos de dato incompatibles para la asignacion.");

      									} break;
      case 24:
# line 118 "AS.y"
      {
      										#ifdef MI_DEBUG
      											printf("++\n");
      										#endif
      										if(TOS[yypvt[-2]].tipo_dato != PR_STRING || TOS[yypvt[-0]].tipo_dato != PR_STRING)
      												yyerror("Solo se pueden concatenar tipo de dato STRING");
      										insertarNodoEnPolaca(0, TOS[yypvt[-2]]);
      										insertarNodoEnPolaca(0, TOS[yypvt[-0]]);
      										insertarValorEnPolaca(1, "++");
      									  } break;
      case 27:
# line 132 "AS.y"
      {
      																				#ifdef MI_DEBUG
      																					printf("\nSentencia IF\n");
      																				#endif
      																				int aux = pilaSaltos[--indexPilaSaltos];
      																				asignarSalto(aux, nroNodoPolaca);
      																				if(esCondicionMultiple == 1){
      																					int aux2 = pilaSaltos[--indexPilaSaltos];
      																					if(pilaOperadorLogico[--indexPilaOperadorLogico] == PR_AND){
      																						asignarSalto(aux2, nroNodoPolaca);
      																					}
      																				}

      																			} break;
      case 28:
# line 146 "AS.y"
      {
      																		#ifdef MI_DEBUG
      																			printf("\nFin del THEN\n");
      																		#endif
      																		int aux = pilaSaltos[--indexPilaSaltos];
      																		asignarSalto(aux, nroNodoPolaca+2);
      																		if(esCondicionMultiple == 1){
      																			int aux2 = pilaSaltos[--indexPilaSaltos];
      																			if(pilaOperadorLogico[--indexPilaOperadorLogico] == PR_AND){
      																				asignarSalto(aux2, nroNodoPolaca+2);
      																			}
      																		}
      																		pilaSaltos[indexPilaSaltos++] = nroNodoPolaca;
      																		insertarValorEnPolaca(1, "");
      																		insertarValorEnPolaca(1, "BI");
      																	} break;
      case 29:
# line 161 "AS.y"
      {
      																										#ifdef MI_DEBUG
      																											printf("\nFin del ELSE\n");
      																										#endif
      																										int aux = pilaSaltos[--indexPilaSaltos];
      																										asignarSalto(aux, nroNodoPolaca);

      																									} break;
      case 30:
# line 171 "AS.y"
      {
      							#ifdef MI_DEBUG
      								printf("\nCondicion simple\n");
      							#endif
      							esCondicionMultiple = 0;
      						} break;
      case 33:
# line 183 "AS.y"
      {
      											#ifdef MI_DEBUG
      												printf("%s\n", operadorLogico);
      											#endif
      											pilaOperadorLogico[indexPilaOperadorLogico++] = yypvt[-0];
      											if(yypvt[-0] == PR_OR){
      												invertirOperadorCondicional();
      												posSaltoPrimeraCondicion = nroNodoPolaca-2;
      												#ifdef MI_DEBUG
      													printf("%s\n", "Apilando salto para OR");
      												#endif
      											}
      										} break;
      case 34:
# line 197 "AS.y"
      {
      														#ifdef MI_DEBUG
      															printf("%s\n", operadorLogico);
      														#endif
      														if(yypvt[-2] == PR_OR){
      															asignarSalto(posSaltoPrimeraCondicion, nroNodoPolaca);
      															#ifdef MI_DEBUG
      																printf("%s\n", "Asignando salto para OR");
      															#endif
      														}
      														esCondicionMultiple = 1;
      													} break;
      case 35:
# line 209 "AS.y"
      {
      															#ifdef MI_DEBUG
      																printf("NOT\n");
      															#endif
      															invertirOperadorCondicional();
      															esCondicionMultiple = 0;
      														} break;
      case 36:
# line 218 "AS.y"
      {
      													insertarValorEnPolaca(1, "CMP");
      													pilaSaltos[indexPilaSaltos++] = nroNodoPolaca;
      													insertarValorEnPolaca(0, "");
      													posicionOperadorComparacion = nroNodoPolaca;
      													insertarValorEnPolaca(1, operadorCond);
      												} break;
      case 37:
# line 227 "AS.y"
      { strcpy(operadorCond, "BGE"); } break;
      case 38:
# line 228 "AS.y"
      { strcpy(operadorCond, "BGT"); } break;
      case 39:
# line 229 "AS.y"
      { strcpy(operadorCond, "BLE"); } break;
      case 40:
# line 230 "AS.y"
      { strcpy(operadorCond, "BLT"); } break;
      case 41:
# line 231 "AS.y"
      { strcpy(operadorCond, "BNE"); } break;
      case 42:
# line 232 "AS.y"
      { strcpy(operadorCond, "BEQ"); } break;
      case 43:
# line 235 "AS.y"
      { strcpy(operadorLogico, "AND"); } break;
      case 44:
# line 236 "AS.y"
      { strcpy(operadorLogico, "OR"); } break;
      case 45:
# line 240 "AS.y"
      {
      						#ifdef MI_DEBUG
      							printf("\nComienzo Iteracion WHILE\n");
      						#endif

      						pilaSaltos[indexPilaSaltos++] = nroNodoPolaca;
      					} break;
      case 46:
# line 246 "AS.y"
      {
      																			#ifdef MI_DEBUG
      																				printf("\nFin Iteracion WHILE\n");
      																			#endif
      																			int aux = pilaSaltos[--indexPilaSaltos];
      																			asignarSalto(aux, nroNodoPolaca +2);
      																			if(esCondicionMultiple == 1){
      																				int aux2 = pilaSaltos[--indexPilaSaltos];
      																				if(pilaOperadorLogico[--indexPilaOperadorLogico] == PR_AND){
      																					asignarSalto(aux2, nroNodoPolaca + 2);
      																				}
      																			}

      																			aux = pilaSaltos[--indexPilaSaltos];
      																			insertarValorEnPolaca(1, "");
      																			asignarSalto(nroNodoPolaca-1, aux);
      																			insertarValorEnPolaca(1, "BI");
      																		} break;
      case 47:
# line 266 "AS.y"
      {
      						#ifdef MI_DEBUG
      							printf("GET\n");
      						#endif
      						insertarValorEnPolaca(1, "GET");
      					} break;
      case 48:
# line 274 "AS.y"
      {
      						#ifdef MI_DEBUG
      							printf("PUT\n");
      						#endif
      						insertarValorEnPolaca(1, "PUT");
      					} break;
      case 49:
# line 280 "AS.y"
      {
      								#ifdef MI_DEBUG
      									printf("PUT\n");
      								#endif
      								insertarValorEnPolaca(1, "PUT");
      							} break;
      case 50:
# line 288 "AS.y"
      {  tipo_dato = PR_STRING;
      					if(!esCONST)
      					{
      						#ifdef MI_DEBUG
      							printf("%s\n",buscarEnTOS(yypvt[-0]));
      						#endif
      						insertarNodoEnPolaca(0, TOS[yypvt[-0]]);
      					}
      				 } break;
      case 51:
# line 297 "AS.y"
      {   tipo_dato = PR_INT;
      						if(!esCONST)
      						{
      							#ifdef MI_DEBUG
      								printf("%s\n",buscarEnTOS(yypvt[-0]));
      							#endif
      							insertarNodoEnPolaca(0, TOS[yypvt[-0]]);
      						}
      					} break;
      case 52:
# line 306 "AS.y"
      {   tipo_dato = PR_REAL;
      								if(!esCONST)
      								{
      									#ifdef MI_DEBUG
      										printf("%s\n",buscarEnTOS(yypvt[-0]));
      									#endif
      									insertarNodoEnPolaca(0, TOS[yypvt[-0]]);
      								}
      				   } break;
      case 53:
# line 317 "AS.y"
      {
      											#ifdef MI_DEBUG
      												printf("+\n");
      											#endif
      											if(TOS[yypvt[-2]].tipo_dato == PR_STRING || TOS[yypvt[-0]].tipo_dato == PR_STRING)
      												yyerror("No es posible realizar una suma con un tipo de dato STRING");
      											insertarValorEnPolaca(1, "+");
      										} break;
      case 54:
# line 325 "AS.y"
      {
      												#ifdef MI_DEBUG
      													printf("-\n");
      												#endif
      												if(TOS[yypvt[-2]].tipo_dato == PR_STRING || TOS[yypvt[-0]].tipo_dato == PR_STRING)
      													yyerror("No es posible realizar una resta con un tipo de dato STRING");
      												insertarValorEnPolaca(1, "-");
      											} break;
      case 57:
# line 337 "AS.y"
      {
      											#ifdef MI_DEBUG
      												printf("*\n");
      												printf("Primer op %s. Segundo op %s\n", TOS[yypvt[-2]].nombre, TOS[yypvt[-0]].nombre);
      											#endif
      											if(TOS[yypvt[-2]].tipo_dato == PR_STRING || TOS[yypvt[-0]].tipo_dato == PR_STRING)
      												yyerror("No es posible realizar una multiplicacion con un tipo de dato STRING");
      											insertarValorEnPolaca(1, "*");
      										} break;
      case 58:
# line 346 "AS.y"
      {
      											#ifdef MI_DEBUG
      												printf("/\n");
      												printf("Primer op %s. Segundo op %s\n", TOS[yypvt[-2]].nombre, TOS[yypvt[-0]].nombre);
      											#endif
      											if(TOS[yypvt[-2]].tipo_dato == PR_STRING || TOS[yypvt[-0]].tipo_dato == PR_STRING)
      												yyerror("No es posible realizar una division con un tipo de dato STRING");
      											insertarValorEnPolaca(1, "/");
      										} break;
      case 60:
# line 358 "AS.y"
      {
      											yyval = yypvt[-1];
      										} break;
      case 63:
# line 365 "AS.y"
      {
      				#ifdef MI_DEBUG
      					printf("%s\n",buscarEnTOS(yypvt[-0]));
      				#endif
      				if(!esCONST)
      					insertarNodoEnPolaca(0, TOS[yypvt[-0]]);
      				yyval = yypvt[-0];
      			} break;
      case 64:
# line 375 "AS.y"
      { printf("\nQequal\n"); } break;
      case 67:
# line 380 "AS.y"
      {
      													esLETDEFAULT = 0;
      													printf("\nLET\n");
      													for(int i = 0; i < nroNodoPolacaIdLet; i++){
      														//chequeo tipos para asignacion
      														if(polacaIdLet[i].nodo.tipo_dato != polacaLetDefault[0].nodo.tipo_dato)
      															yyerror("Tipo de dato incompatible en la asignacion del valor por default");
      														// inserto ID que no tenia expresion
      														polacaInversa[nroNodoPolaca] = polacaIdLet[i];
      														nroNodoPolaca++;
      														// inserto expresion por default
      														for(int j = 0; j < nroNodoPolacaLetDefault; j++){
      															polacaInversa[nroNodoPolaca] = polacaLetDefault[j];
      															nroNodoPolaca++;
      														}
      														insertarValorEnPolaca(1, ":=");
      													}
      													nroNodoPolacaIdLet = 0;
      													nroNodoPolacaLetDefault = 0;

      												} break;
      case 68:
# line 403 "AS.y"
      {
      								esLETDEFAULT = 1;
      							} break;
      case 69:
# line 409 "AS.y"
      {
      							insertarValorEnPolaca(1, ":=");
      							printf("Tipo del ID: %d. Tipo de la exp: %d, Separador: %d", yypvt[-2] ,yypvt[-0], yypvt[-1]);
      							if(TOS[yypvt[-2]].tipo_dato != TOS[yypvt[-0]].tipo_dato){
      								yyerror("Tipos de dato incompatibles para la asignacion.");
      							}
      						} break;
      case 70:
# line 416 "AS.y"
      {
      									polacaIdLet[nroNodoPolacaIdLet].tipo = 0;
      									polacaIdLet[nroNodoPolacaIdLet].nodo = TOS[yypvt[-0]];
      									nroNodoPolacaIdLet++;
      								} break;    }
    goto enstack;
}
