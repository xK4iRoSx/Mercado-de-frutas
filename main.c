#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50


typedef struct Fruta {
    int codigo;
    char nome[MAX_NAME];
    int quantidade;
    float preco;
} Fruta;

typedef struct Nodo {
    Fruta fruta;
    struct Nodo *prox;
} Nodo;


Nodo* criarNodo(Fruta fruta);
void cadastraFrutas(Nodo **lista);
void listaFrutas(Nodo *lista);
void bustaFrutas(Nodo *lista);
void alterarFrutas(Nodo *lista);
void excluirFrutas(Nodo **lista);
void venderFrutas(Nodo *lista);
void salvarFrutas(Nodo *lista);
void carregaFrutas(Nodo **lista);
void registroVendas(int codigo, int quantidade, float valor_total);
Nodo* encontraFrutas(Nodo *lista, int codigo);
int existeFrutas(Nodo *lista, int codigo);

int main() {
    Nodo *lista_frutas = NULL;
    int opcao;

    carregaFrutas(&lista_frutas);

    do {
        printf("\n==== Mercado de Frutas ====\n");
        printf("1- Cadastrar fruta\n");
        printf("2- Listar frutas\n");
        printf("3- Buscar fruta\n");
        printf("4- Alterar fruta\n");
        printf("5- Excluir fruta\n");
        printf("6- Vender fruta\n");
        printf("7- Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        system("cls");

        switch (opcao) {
            case 1: cadastraFrutas(&lista_frutas); break;
            case 2: listaFrutas(lista_frutas); break;
            case 3: bustaFrutas(lista_frutas); break;
            case 4: alterarFrutas(lista_frutas); break;
            case 5: excluirFrutas(&lista_frutas); break;
            case 6: venderFrutas(lista_frutas); break;
            case 7: salvarFrutas(lista_frutas); break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 7);

    return 0;
}


Nodo* criarNodo(Fruta fruta) {
    Nodo *nodo = (Nodo*) malloc(sizeof(Nodo));
    nodo->fruta = fruta;
    nodo->prox = NULL;
    return nodo;
}

void cadastraFrutas(Nodo **lista) {
    Fruta fruta;
    printf("Codigo da fruta: ");
    scanf("%d", &fruta.codigo);

    if (existeFrutas(*lista, fruta.codigo)) {
        printf("Fruta com este codigo ja existe!\n");
        return;
    }

    printf("Nome da fruta: ");
    scanf("%s", fruta.nome);
    printf("Quantidade em estoque: ");
    scanf("%d", &fruta.quantidade);
    printf("Preco da fruta: ");
    scanf("%f", &fruta.preco);

    Nodo *nodo = criarNodo(fruta);
    nodo->prox = *lista;
    *lista = nodo;

    printf("Fruta cadastrada com sucesso!\n");
}

void listaFrutas(Nodo *lista) {
    printf("\nLista de frutas:\n");
    while (lista != NULL) {
        printf("Codigo: %d, Nome: %s, Quantidade: %d, Preco: %.2f\n",
               lista->fruta.codigo, lista->fruta.nome, lista->fruta.quantidade, lista->fruta.preco);
        lista = lista->prox;
    }
}

void bustaFrutas(Nodo *lista) {
    int codigo;
    printf("Codigo da fruta: ");
    scanf("%d", &codigo);
    Nodo *nodo = encontraFrutas(lista, codigo);

    if (nodo) {
        printf("Codigo: %d, Nome: %s, Quantidade: %d, Preco: %.2f\n",
               nodo->fruta.codigo, nodo->fruta.nome, nodo->fruta.quantidade, nodo->fruta.preco);
    } else {
        printf("Fruta nao encontrada.\n");
    }
}


void alterarFrutas(Nodo *lista) {
    int codigo;
    printf("Codigo da fruta: ");
    scanf("%d", &codigo);
    Nodo *nodo = encontraFrutas(lista, codigo);

    if (nodo) {
        printf("Novo nome da fruta: ");
        scanf("%s", nodo->fruta.nome);
        printf("Nova quantidade: ");
        scanf("%d", &nodo->fruta.quantidade);
        printf("Novo preco: ");
        scanf("%f", &nodo->fruta.preco);
        printf("Fruta alterada com sucesso!\n");
    } else {
        printf("Fruta nao encontrada.\n");
    }
}


void excluirFrutas(Nodo **lista) {
    int codigo;
    printf("Codigo da fruta: ");
    scanf("%d", &codigo);

    Nodo *atual = *lista, *anterior = NULL;

    while (atual && atual->fruta.codigo != codigo) {
        anterior = atual;
        atual = atual->prox;
    }

    if (!atual) {
        printf("Fruta nao encontrada.\n");
        return;
    }

    if (atual->fruta.quantidade > 0) {
        printf("Nao e possivel excluir fruta com estoque.\n");
        return;
    }

    if (anterior) {
        anterior->prox = atual->prox;
    } else {
        *lista = atual->prox;
    }

    free(atual);
    printf("Fruta excluida com sucesso!\n");
}


void venderFrutas(Nodo *lista) {
    int codigo, quantidade;
    printf("Codigo da fruta: ");
    scanf("%d", &codigo);
    Nodo *nodo = encontraFrutas(lista, codigo);

    if (!nodo) {
        printf("Fruta nao encontrada.\n");
        return;
    }

    printf("Quantidade para venda: ");
    scanf("%d", &quantidade);

    if (quantidade > nodo->fruta.quantidade) {
        printf("Quantidade indisponivel no estoque.\n");
    } else {
        nodo->fruta.quantidade -= quantidade;
        float valor_total = quantidade * nodo->fruta.preco;
        printf("Venda realizada! Total: R$%.2f\n", valor_total);
        registroVendas(codigo, quantidade, valor_total);
    }
}


Nodo* encontraFrutas(Nodo *lista, int codigo) {
    while (lista && lista->fruta.codigo != codigo) {
        lista = lista->prox;
    }
    return lista;
}


int existeFrutas(Nodo *lista, int codigo) {
    return encontraFrutas(lista, codigo) != NULL;
}


void registroVendas(int codigo, int quantidade, float valor_total) {
    FILE *file = fopen("vendas.txt", "a");
    if (file) {
        fprintf(file, "Codigo: %d, Quantidade: %d, Total: %.2f\n", codigo, quantidade, valor_total);
        fclose(file);
    }
}


void salvarFrutas(Nodo *lista) {
    FILE *file = fopen("frutas.txt", "w");
    if (file) {
        while (lista) {
            fprintf(file, "%d %s %d %.2f\n", lista->fruta.codigo, lista->fruta.nome, lista->fruta.quantidade, lista->fruta.preco);
            lista = lista->prox;
        }
        fclose(file);
    }
}


void carregaFrutas(Nodo **lista) {
    FILE *file = fopen("frutas.txt", "r");
    if (file) {
        Fruta fruta;
        while (fscanf(file, "%d %s %d %f", &fruta.codigo, fruta.nome, &fruta.quantidade, &fruta.preco) != EOF) {
            Nodo *nodo = criarNodo(fruta);
            nodo->prox = *lista;
            *lista = nodo;
        }
        fclose(file);
    }
}
