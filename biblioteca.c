#include "biblioteca.h"

// Variáveis globais
struct Livro L[MAX_ITENS];
struct Usuario U[MAX_ITENS];
struct Emprestimo E[MAX_ITENS];

int qtdLivros = 0;
int qtdUsuarios = 0;
int contadorEmprestimos = 0;
int proximoIdLivro = 1;
int proximoIdUsuario = 1;
int proximoIdEmprestimo = 1;

