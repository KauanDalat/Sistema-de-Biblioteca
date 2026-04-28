#include "biblioteca.h"

int main() {
    // Inicialização
    carregarMaxIds(); 
    carregarDados();  
    
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
                        continue;
                    }
                    LimparBuffer();
                    switch (opcao2) {
                        case 1: AdicionarLivro(); break;
                        case 2: BuscarLivro(); break;
                        case 3: ListarLivro(); break;
                        case 6: ListarLivrosDisponiveis(); break;
                        case 7: AtualizarInformacoes(); break;
                        case 0: break;
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
                        continue;
                    }
                    LimparBuffer();
                    switch (opcao3) {
                        case 1: CadastrarUsuario(); break;
                        case 2: BuscarUsuarioPorNome(); break;
                        case 3: ListarTodosUsuariosCadastrados(); break;
                        case 4: InativarUsuario(); break;
                        case 0: break;
                    }
                }
                break;

            case 3:
                opcao4 = -1;
                while (opcao4 != 0) {
                    MostrarInformacoes4();
                    if (scanf("%d", &opcao4) != 1) {
                        LimparBuffer();
                        continue;
                    }
                    LimparBuffer();
                    switch (opcao4) {
                        case 1: RegistrarEmprestimo(); break;
                        case 2: RegistrarDevolucaoLivro(); break;
                        case 3: ListarTodosEmprestimosAtivo(); break;
                        case 4: ListarHistoricoEmprestimos(); break;
                        case 5: MostrarLivrosEmprestadosPorUsuario(); break;
                        case 0: break;
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
                printf("Opção inválida!\n");
        }
    }
    return 0;
}
