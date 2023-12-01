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

ArvBin* criarArvore() {
    ArvBin *raiz = (ArvBin*) malloc (sizeof(ArvBin));
    if (raiz != NULL) {
        *raiz = NULL;
    } else {
        printf("Erro de alocação...");
        exit(0);
    }
    return raiz;
}

void comprarIngresso(ArvBin* raiz) {
    no *novo = (no *) malloc (sizeof(no));
    if (novo == NULL) {
        printf("Sem memoria disponivel!\n");
        exit(0);
    }
    printf("BEM VINDO A COMPRA DE INGRESSOS\n\n");
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

void libera_ArvBin(ArvBin* raiz) {
    if (*raiz != NULL) {
        libera_NO(*raiz);
    }
    free(raiz);
}

void exibe_preOrder(ArvBin* raiz) {

    if (*raiz == NULL) {
        return;
    }
    printf("%d\n", (*raiz)->num);
    exibe_preOrder(&((*raiz)->esq));
    exibe_preOrder(&((*raiz)->dir));
}

void exibe_inOrder(ArvBin* raiz) {
    if (*raiz == NULL) {
        return;
    }
    exibe_inOrder(&((*raiz)->esq));
    printf("%d\n", (*raiz)->num);
    exibe_inOrder(&((*raiz)->dir));
}

void exibe_posOrder(ArvBin* raiz) {
    if (*raiz == NULL) {
        return;
    }
    exibe_posOrder(&((*raiz)->esq));
    exibe_posOrder(&((*raiz)->dir));
    printf("%d\n", (*raiz)->num);
}

int contaNo(ArvBin raiz) {
   if (raiz == NULL) {
        return;
    }else{
    return 1 + contaNo(raiz->esq) + contaNo(raiz->dir);
    }
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
    if (raiz) {
        if (num == raiz->num) {
            printf("\n%d", raiz->num);
        }
        if (num < raiz->num) {
            return encontrar(raiz->esq, num);
        } else {
            return encontrar(raiz->dir, num);
        }
    }
    return raiz;
}

void deletar(ArvBin *raiz, int num) {
    if (*raiz == NULL) {
        printf("Elemento nao encontrado na arvore\n");
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
        printf("Elemento nao encontrado na arvore\n");
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
}



int main() {

    ArvBin *raiz = criarArvore();

    int exibe, num;

    int op;
    do {
        system(clear);
        printf("O que deseja: \n");
        printf("1 - Comprar ingressos\n");
        printf("2 - Exibir arvore pre-order\n");
        printf("3 - Exibir arvore in-order\n");
        printf("4 - Exibir arvore pos-order\n");
        printf("5 - Contar nos\n");
        printf("6 - Calcula altura da arvore\n");
        printf("7 - Deletar um numero\n");
        printf("0 - Encerrar\n");
        scanf("%d", &op);

        switch (op) {
        case 0:
            break;
        case 1:
            system(clear);
            comprarIngresso(raiz);
            system(pause);
            break;
        case 2:
            system(clear);
            exibe_preOrder(raiz);
            system(pause);
            break;
        case 3:
            system(clear);
            exibe_inOrder(raiz);
            system(pause);
            break;
        case 4:
            system(clear);
            exibe_posOrder(raiz);
            system(pause);
            break;
        case 5:
            system(clear);
         exibe = contaNo(*raiz);
            printf("A arvore possui %i nodes\n\n", exibe);
            system(pause);
            break;
        case 6:
            system(clear);
         exibe = altura(*raiz);
            printf("A altura da arvore e %i\n\n", exibe);
            system(pause);
            break;
        case 7:
            system(clear);
            printf("Digite o numero que deseja deletar: ");
            scanf("%d", &num);
            deletar(raiz, num);
            system(pause);
            break;
        default:
            printf("Numero invalido");
        }
    } while (op != 0);

    libera_ArvBin(raiz);
}

