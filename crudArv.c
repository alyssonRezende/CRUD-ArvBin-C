#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #define clear "cls"
    #define pause "pause"
#elif __linux__
    #define clear "clear"
    #define pause "read -p 'Pressione enter para continuar...' var"
#endif

#define COLOR_RESET "\e[0m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define BOLD_WHT "\e[1;37m"

/*-------------- ESTRUTURA PARA ARVORE BINÁRIA --------------*/

struct No {
    int num;
    struct No *esq;
    struct No *dir;
};

typedef struct No no;
typedef no *ArvBin;

ArvBin criarArvore() {
    ArvBin raiz = (ArvBin)malloc(sizeof(no));
    if (raiz != NULL) {
        raiz = NULL;
    } else {
        printf("Erro de alocação...");
        exit(0);
    }
    return raiz;
}

void comprarIngresso(ArvBin *raiz) {
    no *novo = (no *)malloc(sizeof(no));
    if (novo == NULL) {
        printf( RED "Sem memoria disponivel!\n" COLOR_RESET);
        exit(0);
    }
    printf( BOLD_WHT "--- BEM VINDO A COMPRA DE INGRESSOS ---\n\n");
    printf("Digite o numero do ingresso: ");
    scanf("%d", &novo->num);
    novo->dir = NULL;
    novo->esq = NULL;

    if (novo->num <= 0 || novo->num > 101) {
        printf("\n\e[0;31mIngresso indisponivel,\e[0m escolha uma poltrona de 1 a 100!\n\n");
        free(novo);
        return;
    }

    if (*raiz == NULL) {
        *raiz = novo;
    } else {
        no *atual = *raiz;
        no *ant = NULL;
        while (atual != NULL) {
            ant = atual;

            if (novo->num == atual->num) {
                printf( RED "\nPoltrona ja ocupada!\n" COLOR_RESET );
                free(novo);
                return;
            }
            if (novo->num > atual->num) {
                atual = atual->dir;
            } else {
                atual = atual->esq;
            }
        }
        if (novo->num > ant->num) {
            ant->dir = novo;
        } else {
            ant->esq = novo;
        }
    }
    printf( GRN "\nIngresso comprado com sucesso!\n\n" COLOR_RESET);
}

void libera_NO(no *no_liberar) {
    if (no_liberar == NULL) {
        return;
    } else {
        libera_NO(no_liberar->esq);
        libera_NO(no_liberar->dir);
        free(no_liberar);
    }
}

void libera_ArvBin(ArvBin *raiz) {
    if (*raiz != NULL) {
        libera_NO(*raiz);
    }
    free(*raiz);
}

void exibe_preOrder(ArvBin* raiz) {

    if (*raiz == NULL) {
        return;
    }
    printf("%d\n", (*raiz)->num);
    exibe_preOrder(&((*raiz)->esq));
    exibe_preOrder(&((*raiz)->dir));
}

void exibe_inOrder(ArvBin raiz) {
    if (raiz == NULL) {
        return;
    }
    exibe_inOrder(raiz->esq);
    printf(" - %d ", raiz->num);
    exibe_inOrder(raiz->dir);
}

void exibe_posOrder(ArvBin* raiz) {
    if (*raiz == NULL) {
        return;
    }
    exibe_posOrder(&((*raiz)->esq));
    exibe_posOrder(&((*raiz)->dir));
    printf("%d\n", (*raiz)->num);
}

int altura(ArvBin raiz){
    if(raiz == NULL){
        return -1;
    }
    else{
        int esq = altura(raiz->esq);
        int dir = altura(raiz->dir);
        if (esq>dir){
            return esq + 1;
        }else{
            return dir + 1;
        }
    }
}

ArvBin encontrar(ArvBin raiz, int num) {
    if (raiz != NULL) {
        if (num == raiz->num) {
            printf( RED "\nA poltrona %d ja esta reservada!\n" COLOR_RESET, raiz->num);
        } else {
            printf( GRN "\nA poltrona %d se encontra disponivel para compra!\n" COLOR_RESET, num);
        }
        if (num < raiz->num) {
            return encontrar(raiz->esq, num);
        } else if (num > raiz->num) {
            return encontrar(raiz->dir, num);
        }
    }
    return raiz;
}

void deletar(ArvBin *raiz) {
    int num;

    printf("Digite o numero do ingresso: ");
    scanf("%d", &num);

    if (*raiz == NULL) {
        printf( RED "Mao foi possivel cancelar a compra, pois o ingresso nao foi vendido\n\nOu ja foi cancelado\n\n" COLOR_RESET );
        return;
    }

    no *atual = *raiz;
    no *pai = NULL;
    while (atual != NULL) {
        if (num == atual->num) {
            break;
        }
        pai = atual;
        if (num < atual->num) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }

    if (atual == NULL) {
        printf( RED "Nao foi possivel cancelar a compra, pois o ingresso nao foi vendido\n\nOu ja foi cancelado\n\n" COLOR_RESET );
        return;
    }

    if (atual->esq == NULL && atual->dir == NULL) {
        if (pai != NULL) {
            if (pai->esq == atual) {
                pai->esq = NULL;
            } else {
                pai->dir = NULL;
            }
        } else {
            free(*raiz);
            *raiz = NULL;
        }
        free(atual);
    } else if (atual->esq != NULL && atual->dir != NULL) {
        no *sucessor = atual->dir;
        pai = NULL;
        while (sucessor->esq != NULL) {
            pai = sucessor;
            sucessor = sucessor->esq;
        }
        atual->num = sucessor->num;
        if (pai != NULL) {
            pai->esq = NULL;
        } else {
            atual->dir = sucessor->dir;
        }
        free(sucessor);
    } else {
        no *filho = (atual->esq != NULL) ? atual->esq : atual->dir;
        if (pai != NULL) {
            if (pai->esq == atual) {
                pai->esq = filho;
            } else {
                pai->dir = filho;
            }
        } else {
            *raiz = filho;
        }
        free(atual);
    }
    printf( GRN "Compra cancelada com sucesso!\n\n" COLOR_RESET );
}


/*-------------- ESTRUTURA PARA LISTA DE BALANCEAMENTO ESTÁTICO --------------*/ 

struct Node {
    int num;
    struct Node *prox;
    struct Node *ant;
};

typedef struct Node node;
typedef node *LISTA;

LISTA insereFinal(LISTA lista, int num) {
    node *novo = (node *)malloc(sizeof(node));
    if (novo == NULL) {
        printf( RED "Erro de alocacao...\n" COLOR_RESET);
        exit(0);
    }

    novo->num = num;

    node *tmp;
    tmp = lista;
    novo->prox = NULL;

    if (lista == NULL) {
        return novo;
    } else {
        while (tmp->prox != NULL) {
            tmp = tmp->prox;
        }
        tmp->prox = novo;
        novo->ant = tmp;
        return lista;
    }
}

void exibeLista(LISTA lista) {
    if (lista == NULL) {
        printf("LISTA VAZIA");
    } else {
        node *tmp;
        tmp = lista;
        while (tmp != NULL) {
            printf(" - %d ", tmp->num);
            tmp = tmp->prox;
        }
    }
}

void liberar(LISTA lista) {
    if (lista == NULL) {
        printf("LISTA VAZIA");
    } else {
        node *tmp;

        while (lista != NULL) {
            tmp = lista;
            lista = lista->prox;
            free(tmp);
        }
    }
}

LISTA copiaParaLista(ArvBin raiz, LISTA lista) {
    if (raiz != NULL) {
        lista = copiaParaLista(raiz->esq, lista);
        lista = insereFinal(lista, raiz->num);
        lista = copiaParaLista(raiz->dir, lista);
    }
    return lista;
}


/*------------------- MÉTODOS DE BALANCEAMENTO ----------------------*/

ArvBin criarNo(int num) {
    ArvBin novo = (ArvBin)malloc(sizeof(no));
    if (novo != NULL) {
        novo->num = num;
        novo->esq = NULL;
        novo->dir = NULL;
    } else {
        printf( RED "Erro de alocação..." COLOR_RESET );
        exit(0);
    }
    return novo;
}

int estaBalanceada(ArvBin *raiz) {
    int highDif = altura((*raiz)->dir) - altura((*raiz)->esq);
    printf("A diferenca de altura das sub-arvores e de: %d\n", highDif);
    if(highDif < (-1) || highDif > 1)
        return 0;
    return 1;
}

ArvBin construirArvoreBalanceada(int *array, int inicio, int fim) {
    if (inicio > fim) {
        return NULL;
    }

    int meio = (inicio + fim) / 2;
    ArvBin raiz = criarNo(array[meio]);

    raiz->esq = construirArvoreBalanceada(array, inicio, meio - 1);
    raiz->dir = construirArvoreBalanceada(array, meio + 1, fim);

    return raiz;
}

ArvBin reconstruirBalanceada(LISTA lista) {
    if (lista == NULL) {
        return NULL;
    }

    int count = 0;
    node *tmp = lista;
    while (tmp != NULL) {
        count++;
        tmp = tmp->prox;
    }

    int *array = (int *)malloc(count * sizeof(int));
    tmp = lista;
    for (int i = 0; i < count; i++) {
        array[i] = tmp->num;
        tmp = tmp->prox;
    }

    return construirArvoreBalanceada(array, 0, count - 1);
}

void balancear(ArvBin *raiz){
    LISTA lista = NULL;
    lista = copiaParaLista(*raiz, lista);

    *raiz = reconstruirBalanceada(lista);   
    printf( GRN "Arvore balanceada com sucesso!\n" COLOR_RESET );
}

/*------------------- OUTROS MÉTODOS E MAIN ----------------------*/

no *encontrarNo(ArvBin raiz, int num) {
    if (raiz == NULL) {
        return NULL;
    } else {
        if (num == raiz->num) {
            return raiz;
        } else if (num < raiz->num) {
            return encontrarNo(raiz->esq, num);
        } else {
            return encontrarNo(raiz->dir, num);
        }
    }
}

void editarPoltrona(ArvBin *raiz) {
    int poltronaAntiga, novaPoltrona;

    printf("Digite o numero da poltrona que deseja editar: ");
    scanf("%d", &poltronaAntiga);

    no *noEditar = encontrarNo(*raiz, poltronaAntiga);

    if (noEditar == NULL) {
        printf( RED "\nPoltrona nao encontrada!\n" COLOR_RESET );
        return;
    }

    printf("\nDigite o novo numero para a poltrona: ");
    scanf("%d", &novaPoltrona);

    // Inserir a nova poltrona
    no *novo = (no *)malloc(sizeof(no));
    novo->num = novaPoltrona;
    novo->dir = NULL;
    novo->esq = NULL;

    if (novo->num <= 0 || novo->num > 101) {
        printf("\n\e[0;31mIngresso indisponivel,\e[0m escolha uma poltrona de 1 a 100!\n\n");
        free(novo);
        return;
    }

    if (*raiz == NULL) {
        *raiz = novo;
    } else {
        no *tmp = *raiz;
        no *ant = NULL;
        while (tmp != NULL) {
            ant = tmp;

            if (novo->num == tmp->num) {
                printf( RED "\n \e A poltrona ja esta ocupada!\e\n\n" COLOR_RESET);
                free(novo);
                return;
            }
            if (novo->num > tmp->num) {
                tmp = tmp->dir;
            } else {
                tmp = tmp->esq;
            }
        }
        if (novo->num > ant->num) {
            ant->dir = novo;
        } else {
            ant->esq = novo;
        }
    }

    // Excluir a poltrona antiga
    no *atual = *raiz;
    no *pai = NULL;
    while (atual != NULL) {
        if (poltronaAntiga == atual->num) {
            break;
        }
        pai = atual;
        if (poltronaAntiga < atual->num) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }

    if (atual == NULL) {
        return;
    }

    if (atual->esq == NULL && atual->dir == NULL) {
        if (pai != NULL) {
            if (pai->esq == atual) {
                pai->esq = NULL;
            } else {
                pai->dir = NULL;
            }
        } else {
            free(*raiz);
            *raiz = NULL;
        }
        free(atual);
    } else if (atual->esq != NULL && atual->dir != NULL) {
        no *sucessor = atual->dir;
        pai = NULL;
        while (sucessor->esq != NULL) {
            pai = sucessor;
            sucessor = sucessor->esq;
        }
        atual->num = sucessor->num;
        if (pai != NULL) {
            pai->esq = NULL;
        } else {
            atual->dir = sucessor->dir;
        }
        free(sucessor);
    } else {
        no *filho = (atual->esq != NULL) ? atual->esq : atual->dir;
        if (pai != NULL) {
            if (pai->esq == atual) {
                pai->esq = filho;
            } else {
                pai->dir = filho;
            }
        } else {
            *raiz = filho;
        }
        free(atual);
    }

    printf( GRN "\nPoltrona editada com sucesso!\n" COLOR_RESET);
}

int main() {
    ArvBin raiz = criarArvore();

    int num;

    int op;
    do {
        system(clear);
        printf(BOLD_WHT "---  MENU TEATRO --- \n\n" COLOR_RESET);
        printf("1 - Comprar ingresso\n");
        printf("2 - Exibir ingressos ja reservados\n");
        printf("3 - Buscar poltrona\n");
        printf("4 - Editar poltrona\n");
        printf("5 - Cancelar compra\n");
        printf("7 - Teste de balanceamento\n");
        printf("8 - Exibe preOrdem\n");
        printf("9 - Exibe inOrdem\n");
        printf("10 - Exibe posOrdem\n");
        printf("0 - Encerrar\n");
        scanf("%d", &op);

        switch (op) {
            case 0:
                break;
            case 1:
                system(clear);
                comprarIngresso(&raiz);
                if(!estaBalanceada(&raiz)) // verifica o balanceamento a cada inserção
                    balancear(&raiz);
                system(pause);
                break;
            case 2:
                system(clear);
                printf( BOLD_WHT "--- INGRESSOS VENDIDOS ---\n\n" COLOR_RESET);
                exibe_inOrder(raiz);
                printf("\n\n");
                system(pause);
                break;
            case 3:
                system(clear);
                printf("Digite o numero que deseja buscar: ");
                scanf("%d", &num);
                encontrar(raiz, num);
                system(pause);
                break;
            case 4:
                system(clear);
                editarPoltrona(&raiz);
                system(pause);
                break;
            case 5:
                system(clear);
                deletar(&raiz);
                system(pause); 
                break;
            case 7:
                system(clear);
                if(estaBalanceada(&raiz))
                    printf( GRN "\nEsta balanceada!\n\n" COLOR_RESET);
                else
                    printf( RED "\nNao esta balanceada!\n\n" COLOR_RESET);
                system(pause);
                break;
            case 8:
                system(clear);
                exibe_preOrder(&raiz);
                system(pause);
                break;
            case 9:
                system(clear);
                exibe_inOrder(raiz);
                system(pause);
                break;
            case 10:
                system(clear);
                exibe_posOrder(&raiz);
                system(pause);
                break;
            default:
                printf(RED "Numero invalido" COLOR_RESET);
        }
    } while (op != 0);

    libera_ArvBin(&raiz);

    return 0;
}
