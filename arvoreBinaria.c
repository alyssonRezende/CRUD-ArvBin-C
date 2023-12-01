#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define clear "cls"
#define pause "pause"
#elif __linux__
#define clear "clear"
#define pause "read -p 'Pressione enter para continuar...' var"
#endif

struct No
{
    int num;
    struct No *esq;
    struct No *dir;
};

typedef struct No no;
typedef no *ArvBin;

ArvBin criarArvore()
{
    ArvBin raiz = (ArvBin)malloc(sizeof(no));
    if (raiz != NULL)
    {
        raiz = NULL;
    }
    else
    {
        printf("Erro de alocação...");
        exit(0);
    }
    return raiz;
}

void comprarIngresso(ArvBin *raiz)
{
    no *novo = (no *)malloc(sizeof(no));
    if (novo == NULL)
    {
        printf("Sem memoria disponivel!\n");
        exit(0);
    }
    printf("--- BEM VINDO A COMPRA DE INGRESSOS ---\n\n");
    printf("Digite o numero do ingresso: ");
    scanf("%d", &novo->num);
    novo->dir = NULL;
    novo->esq = NULL;

    if (novo->num <= 0 || novo->num > 101)
    {
        printf("\n\e[0;31mIngresso indisponivel,\e[0m escolha uma poltrona de 1 a 100!\n\n");
        free(novo);
        return;
    }

    if (*raiz == NULL)
    {
        *raiz = novo;
    }
    else
    {
        no *atual = *raiz;
        no *ant = NULL;
        while (atual != NULL)
        {
            ant = atual;

            if (novo->num == atual->num)
            {
                printf("\n\e[0;31mPoltrona ja ocupada!\e[0m\n\n");
                free(novo);
                return;
            }
            if (novo->num > atual->num)
            {
                atual = atual->dir;
            }
            else
            {
                atual = atual->esq;
            }
        }
        if (novo->num > ant->num)
        {
            ant->dir = novo;
        }
        else
        {
            ant->esq = novo;
        }
    }
    printf("\n\e[0;32mIngresso comprado com sucesso!\e[0m\n\n");
}

void libera_NO(no *no_liberar)
{
    if (no_liberar == NULL)
    {
        return;
    }
    else
    {
        libera_NO(no_liberar->esq);
        libera_NO(no_liberar->dir);
        free(no_liberar);
    }
}

void libera_ArvBin(ArvBin *raiz)
{
    if (*raiz != NULL)
    {
        libera_NO(*raiz);
    }
    free(raiz);
}

void exibe_inOrder(ArvBin *raiz)
{

    if (*raiz == NULL)
    {
        return;
    }
    exibe_inOrder(&((*raiz)->esq));
    printf(" - %d ", (*raiz)->num);
    exibe_inOrder(&((*raiz)->dir));
}

int contaNo(ArvBin raiz)
{
    if (raiz == NULL)
    {
        return 0;
    }
    else
    {
        return 1 + contaNo(raiz->esq) + contaNo(raiz->dir);
    }
}

int altura(ArvBin raiz)
{
    if (raiz == NULL)
    {
        return -1;
    }
    else
    {
        int esq = altura(raiz->esq);
        int dir = altura(raiz->dir);
        if (esq > dir)
        {
            return esq + 1;
        }
        else
        {
            return dir + 1;
        }
    }
}

ArvBin encontrar(ArvBin *raiz, int num)
{
    if (*raiz)
    {
        if (num == (*raiz)->num)
        {
            printf("\nA poltrona %d ja esta reservada!\n", (*raiz)->num);
            return 1;
        }
        else
        {
            printf("\nA poltrona %d se encontra disponivel para compra!\n", num);
            return 2;
        }
        if (num < (*raiz)->num)
        {
            return encontrar(&((*raiz)->esq), num);
        }
        else if (num > (*raiz)->num)
        {
            return encontrar(&((*raiz)->dir), num);
        }
    }
    return *raiz;
}

void deletar(ArvBin *raiz, int num)
{
    if (*raiz == NULL)
    {
        printf("nao foi possivel cancelar a compra, pois o ingresso nao foi vendido\n\nOu ja foi cancelado\n\n");
        return;
    }

    no *atual = *raiz;
    no *pai = NULL;
    while (atual != NULL)
    {
        if (num == atual->num)
        {
            break;
        }
        pai = atual;
        if (num < atual->num)
        {
            atual = atual->esq;
        }
        else
        {
            atual = atual->dir;
        }
    }

    if (atual == NULL)
    {
        printf("nao foi possivel cancelar a compra, pois o ingresso nao foi vendido\n\nOu ja foi cancelado\n\n");
        return;
    }

    if (atual->esq == NULL && atual->dir == NULL)
    {
        if (pai != NULL)
        {
            if (pai->esq == atual)
            {
                pai->esq = NULL;
            }
            else
            {
                pai->dir = NULL;
            }
        }
        else
        {
            free(*raiz);
            *raiz = NULL;
        }
        free(atual);
    }
    else if (atual->esq != NULL && atual->dir != NULL)
    {
        no *sucessor = atual->dir;
        pai = NULL;
        while (sucessor->esq != NULL)
        {
            pai = sucessor;
            sucessor = sucessor->esq;
        }
        atual->num = sucessor->num;
        if (pai != NULL)
        {
            pai->esq = NULL;
        }
        else
        {
            atual->dir = sucessor->dir;
        }
        free(sucessor);
    }
    else
    {
        no *filho = (atual->esq != NULL) ? atual->esq : atual->dir;
        if (pai != NULL)
        {
            if (pai->esq == atual)
            {
                pai->esq = filho;
            }
            else
            {
                pai->dir = filho;
            }
        }
        else
        {
            *raiz = filho;
        }
        free(atual);
    }
    printf("\e[0;31mCompra cancelada com sucesso!\e[0m\n\n");
}

void atualizar_poltrona(ArvBin *raiz)
{   
    system(clear);
    int op,op2=0, num, num2;

    if (*raiz == NULL)
    {
        printf("Nenhuma poltrona foi reservada ainda!\n\n");
        return 0;
    }

    printf("Digite o numero da poltrona que deseja editar: ");
    scanf("%d", &num);

    if (encontrar(raiz, num) == 2)
    {
        system(clear);
        printf("\e[0;31mEsta poltrona ainda nao foi reservada!\e[0m\n\n");
        printf("Voce deseja:\n");
        printf("1 - Tentar novamente\n");
        printf("2 - Cancelar troca\n");
        scanf("%d", &op);
        if (op == 1)
        {
            atualizar_poltrona(raiz);
        }
        else if (op == 2)
        {
            system(clear);
            printf("\e[0;31mTroca cancelada!\e[0m\n\n");
            return 0;
        }
    }

    system(clear);
    printf("Digite o numero da nova poltrona: ");
    scanf("%d", &num2);
    system(clear);
    if (num2 <= 0 || num2 > 101)
    {
        printf("\n\e[0;31mIngresso indisponivel,\e[0m escolha uma poltrona de 1 a 100!\n\n");
        return 0;
    }
    else if (encontrar(raiz, num2) == 1)
    {
        system(clear);
        printf("Poltrona ja ocupada!\n\n");
        printf("Voce deseja:\n\n");
        printf("1 - Tentar novamente\n");
        printf("2 - Cancelar troca\n");
        scanf("%d", &op);
        if (op == 1)
        {
            atualizar_poltrona(raiz);
        }
        else if (op == 2)
        {
            printf("\e[0;31mTroca cancelada!\e[0m\n\n");
            return 0;
        }
    }
    else
    {
        do
        {
            system(clear);
            printf("Voce deseja:\n\n");
            printf("1 - Trocar poltrona\n");
            printf("2 - Cancelar troca\n");
            scanf("%d", &op);
            if (op == 1)
            {
                system(clear);
                printf("Sua antiga poltrona era: %d\n", num);
                printf("Sua nova poltrona e: %d\n\n", num2);
                (*raiz)->num = num2;
                printf("\n\e[0;32mPoltrona atualizada com sucesso!\e[0m\n\n");
                op2=0;
            }
            else if (op == 2)
            {
                system(clear);
                printf("\e[0;31mTroca cancelada!\e[0m\n\n");
                op2=0;
            }
            else
            {
                op2 = 1;
            }
        } while (op2 != 0);
    }
}

int main()
{
    ArvBin raiz = criarArvore();

    int num;

    int op;
    do
    {
        system(clear);
        printf("---  MENU TEATRO --- \n\n");
        printf("1 - Comprar ingresso\n");
        printf("2 - Exibir ingressos ja reservados\n");
        printf("3 - Buscar poltrona\n");
        printf("4 - Editar poltrona\n");
        printf("5 - Cancelar compra\n");
        printf("0 - Encerrar\n");
        scanf("%d", &op);

        switch (op)
        {
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
            exibe_inOrder(&raiz);
            printf("\n\n");
            system(pause);
            break;
        case 3:
            system(clear);
            printf("Digite o numero que deseja buscar: ");
            scanf("%d", &num);
            encontrar(&raiz, num);
            system(pause);
            break;
        case 4:
            system(clear);
            atualizar_poltrona(&raiz);
            system(pause);
            break;
        case 5:
            system(clear);
            printf("Digite o numero da poltrona que deseja cancelar: ");
            scanf("%d", &num);
            deletar(&raiz, num);
            system(pause);
            break;
        default:
            printf("Numero invalido");
        }
    } while (op != 0);

    libera_ArvBin(&raiz);

    return 0;
}
