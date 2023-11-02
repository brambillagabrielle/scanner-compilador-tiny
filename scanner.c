#include <stdio.h>

// Tamanho Máximo do conteúdo de um Token
#define MAX_TOKEN 20

// Nome do arquivo com a entrada de teste que vai ser varrida
#define NOME_ARQUIVO "entrada.txt"

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
  PARENTESES,
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
  char lexema[MAX_TOKEN + 1];
} Token;

// Função que reconhece palavras reservadas da linguagem TINY a partir de uma cadeia
int isPalavraReservada(const char *cadeia)
{

  if (cadeia[0] == 'e' && cadeia[1] == 'n' && cadeia[2] == 'd' && cadeia[3] == '\0')
    return 1;
  else if (cadeia[0] == 'i' && cadeia[1] == 'f' && cadeia[2] == '\0')
    return 1;
  else if (cadeia[0] == 't' && cadeia[1] == 'h' && cadeia[2] == 'e' && cadeia[3] == 'n' && cadeia[4] == '\0')
    return 1;
  else if (cadeia[0] == 'r' && cadeia[1] == 'e' && cadeia[2] == 'p' && cadeia[3] == 'e' && cadeia[4] == 'a' && cadeia[5] == 't' && cadeia[6] == '\0')
    return 1;
  else if (cadeia[0] == 'd' && cadeia[1] == 'o' && cadeia[2] == '\0')
    return 1;
  else if (cadeia[0] == 'e' && cadeia[1] == 'l' && cadeia[2] == 's' && cadeia[3] == 'e' && cadeia[4] == '\0')
    return 1;
  else if (cadeia[0] == 'r' && cadeia[1] == 'e' && cadeia[2] == 'a' && cadeia[3] == 'd' && cadeia[4] == '\0')
    return 1;
  else if (cadeia[0] == 'u' && cadeia[1] == 'n' && cadeia[2] == 't' && cadeia[3] == 'i' && cadeia[4] == 'l' && cadeia[5] == '\0')
    return 1;
  else if (cadeia[0] == 'w' && cadeia[1] == 'r' && cadeia[2] == 'i' && cadeia[3] == 't' && cadeia[4] == 'e' && cadeia[5] == '\0')
    return 1;
  else
    return 0;
}

// Função que verifica se um caractere é uma letra do alfabeto (maiúscula ou minúscula)
int isLetra(char c)
{
  if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
    return 1;
  else
    return 0;
}

// Função que verifica se um caractere é um dígito (0-9)
int isDigito(char c)
{
  if (c >= 48 && c <= 57)
    return 1;
  else
    return 0;
}

// Função que verifica se um caractere é um espaço em branco ' '
int isEspaco(char c)
{
  if (c == ' ')
    return 1;
  else
    return 0;
}

// Função que retorna a quantidade de caracteres
int quantCaracteres(const char *cadeia)
{
  int cont = 0;

  while (cadeia[cont] != '\0')
    cont++;

  return cont;
}

// Contador para garantir o tamanho de uma constante alfanumérica
int CONT_CONSTANTE = 0;

// Variável para contar linhas verificadas pelo Scanner
int novaLinha = 1;

// Função que analisa cada cadeia, retornando um Token
Token getProximoToken(FILE *arquivo)
{

  Token token;
  char c;
  int estado = 0;
  int i = 0;

  while ((c = fgetc(arquivo)) != EOF)
  {

    switch (estado)
    {

    case 0: // Estado inicial

      if (c == '\n')
        novaLinha += 1;

      if (c == '{')
      {
        // Comentários {}

        estado = 1;
        token.tipo = COMENTARIO;
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
        CONT_CONSTANTE = 0;
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
      else if (c == '(')
      {
        // Parenteses

        estado = 8;
        token.tipo = PARENTESES;
        token.lexema[i++] = c;
      }
      else if (c == ':')
      {
        // Atribuição

        estado = 9;
        token.lexema[i++] = c;
      }
      else if (isEspaco(c) || c == '\n' || c == '\t')
      {
        // Espaço em branco ou tabulação

        estado = 10;
        token.tipo = ESPACOS;
        token.lexema[i++] = c;
      }
      else
      {
        // Não-reconhecido

        estado = 11;
        token.tipo = NAO_RECONHECIDO;
        token.lexema[i++] = c;
      }

      break;

    case 1: // Estado que reconhece comentários {}

      if (c == '}')
      {
        token.lexema[i] = '\0';
        return token;
      }

      token.lexema[i++] = c;
      break;

    case 2: // Estado que reconhece palavras reservadas ou identificadores

      if (isLetra(c) || isDigito(c))
      {

        if (quantCaracteres(token.lexema) > MAX_TOKEN)
        {
          printf("ERRO NA LINHA: %d | IDENTIFICADOR passa do limite de caracteres: {%s}\n", novaLinha, token.lexema);
          return token;
        }

        token.lexema[i++] = c;
      }
      else
      {

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

        if (quantCaracteres(token.lexema) > MAX_TOKEN)
        {
          printf("ERRO NA LINHA: %d | CADEIA_ALFANUMERICA passa do limite de caracteres: {%s}\n", novaLinha, token.lexema);
          return token;
        }

        token.lexema[i++] = c;
      }
      else
      {
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
      else
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

    case 8: // Estado que reconhece parenteses

      if (c == ')')
      {
        token.lexema[i] = '\0';
        return token;
      }

      token.lexema[i++] = c;
      break;

    case 9: // Estado que reconhece atribuição

      if (c == '=')
      {

        estado = 0;
        token.tipo = ATRIBUICAO;
        token.lexema[i] = c;
        i++;
        token.lexema[i] = '\0';
        return token;
      }
      else
      {
        token.tipo = NAO_RECONHECIDO;
        return token;
      }

    case 10: // Estado que reconhece espaço em branco

      token.lexema[i] = '\0';
      ungetc(c, arquivo);
      return token;

    case 11: // Estado que reconhece Tokens não reconhecidos
      token.lexema[i++] = c;

    default:
      token.tipo = NAO_RECONHECIDO;
      return token;
    }
  }

  token.tipo = TOKEN_EOF;
  return token;
}

// Função Principal
int main(int argc, char *argv[])
{

  FILE *arquivo = fopen(NOME_ARQUIVO, "r");

  if (arquivo == NULL)
  {
    printf("Error: Could not open file %s\n", argv[1]);
    return 1;
  }

  Token token;

  // Looping que varre todo o arquivo lido
  while ((token = getProximoToken(arquivo)).tipo != TOKEN_EOF)
  {
    // Cada valor corresponde à um tipo de Token
    if (token.tipo == 0)
      printf("Linha: %d | Tipo do Token: COMENTARIO | Cadeia: {%s}\n", novaLinha, token.lexema);
    else if (token.tipo == 1)
      printf("Linha: %d | Tipo do Token: PALAVRAS_RESERVADAS | Cadeia: %s\n", novaLinha, token.lexema);
    else if (token.tipo == 2)
      printf("Linha: %d | Tipo do Token: IDENTIFICADOR | Cadeia: %s\n", novaLinha, token.lexema);
    else if (token.tipo == 3)
      printf("Linha: %d | Tipo do Token: CONSTANTE_NUMERICA | Cadeia: %s\n", novaLinha, token.lexema);
    else if (token.tipo == 4)
      printf("Linha: %d | Tipo do Token: CONSTANTE_ALFANUMERICA | Cadeia: \"%s\"\n", novaLinha, token.lexema);
    else if (token.tipo == 5)
      printf("Linha: %d | Tipo do Token: OPERADORES_ARITMETICOS | Cadeia: %s\n", novaLinha, token.lexema);
    else if (token.tipo == 6)
      printf("Linha: %d | Tipo do Token: OPERADORES_COMPARACAO | Cadeia: %s\n", novaLinha, token.lexema);
    else if (token.tipo == 7)
      printf("Linha: %d | Tipo do Token: PARENTESES | Cadeia: %s\n", novaLinha, token.lexema);
    else if (token.tipo == 8)
      printf("Linha: %d | Tipo do Token: ATRIBUICAO | Cadeia: %s\n", novaLinha, token.lexema);
    else if (token.tipo == 9)
      printf("Linha: %d | Tipo do Token: DELIMITADOR | Cadeia: %s\n", novaLinha, token.lexema);
    else if (token.tipo == 10)
      printf("Linha: %d | Tipo do Token: ESPACOS | Cadeia: %s\n", novaLinha, token.lexema);
    else if (token.tipo == 11)
      printf("Linha: %d | Tipo do Token: NAO_RECONHECIDO | Cadeia: %s\n", novaLinha, token.lexema);
    else if (token.tipo == 12)
      printf("Linha: %d | Tipo do Token: TOKEN_EOF | Cadeia: {%s}\n", novaLinha, token.lexema);
  }

  fclose(arquivo);
  return 0;
}
