/* Projeto 1 IAED - Henrique Caroco, 103860 */

/*  Bibliotecas: */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



/*  Constantes: */
#define MAX_AEROPORTOS 40
#define MAX_ID 3
#define MAX_CHAR_PAIS 30
#define MAX_CHAR_CIDADE 50

/* Usada para definir o maximo de caracteres da string lida*/
#define ESPACOS_EXTRA 4

#define MAX_CODIGO 6
#define MAX_VOOS 30000



/*  Funções auxiliares: */

/* Retorna 1 em caso de ID valido (apenas maiusculas), 0 caso contrário */
int val_ID(char id[]){
    int cont = 0, ver = 1;

    for(cont = 0; cont < MAX_ID; cont++){
        if (id[cont] < 'A' || id[cont] > 'Z')
             ver = 0;
    }
    return ver;
}

/* Retorna 1 em caso de codigo valido, 0 caso contrário */
int val_cod_voo(char codigo[]){
    int ver = 1;
    int contador = 0;
    int size_cod = strlen(codigo);

    for(contador = 0; contador < 2; contador++)
        if(codigo[contador] < 'A' || codigo[contador] > 'Z')
            ver = 0;

    for (contador = 2; contador < size_cod; contador++)
        if(codigo[contador] < '0' || codigo[contador] > '9') 
            ver = 0;

    return ver;
}           


/* Verifica validade da data inserida */
int diferenca_datas(int dia, int mes, int ano, int dia2, int mes2, int ano2){
    int diff_ano = 0, diff_mes = 0, diff_dia = 0;
    int diferenca;

    diff_ano = ano - ano2;
    diff_mes = mes - mes2;
    diff_dia = dia - dia2;

    diferenca = diff_dia + diff_mes * 30 + diff_ano * 365;

    if(diferenca < 0)
        return 0;
    else if(diferenca > 365)
        return 0;
    else
        return 1;
}



/* Algoritmo de ordenacao Bubble Sort para strings: */
void BubbleSortStrings(char arr_strings[][MAX_ID + 1], int tamanho){
    int i, j;

    char aux[MAX_ID + 1];

    for (j = 0; j < tamanho - 1; j++)
    {
        for (i = j + 1; i < tamanho; i++){

            /* Troca dos elementos: */
            if (strcmp(arr_strings[j], arr_strings[i]) > 0){

                strcpy(aux, arr_strings[j]);
                strcpy(arr_strings[j], arr_strings[i]);
                strcpy(arr_strings[i], aux);
            }
        }
    }
}

/* Algoritmo de ordenacao Bubble Sort para inteiros: */
void bubbleSort(int arr_int[], int tamanho){
    int step, i, aux;

    for (step = 0; step < tamanho - 1; ++step){
      
        for (i = 0; i < tamanho - step - 1; ++i){
      
            /* Troca dos elementos: */
            if (arr_int[i] > arr_int[i + 1]){

                aux = arr_int[i];
                arr_int[i] = arr_int[i + 1];
                arr_int[i + 1] = aux;
            }
        }
    }
}

/* Funcao que permite ao BubbleSort ordenar os voos, no comando p */
int pre_ordena_p(int dia, int mes, int ano, int horas, int minutos){

    /* Ordenacao por nivel de prioridade das unidades */
    return minutos + horas * 60 + dia * 24 * 60 + mes * 1000000 + ano * 100000000;
}

/* Funcao que permite ao BubbleSort ordenar os voos, no comando c */
int pre_ordena_c(int dia, int mes, int ano, int horas, int minutos, int duracao){

    /* Ordenacao por nivel de prioridade das unidades */
    return minutos + horas * 60 + dia * 24 * 60 + mes * 1000000 + ano * 100000000 + duracao;
}


/* soma_dur retorna a nova hora, minutos ou dia apos a soma da duracao do voo, 
segundo o input do utilizador */
int soma_dur(int hora, int minutos, int duracao, int dia, int mes, int ano, int num){
    int horas_soma = 0;

    if(duracao + minutos >= 60){
        horas_soma = (minutos + duracao) / 60;
        hora += horas_soma;
        minutos = minutos + duracao - (60 * horas_soma);

        if(hora >= 24){
            hora -= 24;
            dia++;
        } 
        if(dia > 31){
            dia -= 31;
            mes++;
        }
        if(mes > 12){
            mes -= 12;
            ano++;
        }
        if(mes == 2 && dia > 28){
            dia -= 28;
            mes++;
        }
        if(dia > 30 && (mes == 4 || mes == 6 || mes == 9 || mes == 11) ){
            dia -= 30;
            mes++;
        }
    }
    else
        minutos += duracao;

    /* Dependente do valor de num, a funcao retorna os minutos, as horas 
    ou o dia de chegada do voo*/
    if(num == 0)
        return hora;
    if(num == 1)
        return minutos;
    if(num == 2)
        return dia;
    if(num == 3)
        return mes;
    if(num == 4)
        return ano;

    /* Nao se aplica*/
    else
        return 0;
}


/* f_print e data_print servem para escrever as datas e horas */
void f_print(int dia, int mes, int ano, int horas, int minutos){
    
    if(dia < 10)
        printf("0");
    printf("%d-", dia);

    if(mes < 10)
        printf("0");
    printf("%d-", mes);

    printf("%d ", ano);

    if(horas < 10)
        printf("0");
    printf("%d:", horas);

    if(minutos < 10)
        printf("0");
    printf("%d\n", minutos);
}

void data_print(int dia, int mes, int ano){
    if(dia < 10)
        printf("0");
    printf("%d-", dia);

    if(mes < 10)
        printf("0");
    printf("%d-", mes);

    printf("%d\n", ano);
}



/* FUNCAO MAIN: */
int main(){ 

/* Estrutura de Dados para Datas introduzidas e a Data do sistema */
typedef struct{
    int dia;
    int mes;
    int ano;
} Datas;

Datas data_atual;
Datas data_input;

/* Estrutura de Dados para cada aeroporto */
typedef struct { 
    char ID[MAX_ID + 1];
    char pais[MAX_CHAR_PAIS + 1];
    char cidade[MAX_CHAR_CIDADE + 1];
    int num_voos;
} Aeroporto;

Aeroporto aero_input;
Aeroporto aeroportos[MAX_AEROPORTOS];

/* Estrutura de Dados para cada voo */
typedef struct{
    char codigo[MAX_CODIGO + 1];
    char ID_part[MAX_ID + 1];
    char ID_cheg[MAX_ID + 1];
    int dia_;
    int mes_;
    int ano_;
    int hora_;
    int minutos_;
    int duracao;
    int capacidade;
} Voo;

Voo voo_input;
Voo voos[MAX_VOOS];

/* Estrutura de Dados para ordenar os voos nos comandos p e c */
typedef struct{
    int pre_ord_;
    char codigo[MAX_CODIGO + 1];
} Ordenar;

Ordenar ordena_c_lista[MAX_VOOS];
Ordenar ordena_p_lista[MAX_VOOS];



/* Variáveis: */

/* Globais: */
char input[100];
int cont, cont_1 = 2, cont_ID = 0, cont2 = 0;
int res = 0, aux = 0;

/* Comando a: */
int cont_pais = 0, cont_cidade = 0;
int var_ID = 1;
int cont_aeroportos = 0;

/* Comando l: */
char array2d_l_2[MAX_AEROPORTOS][MAX_ID + 1];
char store_id[MAX_ID + 1];

/* Comando v: */
int cont_cod = 0, cont_data = 0, cont_hora = 0, cont_voos = 0;
int var_cod = 1, var_ID_part = 0, var_ID_cheg = 0;

/* Comando p: */
int array2d_p_2[MAX_VOOS];

/* Comando c: */
int array2d_c_2[MAX_VOOS];
int pre_ord;

/* comando t: */
data_atual.dia = 1;
data_atual.mes = 1;
data_atual.ano = 2022;


for(cont = 0; cont < MAX_AEROPORTOS; cont++){
    aeroportos[cont].num_voos = 0;
}


fgets(input, sizeof(input), stdin);

/* Programa é um ciclo while, corre até haver um q */
while (input[0] != 'q'){

    /* Comando "a": */
    if (input[0] == 'a'){
            
            /* Introduzir o ID na estrutura de dados: */
            while(input[cont_1] != ' '){
                aero_input.ID[cont_ID] = input[cont_1];
                cont_1++;
                cont_ID++; 
            }
            cont_1++;

            /* Introduzir o pais na estrutura de dados: */
            while(input[cont_1] != ' '){
                aero_input.pais[cont_pais] = input[cont_1];
                cont_1++;
                cont_pais++;
            }
            cont_1++;

            /* Introduzir a cidade na estrutura de dados: */
            while(input[cont_1] != '\0' && input[cont_1] != '\n'){
                aero_input.cidade[cont_cidade] = input[cont_1];
                cont_1++;
                cont_cidade++;
            }

            
            /* Verifica a validade do ID, com recurso à funcao val_ID */
            if (val_ID(aero_input.ID) == 0)
                printf("invalid airport ID\n");

            else{
                if(cont_aeroportos >= MAX_AEROPORTOS)
                    printf("too many airports\n");

                else{

                    /* Verifica se o aeroporto já está na lista de IDs */
                    for(cont = 0; cont < cont_aeroportos; cont++){
                        if(strcmp(aeroportos[cont].ID, aero_input.ID) == 0){
                            printf("duplicate airport\n");
                            var_ID = 0;
                        }
                    }

                    /* Adiciona aeroporto à lista, caso ainda não esteja (verificado pela variável var_ID) 
                    e incrementa variável de contagem de aeroportos */

                    if(var_ID == 1){
                        strcpy(aeroportos[cont_aeroportos].ID, aero_input.ID);
                        strcpy(aeroportos[cont_aeroportos].pais, aero_input.pais);
                        strcpy(aeroportos[cont_aeroportos].cidade, aero_input.cidade);
                        aeroportos[cont_aeroportos].num_voos = 0;

                        printf("airport %s\n", aeroportos[cont_aeroportos].ID);
                        cont_aeroportos++;                 
                    }
                }
            } 

    /* Reset das variaveis, para os seus valores iniciais*/
    cont = 0; 
    cont_1 = 2;
    cont_ID = 0;
    cont_pais = 0;
    cont_cidade = 0;
    var_ID = 1;

    /* Reset do input do pais e cidade, pois tem tamanhos variaveis */
    memset(aero_input.pais, 0, sizeof(aero_input.pais) );
    memset(aero_input.cidade, 0, sizeof(aero_input.cidade) );
    }


    /* Comando l: */
    else if(input[0] == 'l'){

        if(strcmp(input, "l\n") == 0){

            /* Ordenacao por ID */
            for(cont = 0; cont < cont_aeroportos; cont++)
                strcpy(array2d_l_2[cont], aeroportos[cont].ID);

            BubbleSortStrings(array2d_l_2, cont_aeroportos);

            /* Print dos aeroportos */
            for(cont = 0; cont < cont_aeroportos; cont++){
                for(cont2 = 0; cont2 < cont_aeroportos; cont2++){
                    if(strcmp(array2d_l_2[cont], aeroportos[cont2].ID) == 0){
                        printf("%s %s %s %d\n", aeroportos[cont2].ID, aeroportos[cont2].cidade, aeroportos[cont2].pais, aeroportos[cont2].num_voos);
                    }
                }
            }
        }

        else{
            var_ID = 0;
            cont_ID = 2;

            /* Impressao dos aeroportos por ordem do input */
            while(input[cont_1] != '\n' && input[cont_1 - 1] != '\n'){

                for(cont = 0; cont < MAX_ID; cont++){
                    store_id[cont] = input[cont_1];
                    cont_1++;
                }

                if(cont_1 )
                cont_1++;

                for(cont = 0; cont < cont_aeroportos; cont++)
                    if(strcmp(store_id, aeroportos[cont].ID) == 0){
                        var_ID = 1;
                        printf("%s %s %s %d\n", aeroportos[cont].ID, aeroportos[cont].cidade, aeroportos[cont].pais, aeroportos[cont].num_voos);
                    }


            if(var_ID == 0)
                printf("no such airport ID\n");
            }

        }
        var_ID = 1;
        cont_1 = 2;
    }

    /* Comando v: */
    else if(input[0] == 'v'){

        /* Print por ordem de criacao */
        if(strcmp(input, "v\n") == 0)
            	for(cont = 0; cont < cont_voos; cont++){
                    printf("%s %s %s ", voos[cont].codigo, voos[cont].ID_part, voos[cont].ID_cheg);
                    f_print(voos[cont].dia_, voos[cont].mes_, voos[cont].ano_, voos[cont].hora_, voos[cont].minutos_);
                }

        else{

        /* Passagem do input para as variáveis */
        while(input[cont_1] != ' '){
            voo_input.codigo[cont_cod] = input[cont_1]; 
            cont_1++;
            cont_cod++;
        }
        cont_1++;
        cont_cod = 0;

        for(cont_ID = 0; cont_ID < 3; cont_ID++){
            voo_input.ID_part[cont_ID] = input[cont_1];
            cont_1++;
        }
        cont_1++;
        cont_ID = 0;

        for(cont_ID = 0; cont_ID < 3; cont_ID++){
            voo_input.ID_cheg[cont_ID] = input[cont_1];
            cont_1++;
        }
        cont_1++;
        cont_ID = 0;
        
        for(cont_data = 0; cont_data < 10; cont_data++){
            if(cont_data == 0)
                voo_input.dia_ = input[cont_1 + 1] - '0' + (input[cont_1] - '0') * 10;
            else if(cont_data == 3)
                voo_input.mes_ = input[cont_1 + 1] - '0' + (input[cont_1] - '0') * 10;
            else if(cont_data == 6)
                voo_input.ano_ = input[cont_1 + 3] - '0' + (input[cont_1 + 2] - '0') * 10 + (input[cont_1 + 1] - '0') * 100 + (input[cont_1] - '0') * 1000;

            cont_1++;
        }
        cont_1++;

        for(cont_hora = 0; cont_hora < 5; cont_hora++){
            if(cont_hora == 0)
                voo_input.hora_ = input[cont_1 + 1] - '0' + (input[cont_1] - '0') * 10;
                
            else if(cont_hora == 3)
                voo_input.minutos_ = input[cont_1 + 1] - '0' + (input[cont_1] - '0') * 10;
                
            cont_1++;
        }
        cont_1++;

        for(cont = 0; cont < 5; cont++){
            if(cont == 0)
                res += (input[cont_1] - '0') * 10 * 60 + (input[cont_1 + 1] - '0') * 60;

            else if(cont == 3)
                res += (input[cont_1] - '0') * 10 + (input[cont_1 + 1] - '0');

            cont_1++;
        }
        voo_input.duracao = res;
        cont_1++;
        res = 0;
        
        while(input[cont_1] != '\n'){
            res = res * 10 + (input[cont_1] - '0');
            cont_1++;
        }
        voo_input.capacidade = res;
        /* Fim da passagem do input para as variáveis */


        /* Validacao do codigo */
        if(val_cod_voo(voo_input.codigo) == 0)
            printf("invalid flight code\n");
        
        else{
            for(cont = 0; cont < cont_voos; cont++){
                if(strcmp(voos[cont].codigo, voo_input.codigo) == 0 && voo_input.dia_ == voos[cont].dia_ && voo_input.mes_ == voos[cont].mes_ && voo_input.ano_ == voos[cont].ano_){
                    printf("flight already exists\n");
                    var_cod = 0;
                }
            }

            if(var_cod == 1){

                /* Validacao dos aeroportos */
                for(cont = 0; cont < cont_aeroportos; cont++){
                    if(strcmp(aeroportos[cont].ID, voo_input.ID_part) == 0)
                        var_ID_part = 1;

                    if(strcmp(aeroportos[cont].ID, voo_input.ID_cheg) == 0)
                        var_ID_cheg = 1;
                }

                if(var_ID_part != 1)
                    printf("%s: no such airport ID\n", voo_input.ID_part);

                else if(var_ID_cheg != 1)
                    printf("%s: no such airport ID\n", voo_input.ID_cheg);
                
                else{
                    if(cont_voos >= MAX_VOOS)
                        printf("too many flights\n");

                    else{
                    /* Verificacao da duracao e capacidade */
                    if(diferenca_datas(voo_input.dia_, voo_input.mes_, voo_input.ano_, data_atual.dia, data_atual.mes, data_atual.ano) == 0)
                        printf("invalid date\n");
                    
                        else{
                        if(voo_input.duracao > 720)
                            printf("invalid duration\n");

                        else{
                            /* Verificacao da data */
                            if(voo_input.capacidade < 10 || voo_input.capacidade > 100)
                                printf("invalid capacity\n");

                            else{                    
                                /* Copia do input para a estrutura de dados */           
                                memcpy(&voos[cont_voos], &voo_input, sizeof(voo_input) );
                                cont_voos++; 

                                /* Adicionar 1 ao numero de voos do aeroporto */
                                for(cont = 0; cont < cont_aeroportos; cont++){
                                    if(strcmp(aeroportos[cont].ID, voo_input.ID_part) == 0)
                                        aeroportos[cont].num_voos++;
                                }
                            }
                            }
                        }
                        }
                    }
                }
        
        }

        /* Reset das variaveis, para os seus valores iniciais (cont_1 passa a 2, pois e uma variavel global) */
        cont_1 = 2;
        cont_data = 0;
        cont_hora = 0;
        var_cod = 1;
        var_ID = 1;
        res = 0;
        var_ID_part = 0;
        var_ID_cheg = 0;

        /* Reset do codigo do input, pois tem tamanho variavel */
        memset(voo_input.codigo, 0, sizeof(voo_input.codigo));
        }
    }

    else if(input[0] == 'p'){

        for(cont = 2; cont < 2 + MAX_ID; cont++){
            store_id[cont - 2] = input[cont];
        }
        var_ID = 0;

        for(cont = 0; cont < cont_aeroportos; cont++)
            if(strcmp(store_id, aeroportos[cont].ID) == 0)
                var_ID = 1;
        
        /* Verificacao do aeroporto */
        if(var_ID == 0)
            printf("%s: no such airport\n", store_id);
        
        else{

            for(cont = 0; cont < cont_voos; cont++){
                if(strcmp(voos[cont].ID_part, store_id) == 0){

                    pre_ord = pre_ordena_p(voos[cont].dia_, voos[cont].mes_, voos[cont].ano_, voos[cont].hora_, voos[cont].minutos_);
                    var_ID = 0;

                    ordena_p_lista[aux].pre_ord_ = pre_ord;
                    aux++;
                }
            }

            if(var_ID == 0){

                /* Criacao de um vetor para a ordenacao das datas de partida */ 
                for(cont = 0; cont < aux; cont++)
                    array2d_p_2[cont] = ordena_p_lista[cont].pre_ord_;

                bubbleSort(array2d_p_2, aux);

                /* Print dos voos ordenados */
                for(cont = 0; cont < aux; cont++){
                    for(cont2 = 0; cont2 < cont_voos; cont2++){

                        pre_ord = pre_ordena_p(voos[cont2].dia_, voos[cont2].mes_, voos[cont2].ano_, voos[cont2].hora_, voos[cont2].minutos_);

                        if(array2d_p_2[cont] == pre_ord && strcmp(voos[cont2].ID_part, store_id) == 0){

                            printf("%s %s ", voos[cont2].codigo, voos[cont2].ID_cheg);
                            f_print(voos[cont2].dia_, voos[cont2].mes_, voos[cont2].ano_, voos[cont2].hora_, voos[cont2].minutos_);
                            break;
                        }
                    }
                }
                /* Reset da lista ordenada */
                for(cont = 0; cont < cont_voos; cont++){
                    array2d_p_2[cont] = 0;
                }
            }
        }
        for(cont = 0; cont < aux; cont++)
            ordena_p_lista[cont].pre_ord_ = 0;

        var_ID = 1;
        aux = 0;

    }

    else if(input[0] == 'c'){
    int novo_dia, nova_hora, novos_minutos, novo_mes, novo_ano; 

        for(cont = 2; cont < 2 + MAX_ID; cont++)
            store_id[cont - 2] = input[cont];

        var_ID = 0;

        for(cont = 0; cont < cont_aeroportos; cont++)
            if(strcmp(store_id, aeroportos[cont].ID) == 0)
                var_ID = 1;
        
        if(var_ID == 0)
            printf("%s: no such airport\n", store_id);
        
        else{
            
            for(cont = 0; cont < cont_voos; cont++){
                if(strcmp(voos[cont].ID_cheg, store_id) == 0){


                    pre_ord = pre_ordena_c(voos[cont].dia_, voos[cont].mes_, voos[cont].ano_, voos[cont].hora_, voos[cont].minutos_, voos[cont].duracao);
                    var_ID = 0;

                    ordena_c_lista[aux].pre_ord_ = pre_ord;
                    aux++;
                }
            }
            if(var_ID == 0){
                
                /* Criacao de um vetor para a ordenacao das datas de chegada */
                for(cont = 0; cont < aux; cont++)
                    array2d_c_2[cont] = ordena_c_lista[cont].pre_ord_;

                bubbleSort(array2d_c_2, aux);

                /* Print dos voos ordenados */
                for(cont = 0; cont < aux; cont++){
                    for(cont2 = 0; cont2 < cont_voos; cont2++){

                        pre_ord = pre_ordena_c(voos[cont2].dia_, voos[cont2].mes_, voos[cont2].ano_, voos[cont2].hora_, voos[cont2].minutos_, voos[cont2].duracao);

                        if(array2d_c_2[cont] == pre_ord && strcmp(voos[cont2].ID_cheg, store_id) == 0){

                            /* soma_dur soma a duracao a data, incrementando o dia se necessario */
                            nova_hora = soma_dur(voos[cont2].hora_, voos[cont2].minutos_, voos[cont2].duracao, voos[cont2].dia_, voos[cont2].mes_, voos[cont2].ano_, 0);
                            novos_minutos = soma_dur(voos[cont2].hora_, voos[cont2].minutos_, voos[cont2].duracao, voos[cont2].dia_, voos[cont2].mes_, voos[cont2].ano_, 1);
                            novo_dia = soma_dur(voos[cont2].hora_, voos[cont2].minutos_, voos[cont2].duracao, voos[cont2].dia_, voos[cont2].mes_, voos[cont2].ano_, 2);
                            novo_mes = soma_dur(voos[cont2].hora_, voos[cont2].minutos_, voos[cont2].duracao, voos[cont2].dia_, voos[cont2].mes_, voos[cont2].ano_, 3);
                            novo_ano = soma_dur(voos[cont2].hora_, voos[cont2].minutos_, voos[cont2].duracao, voos[cont2].dia_, voos[cont2].mes_, voos[cont2].ano_, 4);

                            printf("%s %s ", voos[cont2].codigo, voos[cont2].ID_part);
                            f_print(novo_dia, novo_mes, novo_ano, nova_hora, novos_minutos);
                            break;
                        }
                    }
                }
                /* Reset da lista ordenada */
                for(cont = 0; cont < cont_voos; cont++){
                    array2d_c_2[cont] = 0;
                }

            }
        }
    for(cont = 0; cont < aux; cont++)
            ordena_c_lista[cont].pre_ord_ = 0;

    aux = 0;
    var_ID = 1;
    }    

    else if(input[0] == 't'){

        /* Passagem do input para a variavel */
        for(cont = 2; cont < 9; cont++){
            if(cont == 2)
                data_input.dia = (input[cont] - '0')* 10 + input[cont + 1] - '0';
            
            else if(cont == 5)
                data_input.mes = (input[cont] - '0')* 10 + input[cont + 1] - '0';

            else if(cont == 8)
                data_input.ano = (input[cont] - '0')* 1000 + (input[cont + 1] - '0')*100 + (input[cont + 2] - '0')*10 + input[cont + 3] - '0';
            
        }

        if(diferenca_datas(data_input.dia, data_input.mes, data_input.ano, data_atual.dia, data_atual.mes, data_atual.ano) == 0)
            printf("invalid date\n");

        else{
            memcpy(&data_atual, &data_input, sizeof(data_atual));

    
            data_print(data_atual.dia, data_atual.mes, data_atual.ano);
        }

        cont = 0;
    }

    /* Reset do input, para nao sobrarem caracteres do input anterior */
    memset(input, 0, sizeof(input));

    /* Novo input */
    fgets(input, sizeof(input), stdin);
}
    return 0;
}
