#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define RED 'R'
#define BLACK 'B'

typedef struct dado
{
    int chave;
} Dado;

typedef struct no
{
    Dado ch;
    char cor;
    struct no *esquerda;
    struct no *direita;
    struct no *pai;
} NoStruct;

typedef NoStruct *No;

typedef struct arv
{
    No raiz;
} *Arvore;

No T_Nil = NULL;

No criarNo(Dado ch)
{
    No aux = (No)malloc(sizeof(NoStruct));
    if (aux == NULL)
    {
        fprintf(stderr, "Erro: falha na alocação de memória para novo nó.\n");
        exit(EXIT_FAILURE);
    }
    aux->ch = ch;
    aux->cor = RED;
    aux->esquerda = NULL;
    aux->direita = NULL;
    aux->pai = NULL;
    return aux;
}

Arvore criarArvore()
{
    Arvore arv = (Arvore)malloc(sizeof(struct arv));
    arv->raiz = T_Nil;
    return arv;
}

void emOrdemRecursivo(No raiz)
{
    if (raiz != T_Nil)
    {
        emOrdemRecursivo(raiz->esquerda);
        printf("%d ", raiz->ch.chave);
        emOrdemRecursivo(raiz->direita);
    }
}

No RB_SearchRecursivo(No raiz, int chave)
{
    while (raiz != T_Nil && chave != raiz->ch.chave)
    {
        if (chave < raiz->ch.chave)
            raiz = raiz->esquerda;
        else
            raiz = raiz->direita;
    }
    return raiz;
}

void emOrdem(Arvore *arvore)
{
    emOrdemRecursivo((*arvore)->raiz);
}

No RB_Search(Arvore *arvore, int chave)
{
    return RB_SearchRecursivo((*arvore)->raiz, chave);
}

void LeftRotate(Arvore *T, No x)
{
    No y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda != NULL)
        y->esquerda->pai = x;
    y->pai = x->pai;
    if (x->pai == NULL)
        (*T)->raiz = y;
    else if (x == x->pai->esquerda)
        x->pai->esquerda = y;
    else
        x->pai->direita = y;
    y->esquerda = x;
    x->pai = y;
}

void RightRotate(Arvore *T, No y)
{
    No x = y->esquerda;
    y->esquerda = x->direita;
    if (x->direita != (*T)->raiz->esquerda)

        x->direita->pai = y;
    x->pai = y->pai;
    if (y->pai == (*T)->raiz)
        (*T)->raiz = x;
    else if (y == y->pai->direita)
        y->pai->direita = x;
    else
        y->pai->esquerda = x;
    x->direita = y;
    y->pai = x;
}

void RB_Insert_Fixup(Arvore *arvore, No z)
{
    No aux;
    while (z != (*arvore)->raiz && z->pai->cor == RED)
    {
        if (z->pai == z->pai->pai->esquerda)
        {
            aux = z->pai->pai->direita;
            if (aux->cor == RED)
            {
                aux->cor = BLACK;
                z->pai->cor = BLACK;
                z->pai->pai->cor = RED;
                z = z->pai->pai;
            }
            else
            {
                if (z == z->pai->direita)
                {
                    z = z->pai;
                    LeftRotate(&(*arvore), z);
                }
                z->pai->cor = BLACK;
                z->pai->pai->cor = RED;
                RightRotate(&(*arvore), z->pai->pai);
            }
        }
        else
        {
            aux = z->pai->pai->esquerda;
            if (aux->cor == RED)
            {
                z->pai->cor = BLACK;
                aux->cor = BLACK;
                z->pai->pai->cor = RED;
                z = z->pai->pai;
            }
            else
            {
                if (z == z->pai->esquerda)
                {
                    z = z->pai;
                    RightRotate(&(*arvore), z);
                }
                z->pai->cor = BLACK;
                z->pai->pai->cor = RED;
                LeftRotate(&(*arvore), z->pai->pai);
            }
        }
    }
    (*arvore)->raiz->cor = BLACK;
}

void RB_Insert(Arvore *arvore, Dado ch)
{
    No z = criarNo(ch);
    No y = T_Nil;
    No x = (*arvore)->raiz;

    while (x != T_Nil)
    {
        y = x;
        if (z->ch.chave < x->ch.chave)
            x = x->esquerda;
        else
            x = x->direita;
    }

    z->pai = y;
    if (y == T_Nil)
        (*arvore)->raiz = z;
    else if (z->ch.chave < y->ch.chave)
        y->esquerda = z;
    else
        y->direita = z;

    z->esquerda = T_Nil;
    z->direita = T_Nil;
    z->cor = RED;

    RB_Insert_Fixup(&(*arvore), z);
}

void RB_Troca(Arvore *arvore, No u, No v)
{
    if (u->pai == T_Nil)
    {
        (*arvore)->raiz = v;
    }
    else
    {
        if (u == u->pai->esquerda)
        {
            u->pai->esquerda = v;
        }
        else
        {
            u->pai->direita = v;
        }
    }
    v->pai = u->pai;
}

No Tree_Minimo(No x)
{
    while (x != T_Nil)
    {
        x = x->esquerda;
    }
    return x;
}

void RB_Delete_Fixup(Arvore *arvore, No x)
{
    No w;
    while ((x != (*arvore)->raiz) && (x->cor == BLACK))
    {
        if (x == x->pai->direita)
        {
            w = x->pai->direita;
            if (w->cor == RED)
            {
                w->cor = BLACK;
                x->pai->cor = RED;
                LeftRotate(&(*arvore), x->pai);
                w = x->pai->direita;
            }
            if ((w->esquerda->cor == BLACK) && (w->direita->cor == BLACK))
            {
                w->cor = RED;
                x = x->pai;
            }
            else
            {
                if (w->direita->cor == BLACK)
                {
                    w->esquerda->cor = BLACK;
                    w->cor = RED;
                    RightRotate(&(*arvore), w);
                    w = x->pai->direita;
                }
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                w->direita->cor = BLACK;
                LeftRotate(&(*arvore), x->pai);
                x = (*arvore)->raiz;
            }
        }
        else
        {
            if (x == x->pai->esquerda)
            {
                w = x->pai->esquerda;
                if (w->cor == RED)
                {
                    w->cor = BLACK;
                    x->pai->cor = RED;
                    LeftRotate(&(*arvore), x->pai);
                    w = x->pai->esquerda;
                }
                if ((w->direita->cor == BLACK) && (w->esquerda->cor == BLACK))
                {
                    w->cor = RED;
                    x = x->pai;
                }
                else
                {
                    if (w->esquerda->cor == BLACK)
                    {
                        w->direita->cor = BLACK;
                        w->cor = RED;
                        RightRotate(&(*arvore), w);
                        w = x->pai->esquerda;
                    }
                    w->cor = x->pai->cor;
                    x->pai->cor = BLACK;
                    w->esquerda->cor = BLACK;
                    LeftRotate(&(*arvore), x->pai);
                    x = (*arvore)->raiz;
                }
            }
        }
    }
    x->cor = BLACK;
}

void RB_Delete(Arvore *arvore, No z)
{
    No y = z;
    No x;
    char y_cor_original = y->cor;
    if (z->esquerda == T_Nil)
    {
        x = z->direita;
        RB_Troca(&(*arvore), z, z->direita);
    }
    else
    {
        if (z->direita == T_Nil)
        {
            x = z->esquerda;
            RB_Troca(&(*arvore), z, z->esquerda);
        }
        else
        {
            y = Tree_Minimo(z->direita);
            y_cor_original = y->cor;
            x = y->direita;
            if (y->pai == z)
            {
                x->pai = y;
            }
            else
            {
                RB_Troca(&(*arvore), y, y->direita);
                y->direita = z->direita;
                y->direita->pai = y;
            }
            RB_Troca(&(*arvore), z, y);
            y->esquerda = z->esquerda;
            y->esquerda->pai = y;
            y->cor = z->cor;
        }
    }
    if (y_cor_original == BLACK)
    {
        RB_Delete_Fixup(&(*arvore), x);
    }
}

void inserir(Arvore *arvore, int chave) {
    Dado novo_dado;
    novo_dado.chave = chave;
    RB_Insert(&(*arvore), novo_dado);
}

void remover(Arvore *arvore, int chave) {
    No no_a_remover = RB_Search(&(*arvore), chave);
    if (no_a_remover == T_Nil) {
        printf("Chave não encontrada.\n");
        return;
    }
    RB_Delete(&(*arvore), no_a_remover);
}

void freeTree(No raiz) {
    if (raiz != T_Nil) {
        freeTree(raiz->esquerda);
        freeTree(raiz->direita);
        free(raiz);
    }
}

void menu(Arvore *arvore)
{
    int opcao;
    Dado n;

    do
    {
        printf("\nMenu: Arvore RedBlack\n");
        printf("1. Busca\n");
        printf("2. Insercao\n");
        printf("3. Remocao\n");
        printf("4. Imprimir em ordem\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
            printf("Digite a chave de busca: ");
            scanf("%d", &n.chave);
            // Implementar função de busca e exibir resultado
            if (RB_Search(&(*arvore), n.chave))
                printf("Elemento encontrado!\n");
            else
                printf("Elemento nao encontrado.\n");
            break;
        case 2:
            printf("Digite a chave a ser inserida: ");
            scanf("%d", &n.chave);
            // Implementar função de inserção
            inserir(&(*arvore), n.chave);
            printf("Insercao realizada com sucesso.\n");
            break;
        case 3:
            printf("Digite a chave a ser removida: ");
            scanf("%d", &n.chave);
            // Implementar função de remoção
            remover(&(*arvore), n.chave);
            printf("Remocao realizada com sucesso.\n");
            break;
        case 4:
            printf("Em ordem traversal: ");
            emOrdem(&(*arvore));
            printf("\n");
            break;
        case 0:
            printf("Saindo do programa.\n");
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);
}

int main()
{
    Arvore arvore = criarArvore();

    printf("Arvore RedBlack: \n");

    menu(&arvore);

    freeTree(arvore->raiz);
    free(arvore);

    return 0;
}

