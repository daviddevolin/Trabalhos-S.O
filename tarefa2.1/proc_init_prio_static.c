#include <stdio.h>

#include "queue.h"
#include "proc.h"
#include "stats.h"

// Esta é a função a ser alterada caso mude a forma de inicialização dos processos
// Assim como a seleção da fila que ele será inserido

// Quantidade de processos a serem criados
extern int NPROC;

// variavel global que indica o tempo maximo que um processo pode executar ao todo
extern int MAX_TIME;


// Utilizando as variáveis globais das filas definidas no 'main'
extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // segunda fila de aptos
extern struct queue * blocked;  // fila de bloqueados
extern struct queue * finished; // fila de finalizados

// Inicializa os 'NPROC' processos informados
void proc_init()
{
    struct proc *p;
    
    for (int i = 0; i < NPROC; i++) 
    {
        // criando um novo processo
        p = createproc();
        
        // cria a thread do processo
        p->tid = createproc_thread(p);

        // pegando o tempo a ser comparado         
        int time_20 = MAX_TIME *0.2;

        if (p->remaining_time<=time_20)
        {
            // determinando fila do processo
            p->queue = 1;
            // adicionando-o na fila de aptos1
            enqueue(ready, p);
        }else
        {
            // determinando fila do processo
            p->queue = 2;
            // adicionando-o na fila de aptos2
            enqueue(ready2, p);
        }

        // contabilizando a entrada do processo na fila de aptos
        count_ready_in(p);
    }
}

