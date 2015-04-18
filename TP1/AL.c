/* --------------------------------------------------------------------------
Lenguajes y Compiladores

GRUPO 26:
Anfuso, Diego
Villaverde, Leonel
 --------------------------------------------------------------------------*/

#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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

//Variable global provisoria

int yylval;

/* -------------------------------------------------------------------------- */
/*                           DEFINCION CONSTANTES                             */
/* -------------------------------------------------------------------------- */
#define ESTADO_FINAL 20
#define MAXLONG 30
#define TAMMAX 100
#define TAMTOKEN 1000

#define NV -1  //ESTADO DE CARACTER NO V?LIDO

#define INT_MAX   65535
#define FLT_MAX     3.40282347e+38F
//#define FLT_PREC    18
//DEFINES TEMPORALES
#define COMENTARIO -2

#define PR_DECLARE 0
#define PR_ENDDECLARE 1
#define PR_INT 2
#define PR_REAL 3
#define PR_STRING 4
#define PR_CONST 5
#define PR_WHILE 6
#define PR_LET 7
#define PR_IF 8
#define PR_QEQUAL 9
#define PR_ELSE 10
#define PR_ENDIF 11
#define PR_BEGIN 12
#define PR_END 13
#define PR_AND 14
#define PR_OR 15
#define PR_GET 16
#define PR_PUT 17
#define PR_NOT 18
#define PR_DEFAULT 19
#define PR_THEN 20
#define CANTPR 21


#define ID 30
#define CTE_ENT 31
#define CTE_REAL 32
#define CTE_STRING 33
#define OP_PABRE 34
#define OP_PCIERRA 35
#define OP_MENOR 39
#define OP_MENORIGUAL 40
#define OP_MAYOR 41
#define OP_MAYORIGUAL 42
#define OP_IGUAL 43
#define OP_SUMA 44
#define OP_RESTA 45
#define OP_MULTIPLI 46
#define OP_DIVISION 47
#define OP_CABRE 48
#define OP_CCIERRA 49
#define OP_ASIG 50
#define OP_LLABRE 51
#define OP_LLCIERRA 52
#define OP_CONCAT 53
#define SEP_DECIMAL 54
#define SEP_SENT 55
#define OP_TIPO 56
#define SEP_LISTA 57
#define OP_GUION 58
#define SEP_DOSP 59

/* -------------------------------------------------------------------------- */
/*                           DECLARACION PROTOTIPOS                           */
/* -------------------------------------------------------------------------- */

// FUNCIONES GENERALES
void leerCaracter();
void mostrarToken();
int  leerColumna(char);
void limpiarToken();

char * buscarEnTOS(int);
int insertarTOS();
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
            yylval = insertarTOS(); //TOS
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

    }

    yylval= insertarTOS(); // TOS
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
        yylval = insertarTOS(); //TOS
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
}

/* CARACTER NO VALIDO ------------------------------------------------------- */
void Caract_No_Val () //REVISAR MENSAJES DE ERROR
{
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
struct tablaDeSimbolo
{
    char nombre[100];
    char tipo  [11];
    char valor [100];
    char ren   [31];
    int longitud;
};

struct tablaDeSimbolo TOS[TAMMAX];

//MATRIZ PUNTERO A FUNCION
void (*proceso[][25])() = {
//			0|					1|					2|					3|					4|					5|					6|					7|						8|						9|					10|						11|					12|						13|					14|					15|					16|					17|					18|					19|					20|						21|					22|					23|					24|
//			+|					-|					*|					/|					Let|				Dig|				=|					<|						>|						"|					.|						[|					]|						{|					}|					(|					)|					,|					_|					;|					:|				        tab|				blan|				ent|				OTRO|
/*E0*/ {	Iniciar_concat,	    Iniciar_Com,	    Inf_OpProducto,	    Inf_OpDivision,	    Iniciar_ID,	        Iniciar_Cte,	    Iniciar_Igual,	    Iniciar_MenorIgual,	    Iniciar_MayorIgual,	    Inciar_CteStr,	    Iniciar_Cte_Float,	    Inf_OpCorchAbre,	Inf_OpCorchCierra,	    Inf_OpLLAbre,	    Inf_OpLLCierra,	    Inf_OpParAbre,	    Inf_OpParCierra,	Inf_SepLista,	    Iniciar_ID,	        Inf_Sep,	        Iniciar_Asignacion,	    Nada,	            Nada,	            Nada,	            Caract_No_Val },
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
static int nEstado[20][25] = {
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
int insertarTOS()
{
	int i,j,x=0;
    int ii=0;
    char aux[100];
    char auxStr[100];

    if (NroToken==CTE_STRING)
    {
        strcpy(auxStr," ");

        for (j=0;j< strlen(token);j++)
        {
            if(token[j]!='"')
            {
                auxStr[x]= token[j];
                x++;
            }
        }

        auxStr[x]='\0';
    }


    for (i=0; i<TOStop;  i++)
    {
        if (NroToken==ID)
        {
            if (strcmp(TOS[i].nombre,token)==0)
                return i;
        }
        else if (NroToken==CTE_STRING)
        {
            if (strcmp(TOS[i].valor,auxStr)==0)
                return i;
        }
        else
        {
            if (strcmp(TOS[i].valor,token)==0)
                return i;
        }
    }

  	switch (NroToken)
    {
        case ID:
            strcat(aux, token);
            strcpy(TOS[TOStop].nombre,token);
            strcpy(TOS[TOStop].tipo,"ID" );
            TOStop++;
        break;
        case CTE_ENT:
            strcpy(aux,"_");
            strcat(aux, token);
            strcpy(TOS[TOStop].nombre, aux);
            strcpy(TOS[TOStop].tipo,"CTE_ENT");
            strcpy(TOS[TOStop].valor, token);
   			TOStop++;
		break;
        case CTE_REAL:
            strcpy(aux,"_");
            strcat(aux, token);
            strcpy(TOS[TOStop].nombre,aux);
            strcpy(TOS[TOStop].tipo,"CTE_REAL");
            strcpy(TOS[TOStop].valor, token);
   			TOStop++;
		break;
       	case CTE_STRING:
            strcpy(aux,"_");
            strcat(aux, auxStr);
            strcpy(TOS[TOStop].nombre, aux);
            strcpy(TOS[TOStop].tipo,"CTE_STRING" );
            strcpy(TOS[TOStop].valor, auxStr);
            TOS[TOStop].longitud = (strlen(auxStr));
            TOStop++;
        break;
    }

    return TOStop-1;
}

void mostrarTOS()
{
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
            return i;
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
    TOStop++;

    strcpy(TOS[TOStop].nombre, "ENDDECLARE");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "INT");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "REAL");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "STRING");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "CONST");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "WHILE");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "LET");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "IF");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "QEqual");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "ELSE");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "ENDIF");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "BEGIN");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "END");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "AND");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "OR");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "GET");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "PUT");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "NOT");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "DEFAULT");
    strcpy(TOS[TOStop].tipo, "PR");
    TOStop++;

    strcpy(TOS[TOStop].nombre, "THEN");
    strcpy(TOS[TOStop].tipo, "PR");
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

                return NroToken;
		    }
		}
    }

    return NroToken;
}

/* -------------------------------------------------------------------------- */
/*                                      MAIN                                  */
/* -------------------------------------------------------------------------- */

int main(int argc, char* argv[])
{
    agregarPalabrasReservadas();

    if (argc > 1)
    {
        if ((input = fopen (argv[1],"r"))== NULL)
        {
            printf("No se puede abrir el archivo de entrada");
            getch();
            exit(1);
        }
    }
    else
    {
        if ((input = fopen ("prueba.txt","r"))== NULL)
        {
            printf("No se puede abrir el archivo de entrada");
            getch();
            exit(1);
        }
    }

    if ((tokens = fopen ("tokens.txt","w"))== NULL)
	{
		printf("No se puede crear el archivo de tokens");
		getch();
		exit(1);
	}

/*-----------------CICLO QUE LEE HASTA FIN DE ARCHIVO (MAIN)-----------------*/

	while (feof(input)== 0 )
	{
        if (yylex()!=EOF)
        {
            if (error==0)
                mostrarToken();
        }
	}

/*---------------------------------------------------------------------------*/

    if(fclose(input)!=0)
    {
        printf("No se puede CERRAR el archivo de entrada");
        getch();
        exit(1);
    }

    if(fclose(tokens)!=0)
    {
        printf("No se puede CERRAR el archivo de tokens");
        getch();
        exit(1);
    }

    if (error==0)
    {
        printf("\n - Analisis Lexico COMPLETO - \n");

        if ((tos = fopen ("tos.txt","w"))== NULL)
        {
            printf("No se puede crear el archivo de la tabla de TOS");
            getch();
            exit(1);
        }

        mostrarTOS();

        if(fclose(tos)!=0)
        {
            printf("No se puede CERRAR el archivo de la tabla de tos");
            getch();
            exit(1);
        }
    }

    if(error==1)
        printf("\n - Analisis Lexico completo con ERRORES - \n");

    return 0;
}
