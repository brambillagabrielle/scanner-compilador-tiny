#include <stdio.h>
#include <string.h>

// Tamanho Máximo do conteúdo de um Token
#define MAX_TOKEN 20

// Nome do arquivo com a entrada de teste que vai ser varrida
#define NOME_ARQUIVO "arquivos/AT01-Estefani_Ferlin-Gabrielle_Brambilla-aceitas.txt"

// Definição dos tipos de Tokens
typedef enum
{
  COMENTARIO,
  PALAVRAS_RESERVADAS,
  IDENTIFICADOR,
  CONSTANTE_NUMERICA,
  CONSTANTE_ALFANUMERICA,
  OPERADORES_ARITMETICOS,
  OPERADORES_COMPARACAO,
  ABRE_PARENTESES,
  FECHA_PARENTESES,
  ATRIBUICAO,
  DELIMITADOR,
  ESPACOS,
  NAO_RECONHECIDO,
  TOKEN_EOF
} TipoToken;

// Estrutura de um Token
typedef struct
{
  TipoToken tipo;
  char lexema[100];
} Token;

// Variável para verificar se há caracteres para descarte
int descarte_caracteres = 0;

// Variável para contar linhas verificadas pelo Scanner
int linha = 1;

// Função que verifica se um caractere é uma letra do alfabeto (maiúscula ou minúscula)
int isLetra(char c);

// Função que verifica se um caractere é um dígito (0-9)
int isDigito(char c);

// Função que retorna a quantidade de caracteres
int quantCaracteres(const char *cadeia);

// // Função que analisa cada cadeia, retornando um Token
Token getProximoToken(FILE *arquivo);

// Função Principal
int main(int argc, char *argv[])
{

  FILE *arquivo = fopen(NOME_ARQUIVO, "r");

  if (arquivo == NULL)
  {
    printf("ERRO: Arquivo não pode ser aberto. %s\n", argv[1]);
    return 1;
  }

  Token token;

  // Looping que varre todo o arquivo lido
  while ((token = getProximoToken(arquivo)).tipo != TOKEN_EOF)
  {
    // Cada valor corresponde à um tipo de Token
    if (token.tipo == 0)
      printf("Linha: %d | Tipo do Token: COMENTARIO | Cadeia: %s\n", linha, token.lexema);
    else if (token.tipo == 1)
      printf("Linha: %d | Tipo do Token: PALAVRAS_RESERVADAS | Cadeia: %s\n", linha, token.lexema);
    else if (token.tipo == 2)
      printf("Linha: %d | Tipo do Token: IDENTIFICADOR | Cadeia: %s\n", linha, token.lexema);
    else if (token.tipo == 3)
      printf("Linha: %d | Tipo do Token: CONSTANTE_NUMERICA | Cadeia: %s\n", linha, token.lexema);
    else if (token.tipo == 4)
      printf("Linha: %d | Tipo do Token: CONSTANTE_ALFANUMERICA | Cadeia: \"%s\"\n", linha, token.lexema);
    else if (token.tipo == 5)
      printf("Linha: %d | Tipo do Token: OPERADORES_ARITMETICOS | Cadeia: %s\n", linha, token.lexema);
    else if (token.tipo == 6)
      printf("Linha: %d | Tipo do Token: OPERADORES_COMPARACAO | Cadeia: %s\n", linha, token.lexema);
    else if (token.tipo == 7)
      printf("Linha: %d | Tipo do Token: ABRE_PARENTESES | Cadeia: %s\n", linha, token.lexema);
    else if (token.tipo == 8)
      printf("Linha: %d | Tipo do Token: FECHA_PARENTESES | Cadeia: %s\n", linha, token.lexema);
    else if (token.tipo == 9)
      printf("Linha: %d | Tipo do Token: ATRIBUICAO | Cadeia: %s\n", linha, token.lexema);
    else if (token.tipo == 10)
      printf("Linha: %d | Tipo do Token: DELIMITADOR | Cadeia: %s\n", linha, token.lexema);
    else if (token.tipo == 11)
      printf("Linha: %d | Tipo do Token: ESPACOS | Cadeia: %s\n", linha, token.lexema);
    else if (token.tipo == 12)
      printf("Linha: %d | Tipo do Token: NAO_RECONHECIDO | Cadeia: %s\n", linha, token.lexema);
  }

  fclose(arquivo);
  return 0;
}

// Função que reconhece palavras reservadas da linguagem TINY a partir de uma cadeia
int isPalavraReservada(const char *cadeia)
{

  if (cadeia[0] == 'e' && cadeia[1] == 'n' && cadeia[2] == 'd' && cadeia[3] == '\0')
    return 1; // end
  else if (cadeia[0] == 'i' && cadeia[1] == 'f' && cadeia[2] == '\0')
    return 1; // if
  else if (cadeia[0] == 't' && cadeia[1] == 'h' && cadeia[2] == 'e' && cadeia[3] == 'n' && cadeia[4] == '\0')
    return 1; // then
  else if (cadeia[0] == 'r' && cadeia[1] == 'e' && cadeia[2] == 'p' && cadeia[3] == 'e' && cadeia[4] == 'a' && cadeia[5] == 't' && cadeia[6] == '\0')
    return 1; // repeat
  else if (cadeia[0] == 'e' && cadeia[1] == 'l' && cadeia[2] == 's' && cadeia[3] == 'e' && cadeia[4] == '\0')
    return 1; // else
  else if (cadeia[0] == 'r' && cadeia[1] == 'e' && cadeia[2] == 'a' && cadeia[3] == 'd' && cadeia[4] == '\0')
    return 1; // read
  else if (cadeia[0] == 'u' && cadeia[1] == 'n' && cadeia[2] == 't' && cadeia[3] == 'i' && cadeia[4] == 'l' && cadeia[5] == '\0')
    return 1; // until
  else if (cadeia[0] == 'w' && cadeia[1] == 'r' && cadeia[2] == 'i' && cadeia[3] == 't' && cadeia[4] == 'e' && cadeia[5] == '\0')
    return 1; // write
  
  return 0;
}

int isLetra(char c)
{
  if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
    return 1;
  else
    return 0;
}

int isDigito(char c)
{
  if (c >= 48 && c <= 57)
    return 1;
  else
    return 0;
}

Token getProximoToken(FILE *arquivo)
{

  Token token;
  char c = '0';
  int estado = 0, i = 0, tokenNaoReconhecido = 0;
  token.lexema[0] = '\0';

  while (c != EOF)
  {
    c = fgetc(arquivo);

    if (c == '\n')
      linha++;

    switch (estado)
    {

    case 0: // Estado inicial

      if (c == '{')
      {
        // Comentários {}

        estado = 1;
        token.tipo = COMENTARIO;
        token.lexema[i++] = c;
        tokenNaoReconhecido = 1;
      }
      else if (isLetra(c))
      {
        // Palavras reservadas ou identificadores

        estado = 2;
        token.tipo = IDENTIFICADOR;
        token.lexema[i++] = c;
      }
      else if (isDigito(c))
      {
        // Constantes numéricas

        estado = 3;
        token.tipo = CONSTANTE_NUMERICA;
        token.lexema[i++] = c;
      }
      else if (c == '"')
      {
        // Constantes alfanuméricas

        estado = 4;
        token.tipo = CONSTANTE_ALFANUMERICA;
      }
      else if (c == '+' || c == '-' || c == '*' || c == '/')
      {
        // Operadores aritméticos

        estado = 5;
        token.tipo = OPERADORES_ARITMETICOS;
        token.lexema[i++] = c;
      }
      else if (c == '=' || c == '<' || c == '>' || c == '!')
      {
        // Operadores de comparação

        estado = 6;
        token.tipo = OPERADORES_COMPARACAO;
        token.lexema[i++] = c;
      }
      else if (c == ';')
      {
        // Delimitador ;

        estado = 7;
        token.tipo = DELIMITADOR;
        token.lexema[i++] = c;
      }
      else if (c == ':')
      {
        // Atribuição

        estado = 8;
        token.lexema[i++] = c;
      }
      else if (c == ' ' || c == '\n' || c == '\t')
      {
        // Espaço em branco, nova linha ou tabulação

        estado = 9;
        token.tipo = ESPACOS;

        if (c == '\n')
        {
          token.lexema[i++] = '\\';
          token.lexema[i++] = 'n';
          token.lexema[i] = '\0';
        }
        else if (c == '\t')
        {
          token.lexema[i++] = '\\';
          token.lexema[i++] = 't';
          token.lexema[i] = '\0';
        }
        else
        {
          token.lexema[i++] = '"';
          token.lexema[i++] = ' ';
          token.lexema[i++] = '"';
          token.lexema[i] = '\0';
        }
      }
      else if (c == '(')
      {
        // Abre parenteses

        estado = 10;
        token.tipo = ABRE_PARENTESES;
        token.lexema[i++] = c;
      }
      else if (c == ')')
      {
        // Fecha parenteses

        estado = 11;
        token.tipo = FECHA_PARENTESES;
        token.lexema[i++] = c;
      }
      else if (c == EOF)
      {
        // Fim do arquivo (EOF)

        token.tipo = TOKEN_EOF;
        return token;
      }
      else
      {
        // Não reconhecido

        token.tipo = NAO_RECONHECIDO;
        token.lexema[i++] = c;
        token.lexema[i] = '\0';
        return token;
      }

      break;

    case 1: // Estado que reconhece comentários {}

      if (c == '}')
      {
        token.lexema[i++] = c;
        token.lexema[i] = '\0';
        return token;
      }

      token.lexema[i++] = c;
      break;

    case 2: // Estado que reconhece palavras reservadas ou identificadores

      if (isLetra(c) || isDigito(c))
      {

        if ((i + 1) == MAX_TOKEN) {
          descarte_caracteres = 1;
          printf("ERRO NA LINHA: %d | IDENTIFICADOR passa do limite de caracteres. Caracteres descartados: ", linha);
        }

        if (i > MAX_TOKEN - 1)
          printf("%c", c);
        else
        {
          token.lexema[i] = c;
          token.lexema[i + 1] = '\0';
        }

        i++;
      }
      else
      {

        if (descarte_caracteres)
          printf("\n");
        
        ungetc(c, arquivo);
        token.lexema[i] = '\0';

        if (isPalavraReservada(token.lexema))
          token.tipo = PALAVRAS_RESERVADAS;
        else
          token.tipo = IDENTIFICADOR;

        return token;
      }

      break;

    case 3: // Estado que reconhece constantes numéricas

      if (isDigito(c))
      {

        if ((i + 1) == MAX_TOKEN) {
          descarte_caracteres = 1;
          printf("ERRO NA LINHA: %d | CONSTANTE_NUMERICA passa do limite de caracteres. Caracteres descartados: ", linha);
        }

        if (i > MAX_TOKEN - 1)
          printf("%c", c);
        else
        {
          token.lexema[i] = c;
          token.lexema[i + 1] = '\0';
        }

        i++;
      }
      else
      {
        if (descarte_caracteres)
          printf("\n");
          
        ungetc(c, arquivo);
        token.lexema[i] = '\0';
        return token;
      }

      break;

    case 4: // Estado que reconhece constantes alfanuméricas

      if (c == '"')
      {
        token.lexema[i] = '\0';
        return token;
      }
      token.lexema[i++] = c;

      break;

    case 5: // Estado que reconhece operadores aritméticos

      token.lexema[i] = '\0';
      ungetc(c, arquivo);
      return token;

    case 6: // Estado que reconhece operadores de comparação

      if (c == '=')
        token.lexema[i++] = c;
      else
        ungetc(c, arquivo);

      token.lexema[i] = '\0';
      return token;

    case 7: // Estado que reconhece o delimitador ;

      token.lexema[i] = '\0';
      ungetc(c, arquivo);
      return token;

    case 8: // Estado que reconhece atribuição

      if (c == '=')
      {

        token.tipo = ATRIBUICAO;
        token.lexema[i++] = c;
        token.lexema[i] = '\0';
        return token;
      }
      else
      {
        token.tipo = NAO_RECONHECIDO;
        return token;
      }

    case 9: // Estado que reconhece espaço em branco
    case 10: // Estado que reconhece abre parenteses
    case 11: // Estado que reconhece fecha parenteses

      token.lexema[i] = '\0';
      ungetc(c, arquivo);
      return token;

    default:
      token.tipo = NAO_RECONHECIDO;
      return token;
    }
  }

  if (tokenNaoReconhecido)
  {
    token.tipo = NAO_RECONHECIDO;
    token.lexema[i] = '\0';
    return token;
  }

  token.tipo = TOKEN_EOF;
  return token;
}
