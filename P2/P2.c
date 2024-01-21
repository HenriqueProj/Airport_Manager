/* segundo projeto de IAED
 * autores: vmm - base do primeiro projeto, Henrique Caroco - 2o Projeto
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUM_AEROPORTOS 40	/* número máximo de areoportos */
#define MAX_NUM_VOOS 30000	/* número máximo de voos */

#define MAX_CODIGO_AEROPORTO 4	/* dimensão do código do aeroporto */
#define MAX_NOME_PAIS 31	/* dimensão do nome do pais */
#define MAX_NOME_CIDADE 51	/* dimensão do nome da cidade */

#define MAX_CODIGO_VOO 7	/* dimensão do código do voo */
#define MAX_DATA 11		/* dimensão da data */
#define MAX_HORA 6		/* dimensão da hora */

#define NAO_EXISTE -1		/* código de erro */

#define ANO_INICIO 2022		/* ano inicial do sistema */
#define DIAS_ANO 365		/* número de dias no ano */
#define HORAS_DIA 24		/* número de horas por dia */
#define MINUTOS_HORA 60		/* número de minutos numa hora */
#define MINUTOS_DIA 1440	/* número de minutos do dia */

#define TRUE 1			/* verdadeiro */
#define FALSE 0			/* falso */

/* Novas constantes: */
#define MAX_INSTRUCAO 65536


/* Tipos de Dados */

typedef struct {
	char id[MAX_CODIGO_AEROPORTO];
	char pais[MAX_NOME_PAIS];
	char cidade[MAX_NOME_CIDADE];
	int numVoos;
} Aeroporto;

typedef struct {
	int dia;
	int mes;
	int ano;
} Data;

typedef struct {
	int hora;
	int minuto;
} Hora;

typedef struct {
	char id[MAX_CODIGO_VOO];
	char partida[MAX_CODIGO_AEROPORTO];
	char chegada[MAX_CODIGO_AEROPORTO];
	Data data;
	Hora hora;
	Hora duracao;
	int capacidade;
	int horaPartida;
	int horaChegada;
	int reservas_pass;
} Voo;

/* ------------------------------------------------------------------------- Novas estruturas: ------------------------------------------------------------------------------------ */
typedef struct {
	char codigo_reserva[MAX_INSTRUCAO];
	int num_passageiros;
	char codigo_voo[MAX_CODIGO_VOO];
	Data data;

} Reserva;




/* Variaveis Globais */

int _numAeroportos = 0;		/* número de aeroportos introduzidos */
Aeroporto _aeroportos[MAX_NUM_AEROPORTOS];	/* vetor de aeroportos */

int _numVoos = 0		/* número de voos introduzidos */;
Voo _voos[MAX_NUM_VOOS];	/* vetor de voos */

Data _hoje = { 1, 1, 2022 };	/* data atual do sistema */

const int _diasMesAc[] =	/* dias acumulados por mês (jan=1) */
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };



/* Novas variaveis: */

int num_reservas = 0;
int cont, var_memory = 1;

Reserva reserva_input;
Reserva** reservas;


/* Funcoes Leitura */

Hora leHora() {
	Hora h;
	scanf("%d:%d", &h.hora, &h.minuto);
	return h;
}


Data leData() {
	Data d;
	scanf("%d-%d-%d", &d.dia, &d.mes, &d.ano);
	return d;
}


int leProximaPalavra(char str[]) {
	char c = getchar();
	int i = 0;
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != ' ' && c != '\t' && c != '\n') {
		str[i++] = c;
		c = getchar();
	}
	str[i] = '\0';
	return (c == '\n');
}


void lePalavraAteFimDeLinha(char str[]) {
	char c = getchar();
	int i = 0;
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != '\n') {
		str[i++] = c;
		c = getchar();
	}
	str[i] = '\0';
}


void leAteFimDeLinha() {
	char c = getchar();
	while (c != '\n')
		c = getchar();
}


/* Funcoes Datas e Horas */

void mostraData(Data d) {
	if (d.dia < 10)
		printf("0");
	printf("%d-", d.dia);
	if (d.mes < 10)
		printf("0");
	printf("%d-%d", d.mes, d.ano);
}


void mostraHora(Hora h) {
	if (h.hora < 10)
		printf("0");
	printf("%d:", h.hora);
	if (h.minuto < 10)
		printf("0");
	printf("%d", h.minuto);
}


int converteDataNum(Data d) {
	return (d.ano - ANO_INICIO) * DIAS_ANO + _diasMesAc[d.mes - 1] +
		d.dia - 1;
}


int converteHoraNum(Hora h) {
	return ((h.hora * MINUTOS_HORA) + h.minuto);
}


int converteDataHoraNum(Data d, Hora h) {
	return converteDataNum(d) * MINUTOS_DIA + converteHoraNum(h);
}


Hora converteNumHora(int num) {
	Hora h;
	h.minuto = num % MINUTOS_HORA;
	h.hora = ((num - h.minuto) / MINUTOS_HORA) % HORAS_DIA;
	return h;
}


Data converteNumData(int num) {
	Data d;
	int i = 0;
	num = (num - (num % MINUTOS_DIA)) / MINUTOS_DIA;
	d.ano = (num / DIAS_ANO) + ANO_INICIO;
	num = num - ((d.ano - ANO_INICIO) * DIAS_ANO);
	while (i <= 11 && num >= _diasMesAc[i])
		i++;
	d.mes = i;
	d.dia = num - _diasMesAc[i - 1] + 1;
	return d;
}


int validaData(Data d) {
	int numData = converteDataNum(d);
	Data proximoAno = _hoje;
	proximoAno.ano++;
	return !(numData < converteDataNum(_hoje)
		 || numData > converteDataNum(proximoAno));
}


int validaHora(Hora h) {
	return !(h.hora > 12 || (h.hora == 12 && h.minuto > 0));
}


/* Algoritmo de ordenação BubbleSort */

void bubbleSort(int indexes[], int size, int (*cmpFunc) (int a, int b)) {
  int i, j, done;
  
  for (i = 0; i < size-1; i++){
    done=1;
    for (j = size-1; j > i; j--) 
      if ((*cmpFunc)(indexes[j-1], indexes[j])) {
	int aux = indexes[j];
	indexes[j] = indexes[j-1];
	indexes[j-1] = aux;
	done=0;
      }
    if (done) break;
  }
}


/* Funcoes Aeroportos */


int aeroportoInvalido(char id[]) {
	int i;
	for (i = 0; id[i] != '\0'; i++)
		if (!(id[i] >= 'A' && id[i] <= 'Z'))
			return TRUE;
	return FALSE;
}


int encontraAeroporto(char id[]) {
	int i;
	for (i = 0; i < _numAeroportos; i++)
		if (!strcmp(id, _aeroportos[i].id))
			return i;
	return NAO_EXISTE;
}


void adicionaAeroporto() {
	Aeroporto a;

	leProximaPalavra(a.id);
	leProximaPalavra(a.pais);
	lePalavraAteFimDeLinha(a.cidade);

	if (aeroportoInvalido(a.id))
		printf("invalid airport ID\n");
	else if (_numAeroportos == MAX_NUM_AEROPORTOS)
		printf("too many airports\n");
	else if (encontraAeroporto(a.id) != NAO_EXISTE)
		printf("duplicate airport\n");
	else {
		strcpy(_aeroportos[_numAeroportos].id, a.id);
		strcpy(_aeroportos[_numAeroportos].pais, a.pais);
		strcpy(_aeroportos[_numAeroportos].cidade, a.cidade);
		_aeroportos[_numAeroportos].numVoos = 0;
		_numAeroportos++;
		printf("airport %s\n", a.id);
	}
}


void mostraAeroporto(int index) {
	printf("%s %s %s %d\n", _aeroportos[index].id,
	       _aeroportos[index].cidade, _aeroportos[index].pais,
	       _aeroportos[index].numVoos);
}


int cmpAeroportos(int a, int b) {
  return (strcmp(_aeroportos[a].id, _aeroportos[b].id) > 0);
}


void listaTodosAeroportos() {
	int i;
	int indexAeroportos[MAX_NUM_AEROPORTOS];

	for (i = 0; i < _numAeroportos; i++)
		indexAeroportos[i] = i;

	bubbleSort(indexAeroportos, _numAeroportos, cmpAeroportos);

	for (i = 0; i < _numAeroportos; i++)
		mostraAeroporto(indexAeroportos[i]);
}


void listaAeroportos() {
	char id[MAX_CODIGO_AEROPORTO];
	int indexAeroporto, ultima = 0;

	ultima = leProximaPalavra(id);
	if (strlen(id) == 0)
		listaTodosAeroportos();
	else {
		while (strlen(id) != 0) {
			indexAeroporto = encontraAeroporto(id);
			if (indexAeroporto == NAO_EXISTE)
				printf("%s: no such airport ID\n", id);
			else
				mostraAeroporto(indexAeroporto);
			if (!ultima)
				ultima = leProximaPalavra(id);
			else
				break;
		}
	}
}



/* Funcoes Voos */

void mostraVoo(int index) {
	if(strcmp("", _voos[index].id))
	{
	printf("%s %s %s ", _voos[index].id, _voos[index].partida,
	       _voos[index].chegada);
	mostraData(_voos[index].data);
	printf(" ");
	mostraHora(_voos[index].hora);
	printf("\n");
	}
}

void mostraVooPartida(int index) {
	printf("%s %s ", _voos[index].id, _voos[index].chegada);
	mostraData(_voos[index].data);
	printf(" ");
	mostraHora(_voos[index].hora);
	printf("\n");
}

void mostraVooChegada(int index) {
	Hora h = converteNumHora(_voos[index].horaChegada);
	printf("%s %s ", _voos[index].id, _voos[index].partida);
	mostraData(converteNumData(_voos[index].horaChegada));
	printf(" ");
	mostraHora(h);
	printf("\n");
}



int encontraVoo(char id[], Data d) {
	int numData = converteDataNum(d);
	int i;

	for (i = 0; i < _numVoos; i++)
		if (!strcmp(id, _voos[i].id)
		    && numData == converteDataNum(_voos[i].data))
			return i;
	return NAO_EXISTE;
}


int validaIDVoo(char id[]) {
	int i = 0, l = strlen(id);
	if (l < 3)
		return FALSE;
	for (i = 0; i < 2; i++)
		if (!(id[i] >= 'A' && id[i] <= 'Z'))
			return FALSE;

	while (id[i] != '\0') {
		if (!(id[i] >= '0' && id[i] <= '9'))
			return FALSE;
		i++;
	}
	return TRUE;
}

int validaVoo(Voo v) {
	if (validaIDVoo(v.id) == FALSE)
		printf("invalid flight code\n");
	else if (encontraVoo(v.id, v.data) != NAO_EXISTE)
		printf("flight already exists\n");
	else if (encontraAeroporto(v.partida) == NAO_EXISTE)
		printf("%s: no such airport ID\n", v.partida);
	else if (encontraAeroporto(v.chegada) == NAO_EXISTE)
		printf("%s: no such airport ID\n", v.chegada);
	else if (_numVoos == MAX_NUM_VOOS)
		printf("too many flihts\n");
	else if (validaData(v.data) == FALSE)
		printf("invalid date\n");
	else if (validaHora(v.duracao) == FALSE)
		printf("invalid duration\n");
	else if (v.capacidade < 10)
		printf("invalid capacity\n");
	else
		return TRUE;
	return FALSE;
}

void criaVoo(Voo v) {
	strcpy(_voos[_numVoos].id, v.id);
	strcpy(_voos[_numVoos].partida, v.partida);
	strcpy(_voos[_numVoos].chegada, v.chegada);
	_voos[_numVoos].data = v.data;
	_voos[_numVoos].hora = v.hora;
	_voos[_numVoos].duracao = v.duracao;
	_voos[_numVoos].capacidade = v.capacidade;
	_voos[_numVoos].horaPartida =
		converteDataHoraNum(_voos[_numVoos].data,
				    _voos[_numVoos].hora);
	_voos[_numVoos].horaChegada =
		_voos[_numVoos].horaPartida +
		converteHoraNum(_voos[_numVoos].duracao);
	_voos[_numVoos].reservas_pass = 0;
	_numVoos++;
}

void adicionaListaVoos() {
	Voo v;
	int i;

	if (leProximaPalavra(v.id)) {
		for (i = 0; i < _numVoos; i++)
			mostraVoo(i);
		return;
	} else {
		leProximaPalavra(v.partida);
		leProximaPalavra(v.chegada);
		v.data = leData();
		v.hora = leHora();
		v.duracao = leHora();
		scanf("%d", &v.capacidade);
		leAteFimDeLinha();
	}

	if (validaVoo(v))
		criaVoo(v);
}


int cmpVoosPartida(int a, int b) {
	return (_voos[a].horaPartida > _voos[b].horaPartida);
}


int cmpVoosChegada(int a, int b) {
	return (_voos[a].horaChegada > _voos[b].horaChegada);
}


void listaVoosPartida() {
	int indexVoos[MAX_NUM_VOOS], i, n = 0;
	char partida[MAX_CODIGO_AEROPORTO];

	lePalavraAteFimDeLinha(partida);

	if (encontraAeroporto(partida) == NAO_EXISTE)
		printf("%s: no such airport ID\n", partida);

	for (i = 0; i < _numVoos; i++) {
		if (!strcmp(_voos[i].partida, partida))
			indexVoos[n++] = i;
	}

	bubbleSort(indexVoos, n, cmpVoosPartida);

	for (i = 0; i < n; i++)
		mostraVooPartida(indexVoos[i]);
}


void listaVoosChegada() {
	int indexVoos[MAX_NUM_VOOS], i, n = 0;
	char chegada[MAX_CODIGO_AEROPORTO];

	lePalavraAteFimDeLinha(chegada);

	if (encontraAeroporto(chegada) == NAO_EXISTE)
		printf("%s: no such airport ID\n", chegada);

	for (i = 0; i < _numVoos; i++) {
		if (!strcmp(_voos[i].chegada, chegada))
			indexVoos[n++] = i;
	}

	bubbleSort(indexVoos, n, cmpVoosChegada);

	for (i = 0; i < n; i++)
		mostraVooChegada(indexVoos[i]);
}


void alteraData() {
	Data d;

	d = leData();
	leAteFimDeLinha();
	if (validaData(d) == FALSE)
		printf("invalid date\n");
	else {
		_hoje = d;
		mostraData(_hoje);
		printf("\n");
	}
}

/* -------------------------------------------------------------------------- Funcoes: ----------------------------------------------------------------------------------------------*/
int comparaDatas(Data d1, Data d2){
    if(d1.dia == d2.dia && d1.mes == d2.mes && d1.ano == d2.ano)
        return 1;
    return 0;
}

int val_cod_reserva(char codigo[]){
	int len = strlen(codigo);

	if(len < 10){
		return 0;
	}

	for(cont = 0; cont < len; cont++)
		if( (codigo[cont] < 'A' || codigo[cont] > 'Z') && 
		(codigo[cont] < '0' || codigo[cont] > '9') ){
			return 0;
		}
	return 1;
}

int EncontraReserva(char cod_res[], Data d) {
	int Data_num = converteDataNum(d);

	for (cont = 0; cont < num_reservas; cont++)
		if (!strcmp(cod_res, reservas[cont]->codigo_reserva) 
		&& Data_num == converteDataNum(_voos[cont].data))
			return cont;
	return NAO_EXISTE;
}

int val_reserva(Reserva reserva_){
	if(!val_cod_reserva(reserva_.codigo_reserva)){
		printf("invalid reservation code\n");
		return 0;
	}

	if(encontraVoo(reserva_.codigo_voo, reserva_.data) == NAO_EXISTE){
		printf("%s: flight does not exist\n", reserva_.codigo_voo);
		return 0;
	}

	for(cont = 0; cont < num_reservas; cont++)
		if(!strcmp(reservas[cont]->codigo_reserva, reserva_.codigo_reserva) ){
			printf("%s: flight reservation already used\n", reserva_.codigo_reserva);
			return 0;
		}

	for(cont = 0; cont < _numVoos; cont++){
		if(!strcmp(_voos[cont].id, reserva_input.codigo_voo) && comparaDatas(_voos[cont].data, reserva_input.data)){
			_voos[cont].reservas_pass += reserva_input.num_passageiros;
			break;
		}
	}

	if(_voos[cont].reservas_pass > _voos[cont].capacidade){
		printf("too many reservations\n");
		cont = 0;
		return 0;
	}
	cont = 0;

	if(validaData(reserva_.data) == FALSE){
		printf("invalid date\n");
		return 0;
	}

	if(reserva_.num_passageiros <= 0){
		printf("invalid passenger number\n");
		return 0;
	}
	return 1;	
}

int CmpReservas(int a, int b){
  return (strcmp(reservas[a]->codigo_reserva, reservas[b]->codigo_reserva) > 0);
}

void mostra_reservas_voo(char cod_voo[], Data data_proc){
    int i = 0;
    int num_print;
    int indiceReservas[MAX_INSTRUCAO];

    for (cont = 0; cont < num_reservas; cont++)
		if(!strcmp(cod_voo, reservas[cont]->codigo_voo)
        && comparaDatas(data_proc, reservas[cont]->data))
            indiceReservas[i++] = cont;
    
    bubbleSort(indiceReservas, i, CmpReservas);

    for(cont = 0; cont < i; cont++){
        num_print = indiceReservas[cont];
        printf("%s ", reservas[num_print]->codigo_reserva);
        printf("%d\n", reservas[num_print]->num_passageiros);
    }

}

void copia_reserva(Reserva res_origem, Reserva *res_destino){
	strcpy(res_destino->codigo_reserva, res_origem.codigo_reserva);
	strcpy(res_destino->codigo_voo, res_origem.codigo_voo);
	res_destino->data = res_origem.data;
	res_destino->num_passageiros = res_origem.num_passageiros;
}

int val_voo_r(Reserva reserva_val){
	if(encontraVoo(reserva_val.codigo_voo, reserva_val.data) == NAO_EXISTE){
		printf("%s: flight does not exist\n", reserva_val.codigo_voo);
		return 0;
	}

	if(validaData(reserva_val.data) == FALSE){
		printf("invalid date\n");
		return 0;
	}

	return 1;
}


void comando_r(){

	leProximaPalavra(reserva_input.codigo_voo);

	reserva_input.data = leData();

	if(val_voo_r(reserva_input)){
		
	if(leProximaPalavra(reserva_input.codigo_reserva)){
		if(val_voo_r(reserva_input))
			mostra_reservas_voo(reserva_input.codigo_voo, reserva_input.data);
	}	
	else{
		scanf("%d", &reserva_input.num_passageiros);
		leAteFimDeLinha();

		if(val_reserva(reserva_input)){

			/* Caso passe a validacao, aloca-se a memoria necessaria */
			reservas[num_reservas] = (Reserva*) malloc(sizeof(Reserva));

			if(!reservas[num_reservas]){
				printf("No memory.\n");
				var_memory = 0;
				for(cont = 0; cont < num_reservas; cont++)
					free(reservas[cont]);
				free(reservas);
			}
			else{
				copia_reserva(reserva_input, reservas[num_reservas]);
				num_reservas++;
			}
		}
	}
	}
}


Voo copiaVoo(Voo voo_o) {
	Voo voo_c;

	strcpy(voo_c.id, voo_o.id);
	strcpy(voo_c.partida, voo_o.partida);
	strcpy(voo_c.chegada, voo_o.chegada);
	voo_c.data = voo_o.data;
	voo_c.hora = voo_o.hora;
	voo_c.duracao = voo_o.duracao;
	voo_c.capacidade = voo_o.capacidade;
	voo_c.horaPartida =
		converteDataHoraNum(voo_c.data,
				    voo_c.hora);
	voo_c.horaChegada =
		voo_c.horaPartida +
		converteHoraNum(voo_c.duracao);
	voo_c.reservas_pass = voo_o.reservas_pass;

	return voo_c;
}

void copiaReserva(Reserva* reserva_o, Reserva* reserva_c){
	strcpy(reserva_c->codigo_reserva, reserva_o->codigo_reserva);
	strcpy(reserva_c->codigo_voo, reserva_o->codigo_voo);
	reserva_c->data = reserva_o->data;
	reserva_c->num_passageiros = reserva_o->num_passageiros;
}

void remove_reserva(int contador){
	int cont_remove_res;
	for(cont_remove_res = 0; cont_remove_res < _numVoos; cont_remove_res++)
		if(!strcmp(reservas[contador]->codigo_voo, _voos[cont_remove_res].id) && comparaDatas(reservas[contador]->data, _voos[cont_remove_res].data)){
			_voos[cont_remove_res].reservas_pass -= reservas[contador]->num_passageiros;
			break;
		}

	for(cont_remove_res = contador; cont_remove_res < num_reservas - 1; cont_remove_res++)
		copiaReserva(reservas[cont_remove_res + 1], reservas[cont_remove_res]);
	free(reservas[num_reservas - 1]);
	num_reservas--;

}

void remove_voos(int contador){
	char save_cod[MAX_CODIGO_VOO];
	int cont_remove;

	strcpy(save_cod, _voos[contador].id);

	for(cont_remove = contador; cont_remove < _numVoos - 1; cont_remove++)
		_voos[cont_remove] = copiaVoo(_voos[cont_remove + 1]);
		
	memset(&_voos[_numVoos - 1], 0, sizeof(_voos[_numVoos - 1]));
	
	_numVoos--;
	cont_remove = 0;

	while(cont_remove < num_reservas){
		if(!strcmp(save_cod, reservas[cont_remove]->codigo_voo)){
				remove_reserva(cont_remove);
		}
		else
			cont_remove++;
	}
}

void comando_e(){
	int val_e = 0;
	char codigo[MAX_INSTRUCAO];

	leProximaPalavra(codigo);

	if((validaIDVoo(codigo) == TRUE)){
		cont = 0;
		while(cont < _numVoos){
			if(!strcmp(codigo, _voos[cont].id)){
				remove_voos(cont);
				val_e = 1;
			}
			else
				cont++;
		}
		if(!val_e)
			printf("not found\n");

	}

	else if(val_cod_reserva(codigo)){
		for(cont = 0; cont < num_reservas; cont++)
			if(!strcmp(codigo, reservas[cont]->codigo_reserva)){
				remove_reserva(cont);
				val_e = 1;
				break;
			}
		if(!val_e)
			printf("not found\n");
	}
	else	
		printf("not found\n");
}


/* Funcao MAIN */

int main() {
	int c;
    reservas = (Reserva**) malloc(sizeof(Reserva*));


	while ((c = getchar()) != EOF) {
		switch (c) {
		case 'q':
			for(cont = 0; cont < num_reservas; cont++)
				free(reservas[cont]);
			free(reservas);
			return 0;
		case 'a': adicionaAeroporto();
			break;
		case 'l': listaAeroportos();
			break;
		case 'v': adicionaListaVoos();
			break;
		case 'p': listaVoosPartida();
			break;
		case 'c': listaVoosChegada();
			break;
		case 't': alteraData();
			break;
		/* Comandos adicionais: */
		case 'r': comando_r();
			break;
		case 'e': comando_e();
			break;
		default: printf("Invalid comand: %c\n", c);
		}
		if(var_memory == 0)
			return 0;
	}

	return 0;
}
