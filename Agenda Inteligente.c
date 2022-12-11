#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define MAX_STRING 100
#define LIMIAR 0.2

typedef struct{
    int dia;
    int mes;
    int ano;
}data_t;

typedef struct{
    float samba;
    float rock;
    float mpb;
}preferencias_t;

typedef struct{
    char nome_completo[MAX_STRING];
    data_t nascimento;
    char cidade[MAX_STRING];
    char uf[MAX_STRING];
    preferencias_t preferencias;
    int grupo;
}pessoa_t;

void buscar_registro(pessoa_t *agenda, int cont);
void imprimir_menu();
FILE *abrir_arquivo();
FILE *abrir_arquivo_saida();
pessoa_t *importar_registros(pessoa_t *agenda, FILE *arquivo, int *cont);
pessoa_t *editar_registro(pessoa_t *agenda, int i);
pessoa_t *deletar_registro(pessoa_t *agenda);
pessoa_t *novo_registro(pessoa_t *agenda, int cont);
pessoa_t *ordenar_registro(pessoa_t *agenda, int cont);
pessoa_t *agrupar_registros(pessoa_t *agenda, int cont);    
pessoa_t *trocar_posicoes(pessoa_t *agenda, int i, int j);
void imprimir_registro(pessoa_t *agenda, int cont);


int main()
{
    // Declaracao das variaveis
    FILE *arquivo;
    FILE *arquivo_saida;
    pessoa_t *agenda = NULL;
    int cont;
    int opcao;
    int i;
    char aux_nome[MAX_STRING];

    // Abrindo o arquivo
    arquivo = abrir_arquivo();
    assert(arquivo);

    do
    {   
        imprimir_menu();

        // Lendo a opcao desejada
        scanf("%d", &opcao);

        // Switch com as operacoes
        switch (opcao)
        {
        case 1:
            // Importar registro
            agenda = importar_registros(agenda, arquivo, &cont);
            break;
        case 2:
            // Novo registro
            cont++;
            novo_registro(agenda, cont);
            break;
        case 3:
            // Editar registro
            // Lendo o registro que sera editado
            printf("Registro desejado: ");
            scanf("%d", &i);
            // Editando o registro
            agenda = editar_registro(agenda, i);
            break;
        case 4:
            // Excluir registro
            agenda = deletar_registro(agenda);
            break;
        case 5:
            // Buscar registro
            buscar_registro(agenda, cont);
            break;
        case 6:
            // Ordenar os registros
            agenda = ordenar_registro(agenda, cont);
            break;
        case 7:
            // Agrupar os registros
            agenda = agrupar_registros(agenda, cont);
            break;
        case 8:
            // Abrindo o arquivo de saida
            arquivo_saida = abrir_arquivo_saida();
            assert(arquivo_saida);
            // escrevendo o vetor no arquivo de saida
            fwrite(agenda, sizeof(pessoa_t), cont, arquivo_saida);
            break;
        case 9:
            imprimir_registro(agenda, cont);
            break;
        case 0:
            // Sair do laco
            printf("Bye\n");
            break;
        default:
            printf("Operação inválida!\n");
            break;
        }
    } while (opcao != 0);

    // Fechando o arquivo e desalocando o vetor
    fclose(arquivo);
    free(agenda);

    return 0;
}

void imprimir_registro(pessoa_t *agenda, int cont)
{
    for(int i = 0; i < cont; i++)
    {
        if(agenda[i].grupo != -1)
        {
            printf("%s\n", agenda[i].nome_completo);
            printf("%d/%d/%d\n", agenda[i].nascimento.dia, 
                                agenda[i].nascimento.mes, 
                                agenda[i].nascimento.ano);
            printf("%s\n", agenda[i].cidade);
            printf("%s\n", agenda[i].uf);
            printf("Samba: %.2f\n", agenda[i].preferencias.samba);
            printf("Rock: %.2f\n", agenda[i].preferencias.rock);
            printf("MPB: %.2f\n", agenda[i].preferencias.mpb);
            printf("Grupo: %d\n", agenda[i].grupo);
            printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
        }
    }
}

void imprimir_menu()
{
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("                    MENU                        \n");
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("[0] Sair\n");
    printf("[1] Importar registro\n");
    printf("[2] Criar um novo registro\n");
    printf("[3] Editar registro\n");
    printf("[4] Deletar registro\n");
    printf("[5] Buscar registro\n");
    printf("[6] Ordenar registro\n");
    printf("[7] Agrupar registro\n");
    printf("[8] Exportar registro\n");
    printf("[9] Imprimir registro\n");
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}

pessoa_t *agrupar_registros(pessoa_t *agenda, int cont)
{
    // Declaracao das variaveis
    float distancia;
    float menor_distancia = 2000;
    int grupo_menor_distancia;
    int quantidade_grupos = 1;

    // Os registros com grupo = -1 foram excluidos
    //  Inicializando todos os registros nao excluidos com 0, que vai servir como flag
    for(int i = 0; i < cont; i++)
    {
        if(agenda[i].grupo != -1)
        {
            agenda[i].grupo = 0;
        }
    }

    // definindo o primeiro elemento nao excluido como pertencente ao grupo 1
    for(int i = 0; i < cont; i++)
    {
        if(agenda[i].grupo != -1)
        {
            agenda[i].grupo = 1;
            break;
        }
    }

    // Agrupando os registros
    // a agenda[i] eh o registro cujo grupo vai ser definido
    for(int i = 0; i < cont; i++)
    {  
        // Inicializando a distancia como um numero muito grande
        menor_distancia = 2000;

        // Comparando agenda[i] com todos os outros registros
        for(int j = 0; j < cont; j++)
        {
            // Se o registro foi exluido, nao eh necessario agrupar
            if(agenda[i].grupo == -1 )
            {
                break;
            } 
            // Se o registro nao foi excluido e ja foi agrupado, se calcula a distancia
            if(agenda[j].grupo != -1 && agenda[j].grupo != 0 && i != j)
            {
                distancia = pow((agenda[i].preferencias.rock-agenda[j].preferencias.rock),2);
                distancia += pow((agenda[i].preferencias.samba-agenda[j].preferencias.samba),2);
                distancia += pow((agenda[i].preferencias.mpb-agenda[j].preferencias.mpb),2);
                distancia = sqrt(distancia);
                // Atualizando a menor distancia
                if(distancia < menor_distancia)
                {
                    menor_distancia = distancia;
                    grupo_menor_distancia = j;
                }
            }
        }
        // Caso a menor distancia seja menor que um limiar, agrupa-se agenda[i] junto com grupo_menor_distancia
        if(menor_distancia < LIMIAR && menor_distancia != 0)
        {
            agenda[i].grupo = agenda[grupo_menor_distancia].grupo;
        }
        else // Caso contrario, se cria um novo grupo
        {
            quantidade_grupos++;
            agenda[i].grupo = quantidade_grupos;
        }
    }

    return agenda;
}

pessoa_t *trocar_posicoes(pessoa_t *agenda, int i, int j)
{
    // Declaracao das variaveis
    int aux;
    float auxf;
    char temp[MAX_STRING];

    // Percorre cada atributo e os troca de posicao

    // Trocar os nomes
    strcpy(temp, agenda[j].nome_completo);
    strcpy(agenda[j].nome_completo, agenda[j+1].nome_completo);
    strcpy(agenda[j+1].nome_completo, temp);

    // Trocar o aniversário
    aux = agenda[j].nascimento.dia;
    agenda[j].nascimento.dia = agenda[j+1].nascimento.dia;
    agenda[j+1].nascimento.dia = aux;
    aux = agenda[j].nascimento.mes;
    agenda[j].nascimento.mes = agenda[j+1].nascimento.mes;
    agenda[j+1].nascimento.mes = aux;
    aux = agenda[j].nascimento.ano;
    agenda[j].nascimento.ano = agenda[j+1].nascimento.ano;
    agenda[j+1].nascimento.ano = aux;

    //Cidade
    strcpy(temp, agenda[j].cidade);
    strcpy(agenda[j].cidade, agenda[j+1].cidade);
    strcpy(agenda[j+1].cidade, temp);
    // UF
    strcpy(temp, agenda[j].uf);
    strcpy(agenda[j].uf, agenda[j+1].uf);
    strcpy(agenda[j+1].uf, temp);

    // Preferencias
    auxf = agenda[j].preferencias.samba;
    agenda[j].preferencias.samba = agenda[j+1].preferencias.samba;
    agenda[j+1].preferencias.samba = auxf;
    auxf = agenda[j].preferencias.rock;
    agenda[j].preferencias.rock = agenda[j+1].preferencias.rock;
    agenda[j+1].preferencias.rock = auxf;
    auxf = agenda[j].preferencias.mpb;
    agenda[j].preferencias.mpb = agenda[j+1].preferencias.mpb;
    agenda[j+1].preferencias.mpb = auxf;

    // Grupo
    aux = agenda[j].grupo;
    agenda[j].grupo = agenda[j+1].grupo;
    agenda[j+1].grupo = aux;

    return agenda;
}

pessoa_t *ordenar_registro(pessoa_t *agenda, int cont)
{
    int opt;

    // Lendo a opcao desejada
    printf("[1] Ordenar por nome\n[2]Ordenar por data de nascimento\n[3]Ordenar por grupo\n");
    scanf("%d", &opt);

    switch (opt)
    {
    case 1:
        // Ordenar por nome
        for (int i = 0; i < cont; i++) // Repete o bubblesort ate todos os nomes estarem organizados
        { 
            for (int j = 0; j < cont-1; j++) // Percorre cada nome e o compara com o proximo
            {
                for (int k = 0; k < strlen(agenda[j].nome_completo); k++) // Percorre cada caractere
                {
                    // Se o sobrenome anterior possui um caractere menor, precisamos troca-lo
                    if ((int)agenda[j].nome_completo[k] > (int)agenda[j+1].nome_completo[k]) 
                    {
                        agenda = trocar_posicoes(agenda, i, j);

                        break;
                    }
                    // Caso ele seja menor, ja esta ordenado
                    else if ((int)agenda[j].nome_completo[k] < (int)agenda[j+1].nome_completo[k]) {
                        break;
                    }
                }
            }
        }
        break;
    case 2:
        // Ordenar por data de nascimento
        for (int i = 0; i < cont; i++) // Repete cont vezes
        { 
            for (int j = 0; j < cont-1; j++) // Compara cada posicao com a proxima
            {
                if (agenda[j].nascimento.ano < agenda[j+1].nascimento.ano // Ano1 < Ano2
                || (agenda[j].nascimento.ano == agenda[j+1].nascimento.ano &&  // Ano1 == Ano2, Mes1 < Mes2
                    agenda[j].nascimento.mes < agenda[j+1].nascimento.mes) 
                || (agenda[j].nascimento.ano == agenda[j+1].nascimento.ano && // Ano1 == Ano2, Mes1 == Mes2, Dia1 < Dia2
                    agenda[j].nascimento.mes == agenda[j+1].nascimento.mes && 
                    agenda[j].nascimento.dia < agenda[j+1].nascimento.dia)) 
                {
                    agenda = trocar_posicoes(agenda, i, j);
                    break;
                }
            }
        }
        break;
    case 3:
        // Ordenar por grupo
        for (int i = 0; i < cont; i++) // Repete cont vezes
        { 
            for (int j = 0; j < cont-1; j++) // Compara cada posicao com a prox
            {
                if (agenda[j].grupo > agenda[j+1].grupo) //se o grupo e menor que o prox, invertemos as posicoes
                {
                    agenda = trocar_posicoes(agenda, i, j);
                    break;
                }
            }
        }
        break;

    default:
        break;
    }

    return agenda;
}

void buscar_registro(pessoa_t *agenda, int cont)
{
    // Declaracao das variaveis
    int opt;
    char str[MAX_STRING];
    int aux;
    int aux1;
    int aux2;

    // Menu com as opcoes
    printf("[1] Buscar por nome\n[2]Buscar por data de nascimento\n[3]Buscar por grupo\n");
    // Lendo a opcao desejada
    scanf("%d", &opt);

    switch (opt)
    {
    case 1:
        // Buscando registros que contem um nome digitado

        // Lendo o nome que sera buscado
        printf("Nome: ");
        scanf("%s", str);

        // Percorrendo os registros
        for(int i = 0; i < cont; i++)
        {
            // Se o registro contem o nome e nao foi exluido, o imprimimos
            if(strstr(agenda[i].nome_completo,str) != NULL && agenda[i].grupo != -1)
            {
                // Imprime cada atributo do registro
                printf("%s\n", agenda[i].nome_completo);
                printf("%d/%d/%d\n", agenda[i].nascimento.dia, 
                                    agenda[i].nascimento.mes, 
                                    agenda[i].nascimento.ano);
                printf("%s\n", agenda[i].cidade);
                printf("%s\n", agenda[i].uf);
                printf("Samba: %.2f\n", agenda[i].preferencias.samba);
                printf("Rock: %.2f\n", agenda[i].preferencias.rock);
                printf("MPB: %.2f\n", agenda[i].preferencias.mpb);
                printf("Grupo: %d\n", agenda[i].grupo);
                printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
            }
        }
        break;
    case 2:
        // Buscando registros por data de nascimento
        printf("Dia: ");
        scanf("%d", &aux);
        printf("Mes: ");
        scanf("%d", &aux1);
        printf("Ano: ");
        scanf("%d", &aux2);

        // Percorre todos os registros
        for(int i = 0; i < cont; i++)
        {
            // Se a data de nascimento coincidir e o registro nao ter sido excluido, imprimimos o registro
            if( agenda[i].nascimento.dia == aux && 
                agenda[i].nascimento.mes == aux1 && 
                agenda[i].nascimento.ano == aux2 && 
                agenda[i].grupo != -1)
            {
                printf("%s\n", agenda[i].nome_completo);
                printf("%d/%d/%d\n", agenda[i].nascimento.dia, 
                                    agenda[i].nascimento.mes, 
                                    agenda[i].nascimento.ano);
                printf("%s\n", agenda[i].cidade);
                printf("%s\n", agenda[i].uf);
                printf("Samba: %.2f\n", agenda[i].preferencias.samba);
                printf("Rock: %.2f\n", agenda[i].preferencias.rock);
                printf("MPB: %.2f\n", agenda[i].preferencias.mpb);
                printf("Grupo: %d\n", agenda[i].grupo);
            }
        }
        break;
    case 3:
        // Buscando registros por grupo

        // Lendo o grupo desejado
        printf("Grupo: ");
        scanf("%d", &aux);

        // Percorrendo todos os registros
        for(int i = 0; i < cont; i++)
        {
            // Se o registro pertencer ao grupo, o imprimimos
            if(aux == agenda[i].grupo && agenda[i].grupo != -1)
            {
                printf("%s\n", agenda[i].nome_completo);
                printf("%d/%d/%d\n", agenda[i].nascimento.dia, 
                                    agenda[i].nascimento.mes, 
                                    agenda[i].nascimento.ano);
                printf("%s\n", agenda[i].cidade);
                printf("%s\n", agenda[i].uf);
                printf("Samba: %.2f\n", agenda[i].preferencias.samba);
                printf("Rock: %.2f\n", agenda[i].preferencias.rock);
                printf("MPB: %.2f\n", agenda[i].preferencias.mpb);
                printf("Grupo: %d\n", agenda[i].grupo);
            }
        }
        break;
    default:
        break;
    }
}

pessoa_t *novo_registro(pessoa_t *agenda, int cont)
{
    // Aloca espaco para mais um registro
    agenda = realloc(agenda, cont*sizeof(pessoa_t));
    assert(agenda);

    // Vamos editar o registro na ultima posicao, 
    // criando o mesmo efeito que criar um novo registro
    editar_registro(agenda, (cont-1));

    return agenda;
}

pessoa_t *deletar_registro(pessoa_t *agenda)
{
    // Para deletar um registros, vamos marca-lo como excluido
    // Arquivos excluidos terao o grupo = -1, e sempre que se imprime um registro, 
    // eh testado testa se ele nao foi excluido (se seu grupo eh != -1)

    // // Lendo a posicao do registro que sera exluido
    int i;
    printf("Registro desejado: ");
    scanf("%d", &i);

    // Marcando o grupo do registro como -1
    agenda[i].grupo = -1;

    return agenda;
}

pessoa_t *editar_registro(pessoa_t *agenda, int i)
{
    // Declaracao das variaveis
    char str[MAX_STRING];
    int aux;
    float auxf;

    // Serao pedidos todos os atributos do registro, que serao escritos por cima dos atributos antigos

    printf("Nome: ");
    scanf("%s", str);
    strcpy(agenda[i].nome_completo, str);

    printf("Dia de nascimento: ");
    scanf("%d", &aux);
    agenda[i].nascimento.dia = aux;

    printf("Mes de nascimento: ");
    scanf("%d", &aux);
    agenda[i].nascimento.mes = aux;

    printf("Ano de nascimento: ");
    scanf("%d", &aux);
    agenda[i].nascimento.ano = aux;

    printf("Cidade: ");
    scanf("%s", str);
    strcpy(agenda[i].cidade, str);

    printf("UF: ");
    scanf("%s", str);
    strcpy(agenda[i].uf, str);

    printf("Preferências\n");
    printf("Samba (0-1): ");
    scanf("%f", &auxf);
    agenda[i].preferencias.samba = auxf;
    printf("Rock (0-1): ");
    scanf("%f", &auxf);
    agenda[i].preferencias.rock = auxf;
    printf("MPB (0-1): ");
    scanf("%f", &auxf);
    agenda[i].preferencias.mpb = auxf;

    return agenda;
}

FILE *abrir_arquivo()
{
    // Declaracao das variaveis
    FILE *arquivo;
    char nome_arquivo[MAX_STRING];

    // Lendo o nome do arquivo
    printf("Digite o nome do arquivo: ");
    scanf("%s", nome_arquivo);

    // Abrindo o arquivo
    arquivo = fopen(nome_arquivo, "rb");
    assert(arquivo);

    return arquivo;
}

FILE *abrir_arquivo_saida()
{
    // Declaracao das variaveis
    FILE *arquivo;
    char nome_arquivo[MAX_STRING];

    // Abrindo o arquivo de texto
    printf("Digite o nome do arquivo: ");
    scanf("%s", nome_arquivo);

    // Abrindo o arquivo
    arquivo = fopen(nome_arquivo, "wb");
    assert(arquivo);

    return arquivo;
}

pessoa_t *importar_registros(pessoa_t *agenda, FILE *arquivo, int *cont)
{
    // Inicializando o contador como 0
    *cont = 0;

    // Determinando o tamanho do arquivo
    agenda = calloc(1, sizeof(pessoa_t));
    assert(agenda);
    while (fread(agenda, sizeof(pessoa_t), 1, arquivo) != 0)
    {
        (*cont)++;
    }
    printf("Numero de registros: %d\n", *cont);

    // Realozando memoria para o vetor
    agenda = realloc(agenda, (*cont)*sizeof(pessoa_t));
    assert(agenda);
    // Colocando o leitor do arquivo no comeco
    fseek(arquivo, 0, SEEK_SET);
    // Colocando o arquivo no vetor
    fread(agenda, sizeof(pessoa_t), (*cont), arquivo);

    return agenda;
}