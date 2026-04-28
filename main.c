#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Definição do tamanho máximo dos vetores para um sistema mais funcional
#define MAX_ITENS 200
#define ARQUIVO_LIVROS "livros.txt"
#define ARQUIVO_USUARIOS "usuarios.txt"
#define ARQUIVO_EMPRESTIMOS "emprestimos.txt"
#define ARQUIVO_MAX_IDS "max_ids.txt"


struct Livro {
    int idLivro;
    char titulo[100];
    char autor[100];
    int anoPub;
    int status; // 1: Disponível, 0: Emprestado
};

struct Usuario {
    int idUsuario;
    char nome[100];
    char email[100];
    int ativo; // 1: Ativo, 0: Inativo
};

struct Emprestimo {
    int idEmprestimo;
    int idUsuario;
    int idLivro;
    char dataEmprestimo[11]; 
    int ativo; // 1: Ativo, 0: Concluído (devolvido)
};

// =========================================================================
// VARIÁVEIS GLOBAIS DE CONTROLE E DADOS (ATUALIZADAS APÓS CARGA)
// =========================================================================

struct Livro L[MAX_ITENS];
struct Usuario U[MAX_ITENS];
struct Emprestimo E[MAX_ITENS];

int qtdLivros = 0;
int qtdUsuarios = 0;
int contadorEmprestimos = 0; // Quantidade total de registros de empréstimo (histórico + ativos)

// Variáveis para garantir que os IDs sejam sequenciais e não colidam ao reabrir o sistema
int proximoIdLivro = 1;
int proximoIdUsuario = 1;
int proximoIdEmprestimo = 1;

// =========================================================================
// FUNÇÕES AUXILIARES
// =========================================================================

void LimparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

// =========================================================================
// FUNÇÕES OBRIGATÓRIAS RELACIONADAS A ARQUIVOS
// =========================================================================

void carregarMaxIds() {
    FILE *f = fopen(ARQUIVO_MAX_IDS, "r");
    if (f == NULL) {
        // Arquivo não existe na primeira execução, IDs já estão em 1, contador em 0.
        printf("[INFO] Arquivo de IDs '%s' não encontrado. Iniciando contadores.\n", ARQUIVO_MAX_IDS);
        return;
    }

    // Leitura: proximoIdLivro;proximoIdUsuario;proximoIdEmprestimo;contadorEmprestimos
    if (fscanf(f, "%d;%d;%d;%d", &proximoIdLivro, &proximoIdUsuario, &proximoIdEmprestimo, &contadorEmprestimos) == 4) {
        printf("[INFO] IDs e Contador de Empréstimos carregados com sucesso.\n");
    } else {
        printf("[AVISO] Erro na leitura do arquivo de IDs. Iniciando contadores.\n");
    }
    fclose(f);
}


void salvarMaxIds() {
    FILE *f = fopen(ARQUIVO_MAX_IDS, "w");
    if (f == NULL) {
        printf("[ERRO] Nao foi possivel abrir o arquivo de IDs '%s' para escrita.\n", ARQUIVO_MAX_IDS);
        return;
    }

    // Escrita: proximoIdLivro;proximoIdUsuario;proximoIdEmprestimo;contadorEmprestimos
    fprintf(f, "%d;%d;%d;%d\n", proximoIdLivro, proximoIdUsuario, proximoIdEmprestimo, contadorEmprestimos);
    fclose(f);
    printf("[INFO] IDs e Contador de Empréstimos salvos com sucesso.\n");
}


void carregarDados() {
    // 1. CARREGAR LIVROS
    FILE *fLivros = fopen(ARQUIVO_LIVROS, "r");
    if (fLivros == NULL) {
        printf("[INFO] Arquivo de Livros '%s' não encontrado. Será criado na saída.\n", ARQUIVO_LIVROS);
    } else {
        while (qtdLivros < MAX_ITENS &&
               fscanf(fLivros, "%d;%99[^;];%99[^;];%d;%d\n",
                      &L[qtdLivros].idLivro,
                      L[qtdLivros].titulo,
                      L[qtdLivros].autor,
                      &L[qtdLivros].anoPub,
                      &L[qtdLivros].status) == 5) {
            qtdLivros++;
        }
        fclose(fLivros);
        printf("[INFO] %d Livros carregados.\n", qtdLivros);
    }

    // 2. CARREGAR USUÁRIOS
    FILE *fUsuarios = fopen(ARQUIVO_USUARIOS, "r");
    if (fUsuarios == NULL) {
        printf("[INFO] Arquivo de Usuários '%s' não encontrado. Será criado na saída.\n", ARQUIVO_USUARIOS);
    } else {
        while (qtdUsuarios < MAX_ITENS &&
               fscanf(fUsuarios, "%d;%99[^;];%99[^;];%d\n",
                      &U[qtdUsuarios].idUsuario,
                      U[qtdUsuarios].nome,
                      U[qtdUsuarios].email,
                      &U[qtdUsuarios].ativo) == 4) {
            qtdUsuarios++;
        }
        fclose(fUsuarios);
        printf("[INFO] %d Usuários carregados.\n", qtdUsuarios);
    }

    // 3. CARREGAR EMPRÉSTIMOS
    FILE *fEmprestimos = fopen(ARQUIVO_EMPRESTIMOS, "r");
    if (fEmprestimos == NULL) {
        printf("[INFO] Arquivo de Empréstimos '%s' não encontrado. Será criado na saída.\n", ARQUIVO_EMPRESTIMOS);
    } else {
        while (contadorEmprestimos < MAX_ITENS &&
               fscanf(fEmprestimos, "%d;%d;%d;%10[^;];%d\n",
                      &E[contadorEmprestimos].idEmprestimo,
                      &E[contadorEmprestimos].idUsuario,
                      &E[contadorEmprestimos].idLivro,
                      E[contadorEmprestimos].dataEmprestimo,
                      &E[contadorEmprestimos].ativo) == 5) {
            contadorEmprestimos++;
        }
        fclose(fEmprestimos);
        printf("[INFO] %d Empréstimos (histórico + ativos) carregados.\n", contadorEmprestimos);
    }
}


void salvarDados() {
    // 1. SALVAR LIVROS
    FILE *fLivros = fopen(ARQUIVO_LIVROS, "w");
    if (fLivros == NULL) {
        printf("[ERRO] Nao foi possivel abrir o arquivo de Livros '%s' para escrita.\n", ARQUIVO_LIVROS);
        return;
    }
    for (int i = 0; i < qtdLivros; i++) {
        fprintf(fLivros, "%d;%s;%s;%d;%d\n",
                L[i].idLivro, L[i].titulo, L[i].autor, L[i].anoPub, L[i].status);
    }
    fclose(fLivros);
    printf("[INFO] %d Livros salvos em '%s'.\n", qtdLivros, ARQUIVO_LIVROS);

    // 2. SALVAR USUÁRIOS
    FILE *fUsuarios = fopen(ARQUIVO_USUARIOS, "w");
    if (fUsuarios == NULL) {
        printf("[ERRO] Nao foi possivel abrir o arquivo de Usuários '%s' para escrita.\n", ARQUIVO_USUARIOS);
        return;
    }
    for (int i = 0; i < qtdUsuarios; i++) {
        fprintf(fUsuarios, "%d;%s;%s;%d\n",
                U[i].idUsuario, U[i].nome, U[i].email, U[i].ativo);
    }
    fclose(fUsuarios);
    printf("[INFO] %d Usuários salvos em '%s'.\n", qtdUsuarios, ARQUIVO_USUARIOS);

    // 3. SALVAR EMPRÉSTIMOS
    FILE *fEmprestimos = fopen(ARQUIVO_EMPRESTIMOS, "w");
    if (fEmprestimos == NULL) {
        printf("[ERRO] Nao foi possivel abrir o arquivo de Empréstimos '%s' para escrita.\n", ARQUIVO_EMPRESTIMOS);
        return;
    }
    for (int i = 0; i < contadorEmprestimos; i++) {
        fprintf(fEmprestimos, "%d;%d;%d;%s;%d\n",
                E[i].idEmprestimo, E[i].idUsuario, E[i].idLivro, E[i].dataEmprestimo, E[i].ativo);
    }
    fclose(fEmprestimos);
    printf("[INFO] %d Empréstimos (histórico + ativos) salvos em '%s'.\n", contadorEmprestimos, ARQUIVO_EMPRESTIMOS);
}

// =========================================================================
// FUNÇÕES DE EXIBIÇÃO DE MENU
// =========================================================================

void MostrarInformacoes() {
    printf("\n============ Sistema de Biblioteca ============\n");
    printf("1- Biblioteca\n2- Usuário\n3- Empréstimo\n4- Relatorio\n0- Sair\n");
    printf("* Digite um número correspondente a cada ação realizada.\n");
    printf("===============================================\n");
}

void MostrarInformacoes2() {
    printf("\n============ Gerenciamento de Biblioteca ============\n");
    printf("1- Adicionar Livro\n2- Buscar Livro\n3- Listar Livros\n4- Marcar Livro como Emprestado\n5- Marcar Livro como Devolvido\n6- Listar Livros Disponiveis\n7- Atualizar Informacoes\n0- Voltar\n");
    printf("* Digite um número correspondente a cada ação realizada.\n");
    printf("===============================================\n");
}

void MostrarInformacoes3() {
    printf("\n============ Gerenciamento de Usuário ============\n");
    printf("1- Cadastrar Usuário\n2- Buscar Usuário por Nome\n3- Listar Todos os Usuários Cadastrados\n4- Inativar Usuário\n0- Voltar\n");
    printf("* Digite um número correspondente a cada ação realizada.\n");
    printf("==================================================\n");
}

void MostrarInformacoes4() {
    printf("\n============ Gerenciamento de Empréstimo ============\n");
    printf("1- Registrar Novo Empréstimo\n2- Registrar Devolução de um Livro\n3- Listar Todos os Empréstimos Ativos\n4- Listar Histórico de Empréstimos\n5- Mostrar Todos os Livros Emprestados por um Usuário Específico\n0- Voltar\n");
    printf("* Digite um número correspondente a cada ação realizada.\n");
    printf("=====================================================\n");
}

// =========================================================================
// FUNÇÕES DE BIBLIOTECA
// =========================================================================

void AdicionarLivro() {
    if (qtdLivros >= MAX_ITENS) {
        printf("[ERRO] Limite máximo de livros atingido (%d).\n", MAX_ITENS);
        return;
    }

    printf("\n[Livro: %d]\n", qtdLivros + 1);
    L[qtdLivros].idLivro = proximoIdLivro++; // Usa o ID persistente

    printf("Titulo: ");
    fgets(L[qtdLivros].titulo, sizeof(L[qtdLivros].titulo), stdin);
    L[qtdLivros].titulo[strcspn(L[qtdLivros].titulo, "\n")] = 0;

    // Verificação de título duplicado
    for (int i = 0; i < qtdLivros; i++) {
        if (strcmp(L[qtdLivros].titulo, L[i].titulo) == 0) {
            printf("[ERRO] Titulo já cadastrado. Cadastro cancelado.\n");
            proximoIdLivro--;
            return;
        }
    }

    printf("Autor: ");
    fgets(L[qtdLivros].autor, sizeof(L[qtdLivros].autor), stdin);
    L[qtdLivros].autor[strcspn(L[qtdLivros].autor, "\n")] = 0;

    printf("Ano publicado: ");
    if (scanf("%d", &L[qtdLivros].anoPub) != 1) {
        LimparBuffer();
        printf("[ERRO] Entrada inválida para Ano Publicado. Cadastro cancelado.\n");
        proximoIdLivro--;
        return;
    }
    LimparBuffer();

    L[qtdLivros].status = 1; // Novo livro sempre disponível por padrão

    printf("\nLivro '%s' cadastrado com ID %d e STATUS: Disponível.\n", L[qtdLivros].titulo, L[qtdLivros].idLivro);
    qtdLivros++;
}

void BuscarLivro() {
    char termoBusca[100];
    int opcao;
    int encontrado = 0;

    printf("Digite 1 para pesquisar por titulo ou 2 para pesquisar por nome do autor: ");
    if (scanf("%d", &opcao) != 1) {
        LimparBuffer();
        printf("[ERRO] Opção inválida.\n");
        return;
    }
    LimparBuffer();

    printf("Termo de Busca: ");
    fgets(termoBusca, sizeof(termoBusca), stdin);
    termoBusca[strcspn(termoBusca, "\n")] = 0;

    for (int i = 0; i < qtdLivros; i++) {
        int match = 0;
        if (opcao == 1 && strstr(L[i].titulo, termoBusca) != NULL) {
            match = 1;
        } else if (opcao == 2 && strstr(L[i].autor, termoBusca) != NULL) {
            match = 1;
        }

        if (match) {
            encontrado = 1;
            printf("=======================\n");
            printf("[ID Livro: %d]\n", L[i].idLivro);
            printf("[Titulo: %s]\n", L[i].titulo);
            printf("[Autor: %s]\n", L[i].autor);
            printf("[Ano publicado: %d]\n", L[i].anoPub);
            printf("[Status: %s]\n", L[i].status == 1 ? "Disponivel" : "Emprestado");
        }
    }

    if (!encontrado) {
        printf("\nNenhum livro encontrado com o termo '%s'.\n", termoBusca);
    }
    printf("=======================\n");
}

void ListarLivro() {
    if (qtdLivros == 0) {
        printf("\nNenhum livro cadastrado.\n");
        return;
    }
    for (int i = 0; i < qtdLivros; i++) {
        printf("=======================\n");
        printf("[ID Livro: %d]\n", L[i].idLivro);
        printf("[Titulo: %s]\n", L[i].titulo);
        printf("[Autor: %s]\n", L[i].autor);
        printf("[Ano publicado: %d]\n", L[i].anoPub);
        printf("[Status: %s]\n", L[i].status == 1 ? "Disponivel" : "Emprestado");
    }
    printf("=======================\n");
}

void ListarLivrosDisponiveis() {
    int count = 0;
    printf("\n--- LIVROS DISPONÍVEIS ---\n");
    for (int i = 0; i < qtdLivros; i++) {
        if (L[i].status == 1) {
            printf("ID: %d | Título: %s | Autor: %s\n", L[i].idLivro, L[i].titulo, L[i].autor);
            count++;
        }
    }
    if (count == 0) {
        printf("Nenhum livro disponível para empréstimo.\n");
    }
    printf("---------------------------\n");
}



void AtualizarInformacoes() {
    char tituloBusca[100];
    int livroEncontrado = 0;

    printf("Titulo do livro a ser atualizado: ");
    fgets(tituloBusca, sizeof(tituloBusca), stdin);
    tituloBusca[strcspn(tituloBusca, "\n")] = 0;

    for (int i = 0; i < qtdLivros; i++) {
        if (strcmp(tituloBusca, L[i].titulo) == 0) {
            livroEncontrado = 1;

            printf("\n--- ATUALIZAR LIVRO ID %d ---\n", L[i].idLivro);

            printf("Novo titulo (atual: %s): ", L[i].titulo);
            fgets(L[i].titulo, sizeof(L[i].titulo), stdin);
            L[i].titulo[strcspn(L[i].titulo, "\n")] = 0;

            printf("Novo autor (atual: %s): ", L[i].autor);
            fgets(L[i].autor, sizeof(L[i].autor), stdin);
            L[i].autor[strcspn(L[i].autor, "\n")] = 0;

            printf("Novo ano publicado (atual: %d): ", L[i].anoPub);
            if (scanf("%d", &L[i].anoPub) != 1) {
                printf("[AVISO] Entrada de Ano inválida, mantendo valor antigo.\n");
            }
            LimparBuffer();

            printf("\nInformacoes do livro atualizadas com sucesso!\n");
            break;
        }
    }

    if (!livroEncontrado) {
        printf("\nLivro com o titulo '%s' nao encontrado.\n", tituloBusca);
    }
}

// =========================================================================
// FUNÇÕES DE USUÁRIO
// =========================================================================

void CadastrarUsuario() {
    if (qtdUsuarios >= MAX_ITENS) {
        printf("[ERRO] Limite máximo de usuários atingido (%d).\n", MAX_ITENS);
        return;
    }

    printf("\n============ Cadastrar Usuario ============\n");
    printf("ID gerado: %d\n", proximoIdUsuario);
    U[qtdUsuarios].idUsuario = proximoIdUsuario++;

    printf("Nome: ");
    fgets(U[qtdUsuarios].nome, sizeof(U[qtdUsuarios].nome), stdin);
    U[qtdUsuarios].nome[strcspn(U[qtdUsuarios].nome, "\n")] = 0;

    printf("Email: ");
    fgets(U[qtdUsuarios].email, sizeof(U[qtdUsuarios].email), stdin);
    U[qtdUsuarios].email[strcspn(U[qtdUsuarios].email, "\n")] = 0;

    U[qtdUsuarios].ativo = 1; // Novo usuário sempre ativo

    printf("Usuário '%s' (ID %d) cadastrado e ATIVO.\n", U[qtdUsuarios].nome, U[qtdUsuarios].idUsuario);
    qtdUsuarios++;
    printf("===============================================\n");
}

void BuscarUsuarioPorNome() {
    char nomeUsuario[100];
    int encontrado = 0;
    printf("Nome do Usuário: ");
    fgets(nomeUsuario, sizeof(nomeUsuario), stdin);
    nomeUsuario[strcspn(nomeUsuario, "\n")] = 0;

    for (int i = 0; i < qtdUsuarios; i++) {
        if (strcmp(nomeUsuario, U[i].nome) == 0) {
            encontrado = 1;
            printf("=======================\n");
            printf("Nome: %s\n", U[i].nome);
            printf("ID: %d\n", U[i].idUsuario);
            printf("Email: %s\n", U[i].email);
            printf("Status: %s\n", U[i].ativo == 1 ? "Ativo" : "Inativo");
            printf("=======================\n");
        }
    }
    if (!encontrado) {
        printf("\nUsuário com o nome '%s' não encontrado.\n", nomeUsuario);
    }
}

void ListarTodosUsuariosCadastrados() {
    if (qtdUsuarios == 0) {
        printf("\nNenhum usuário cadastrado.\n");
        return;
    }
    printf("\n--- LISTA DE USUÁRIOS ---\n");
    for (int i = 0; i < qtdUsuarios; i++) {
        printf("=======================\n");
        printf("Nome: %s\n", U[i].nome);
        printf("ID: %d\n", U[i].idUsuario);
        printf("Email: %s\n", U[i].email);
        printf("Status: %s\n", U[i].ativo == 1 ? "Ativo" : "Inativo");
    }
    printf("=======================\n");
}

void InativarUsuario() {
    int idUsuarioBusca;
    int encontrado = 0;
    printf("ID do Usuário a ser inativado: ");
    if (scanf("%d", &idUsuarioBusca) != 1) {
        LimparBuffer();
        printf("[ERRO] ID inválido.\n");
        return;
    }
    LimparBuffer();

    for (int i = 0; i < qtdUsuarios; i++) {
        if (U[i].idUsuario == idUsuarioBusca) {
            encontrado = 1;
            if (U[i].ativo == 0) {
                printf("[AVISO] Usuário '%s' (ID %d) já está inativo.\n", U[i].nome, idUsuarioBusca);
            } else {
                U[i].ativo = 0;
                printf("Usuário '%s' (ID %d) inativado com sucesso.\n", U[i].nome, idUsuarioBusca);
            }
            break;
        }
    }
    if (!encontrado) {
        printf("[ERRO] Usuário ID %d não encontrado.\n", idUsuarioBusca);
    }
}

// =========================================================================
// FUNÇÕES DE EMPRÉSTIMO
// =========================================================================

void RegistrarEmprestimo() {
    if (contadorEmprestimos >= MAX_ITENS) {
        printf("[ERRO] Limite máximo de registros de empréstimo atingido (%d).\n", MAX_ITENS);
        return;
    }
    
    int idUsuarioBusca, idLivroBusca;
    int posLivro = -1;
    int posUser = -1;

    printf("\n--- REGISTRAR NOVO EMPRÉSTIMO ---\n");
    
    printf("ID do Usuário: ");
    if (scanf("%d", &idUsuarioBusca) != 1) { LimparBuffer(); printf("[ERRO] ID de Usuário inválido.\n"); return; }
    LimparBuffer();
    
    printf("ID do Livro: ");
    if (scanf("%d", &idLivroBusca) != 1) { LimparBuffer(); printf("[ERRO] ID de Livro inválido.\n"); return; }
    LimparBuffer();

    // 1. Validar Usuário
    for(int i = 0; i < qtdUsuarios; i++){
        if(U[i].idUsuario == idUsuarioBusca){
            if(U[i].ativo == 1){
                posUser = i;
            } else {
                printf("ERRO: Usuário inativo. Não pode realizar empréstimo.\n");
                return;
            }
            break;
        }
    }
    if (posUser == -1) { printf("ERRO: Usuário ID %d não encontrado.\n", idUsuarioBusca); return; }

    // 2. Validar Livro
    for(int i = 0; i < qtdLivros; i++){
        if(L[i].idLivro == idLivroBusca){
            if(L[i].status == 1){
                posLivro = i;
            } else {
                printf("ERRO: Livro '%s' indisponível (já emprestado).\n", L[i].titulo);
                return;
            }
            break;
        }
    }
    if (posLivro == -1) { printf("ERRO: Livro ID %d não encontrado.\n", idLivroBusca); return; }

    // 3. Registrar o Empréstimo
    E[contadorEmprestimos].idEmprestimo = proximoIdEmprestimo++; // Novo ID
    E[contadorEmprestimos].idUsuario = idUsuarioBusca;
    E[contadorEmprestimos].idLivro = idLivroBusca;
    
    // Obter data atual
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(E[contadorEmprestimos].dataEmprestimo, sizeof(E[contadorEmprestimos].dataEmprestimo), "%d/%m/%Y", tm);
    
    E[contadorEmprestimos].ativo = 1;

    // 4. Atualizar Status do Livro
    L[posLivro].status = 0;
    
    printf("\nEmpréstimo ID %d registrado com sucesso.\n", E[contadorEmprestimos].idEmprestimo);
    printf("Livro: %s | Usuário: %s | Data: %s\n", L[posLivro].titulo, U[posUser].nome, E[contadorEmprestimos].dataEmprestimo);
    contadorEmprestimos++; // Incrementa a contagem total de registros
}

void RegistrarDevolucaoLivro() {
    int idEmprestimo;
    int encontrado = 0;

    printf("ID do Emprestimo a ser devolvido: ");
    if (scanf("%d", &idEmprestimo) != 1) {
        LimparBuffer();
        printf("[ERRO] ID de Empréstimo inválido.\n");
        return;
    }
    LimparBuffer();

    for (int i = 0; i < contadorEmprestimos; i++) {
        if (E[i].idEmprestimo == idEmprestimo) {
            encontrado = 1;
            if (E[i].ativo == 0) {
                printf("[AVISO] Este empréstimo já foi concluído/devolvido.\n");
                return;
            }
            
            // 1. Marca empréstimo como concluído
            E[i].ativo = 0;
            
            // 2. Marca o livro como disponível
            for (int j = 0; j < qtdLivros; j++) {
                if (L[j].idLivro == E[i].idLivro) {
                    L[j].status = 1;
                    printf("Livro '%s' marcado como DISPONÍVEL.\n", L[j].titulo);
                    break;
                }
            }
            
            printf("Empréstimo ID %d devolvido com sucesso.\n", idEmprestimo);
            return;
        }
    }

    if (!encontrado) {
        printf("[ERRO] Empréstimo ID %d não encontrado.\n", idEmprestimo);
    }
}

void ListarTodosEmprestimosAtivo() {
    int count = 0;
    printf("\n--- LISTA DE EMPRÉSTIMOS ATIVOS ---\n");

    for (int i = 0; i < contadorEmprestimos; i++) {
        if (E[i].ativo == 1) {
            count++;
            char tituloLivro[100] = "Desconhecido";
            char nomeUsuario[100] = "Desconhecido";
            
            // Busca o título do livro
            for (int k = 0; k < qtdLivros; k++) {
                if (E[i].idLivro == L[k].idLivro) {
                    strcpy(tituloLivro, L[k].titulo);
                    break;
                }
            }
            
            // Busca o nome do usuário
            for (int j = 0; j < qtdUsuarios; j++) {
                if (E[i].idUsuario == U[j].idUsuario) {
                    strcpy(nomeUsuario, U[j].nome);
                    break;
                }
            }

            printf("======================================\n");
            printf("ID Emprestimo: %d\n", E[i].idEmprestimo);
            printf("Livro: %s (ID: %d)\n", tituloLivro, E[i].idLivro);
            printf("Emprestado para: %s (ID: %d)\n", nomeUsuario, E[i].idUsuario);
            printf("Data: %s\n", E[i].dataEmprestimo);
        }
    }
    if (count == 0) {
        printf("Nenhum empréstimo ativo no momento.\n");
    }
    printf("--------------------------------------\n");
}

void ListarHistoricoEmprestimos() {
    if (contadorEmprestimos == 0) {
        printf("\nNenhum empréstimo registrado no histórico.\n");
        return;
    }
    printf("\n--- HISTÓRICO DE EMPRÉSTIMOS ---\n");

    for (int i = 0; i < contadorEmprestimos; i++) {
        char tituloLivro[100] = "Desconhecido";
        char nomeUsuario[100] = "Desconhecido";

        // Busca o título do livro
        for (int k = 0; k < qtdLivros; k++) {
            if (E[i].idLivro == L[k].idLivro) {
                strcpy(tituloLivro, L[k].titulo);
                break;
            }
        }

        // Busca o nome do usuário
        for (int j = 0; j < qtdUsuarios; j++) {
            if (E[i].idUsuario == U[j].idUsuario) {
                strcpy(nomeUsuario, U[j].nome);
                break;
            }
        }
        
        printf("======================================\n");
        printf("ID Emprestimo: %d\n", E[i].idEmprestimo);
        printf("Livro: %s\n", tituloLivro);
        printf("Emprestado para: %s\n", nomeUsuario);
        printf("Data de Empréstimo: %s\n", E[i].dataEmprestimo);
        printf("Status: %s\n", E[i].ativo == 1 ? "Ativo" : "Concluído");
    }
    printf("--------------------------------------\n");
}

void MostrarLivrosEmprestadosPorUsuario() {
    int idUsuarioBusca;
    char nomeUsuario[100] = "Usuário Desconhecido";
    int encontrado = 0;
    
    printf("ID do Usuário para buscar empréstimos ativos: ");
    if (scanf("%d", &idUsuarioBusca) != 1) { LimparBuffer(); printf("[ERRO] ID de Usuário inválido.\n"); return; }
    LimparBuffer();

    // Encontra o nome do usuário
    for(int i = 0; i < qtdUsuarios; i++){
        if(U[i].idUsuario == idUsuarioBusca){
            strcpy(nomeUsuario, U[i].nome);
            encontrado = 1;
            break;
        }
    }

    if(!encontrado){
        printf("Usuário ID %d não encontrado.\n", idUsuarioBusca);
        return;
    }

    printf("\n--- LIVROS EMPRESTADOS POR %s (ID: %d) ---\n", nomeUsuario, idUsuarioBusca);

    // Lista os empréstimos ativos para este usuário
    int count = 0;
    for(int i = 0; i < contadorEmprestimos; i++){
        if(E[i].idUsuario == idUsuarioBusca && E[i].ativo == 1){
            count++;
            char tituloLivro[100] = "Livro Desconhecido";
            
            for(int j = 0; j < qtdLivros; j++){
                if(L[j].idLivro == E[i].idLivro){
                    strcpy(tituloLivro, L[j].titulo);
                    break;
                }
            }
            
            printf("Título: %s | ID Livro: %d\n", tituloLivro, E[i].idLivro);
            printf("Data de Empréstimo: %s\n", E[i].dataEmprestimo);
            printf("-----------------------\n");
        }
    }

    if(count == 0){
        printf("%s não possui livros emprestados no momento.\n", nomeUsuario);
    }
}

// =========================================================================
// FUNÇÃO DE RELATÓRIOS
// =========================================================================

void Relatorios() {
    int opcao;
    do {
        printf("\n===== MENU DE RELATÓRIOS =====\n");
        printf("1. Quantidade total de livros cadastrados\n");
        printf("2. Quantidade de livros disponíveis e emprestados\n");
        printf("3. Usuário com mais empréstimos ativos\n");
        printf("4. Livro mais emprestado (Histórico)\n");
        printf("0. Voltar\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            LimparBuffer();
            printf("Entrada inválida.\n");
            continue;
        }
        LimparBuffer();

        switch (opcao) {
            case 1: {
                printf("\nTotal de livros cadastrados: %d\n", qtdLivros);
                break;
            }
            case 2: {
                int disponiveis = 0, emprestados = 0;
                for (int i = 0; i < qtdLivros; i++) {
                    if (L[i].status == 1) disponiveis++;
                    else emprestados++;
                }
                printf("\nLivros disponíveis: %d\n", disponiveis);
                printf("Livros emprestados: %d\n", emprestados);
                break;
            }
            case 3: {
                // Usuário com mais empréstimos ativos
                int contagemEmprestimos[MAX_ITENS] = {0}; // Inicializa contadores
                int maiorQtd = 0;
                int idUsuarioMaisEmprestou = -1;
                
                // 1. Conta empréstimos ativos por ID de usuário
                for (int j = 0; j < contadorEmprestimos; j++) {
                    if (E[j].ativo == 1) {
                        for(int i = 0; i < qtdUsuarios; i++){
                             if(E[j].idUsuario == U[i].idUsuario){
                                contagemEmprestimos[i]++;
                                if (contagemEmprestimos[i] > maiorQtd) {
                                    maiorQtd = contagemEmprestimos[i];
                                    idUsuarioMaisEmprestou = U[i].idUsuario;
                                }
                                break;
                            }
                        }
                    }
                }
                
                // 2. Exibe o resultado
                if (idUsuarioMaisEmprestou != -1 && maiorQtd > 0) {
                    char nomeUsuario[100] = "Desconhecido";
                    for(int i = 0; i < qtdUsuarios; i++){
                        if(U[i].idUsuario == idUsuarioMaisEmprestou){
                            strcpy(nomeUsuario, U[i].nome);
                            break;
                        }
                    }
                    printf("\nUsuário com mais empréstimos ativos: %s (ID: %d) - %d empréstimos ativos\n",
                           nomeUsuario, idUsuarioMaisEmprestou, maiorQtd);
                } else {
                    printf("\nNenhum empréstimo ativo encontrado.\n");
                }
                break;
            }
            case 4: {
                // Livro mais emprestado (contagem total no histórico)
                int contagemEmprestimosLivro[MAX_ITENS] = {0};
                int maiorQtd = 0;
                int idLivroMaisEmprestado = -1;

                // 1. Conta total de empréstimos por ID de livro
                for (int j = 0; j < contadorEmprestimos; j++) {
                    for(int i = 0; i < qtdLivros; i++){
                        if(E[j].idLivro == L[i].idLivro){
                            contagemEmprestimosLivro[i]++;
                            if (contagemEmprestimosLivro[i] > maiorQtd) {
                                maiorQtd = contagemEmprestimosLivro[i];
                                idLivroMaisEmprestado = L[i].idLivro;
                            }
                            break;
                        }
                    }
                }

                // 2. Exibe o resultado
                if (idLivroMaisEmprestado != -1 && maiorQtd > 0) {
                    char tituloLivro[100] = "Desconhecido";
                    for(int i = 0; i < qtdLivros; i++){
                        if(L[i].idLivro == idLivroMaisEmprestado){
                            strcpy(tituloLivro, L[i].titulo);
                            break;
                        }
                    }
                    printf("\nLivro mais emprestado: %s (ID: %d) - %d vezes.\n",
                           tituloLivro, idLivroMaisEmprestado, maiorQtd);
                } else {
                    printf("\nNenhum empréstimo registrado para determinar o livro mais emprestado.\n");
                }
                break;
            }
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);
}

// =========================================================================
// FUNÇÃO PRINCIPAL
// =========================================================================

int main()
{
    // === PONTO 2: Leitura dos Arquivos na Inicialização ===
    carregarMaxIds(); // Primeiro carrega os IDs
    carregarDados();  // Depois carrega os dados (que dependem da lógica dos IDs)
    
    int opcao = -1;
    int opcao2 = -1;
    int opcao3 = -1;
    int opcao4 = -1;

    while (1) {
        
        MostrarInformacoes();
        
        if (scanf("%d", &opcao) != 1) {
            LimparBuffer();
            printf("[ERRO] Opção inválida. Tente novamente.\n");
            continue;
        }
        LimparBuffer();

        switch (opcao) {
            case 1:
                opcao2 = -1;
                while (opcao2 != 0) {
                    MostrarInformacoes2();
                    if (scanf("%d", &opcao2) != 1) {
                        LimparBuffer();
                        printf("[ERRO] Opção inválida. Tente novamente.\n");
                        continue;
                    }
                    LimparBuffer();

                    switch (opcao2) {
                        case 1: AdicionarLivro(); break;
                        case 2: BuscarLivro(); break;
                        case 3: ListarLivro(); break;
                        case 4: printf("Use '3- Empréstimo -> 1- Registrar Novo Empréstimo' para marcar como emprestado.\n"); break;
                        case 5: printf("Use '3- Empréstimo -> 2- Registrar Devolução' para marcar como devolvido.\n"); break;
                        case 6: ListarLivrosDisponiveis(); break;
                        case 7: AtualizarInformacoes(); break;
                        case 0: printf("Voltando ao Menu Principal\n"); break;
                        default: printf("Opção inválida!\n");
                    }
                }
                break;

            case 2:
                opcao3 = -1;
                while (opcao3 != 0) {
                    MostrarInformacoes3();
                    if (scanf("%d", &opcao3) != 1) {
                        LimparBuffer();
                        printf("[ERRO] Opção inválida. Tente novamente.\n");
                        continue;
                    }
                    LimparBuffer();
                    
                    switch (opcao3) {
                        case 1: CadastrarUsuario(); break;
                        case 2: BuscarUsuarioPorNome(); break;
                        case 3: ListarTodosUsuariosCadastrados(); break;
                        case 4: InativarUsuario(); break;
                        case 0: printf("Voltando ao Menu Principal...\n"); break;
                        default: printf("Opção inválida!\n");
                    }
                }
                break;

            case 3:
                opcao4 = -1;
                while (opcao4 != 0) {
                    MostrarInformacoes4();
                    if (scanf("%d", &opcao4) != 1) {
                        LimparBuffer();
                        printf("[ERRO] Opção inválida. Tente novamente.\n");
                        continue;
                    }
                    LimparBuffer();
                    
                    switch (opcao4) {
                        case 1: RegistrarEmprestimo(); break;
                        case 2: RegistrarDevolucaoLivro(); break;
                        case 3: ListarTodosEmprestimosAtivo(); break;
                        case 4: ListarHistoricoEmprestimos(); break;
                        case 5: MostrarLivrosEmprestadosPorUsuario(); break;
                        case 0: printf("Voltando ao Menu Principal\n"); break;
                        default: printf("Opção inválida!\n");
                    }
                }
                break;
            
            case 4:
                Relatorios();
                break;
            
            case 0:
                printf("Encerrando o sistema...\n");
                salvarDados();
                salvarMaxIds();
                return 0;
            default:
                printf("Opção inválida! Escolha de 0 a 4.\n");
        }
    }

    return 0;
}