/*  David Costa Pereira                               */
/*  Turma 3                                           */
/*  Exercicio 4 : Agenda Eletrônica + vetor circular  */
/*                                                    */
/*  Programa compilado com CodeBlocks 20.03           */

#include <iostream>
#include <string.h>
typedef struct lista lista;
typedef struct dados vetor;
struct dados{
    char tarefa[45];
    int prioridade;
};
struct lista{
    vetor *vet;
    int n_elementos;
    int n_max;
};
///****************************************************************************
///                      FUNCOES PRIVADAS
///****************************************************************************
///Funcao recursiva que recebe a lista e a posicao a partir de onde deve ser reestruturado
void Reestruturar(lista *L, int posi){
    ///Tem filho esquerdo e direito
    if(2*posi +1 <= L->n_elementos){
        if(L->vet[2*posi+1].prioridade >= L->vet[2*posi].prioridade){
            if(L->vet[2*posi+1].prioridade > L->vet[posi].prioridade) {
                dados aux;
                aux = L->vet[posi];
                L->vet[posi] = L->vet[2 * posi + 1];
                L->vet[2 * posi + 1] = aux;
                Reestruturar(L, 2 * posi + 1);
            }
        } else if(L->vet[2*posi].prioridade > L->vet[posi].prioridade){
            dados aux;
            aux = L->vet[posi];
            L->vet[posi] = L->vet[2 * posi];
            L->vet[2 * posi] = aux;
            Reestruturar(L, 2 * posi);
        }
    }
    ///So tem filho esquerdo
    else if (2*posi == L->n_elementos){
        if(L->vet[2*posi].prioridade > L->vet[posi].prioridade) {
            dados aux;
            aux = L->vet[posi];
            L->vet[posi] = L->vet[2 * posi];
            L->vet[2 * posi] = aux;
            Reestruturar(L, 2 * posi);
        }
    }
}
///****************************************************************************
///                      FUNCOES PUBLICAS
///****************************************************************************
///Recebe um pointer para a lista e o numero max de elementos e inicializa a lista, fazendo malloc.
void Inicializar(lista *L,int mx){
    L->n_elementos=0;
    L->n_max=mx;
    L->vet = (vetor *)(malloc((mx+2) * sizeof(vetor)));
}
///****************************************************************************
///Insere um elemento na lista
void Insert(lista *L, char tarefa[], int prioridade){
    L->n_elementos++;
    L->vet[L->n_elementos].prioridade = prioridade;
    strcpy(L->vet[L->n_elementos].tarefa, tarefa);
    if(L->n_elementos>1) {
        int k = L->n_elementos;
        dados aux;
        while (k != 1 and L->vet[k / 2].prioridade < L->vet[k].prioridade) {
            aux = L->vet[k / 2];
            L->vet[k / 2] = L->vet[k];
            L->vet[k] = aux;
            k = k / 2;
        }
    }
}
///****************************************************************************
///Recebe a lista e da free.
void Finalizar(lista *L){
    free(L->vet);
}
///****************************************************************************
///Recebe a lista e devolve o item de maior prioridade.
dados Maximum(lista *L){
    return L->vet[1];
}
///****************************************************************************
///Remove o item de maior prioridade.
void RemoveMax(lista *L){
    if(L->n_elementos == 1){
        L->n_elementos = 0;
    } else{
        L->vet[1]=L->vet[L->n_elementos];
        L->n_elementos--;
        Reestruturar(L, 1);
    }
}
///****************************************************************************
///Verifica se a fila esta cheia
bool FilaCheia(lista *L){
    if(L->n_elementos == L->n_max)
        return true;
    return false;
}
///****************************************************************************
///Verifica se a fila esta vazia
bool FilaVazia(lista *L){
    if(L->n_elementos==0)
        return true;
    return false;
}

int main(){
    ///Arquivos de entrada e saida
    FILE *entrada, *saida;
    entrada = fopen("entrada6.txt","r");
    saida = fopen("Lab6_David_Costa_Pereira.txt","w");
    ///Lista de prioridade
    lista list_p;
    int prioridade;
    char tarefa[45];
    int n;
    char leitura[72];
    ///Linhas iniciais do arquivo de saida
    fprintf(saida,"Bom dia flor do dia\n");
    fprintf(saida,"Hora de organizar seu dia\n");
    fprintf(saida,"Ou fazer as tarefas do dia anterior\n");
    fprintf(saida,"Bom dia flor do dia\n");
    fprintf(saida,"Bom dia flor do dia\n");
    fprintf(saida,"--------------------------------------------------\n");
    fprintf(saida,"RESPOSTAS DAS CONSULTAS\n");
    fprintf(saida,"--------------------------------------------------\n");
    for (int i = 0; i <5; ++i) {
        fgets(leitura, 75, entrada);
    }
    ///Numero max de lementos na agenda
    fscanf(entrada," %d ", &n);
    for (int i = 0; i <3; ++i) {
        fgets(leitura, 72, entrada);
    }
    ///Inicializando a agenda
    Inicializar(&list_p,n);
    ///Lendo operacoes
    do{
        fscanf(entrada," %s ",&leitura);
        if(strcmp(leitura,"NOVA")==0){
            fscanf(entrada," %d ",&prioridade);
            fgets(tarefa,45,entrada);
            tarefa[strlen(tarefa)-1]='\0';
            if(!FilaCheia(&list_p)) {
                Insert(&list_p, tarefa, prioridade);
            } else fprintf(saida,"ERRO     Agenda cheia. Impossivel inserir.\n");
        } else if (strcmp(leitura,"PROXIMA")==0){
            if(!FilaVazia(&list_p)) {
                fprintf(saida, "%2.d       ", Maximum(&list_p).prioridade);
                fprintf(saida, "%s\n", Maximum(&list_p).tarefa);
                RemoveMax(&list_p);
            } else fprintf(saida,"AVISO    Nao ha tarefas na agenda\n");
        }
    } while (strcmp(leitura,"FIM")!=0);
    fprintf(saida,"--------------------------------------------------\n");
    fprintf(saida,"FICA PARA O DIA SEGUINTE\n");
    fprintf(saida,"--------------------------------------------------\n");
    ///Mensagenm final
    if(!FilaVazia(&list_p)) {
        while (!FilaVazia(&list_p)) {
            fprintf(saida, "%2.d       %s\n", Maximum(&list_p).prioridade, Maximum(&list_p).tarefa);
            RemoveMax(&list_p);
        }
    } else fprintf(saida,"Agenda vazia! Nao restam tarefas para o dia seguinte.");
    ///Finalizando a agenda
    Finalizar(&list_p);
    ///Fechand arquivos de entrada e saida de dados
    fclose(entrada);
    fclose(saida);
    return 0;
}

