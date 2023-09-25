/* iaed-23 - ist1106603 - project2 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define VERDADE 1
#define FALSO 0
#define NAO_ENCONTRADO -1
#define ENCONTRADO 1
#define ERRO_PARSING 1
#define MAX_TAMANHO_LINHA BUFSIZ

/* Estruturas de Dados*/
typedef struct Paragem {
    char* nome;
    double latitude, longitude;
    struct Paragem* next;
} Paragem;

typedef struct Ligacao {
    char* nomeOrigem;
    char* nomeDestino;
    double custo, duracao;
    struct Ligacao* next;
    struct Ligacao* prev;
} Ligacao;  

typedef struct Carreira {
    char* nome;
    double custoTotal;
    double duracaoTotal; 
    Ligacao* ligacoes; 
    struct Carreira* next; 
} Carreira;




/* Função ler texto*/
int leEspacos() {
    int c;
    while ((c = getchar()) == ' ' || c == '\t');
    if (c == '\n') 
        return 0;
    ungetc(c, stdin);
    return 1;
}

/* Le um nome para a string que recebe como parametro. */
int verificaInversoOk(char *s) {
    const char *inv = "inverso";
    int size = strlen(s), i;

    if (size < 3 || size > 7)
        return FALSO;
    for (i = 0; i < size; i++)
        if (inv[i] != s[i])
            return FALSO;
    return VERDADE;
}

char *leNome() {
    int i = 0, c;
    int tamanho = 10;
    char *s = (char *) malloc(tamanho * sizeof(char));

    if (!s) {
        printf("No memory.\n");
        return NULL;
    }

    c = getchar();
    if (c != '"') {
        s[i++] = c;
        while ((c = getchar()) != ' ' && c != '\t' && c != '\n') {
            if (i >= tamanho) {
                tamanho *= 2;
                s = (char *) realloc(s, tamanho * sizeof(char));
                if (!s) {
                    printf("No memory.\n");
                    return NULL;
                }
            }
            s[i++] = c;
        }
        ungetc(c, stdin);
    } else {
        while((c = getchar()) != '"') {
            if (i >= tamanho) {
                tamanho *= 2;
                s = (char *) realloc(s, tamanho * sizeof(char));
                if (!s) {
                    printf("No memory.\n");
                    return NULL;
                }
            }
            s[i++] = c;
        }
    }
    s[i++] = '\0';

    return s;
}

/* Le todo o texto até ao final de linha. */

char *leAteFinalLinha() {
    char c;
    int i = 0;
    int tamanho = 10;
    char *s = (char *) malloc(tamanho * sizeof(char));

    if (!s) {
        printf("No memory.\n");
        return NULL;
    }

    while ((c = getchar()) != '\n') {
        if (i >= tamanho) {
            tamanho *= 2;
            s = (char *) realloc(s, tamanho * sizeof(char));
            if (!s) {
                printf("No memory.\n");
                return NULL;
            }
        }
        s[i++] = c;
    }
    s[i] = '\0';

    return s;
}


/*Encontra Carreira Pelo Nome*/
Carreira* encontraCarreira(Carreira* _carreiras, const char* nome) {
    Carreira* current = _carreiras;

    while (current != NULL) {
        if (strcmp(current->nome, nome) == 0) {
            return current;
        }
        current = (Carreira*) current->next;
    }
    return NULL;
}
/*Adiciona Carreira No Fim*/
void adicionaCarreira(Carreira** _carreiras, Carreira* novaCarreira) {
    Carreira* current = *_carreiras;
    if (current == NULL) {
        *_carreiras = novaCarreira;
    } else {
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = novaCarreira;
    }
}

/*Cria Carreira com o nome*/
Carreira* criaCarreira(char* nome) {
    Carreira* novaCarreira = (Carreira*) malloc(sizeof(Carreira));

    if (novaCarreira == NULL) {
        printf("No memory. \n");
        return NULL;
    }
    novaCarreira->nome = (char *) malloc(strlen(nome) + 1);
    if (novaCarreira->nome == NULL) {
        printf("No memory. \n");
        return NULL;
    }
    novaCarreira->next = NULL;
    novaCarreira->ligacoes= NULL;
    strcpy(novaCarreira->nome, nome);
    novaCarreira->custoTotal = 0.0;
    novaCarreira->duracaoTotal = 0.0;
    return novaCarreira;
}

/*Encontra uma Paragem com o nome*/
Paragem* encontraParagem(char* nomeParagem, Paragem* _paragens) {
    Paragem* atualParagem;
    for (atualParagem = _paragens; atualParagem!=NULL; atualParagem=atualParagem->next)
        if (strcmp(nomeParagem,atualParagem->nome) == 0)
            return atualParagem;
    return NULL;
}

/*Mostra as propriedades de uma paragem tal como é pedido*/ 
void mostraCarreiras(Carreira* _carreiras) {
    Ligacao* _ligacao;

    char* nomeOrigem;
    char* nomeDestino;
    double custoTotal = 0.0;
    double duracaoTotal = 0.0;
    Carreira* carreira = _carreiras;

    while (carreira != NULL) {
        int numParagensDiferentes = 1;
        nomeDestino = "";
        if (carreira->ligacoes != NULL) {
            nomeOrigem = carreira->ligacoes->nomeOrigem;
            for (_ligacao = carreira->ligacoes; _ligacao != NULL; _ligacao = _ligacao->next) {
                numParagensDiferentes++; 
                custoTotal += _ligacao->custo;
                duracaoTotal += _ligacao->duracao;
                nomeDestino = _ligacao->nomeDestino;
            }
            printf("%s %s %s %d %.2f %.2f\n", carreira->nome, nomeOrigem, nomeDestino, numParagensDiferentes, custoTotal, duracaoTotal);
        } else {
            printf("%s %d %.2f %.2f\n", carreira->nome, 0, 0.0, 0.0);
        }
        carreira = carreira->next;
        custoTotal = 0;
        duracaoTotal = 0;
    }
}


/*Permite saber as ligacoes de uma carreira de forma normal ou invertida dependendo do input to utilizador*/
void mostraLigacoesCarreira(char* nomeCarreira, int inverso, Carreira* _carreiras) {
    Carreira* carreira = encontraCarreira(_carreiras, nomeCarreira);
    Ligacao* _ligacao;

    if (carreira->ligacoes == NULL) 
        return;

    if (inverso == FALSO) {
        for (_ligacao = carreira->ligacoes; _ligacao->next != NULL; _ligacao = _ligacao->next) {
            printf("%s", _ligacao->nomeOrigem);
            if (_ligacao->next != NULL) {
                printf(", ");
            }
        }
        printf("%s", _ligacao->nomeOrigem);
        printf(", %s", _ligacao->nomeDestino);
        printf("\n");
    }
    else {
        for (_ligacao = carreira->ligacoes; _ligacao->next != NULL; _ligacao = _ligacao->next);
        for (; _ligacao->prev != NULL; _ligacao = _ligacao->prev) {
            printf("%s", _ligacao->nomeOrigem);
            if (_ligacao->prev != NULL) {
                printf(", ");
            }
        }
        printf("%s", _ligacao->nomeOrigem);
        printf(", %s", _ligacao->nomeDestino);
        printf("\n");
    }
}


/*Permite saber se uma Paragem pertence a uma carreira pelos nomes*/
int encontraParagemCarreira(char* nomeCarreira, char* nomeParagem, Carreira* _carreiras) {
    Carreira* carreira = encontraCarreira(_carreiras, nomeCarreira);
    Ligacao* _ligacao;
    for (_ligacao = carreira->ligacoes; _ligacao != NULL; _ligacao = _ligacao->next) {
        if (strcmp(_ligacao->nomeDestino, nomeParagem) == 0 || strcmp(_ligacao->nomeOrigem, nomeParagem) == 0) {
            return ENCONTRADO;
        }
    }
    return NAO_ENCONTRADO;
}

/*Funcao que gere o comando C*/
void carreiras(Carreira** _carreiras) {
    char *nomeCarreira;
    Carreira* carreiraEncontrada;
    int fimLinha = leEspacos();
    Carreira *tempCarreira;
    
    if (!fimLinha) {
        mostraCarreiras(*_carreiras);
        return;
    }
    nomeCarreira = leNome();
    carreiraEncontrada = encontraCarreira(*_carreiras,nomeCarreira);
    fimLinha = leEspacos();
    if (!fimLinha) {
        if (carreiraEncontrada == NULL) {
            tempCarreira=criaCarreira(nomeCarreira);
            adicionaCarreira(_carreiras, tempCarreira);   
        } else {
            mostraLigacoesCarreira(nomeCarreira,FALSO,*_carreiras);
        }
    } else {
        char *buffer = leNome();
        if (verificaInversoOk(buffer)) {
            mostraLigacoesCarreira(nomeCarreira,VERDADE, *_carreiras);
            }
        else{
            printf("incorrect sort option.\n");
        }
        free(buffer);
        buffer = leAteFinalLinha();
        free(buffer);
    }

    free(nomeCarreira);
}

/* Funções para tratar as paragens */

/*Permite observar todas as paragens da forma como é pedida*/
void mostraParagens(Paragem* _paragens, Carreira* _carreiras) {
    int cnt = 0;
    Paragem* ParagemAtual;
    Carreira* CarreiraAtual;
    
    for (ParagemAtual = _paragens; ParagemAtual != NULL; ParagemAtual = ParagemAtual->next) {
        cnt = 0;
        for (CarreiraAtual = _carreiras; CarreiraAtual != NULL; CarreiraAtual = CarreiraAtual->next) {
            if(encontraParagemCarreira(CarreiraAtual->nome,ParagemAtual->nome,_carreiras)== ENCONTRADO)
                cnt++;
        }
        printf("%s: %16.12f %16.12f %d\n", ParagemAtual->nome, ParagemAtual->latitude, ParagemAtual->longitude, cnt);
    }
}

/*Adiciona uma paragem no fim das paragens*/
void adicionaParagem(Paragem** _paragens, Paragem* novaParagem) {
    Paragem* current;

    if (*_paragens == NULL) {
        *_paragens = novaParagem;
    } else {
        for (current = *_paragens; current->next != NULL; current = current->next);
        current->next = novaParagem;
    }
}

/*Permite criar uma paragem com o seu nome e coordenadas*/
Paragem *criaParagem(char* nomeParagem, double latitude, double longitude) {
    Paragem *novaParagem = (Paragem *) malloc(sizeof(Paragem));

    if (!novaParagem) {
        printf("No memory.\n");
        return NULL;
    }

    novaParagem->nome = (char *) malloc(strlen(nomeParagem) + 1);
    if (!novaParagem->nome) {
        printf("No memory.\n");
        free(novaParagem);
        return NULL;
    }

    strcpy(novaParagem->nome, nomeParagem);
    novaParagem->latitude = latitude;
    novaParagem->longitude = longitude;
    novaParagem->next = NULL;
    return novaParagem;
}

/* Funçao que gere o comando P*/
void paragens(Paragem **_paragens,Carreira** _carreiras) {
    char *nomeParagem;
    Paragem* novaParagem;
    int fimLinha = leEspacos();
    if (!fimLinha) {
        mostraParagens(*_paragens,*_carreiras);
        return;
    }

    nomeParagem=leNome();
    fimLinha = leEspacos();
    if (!fimLinha) {
        novaParagem = encontraParagem(nomeParagem,*_paragens);
        if (encontraParagem(nomeParagem, *_paragens) == NULL){
            printf("%s: no such stop.\n", nomeParagem);
        }
        else {
            printf("%16.12f %16.12f\n", novaParagem->latitude,novaParagem->longitude);
        }
    }
    else {
        double latitude, longitude;
        scanf("%lf%lf", &latitude, &longitude);
        if (encontraParagem(nomeParagem, *_paragens) == NULL){
            novaParagem=criaParagem(nomeParagem,latitude,longitude);
            adicionaParagem(_paragens,novaParagem);
        }
        else
            printf("%s: stop already exists.\n", nomeParagem);
        leAteFinalLinha();
    }

    free(nomeParagem);
}

/* Funções para tratar as Ligações */


/*Permite adicionar uma ligacao no fim das ligacoes de uma carreira*/
void adicionaLigacao(Carreira* carreira, Ligacao* novaLigacao) {
    Ligacao* current;


    if (carreira->ligacoes == NULL) {
        current =  novaLigacao;
        return;
    }
    for (current =carreira->ligacoes; current->next != NULL; current =  current->next);
    current->next = novaLigacao;
    novaLigacao->prev= current;
}
 /*Permite criar uma ligacao com os nomes das duas paragens e o custo e duracao*/
Ligacao* criaLigacao(char* nomeParagemOrigem, char* nomeParagemDestino,
                 double custo, double duracao) {
    Ligacao* novaLigacao = (Ligacao*) malloc(sizeof(Ligacao));

    if (novaLigacao == NULL) {
        printf("No memory. \n");
        return NULL;
    }

    novaLigacao->nomeOrigem = nomeParagemOrigem;
    novaLigacao->nomeDestino = nomeParagemDestino;
    novaLigacao->custo = custo;
    novaLigacao->duracao = duracao;
    novaLigacao->next = NULL;
    novaLigacao->prev = NULL;
    return novaLigacao;
}

/*Permite obter a ultima ligacao de uma carreira*/
Ligacao* ultimaLigacao(Carreira* carreira){
    Ligacao* current;
    for(current= carreira->ligacoes; current->next !=NULL; current=current->next);
    return current;
}
/* Adiciona uma nova ligação a uma carreira. */
void addLigacao(Carreira* carreira, Paragem* ParagemOrigem, Paragem* ParagemDestino,
                     double custo, double duracao) {
    Ligacao* novaLigacao = criaLigacao(ParagemOrigem->nome, ParagemDestino ->nome, custo, duracao);
    if (carreira->ligacoes == NULL) 
        carreira->ligacoes = novaLigacao;
    else {

        if (strcmp(ParagemOrigem->nome,ultimaLigacao(carreira)->nomeDestino)==0 ) {
            adicionaLigacao(carreira,novaLigacao);
        }
        else if (strcmp(ParagemDestino->nome,carreira->ligacoes->nomeOrigem)==0) {
            carreira->ligacoes->prev= novaLigacao;
            novaLigacao->next= carreira->ligacoes;
            carreira->ligacoes=novaLigacao;
        }
        else 
            printf("link cannot be associated with bus line.\n");
    }
}

/*Gere o comando l*/
void ligacoes(Carreira **_carreiras, Paragem **_paragens) {
    char *nomeCarreira, *nomeOrigem, *nomeDestino;
    double custo, duracao;
    Carreira* _Carreira;
    Paragem* ParagemOrigem;
    Paragem* ParagemDestino;
    leEspacos();
    nomeCarreira = leNome();
    leEspacos();
    nomeOrigem = leNome();
    leEspacos();
    nomeDestino = leNome();
    scanf("%lf%lf", &custo, &duracao);
    leAteFinalLinha();
    _Carreira = encontraCarreira(*_carreiras,nomeCarreira);
    if (_Carreira == NULL)
        printf("%s: no such line.\n", nomeCarreira);
    else {
        ParagemOrigem = encontraParagem(nomeOrigem, *_paragens);
        if (ParagemOrigem == NULL)
            printf("%s: no such stop.\n", nomeOrigem);
        else {
            ParagemDestino = encontraParagem(nomeDestino, *_paragens);
            if (ParagemDestino == NULL)
                printf("%s: no such stop.\n", nomeDestino);
            else if (custo < 0.0 || duracao < 0.0)
                printf("negative cost or duration.\n");
            else
                addLigacao(_Carreira, ParagemOrigem, ParagemDestino, custo, duracao);
        }
    }

    free(nomeCarreira);
    free(nomeOrigem);
    free(nomeDestino);
}

/*Permite ordenar carreiras*/
void ordenaSortCarreira(Carreira** _carreiras) {
    Carreira *current;
    int houveTroca = VERDADE;

    while (houveTroca) {
        houveTroca = FALSO;
        current = *_carreiras;

        while (current != NULL && current->next != NULL) {
            if (strcmp(current->nome, current->next->nome) > 0) {
                Carreira* temp = current;
                current = current->next;
                temp->next = current->next;
                current->next = temp;
                houveTroca = VERDADE;
            }
            else {
                current = current->next;
            }
        }
    }
}

/* Libera a memoria das paragens*/
void liberaParagens(Paragem *paragens) {
    Paragem *paragemAtual, *paragemTemp;
    paragemAtual = paragens;

    while (paragemAtual != NULL) {
        paragemTemp = paragemAtual;
        paragemAtual = paragemAtual->next;
        free(paragemTemp->nome); 
        free(paragemTemp); 
        free(paragemTemp);
    }
}

/*Libera a memoria das ligacoes*/
void liberaLigacoes(Ligacao *ligacoes) {
    Ligacao *ligacaoAtual, *ligacaoTemp;
    ligacaoAtual = ligacoes;

    while (ligacaoAtual != NULL) {
        ligacaoTemp = ligacaoAtual;
        ligacaoAtual = ligacaoAtual->next;
        free(ligacaoTemp->nomeOrigem);
        free(ligacaoTemp->nomeDestino);
        free(ligacaoTemp);
    }
}
/*Libera a memoria das carreiras*/
void liberaCarreiras(Carreira *carreiras) {
    Carreira *carreiraAtual, *carreiraTemp;
    carreiraAtual = carreiras;

    while (carreiraAtual != NULL) {
        carreiraTemp = carreiraAtual;
        carreiraAtual = carreiraAtual->next;
        free(carreiraTemp->nome);
        liberaLigacoes(carreiraTemp->ligacoes);
        free(carreiraTemp);
    }
}
/*Gere o comando i que deseja saber todas as intersecoes*/
void intersecoes(Carreira** _carreiras, Paragem** _paragens) {
    Paragem* ParagemAtual;
    Carreira* CarreiraAtual;
    Carreira* ListaCarreiras;
    Carreira* current;
    Carreira* lastCarreira;
    int numCarreiras = 0;

    for (ParagemAtual = *_paragens; ParagemAtual != NULL; ParagemAtual = ParagemAtual->next) {
        ListaCarreiras = NULL;
        lastCarreira = NULL;
        numCarreiras = 0;

        for (CarreiraAtual = *_carreiras; CarreiraAtual != NULL; CarreiraAtual = CarreiraAtual->next) {
            if (encontraParagemCarreira(CarreiraAtual->nome, ParagemAtual->nome, *_carreiras) == ENCONTRADO) {
                Carreira *novaCarreira = criaCarreira(CarreiraAtual->nome);
                if (ListaCarreiras == NULL) {
                    ListaCarreiras = novaCarreira;
                } else {
                    lastCarreira->next = novaCarreira;
                }
                lastCarreira = novaCarreira;
                numCarreiras++;
            }
        }

        ordenaSortCarreira(&ListaCarreiras);
        printf("%s %d: ", ParagemAtual->nome, numCarreiras);
        for (current = ListaCarreiras; current != NULL; current = current->next) {
            if (current->nome != NULL) {
                printf("%s", current->nome);
                if (current->next != NULL) {
                    printf(" ");
                }
            }
        }
        printf("\n");
        liberaCarreiras(ListaCarreiras);
    }
}








/*Libera a memoria de uma carreira*/
void liberaCarreira(Carreira *carreira) {
    Ligacao *ligacao, *ligacao_temp;
    
    free(carreira->nome);
    
    ligacao = carreira->ligacoes;
    while (ligacao != NULL) {
        ligacao_temp = ligacao;
        ligacao = ligacao->next;
        free(ligacao_temp);
    }
    
    free(carreira);
}


/*Gere o comando R que tem como objetivo eliminar uma carreira*/
void comandoR(Carreira **_carreiras) {
    char *nomeCarreira;
    Carreira *CarreiraEliminar;
    Carreira *CarreiraAtual;
    Carreira *CarreiraAnterior;
    leEspacos();
    nomeCarreira = leNome();
    leAteFinalLinha();
    CarreiraEliminar = encontraCarreira(*_carreiras, nomeCarreira);
    if (CarreiraEliminar== NULL){
        printf("%s: no such line.\n",CarreiraEliminar->nome);
        return;
    }
    if (CarreiraEliminar != NULL) {
        if (*_carreiras == CarreiraEliminar) {
            *_carreiras = (*_carreiras)->next;
            liberaCarreira(CarreiraEliminar);
        } else {
            CarreiraAnterior = *_carreiras;
            CarreiraAtual = (*_carreiras)->next;
            while (CarreiraAtual != NULL && CarreiraAtual != CarreiraEliminar) {
                CarreiraAnterior = CarreiraAtual;
                CarreiraAtual = CarreiraAtual->next;
            }
            if (CarreiraAtual != NULL) {
                CarreiraAnterior->next = CarreiraAtual->next;
                liberaCarreira(CarreiraAtual);
            }
        }
    }

    free(nomeCarreira);
}

/*Remove uma paragem*/
void removeParagem(Paragem** _paragens, char* nomeParagem) {
    Paragem *current = *_paragens;
    Paragem *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->nome, nomeParagem) == 0) {
            if (previous == NULL) {
                *_paragens = current->next;
            } else {
                previous->next = current->next;
            }
            free(current->nome);
            free(current);
            break;
        }
        previous = current;
        current = current->next;
    }
}

/*Gere o comando E*/
void comandoE(Carreira **_carreiras, Paragem** paragens) {
    char *nomeParagem;
    Carreira *CarreiraAtual;
    Ligacao *LigacaoAtual, *LigacaoProxima;
    leEspacos();
    nomeParagem = leNome();
    leAteFinalLinha();
    for (CarreiraAtual = *_carreiras; CarreiraAtual != NULL; CarreiraAtual = CarreiraAtual->next) {
        LigacaoAtual = CarreiraAtual->ligacoes;
        LigacaoProxima = (LigacaoAtual != NULL) ? LigacaoAtual->next : NULL;
        while (LigacaoAtual != NULL) {
            if (strcmp(LigacaoAtual->nomeOrigem, nomeParagem) == 0) {
                CarreiraAtual->ligacoes = LigacaoProxima;
                LigacaoAtual = LigacaoProxima;
            } else if (strcmp(LigacaoAtual->nomeDestino, nomeParagem) == 0) {
                if (LigacaoProxima == NULL) {
                    if (LigacaoAtual->prev != NULL) {
                        LigacaoAtual->prev->next = NULL;
                    }
                    else{
                        CarreiraAtual->ligacoes = LigacaoProxima;
                        LigacaoAtual = LigacaoProxima;
                    }
                }

                else{
                    LigacaoProxima->nomeOrigem= LigacaoAtual->nomeOrigem;
                    LigacaoProxima->prev=LigacaoAtual->prev;
                    LigacaoProxima->custo+=LigacaoAtual->custo;
                    LigacaoProxima->duracao+=LigacaoAtual->duracao;
                    if(LigacaoAtual->prev ==NULL)
                        CarreiraAtual->ligacoes=LigacaoProxima;
                    
                    LigacaoAtual=LigacaoProxima;
                    LigacaoProxima = LigacaoProxima->next;
                }
            } else {
                LigacaoAtual = LigacaoProxima;
                LigacaoProxima = (LigacaoAtual != NULL) ? LigacaoAtual->next : NULL;

            }
        }
    }
    removeParagem(paragens, nomeParagem);
}




/*Gere o comando A que tem como objetivo eliminar todos os dados*/
void comandoA(Paragem **paragens, Carreira **carreiras) {
    liberaParagens(*paragens);
    *paragens = NULL;
    liberaCarreiras(*carreiras);
    *carreiras = NULL;
}


int main() {
    int c;
    Carreira* _carreiras = NULL;
    Paragem* _paragens = NULL;

    do {
        c = getchar();
        switch(c) {
            case 'c':
                carreiras(&_carreiras);
                break;

            case 'p':
                paragens(&_paragens,&_carreiras);
                break;
               
            case 'l':
                ligacoes(&_carreiras, &_paragens);
                break;
            case 'r':
                comandoR(&_carreiras);
                break;    
            case 'i':
                intersecoes(&_carreiras, &_paragens);
                break;
            case 'e':
                comandoE(&_carreiras,&_paragens);
                break;
            case 'a':
                comandoA(&_paragens,&_carreiras);
                break;
            case 'q':
                return 0;
                break;
            default:
                /* Ignorar linhas em branco */
                if (c == ' ' || c == '\t' || c == '\n') break;
        }
    } while (c != 'q');

    return 0;
}
