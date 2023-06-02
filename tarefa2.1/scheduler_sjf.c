#include <stdio.h>

#include "queue.h" // contem funções uteis para filas
#include "proc.h"  // possui as funções dos processos
#include "stats.h" // possui as funções de estatisticas 
#include "utils.h" // possui funções uteis 
#include "scheduler_sjf.h"

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // segunda fila de aptos
extern struct queue * blocked;  // fila de bloqueados
extern struct queue * finished; // fila de finalizados
// NOTE: essa fila de finalizados é utilizada apenas para
// as estatisticas finais

// variavel global que indica o tempo maximo que um processo pode executar ao todo
extern int MAX_TIME;


struct proc * scheduler(struct proc * current)
{
    struct proc * selected ; 

    if (current != NULL)
    {
        // Verificando o estado em que o processo executando está
        switch (current->state) 
        {
            // Caso esteja 'apto' é porque saiu por preempção
            case READY:

                // Adicionando no final da fila de aptos
                enqueue(ready, current);

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
    /*
     *   Estratégia de seleção de um novo processo para executar
     */

    // Se a fila de aptos está vazia, nao há o que fazer
    if (isempty(ready))
    {
        printf("== sched SJF: fila READY is empty\n");
        return NULL;
    }

    // Pegando o menor processo da fila de aptos para executar
    selected = get_lower_process(ready);
    
    count_ready_out(selected);

    // Alterando o estado do processo selecionado para executando
    selected->state = RUNNING;
        
    // Retornando o processo selecionado
    return selected;
}


struct proc * get_lower_process(struct queue * ready){
    struct proc * p_lower_time;
    int min_time = 99999;

    for (struct proc * p = ready->head; p != NULL;  p = p->next){
        if (p->remaining_time < min_time){
            min_time = p->remaining_time;
            p_lower_time = p;
        }
    }

    if (p_lower_time!= NULL){
        dequeue_bypid(ready, p_lower_time->pid);
    }

    return p_lower_time;
}



	