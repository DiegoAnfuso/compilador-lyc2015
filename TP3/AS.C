
# line 9 "AS.y"
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MI_DEBUG 1
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

# line 352 "AS.y"


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
#define TAMMAX 100
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
int esCONST = 0;

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
			if (strcmp(TOS[i].nombre,lexema)==0)
				return i;
		}
	}

    switch (NroToken)
    {
        case ID:
            /*if (!enDECLARE) {
                yyerror("Variable no declarada.");
            }

            if (!esCONST)*/
                strcpy(TOS[TOStop].tipo,"ID" );
            /*else
                strcpy(TOS[TOStop].tipo,"CONST" );*/

            TOS[TOStop].tipo_dato = tipo_dato;
			strcpy(TOS[TOStop].nombre, lexema);
			strcpy(TOS[TOStop].valor, lexema);
            break;
        case CTE_ENT:
			strcpy(aux,"_");
            strcat(aux, lexema);
            strcpy(TOS[TOStop].tipo,"CTE_ENT");
            TOS[TOStop].tipo_dato = PR_INT;
			strcpy(TOS[TOStop].nombre, aux);
			strcpy(TOS[TOStop].valor, lexema);
            break;
        case CTE_REAL:
			strcpy(aux,"_");
            strcat(aux, lexema);
            strcpy(TOS[TOStop].tipo,"CTE_REAL");
            TOS[TOStop].tipo_dato = PR_REAL;
			strcpy(TOS[TOStop].nombre, aux);
			strcpy(TOS[TOStop].valor, lexema);
            break;
        case CTE_STRING:
			strcpy(aux,"_");
            strcat(aux, auxStr);
            strcpy(TOS[TOStop].tipo,"CTE_STRING" );
            TOS[TOStop].tipo_dato = PR_STRING;
            TOS[TOStop].longitud = (strlen(auxStr));
			strcpy(TOS[TOStop].nombre, aux);
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
    fprintf(tos,"Nro\t | Nombre\t\t\t | Tipo\t | Valor\t | Longitud \n");
    for (i=0; i<TOStop; i++)
    {
      //  printf ("%d     \t | %s     \t\t\t | %s     \t | %s \n",i,TOS[i].nombre, TOS[i].tipo, TOS[i].valor);
        fprintf(tos,"%d     \t | %s     \t\t\t | %s     \t | %s \t | %d \n",i,TOS[i].nombre, TOS[i].tipo, TOS[i].valor, TOS[i].longitud);
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
int pilaOperadorLogico[MAXLONG]; //NO SOPORTA MAS DE 30
int indexPilaOperadorLogico = 0;

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
    polacaInversa[nroNodoPolaca].tipo = tipo;
    polacaInversa[nroNodoPolaca].nodo = nodo;
    nroNodoPolaca++;
}

void insertarValorEnPolaca(int tipo, const char * valor)
{
    polacaInversa[nroNodoPolaca].tipo = tipo;
    strcpy(polacaInversa[nroNodoPolaca].nodo.valor, valor);
    nroNodoPolaca++;
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
int yyerror(char *s);
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
  -1, 46,
  280, 22,
  -2, 58,
  -1, 47,
  280, 23,
  -2, 47,
  0,
};

#define YYNPROD 68
#define YYLAST 277

int yyact[] = {
      68,      69,      68,      69,     125,      68,      69,      86,
      82,      83,      84,      85,     126,      64,      61,      25,
      23,      22,      21,      66,      45,      63,     118,      65,
      59,      50,      51,      60,      48,      87,     113,      45,
      97,     128,     117,      80,      57,      59,      50,      51,
      60,      48,      45,     107,      76,      62,      73,      52,
      59,      50,      51,      60,      48,     103,      30,      28,
     104,      31,      37,      46,      50,      51,      47,      48,
      11,      15,      19,     130,      68,      69,      16,      17,
      14,      18,      11,      15,      34,     120,      29,      24,
      16,      17,      14,      18,      79,      78,     106,      11,
      15,      70,      71,      13,     119,      16,      17,      14,
      18,       3,      11,      15,      72,      13,      11,      15,
      16,      17,      14,      18,      16,      17,      14,      18,
      38,      26,      13,      68,      69,     108,     124,      94,
      95,      96,      91,      58,       4,      13,      36,      43,
       5,      13,      33,      53,      32,      20,     122,      42,
      49,      27,      81,     109,      77,      55,      54,     121,
      56,      40,      12,      10,       9,       5,      39,       8,
       7,       6,      93,      35,      44,       2,       1,       0,
       0,       0,      67,      38,       0,       0,       0,       0,
       0,       0,       0,       0,      74,       0,       0,       0,
      41,       0,       0,       0,       0,       0,       0,       0,
      75,      88,       0,      90,       0,       0,       0,      48,
      92,      89,       0,       0,       0,     105,      61,       0,
     102,       0,       0,       0,       0,     111,       0,      53,
       0,       0,       0,       0,      63,       0,      73,       0,
     112,       0,       0,      36,      33,       0,       0,       0,
       0,     110,       0,     100,     101,      81,      43,       0,
     114,     115,       0,     118,       0,     107,       0,     108,
     124,      20,     123,      20,      20,      98,      99,     126,
       0,     127,     129,       0,       4,      58,     116,     109,
      20,      71,      44,       0,       0,      56,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,      69,      41,
};

int yypact[] = {
    -160,   -1000,   -1000,    -227,    -164,   -1000,    -285,   -1000,
   -1000,    -286,    -287,    -210,    -288,    -162,   -1000,    -238,
    -211,    -235,    -213,    -231,   -1000,   -1000,   -1000,   -1000,
    -163,   -1000,    -230,    -246,    -252,   -1000,   -1000,   -1000,
    -259,   -1000,    -280,    -281,   -1000,    -282,    -241,    -161,
   -1000,    -189,   -1000,    -180,   -1000,    -247,   -1000,   -1000,
    -241,   -1000,   -1000,   -1000,    -252,    -250,   -1000,   -1000,
    -197,    -258,    -276,   -1000,   -1000,    -241,    -213,    -241,
    -136,    -231,    -140,    -271,    -265,    -265,    -265,    -265,
    -236,    -241,    -208,    -251,    -157,   -1000,   -1000,   -1000,
    -241,    -241,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
    -161,   -1000,    -161,    -164,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,    -189,    -189,   -1000,   -1000,   -1000,   -1000,
   -1000,    -274,   -1000,    -164,    -164,    -241,    -260,    -161,
    -164,    -273,    -175,    -188,   -1000,   -1000,    -241,   -1000,
   -1000,    -146,    -292,    -161,    -164,    -261,    -241,    -198,
   -1000,    -161,   -1000,
};

int yypgo[] = {
       0,     158,     157,     155,     124,     126,     154,     128,
     153,     152,     151,     148,     147,     123,     146,     145,
     127,     131,     143,     142,     141,     144,     140,     139,
     138,     137,     136,     176,     135,     156,     134,     132,
     130,
};

int yyr1[] = {
       0,       1,       2,       2,       3,       3,       5,       6,
       6,       6,       4,       4,       7,       7,       7,       7,
       7,       7,       7,       8,       8,      15,      16,      16,
      10,      18,      10,      17,      17,      19,      23,      20,
      20,      21,      24,      24,      24,      24,      24,      24,
      22,      22,      25,       9,      11,      12,      12,      26,
      26,      26,      13,      13,      13,      13,      27,      27,
      27,      29,      29,      29,      28,      30,      30,      14,
      32,      32,      31,      31,
};

int yyr2[] = {
       2,       1,       6,       1,       1,       3,       3,       1,
       1,       1,       1,       2,       2,       1,       1,       2,
       2,       5,       2,       3,       3,       3,       1,       1,
       7,       0,      10,       1,       1,       1,       0,       4,
       4,       3,       1,       1,       1,       1,       1,       1,
       1,       1,       0,       7,       2,       2,       2,       1,
       1,       1,       3,       3,       1,       1,       3,       3,
       1,       3,       1,       1,       8,       1,       3,       4,
       3,       1,       1,       3,
};

int yychk[] = {
   -1000,      -1,      -2,     257,      -4,      -7,      -8,      -9,
     -10,     -11,     -12,     262,     -14,     289,     270,     263,
     268,     269,     271,     293,      -7,     303,     303,     303,
     289,     303,     275,     -25,     293,     289,     289,     292,
     -31,     -32,     289,      -3,      -5,     289,     275,     -13,
     -15,     -27,     -28,     -16,     -29,     272,     289,     292,
     293,     -26,     290,     291,     293,     -17,     -19,     -20,
     -21,     288,     -13,     289,     292,     273,     304,     301,
     294,     304,     301,     -13,     276,     277,     278,     279,
     280,     293,     -13,     -17,     294,     -22,     282,     281,
     293,     -24,     284,     285,     286,     287,     283,     305,
     -13,     -32,     -13,     258,      -5,      -6,     259,     260,
     261,     303,     -27,     -27,     -29,     -29,     -16,     289,
     292,     -13,     294,     294,     274,     -23,     -21,     -13,
      -4,     304,      -4,      -4,     -21,     294,     295,     267,
     265,     -18,     -30,     -13,     264,     296,     304,      -4,
     294,     -13,     265,
};

int yydef[] = {
       0,      -2,       1,       0,       3,      10,       0,      13,
      14,       0,       0,       0,       0,       0,      42,       0,
       0,       0,       0,       0,      11,      12,      15,      16,
       0,      18,       0,       0,       0,      44,      45,      46,
       0,      66,      65,       0,       4,       0,       0,      19,
      20,      52,      53,       0,      56,       0,      -2,      -2,
       0,      59,      48,      49,       0,       0,      27,      28,
      29,       0,       0,      58,      47,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,      30,      40,      41,
       0,       0,      34,      35,      36,      37,      38,      39,
      63,      67,      64,       0,       5,       6,       7,       8,
       9,      17,      50,      51,      54,      55,      21,      22,
      23,       0,      57,       0,       0,       0,       0,      33,
       2,       0,       0,      25,      31,      32,       0,      43,
      24,       0,       0,      61,       0,       0,       0,       0,
      60,      62,      26,
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

      case 6:
# line 55 "AS.y"
      {
      												#ifdef MI_DEBUG
      													printf("\nDeclaracion de variable\n");
      												#endif
      											} break;
      case 7:
# line 62 "AS.y"
      { tipo_dato = PR_INT; } break;
      case 8:
# line 63 "AS.y"
      { tipo_dato = PR_REAL; } break;
      case 9:
# line 64 "AS.y"
      { tipo_dato = PR_STRING; } break;
      case 12:
# line 71 "AS.y"
      {
      									#ifdef MI_DEBUG
      										printf("%s\n",buscarEnTOS(yypvt[-1]));
      									#endif
      									insertarNodoEnPolaca(0, TOS[yypvt[-1]]);

									#ifdef MI_DEBUG
      										printf(":=\n");
      									#endif
      									insertarValorEnPolaca(1, ":=");
      								} break;
      case 19:
# line 90 "AS.y"
      {
      										#ifdef MI_DEBUG
      											printf("\nAsignacion\n");
      										#endif
      									} break;
      case 21:
# line 98 "AS.y"
      {
      										#ifdef MI_DEBUG
      											printf("++\n");
      										#endif
      										insertarNodoEnPolaca(0, TOS[yypvt[-2]]);
      										insertarNodoEnPolaca(0, TOS[yypvt[-0]]);
      										insertarValorEnPolaca(1, "++");
      									  } break;
      case 24:
# line 110 "AS.y"
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
      case 25:
# line 124 "AS.y"
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
      case 26:
# line 139 "AS.y"
      {
      																										#ifdef MI_DEBUG
      																											printf("\nFin del ELSE\n");
      																										#endif
      																										int aux = pilaSaltos[--indexPilaSaltos];
      																										asignarSalto(aux, nroNodoPolaca);

      																									} break;
      case 27:
# line 149 "AS.y"
      {
      							#ifdef MI_DEBUG
      								printf("\nCondicion simple\n");
      							#endif
      							esCondicionMultiple = 0;
      						} break;
      case 30:
# line 161 "AS.y"
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
      case 31:
# line 175 "AS.y"
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
      case 32:
# line 187 "AS.y"
      {
      															#ifdef MI_DEBUG
      																printf("NOT\n");
      															#endif
      															invertirOperadorCondicional();
      															esCondicionMultiple = 0;
      														} break;
      case 33:
# line 196 "AS.y"
      {
      													insertarValorEnPolaca(1, "CMP");
      													pilaSaltos[indexPilaSaltos++] = nroNodoPolaca;
      													insertarValorEnPolaca(0, "");
      													posicionOperadorComparacion = nroNodoPolaca;
      													insertarValorEnPolaca(1, operadorCond);
      												} break;
      case 34:
# line 205 "AS.y"
      { strcpy(operadorCond, "BGE"); } break;
      case 35:
# line 206 "AS.y"
      { strcpy(operadorCond, "BGT"); } break;
      case 36:
# line 207 "AS.y"
      { strcpy(operadorCond, "BLE"); } break;
      case 37:
# line 208 "AS.y"
      { strcpy(operadorCond, "BLT"); } break;
      case 38:
# line 209 "AS.y"
      { strcpy(operadorCond, "BNE"); } break;
      case 39:
# line 210 "AS.y"
      { strcpy(operadorCond, "BEQ"); } break;
      case 40:
# line 213 "AS.y"
      { strcpy(operadorLogico, "AND"); } break;
      case 41:
# line 214 "AS.y"
      { strcpy(operadorLogico, "OR"); } break;
      case 42:
# line 218 "AS.y"
      {
      						#ifdef MI_DEBUG
      							printf("\nComienzo Iteracion WHILE\n");
      						#endif

      						pilaSaltos[indexPilaSaltos++] = nroNodoPolaca;
      					} break;
      case 43:
# line 224 "AS.y"
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
      case 44:
# line 244 "AS.y"
      {
      						#ifdef MI_DEBUG
      							printf("GET\n");
      						#endif
      						insertarValorEnPolaca(1, "GET");
      					} break;
      case 45:
# line 252 "AS.y"
      {
      						#ifdef MI_DEBUG
      							printf("PUT\n");
      						#endif
      						insertarValorEnPolaca(1, "PUT");
      					} break;
      case 46:
# line 258 "AS.y"
      {
      								#ifdef MI_DEBUG
      									printf("PUT\n");
      								#endif
      								insertarValorEnPolaca(1, "PUT");
      							} break;
      case 47:
# line 266 "AS.y"
      {  tipo_const = PR_STRING;
      					if(!esCONST)
      					{
      						#ifdef MI_DEBUG
      							printf("%s\n",buscarEnTOS(yypvt[-0]));
      						#endif
      						insertarNodoEnPolaca(0, TOS[yypvt[-0]]);
      					}
      				 } break;
      case 48:
# line 275 "AS.y"
      {   tipo_const = PR_INT;
      						if(!esCONST)
      						{
      							#ifdef MI_DEBUG
      								printf("%s\n",buscarEnTOS(yypvt[-0]));
      							#endif
      							insertarNodoEnPolaca(0, TOS[yypvt[-0]]);
      						}
      					} break;
      case 49:
# line 284 "AS.y"
      {   tipo_const = PR_REAL;
      								if(!esCONST)
      								{
      									#ifdef MI_DEBUG
      										printf("%s\n",buscarEnTOS(yypvt[-0]));
      									#endif
      									insertarNodoEnPolaca(0, TOS[yypvt[-0]]);
      								}
      				   } break;
      case 50:
# line 295 "AS.y"
      {
      											#ifdef MI_DEBUG
      												printf("+\n");
      											#endif
      											insertarValorEnPolaca(1, "+");
      										} break;
      case 51:
# line 301 "AS.y"
      {
      												#ifdef MI_DEBUG
      													printf("-\n");
      												#endif
      												insertarValorEnPolaca(1, "-");
      											} break;
      case 54:
# line 311 "AS.y"
      {
      											#ifdef MI_DEBUG
      												printf("*\n");
      											#endif
      											insertarValorEnPolaca(1, "*");
      										} break;
      case 55:
# line 317 "AS.y"
      {
      											#ifdef MI_DEBUG
      												printf("/\n");
      											#endif
      											insertarValorEnPolaca(1, "/");
      										} break;
      case 57:
# line 326 "AS.y"
      {
      											yyval = yypvt[-1];
      										} break;
      case 58:
# line 329 "AS.y"
      {
      				#ifdef MI_DEBUG
      					printf("%s\n",buscarEnTOS(yypvt[-0]));
      				#endif
      				insertarNodoEnPolaca(0, TOS[yypvt[-0]]);
      			} break;
      case 60:
# line 338 "AS.y"
      { printf("\nQequal\n"); } break;
      case 63:
# line 343 "AS.y"
      { printf("\nLET\n"); } break;    }
    goto enstack;
}
