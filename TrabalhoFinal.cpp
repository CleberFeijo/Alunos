#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct professor{
int matricula; //identificador do professor
char nome[40],departamento[20];
};

struct ListaProfs{
    int matricula; //identificador do professor
    char nome[40],departamento[20];
    struct ListaProfs *prox, *anterior;
};

struct ListaProfs *CriaListaP(){
    struct ListaProfs *sentinela = (struct ListaProfs *) malloc (sizeof(struct ListaProfs));
	sentinela->prox = sentinela;
	sentinela->anterior = sentinela;
	return sentinela;
};

struct ListaProfs* CriarElemento(struct professor p){
    struct ListaProfs *lista = (struct ListaProfs*) malloc( sizeof(struct ListaProfs));
    strcpy (lista->departamento, p.departamento);
    strcpy (lista->nome, p.nome);
    lista->matricula = p.matricula;
	lista->prox = NULL;
	lista->anterior = NULL;
	return lista;
}

void InsereElemento(struct ListaProfs *lista, struct professor p){
    struct ListaProfs *novo = CriarElemento(p);
    novo->prox = lista;
    novo->anterior = lista->anterior;
    lista->anterior->prox = novo;
    lista->anterior = novo;
}

int ChecarMatricula (struct ListaProfs *lista, int mat){
    struct ListaProfs *ajudante = lista->prox;

    while(ajudante != lista){
        if(ajudante->matricula == mat){
            return 1;
        }
        ajudante = ajudante->prox;
    }
    return 0;
}

void PassarBinProf (struct ListaProfs *lista){
	FILE *arq;
	arq = fopen("professores.bin", "wb");
    struct professor p;
    struct ListaProfs *ajudante = lista->prox;
    while(ajudante != lista){
        p.matricula = ajudante->matricula;
        strcpy(p.departamento, ajudante->departamento);
        strcpy(p.nome, ajudante->nome);
        fwrite(&p, sizeof(struct professor), 1, arq);
        ajudante = ajudante->prox;
    }
    fclose(arq);
}

void ListarProfs (struct ListaProfs *lista){
    struct ListaProfs *aux = lista->prox;
    while(aux != lista){
        printf("%s\t", aux->nome);
        printf("%s\t", aux->departamento);
        printf("%d\n", aux->matricula);
        aux = aux->prox;
    }
}

struct webnario{
	int id; //identificador do webnário
	char titulo[40],url[20];
	int dia,mes,ano,hora,minuto;
	int qtdProfs; //quantidade de professores que vão participar do webnário
	int matriculaProf[3]; //matriculas dos professores que vão participar do webnário
};

struct ListaWeb{
	int id;
	char titulo[40],url[20];
	int dia,mes,ano,hora,minuto;
	int qtdProfs;
	int matriculaProf[3];
	struct ListaWeb *prox, *ant;
};

struct ListaWeb *CriaListaW(){
	struct ListaWeb *sentinela = (struct ListaWeb*) malloc(sizeof(struct ListaWeb));
	sentinela->prox=sentinela;
	sentinela->ant=sentinela;
	return sentinela;
}

struct ListaWeb *CriaElemento (struct webnario novo){
	struct ListaWeb *resp = (struct ListaWeb*) malloc(sizeof(struct ListaWeb));
	resp->id=novo.id;
	strcpy(resp->titulo, novo.titulo);
	strcpy(resp->url,novo.url);
	resp->dia=novo.dia;
	resp->mes=novo.mes;
	resp->ano=novo.ano;
	resp->hora=novo.hora;
	resp->minuto=novo.minuto;
	resp->qtdProfs=novo.qtdProfs;
	for(int i=0;i<3;i++)
		resp->matriculaProf[i]=novo.matriculaProf[i];
	resp->prox=NULL;
	resp->ant=NULL;
	return resp;
}

void ListarWeb (struct ListaWeb *lista){
    struct ListaWeb *aux = lista->prox;
    while(aux != lista){
    	printf("id: %d\n", aux->id);
    	printf("Titulo: %s\n", aux->titulo);
    	printf("Link: %s\n", aux->url);
    	printf("Data: %d/%d/%d\n", aux->dia, aux->mes, aux->ano);
    	printf("Horario: %d:%d\n", aux->hora, aux->minuto);
    	printf("Quantidade de professores: %d\n", aux->qtdProfs);
    	printf("Professores envolvidos: ");
    	for(int i=0;i<aux->qtdProfs;i++){
        	printf(" %d |", aux->matriculaProf[i]);
    	}
        printf("\n\n");
        aux = aux->prox;
    }
}

void InsereWeb(struct ListaWeb *lista, struct webnario w){
    struct ListaWeb *novo = CriaElemento(w);
    novo->prox = lista;
    novo->ant = lista->ant;
    lista->ant->prox = novo;
    lista->ant = novo;
}

void PassarBinWeb(struct ListaWeb *lista){
	FILE *arq;
	arq = fopen("webnarios.bin", "wb");
    struct ListaWeb *aux = lista->prox;
    while(aux != lista){
        fwrite(aux, sizeof(struct webnario), 1, arq);
        aux = aux->prox;
    }
    fclose(arq);
}

void PreencheLista(struct ListaWeb *webnarios, struct ListaProfs *professores){

	FILE *arq;

	arq = fopen ("webnarios.bin", "rb");

	struct webnario novoweb;
	while (fread(&novoweb,sizeof(struct webnario),1,arq) > 0 ){

		struct ListaWeb *novo = CriaElemento(novoweb);

		novo->prox=webnarios;
		novo->ant=webnarios->ant;
		webnarios->ant->prox=novo;
		webnarios->ant=novo;
	}

	fclose(arq);

    FILE *prof;
    prof = fopen("professores.bin", "rb");

    struct professor profes;

    while (fread(&profes,sizeof(struct professor),1,prof) > 0 ){
        InsereElemento(professores, profes);
    }
    fclose(prof);
}

void CadastraProf(struct ListaProfs *professores){
    int checar;
    struct professor profes;

	printf("\nEscreva a matricula do professor: ");
	scanf("%d", &profes.matricula);
    checar = ChecarMatricula(professores, profes.matricula);
    if(checar == 1){
        printf("\nProfessor ja existente!\n");
    }else{
    	printf("\nEscreva o nome do professor: ");
    	fflush(stdin);
        gets(profes.nome);
        printf("\nEscreva o departamento do professor: ");
        gets(profes.departamento);
        InsereElemento(professores, profes);
    }

}

int ChecarIdWeb(struct ListaWeb *webnarios, int id){
    struct ListaWeb *ajudante = webnarios->prox;

    while(ajudante != webnarios){
        if(ajudante->id == id){
            return 1;
        }
        ajudante = ajudante->prox;
    }
    return 0;
}

void IncluiWeb(struct ListaWeb *webnarios, struct ListaProfs *professores){
	int checarP=0, checarW, mat, sair = 2;
	struct webnario web;

	do{
		printf("\nEscreva a identificacao do webnario: ");
		scanf("%d", &web.id);
		checarW = ChecarIdWeb(webnarios, web.id);
    	if(checarW == 1){
			printf("\nEsse ID ja existe! Deseja sair(1 - sim / 2 - nao)\n");
			scanf("%d", &sair);
			if(sair == 1){
				checarP=1;
				checarW=0;
			}
		}
	}while(checarW==1);
	while(checarP==0){
		printf("\nEscreva a matricula do professor que deseja adicionar no webnario: ");
		scanf("%d", &mat);
		checarP = ChecarMatricula(professores, mat);
		if(checarP == 0){
			printf("\nProfessor nao existente! Deseja sair(1 - sim / 2 - nao)\n");
			scanf("%d", &sair);
			if(sair == 1)
				checarP=1;
		}
	}

    if(sair==2){
		printf("\nEscreva o titulo: ");
		fflush(stdin);
		gets(web.titulo);
		printf("\nEscreva o URL: ");
		gets(web.url);
		printf("\nEscreva a data do webnario (DD MM AAAA): ");
		scanf("%d %d %d", &web.dia, &web.mes, &web.ano);
		printf("\nEscreva o horario do webnario (HH MM): ");
		scanf("%d %d", &web.hora, &web.minuto);
		web.qtdProfs=1;
		web.matriculaProf[0]=mat;
		InsereWeb(webnarios, web);
	}
}

void AdicionaProf(struct ListaProfs *professores, struct ListaWeb *webnarios){
	int checarP=0, checarW, mat, id, sair=2, aux1 = 2;

	do{
		printf("\nEscreva a identificacao do webnario: ");
		scanf("%d", &id);
		checarW = ChecarIdWeb(webnarios, id);
    	if(checarW == 0){
			printf("\nWebnario nao existente! Deseja sair(1 - sim / 2 - nao)\n");
			scanf("%d", &sair);
			if(sair == 1){
				checarP=1;
				checarW=1;
			}
		}
	}while(checarW==0);
	while(checarP==0){
		printf("\nEscreva a matricula do professor que deseja adicionar no webnario: ");
		scanf("%d", &mat);
		checarP = ChecarMatricula(professores, mat);
		if(checarP == 0){
			printf("\nProfessor nao existente! Deseja sair(1 - sim / 2 - nao)\n");
			scanf("%d", &sair);
			if(sair == 1)
				checarP=1;
		}
	}
    if(sair==2){
		struct ListaWeb *aux = webnarios->prox;
		while(aux!=webnarios){
			if(aux->id==id){
				for(int i = 0;i<aux->qtdProfs;i++){
					if(aux->matriculaProf[i]==mat)
						aux1 = 0;
				}
				if (aux->qtdProfs==3){
					printf("\nO webnario atindiu a quantidade maxima de professores!\n");
				}else if(aux1==0){
					printf("\nO professor escolhido ja faz parte do Webnario.\n");
				}else{
					aux->qtdProfs++;
					aux->matriculaProf[aux->qtdProfs-1]=mat;
				}
			}
			aux=aux->prox;
		}
	}
}

void RemoveProf(struct ListaProfs *professores, struct ListaWeb *webnarios){
	int checarP=0, checarW=1, mat, id, sair=2, aux1=2;

	do{
		printf("\nEscreva a identificacao do webnario: ");
		scanf("%d", &id);
		checarW = ChecarIdWeb(webnarios, id);
    	if(checarW == 0){
			printf("\nWebnario nao existente! Deseja sair(1 - sim / 2 - nao)\n");
			scanf("%d", &sair);
			if(sair == 1){
				checarP=1;
				checarW=1;
			}
		}
	}while(checarW==0);
	while(checarP==0){
		printf("\nEscreva a matricula do professor que deseja remover do webnario: ");
		scanf("%d", &mat);
		checarP = ChecarMatricula(professores, mat);
		if(checarP == 0){
			printf("\nProfessor nao existente! Deseja sair(1 - sim / 2 - nao)\n");
			scanf("%d", &sair);
			if(sair == 1)
				checarP=1;
		}
	}
	if(sair == 2){
		struct ListaWeb *aux = webnarios->prox;

		while(aux!=webnarios){
			if(aux->id==id){
				for(int i=0;i<aux->qtdProfs;i++){
					if(aux->matriculaProf[i]==mat){
						for(int j=i;j<aux->qtdProfs;j++)
							aux->matriculaProf[j]=aux->matriculaProf[j+1];
						aux->qtdProfs--;
						aux1++;
					}
				}
				if(aux1==0){
					printf("\nO professor nao faz parte do Webnario\n");
				}
				if(aux->qtdProfs==0){
					aux->prox->ant=aux->ant;
					aux->ant->prox=aux->prox;
				}
			}
			aux=aux->prox;
		}
	}
}

int Contador(struct ListaWeb* w){
    struct ListaWeb* aux = w->prox;
    int i=0;
    while(aux!=w){
        i++;
        aux=aux->prox;
    }
    return i;
}

void ListarVet(struct webnario *vet, int tam){
    for(int i=0; i<tam; i++){
        printf("id: %d\n", vet[i].id);
    	printf("Titulo: %s\n", vet[i].titulo);
    	printf("Link: %s\n", vet[i].url);
    	printf("Data: %d/%d/%d\n", vet[i].dia, vet[i].mes, vet[i].ano);
    	printf("Horario: %d:%d\n", vet[i].hora, vet[i].minuto);
    	printf("Quantidade de professores: %d\n", vet[i].qtdProfs);
    	printf("Professores envolvidos: ");
    	for(int j=0;j<vet[i].qtdProfs;j++){
        	printf(" %d |", vet[i].matriculaProf[j]);
    	}
        printf("\n\n");
    }
}

void Merge(struct webnario *vet,int ini, int meio, int ult, int tam){
	int i,j,k,x;
	struct webnario *aux=(struct webnario*) malloc(tam * sizeof(struct webnario));
	for(i=ini;i<=ult;i++)
		aux[i] = vet[i];
	i=ini;
	j=meio;
	k=ini;
	while(i<meio&&j<=ult){
        if(aux[i].id == aux[j].id){
            j++;
            continue;
        }
            if(aux[i].ano<aux[j].ano){
                vet[k] = aux[i];
                i++;
                k++;
            }
            else if(aux[i].ano == aux[j].ano && aux[i].mes < aux[j].mes){
                vet[k] = aux[i];
                i++;
                k++;
            }
            else if(aux[i].ano == aux[j].ano && aux[i].mes == aux[j].mes && aux[i].dia < aux[j].dia){
                vet[k] = aux[i];
                i++;
                k++;
            }
            else if(aux[i].ano == aux[j].ano && aux[i].mes == aux[j].mes && aux[i].dia == aux[j].dia && aux[i].hora < aux[j].hora){
                vet[k] = aux[i];
                i++;
                k++;
            }
            else if(aux[i].ano == aux[j].ano && aux[i].mes == aux[j].mes && aux[i].dia == aux[j].dia && aux[i].hora == aux[i].hora && aux[i].minuto < aux[j].minuto){
                vet[k] = aux[i];
                i++;
                k++;
            }
            else if(aux[i].ano == aux[j].ano && aux[i].mes == aux[j].mes && aux[i].dia == aux[j].dia && aux[i].hora == aux[i].hora && aux[i].minuto == aux[j].minuto){
                vet[k] = aux[i];
                i++;
                k++;
            }

            if(aux[j].ano<aux[i].ano){
                vet[k] = aux[j];
                j++;
                k++;
            }
            else if(aux[i].ano == aux[j].ano && aux[j].mes < aux[i].mes){
                vet[k] = aux[j];
                j++;
                k++;
            }
            else if(aux[i].ano == aux[j].ano && aux[i].mes == aux[j].mes && aux[j].dia < aux[i].dia){
                vet[k] = aux[j];
                j++;
                k++;
            }
            else if(aux[i].ano == aux[j].ano && aux[i].mes == aux[j].mes && aux[i].dia == aux[j].dia && aux[j].hora < aux[i].hora){
                vet[k] = aux[j];
                j++;
                k++;
            }
            else if(aux[i].ano == aux[j].ano && aux[i].mes == aux[j].mes && aux[i].dia == aux[j].dia && aux[i].hora == aux[i].hora && aux[j].minuto < aux[i].minuto){
                vet[k] = aux[j];
                j++;
                k++;
            }
    }
	if(i==meio){
		for(;j<=ult;j++){
			vet[k] = aux[j];
			k++;
		}
	}else{
		for(;i<meio;i++){
			vet[k] = aux[i];
			k++;
		}
	}
	free(aux);
}

void MergeSort(struct webnario *vet,int prim,int ult, int tam){
    if (prim==ult) return;

    int meio = (prim+ult+1)/2;
    MergeSort(vet,prim,meio-1, tam);
    MergeSort(vet,meio,ult, tam);
    Merge(vet,prim,meio,ult, tam);
}
void PassaVet (struct ListaWeb *w, struct webnario *vet, int tam){
	int i;
	struct ListaWeb *aux1 = w->prox;
	for(i=0;i<tam;i++){
		vet[i].id=aux1->id;
	    strcpy(vet[i].titulo, aux1->titulo);
	    strcpy(vet[i].url,aux1->url);
	    vet[i].dia=aux1->dia;
	    vet[i].mes=aux1->mes;
	    vet[i].ano=aux1->ano;
	    vet[i].hora=aux1->hora;
	    vet[i].minuto=aux1->minuto;
	    vet[i].qtdProfs=aux1->qtdProfs;
	    for(int y=0;y<aux1->qtdProfs;y++)
	        vet[i].matriculaProf[y]=aux1->matriculaProf[y];
		aux1=aux1->prox;
	}
}

int main(){

	struct ListaProfs *professores = CriaListaP();
	struct ListaWeb *webnarios = CriaListaW();
	int aux, TamWeb, i;

	PreencheLista(webnarios, professores);

	do{
		printf("=====PROFESSORES=====\n\n");
		ListarProfs(professores);
		printf("\n=====WEBNARIO=====\n\n");
		ListarWeb(webnarios);
		printf("1 - Cadastrar um novo professor.\n");
		printf("2 - Incluir um novo Webnario.\n");
		printf("3 - Acrescentar professores a um Webnario.\n");
		printf("4 - Retirar professores de um Webnario.\n");
		printf("0 - Sair\n");
		scanf("%d", &aux);
		switch(aux){
			case 1:
				CadastraProf(professores);
				break;
			case 2:
				IncluiWeb(webnarios, professores);
				break;
			case 3:
				AdicionaProf(professores, webnarios);
				break;
			case 4:
				RemoveProf(professores, webnarios);
				break;
		}
	}while(aux!=0);

	TamWeb = Contador(webnarios);

	struct webnario *vet=(struct webnario*) malloc(TamWeb* sizeof(struct webnario));

	PassaVet(webnarios, vet, TamWeb);

	MergeSort(vet, 0, TamWeb-1, TamWeb);

	system("pause");

	ListarVet(vet, TamWeb);

	PassarBinProf(professores);
	PassarBinWeb(webnarios);
}
