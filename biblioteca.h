#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ITENS 200
#define ARQUIVO_LIVROS "livros.txt"
#define ARQUIVO_USUARIOS "usuarios.txt"
#define ARQUIVO_EMPRESTIMOS "emprestimos.txt"
#define ARQUIVO_MAX_IDS "max_ids.txt"

// Structs (R01)
struct Livro {
    int idLivro;
    char titulo[100];
    char autor[100];
    int anoPub;
    int status;
};

struct Usuario {
    int idUsuario;
    char nome[100];
    char email[100];
    int ativo;
};

struct Emprestimo {
    int idEmprestimo;
    int idUsuario;
    int idLivro;
    char dataEmprestimo[11]; 
    int ativo;
};

// Protótipos das Funções (R09)
void LimparBuffer();
void carregarMaxIds();
void salvarMaxIds();
void carregarDados();
void salvarDados();
void MostrarInformacoes();
void MostrarInformacoes2();
void MostrarInformacoes3();
void MostrarInformacoes4();
void AdicionarLivro();
void BuscarLivro();
void ListarLivro();
void ListarLivrosDisponiveis();
void AtualizarInformacoes();
void CadastrarUsuario();
void BuscarUsuarioPorNome();
void ListarTodosUsuariosCadastrados();
void InativarUsuario();
void RegistrarEmprestimo();
void RegistrarDevolucaoLivro();
void ListarTodosEmprestimosAtivo();
void ListarHistoricoEmprestimos();
void MostrarLivrosEmprestadosPorUsuario();
void Relatorios();

#endif
