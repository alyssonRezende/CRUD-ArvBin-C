#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #define clear "cls"
    #define pause "pause"
#elif __linux__
    #define clear "clear"
    #define pause "read -p 'Pressione enter para continuar...' var"
#endif

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
        printf("Sem memoria disponivel!\n");
        exit(0);
    }
    printf("--- BEM VINDO A COMPRA DE INGRESSOS ---\n\n");
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
                printf("\n\e[0;31mPoltrona ja ocupada!\e[0m\n\n");
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
    printf("\n\e[0;32mIngresso comprado com sucesso!\e[0m\n\n");
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

void exibe_inOrder(ArvBin raiz) {
    if (raiz == NULL) {
        return;
    }
    exibe_inOrder(raiz->esq);
    printf(" - %d ", raiz->num);
    exibe_inOrder(raiz->dir);
}

ArvBin encontrar(ArvBin raiz, int num) {
    if (raiz != NULL) {
        if (num == raiz->num) {
            printf("\nA poltrona %d ja esta reservada!\n", raiz->num);
        } else {
            printf("\nA poltrona %d se encontra disponivel para compra!\n", num);
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
        printf("nao foi possivel cancelar a compra, pois o ingresso nao foi vendido\n\nOu ja foi cancelado\n\n");
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
        printf("nao foi possivel cancelar a compra, pois o ingresso nao foi vendido\n\nOu ja foi cancelado\n\n");
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
    printf("Compra cancelada com sucesso!\n\n");
}

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
        printf("Erro de alocacao...\n");
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


ArvBin criarNo(int num) {
    ArvBin novo = (ArvBin)malloc(sizeof(no));
    if (novo != NULL) {
        novo->num = num;
        novo->esq = NULL;
        novo->dir = NULL;
    } else {
        printf("Erro de alocação...");
        exit(0);
    }
    return novo;
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
    printf("Arvore balanceada com sucesso!\n");
}

int main() {
    ArvBin raiz = criarArvore();

    int num;

    int op;
    do {
        system(clear);
        printf("---  MENU TEATRO --- \n\n");
        printf("1 - Comprar ingresso\n");
        printf("2 - Exibir ingressos ja reservados\n");
        printf("3 - Buscar poltrona\n");
        printf("4 - Editar poltrona\n");
        printf("5 - Cancelar compra\n");
        // printf("7 - Teste de balanceamento\n");
        printf("0 - Encerrar\n");
        scanf("%d", &op);

        switch (op) {
            case 0:
                break;
            case 1:
                system(clear);
                comprarIngresso(&raiz);
                system(pause);
                break;
            case 2:
                system(clear);
                printf("--- INGRESSOS VENDIDOS ---\n\n");
                exibe_inOrder(raiz);
                printf("\n\n");
                balancear(&raiz);
                system(pause);
                break;
            case 3:
                system(clear);
                printf("Digite o numero que deseja buscar: ");
                scanf("%d", &num);
                encontrar(raiz, num);
                balancear(&raiz);
                system(pause);
                break;
            case 4:
                system(clear);
                // Implementar função de editar poltronas - excluir e adicionar, arrumar campos de texto
                balancear(&raiz);
                system(pause);
                break;
            case 5:
                system(clear);
                deletar(&raiz);
                balancear(&raiz);
                system(pause);
                break;
            case 7:
                system(clear);
                balancear(&raiz);
                system(pause);
                break;
            default:
                printf("Numero invalido");
        }
    } while (op != 0);

    libera_ArvBin(&raiz);

    return 0;
}
