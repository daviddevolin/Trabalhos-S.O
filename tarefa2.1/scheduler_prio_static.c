#include <stdio.h>
#include <stdlib.h>
#include "queue.h" // contem funções uteis para filas
#include "proc.h"  // possui as funções dos processos
#include "stats.h" // possui as funções de estatisticas 
#include "utils.h" // possui funções uteis 

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // segunda fila de aptos
extern struct queue * blocked;  // fila de bloqueados
extern struct queue * finished; // fila de finalizados
// NOTE: essa fila de finalizados é utilizada apenas para
// as estatisticas finais

// variavel global que indica o tempo maximo que um processo pode executar ao todo
extern int MAX_TIME;

struct proc * get_process(struct queue * ready, struct queue * ready2);

struct proc * scheduler(struct proc * current)
{
    struct proc * selected; 

    if (current != NULL)
    {
        // Verificando o estado em que o processo executando está
        switch (current->state) 
        {
            // Caso esteja 'apto' é porque saiu por preempção
            case READY:

                // testando a qual fila o processo merece
                if (current->queue == 1)
                {
                    enqueue(ready, current);
                }else
                {
                    enqueue(ready2, current);
                }
                // Realizando as estatisticas para o processo que 
                // entra na fila de aptos
                count_ready_in(current);
                
                break;

            // Caso o processo tenha saído como 'bloqueado', é porque fez uma E/S
            case BLOCKED:

                // Adicionando no final da fila de bloqueados
                enqueue(blocked, current);
                
                // Realizando as estatisticas para o processo que 
                // entra na fila de bloqueados
                count_blocked_in(current);

                break;

            // Caso o processo tenha finalizado, estado 'finished'
            case FINISHED:

                // Adicionando-o na fila de finalizados
                enqueue(finished, current);

                // Realizando as estatisticas para o processo que
                // entra na fila de finalizados
                count_finished_in(current);

                break;

            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }
    

    // Se a fila de aptos está vazia, nao há o que fazer
    if (isempty(ready)&& isempty(ready2))
    {
        printf("== sched PRIO Static: fila READY is empty\n");
        return NULL;
    }

    // Pegando o menor processo da fila de aptos para executar
    selected = get_process(ready, ready2);
    
    count_ready_out(selected);

    // Alterando o estado do processo selecionado para executando
    selected->state = RUNNING;
        
    // Retornando o processo selecionado
    return selected;
}

struct proc * get_process(struct queue * ready, struct queue * ready2){

    int number = rand()%100;
    
    if (number<80 && !isempty(ready))
    {
        return dequeue(ready);
    }else if (!isempty(ready2))
    {
        return dequeue(ready2);
    }else if (!isempty(ready))
    {
        return dequeue(ready);
    }else
    {
        return NULL;
    }
    
}
