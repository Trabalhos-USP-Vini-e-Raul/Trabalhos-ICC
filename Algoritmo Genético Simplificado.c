#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define LIMITE 40

int main()
{
    // Declaracao das variaveis
    int tamanhoPopulacao;
    int maximoGeracao;
    int a,b,c,d,e,f;
    int melhorIndividuo; 
    int resultadoMelhorInd;
    int taxaMutacao;
    float distanciaAlvo;
    float porcentSelecionados;

    // Declaracao dos vetores
    int populacaoAtual[tamanhoPopulacao];

    // Criando uma seed para gerar numeros aleatorios
    srand(time(NULL));

    // Inicialização das variáveis
    melhorIndividuo = 2147483647;
    resultadoMelhorInd = 2147483647;
    distanciaAlvo = 0;
    porcentSelecionados = 0.25;

    // Lendo as variaveis
    printf("Digite o tamanho da população: ");
    scanf("%d", &tamanhoPopulacao);
    printf("Digite a taxa de mutação (0 a 100): ");
    scanf("%d", &taxaMutacao);
    printf("Digite o máximo de gerações: ");
    scanf("%d", &maximoGeracao);
    printf("Digite o valor de A: ");
    scanf("%d", &a);
    printf("Digite o valor de B: ");
    scanf("%d", &b);
    printf("Digite o valor de C: ");
    scanf("%d", &c);
    printf("Digite o valor de D: ");
    scanf("%d", &d);
    printf("Digite o valor de E: ");
    scanf("%d", &e);
    printf("Digite o valor de F: ");
    scanf("%d", &f);

    // Declarando vetores que precisam dos valores das variaveis
    int resultado[tamanhoPopulacao];
    int individuosSelecionados[(int)(tamanhoPopulacao*porcentSelecionados)]; // Vetor dos 25% selecionados
    int populacaoAtualBinario[tamanhoPopulacao];

    // Gerar aleatoriamente a população inicial
    for(int i = 0; i < tamanhoPopulacao; i++)
    {
        populacaoAtual[i] = (rand() % LIMITE) - LIMITE/2; // Subtraindo limite/2 para incluir negativos
    }

    // Laço das gerações
    for(int geracao = 0; geracao < maximoGeracao; geracao++)
    {
        // Fase 1
        // Avaliar população para o seu problema
        for(int j = 0; j < tamanhoPopulacao; j++)
        {
            // Calculando a distância do resultado até 0
            resultado[j] = abs(a*pow(populacaoAtual[j],5) 
            + b*pow(populacaoAtual[j],4) + c*pow(populacaoAtual[j],3) 
            + d*pow(populacaoAtual[j],2) + e*populacaoAtual[j] + f);
        }

        // Ordenando o vetor resultado e o vetor populacao
        for(int l = 0; l < tamanhoPopulacao; l++)
        {
            // Declarando variaveis temporarias para trocar os valores
            int temp;
            int tempPop;
            for(int k = 0; k < tamanhoPopulacao-1; k++)
            {
                if(resultado[k] > resultado[k+1])
                {
                    temp = resultado[k];
                    resultado[k] = resultado[k+1];
                    resultado[k+1] = temp;

                    // Ordenando para que populacaoAtual[k] corresponda a resultado[k] 
                    tempPop = populacaoAtual[k];
                    populacaoAtual[k] = populacaoAtual[k+1];
                    populacaoAtual[k+1] = tempPop;
                }
            }
        }

        // Fase 2
        // Armazenar o melhor indivíduo até o momento
        // Precisamos apenas comparar o resultado[0], pois o vetor esta ordenado por distancia crescente
        if(abs(resultado[0]) < resultadoMelhorInd)
        {
            melhorIndividuo = populacaoAtual[0];
            resultadoMelhorInd = resultado[0];
        }

        // Fase 3
        // Verificar se a solução está aceitável (se sim, encerrar o laço)
        if(abs(resultadoMelhorInd) <= distanciaAlvo)
        {
            printf("Resultado encontrado na geracao %d\n", (geracao + 1));
            break;
        }

        // Fase 4
        // Selecionar os melhores individuos
        for(int i = 0; i < (int)(tamanhoPopulacao*porcentSelecionados);)
        {
            int roleta = rand()%100; // criamos uma "roleta", que vai decidir se o individuo[i] é selecionado 
            if(roleta < (99 - ((float) i / (tamanhoPopulacao-1))*98)) 
            {
                individuosSelecionados[i] = populacaoAtual[i];  
                i++;
            }
        }

        // Fase 5 
        /// Cruzamento
        // i foi declarado com esse valor para que os individuos selecionados nao sejam alterados
        for (int i = (int)(tamanhoPopulacao*porcentSelecionados)+1; i<tamanhoPopulacao; i++)
        {
            //Criando as mascaras
            int mascaraA = -1431655766; // 10101010101010101010101010101010 em binario
            int mascaraB = 1431655765;  // 01010101010101010101010101010101 em binario

            // Vamos atribuir a mascaraA ao individuo i e a mascara B para um individuo aleatorio 
            int genesA = populacaoAtual[i] & mascaraA;
            int genesB = populacaoAtual[rand()%(tamanhoPopulacao-1)] & mascaraB;

            // Por fim, finalizamos crossover
            populacaoAtual[i] = (genesA + genesB) % LIMITE;
        }   

        // Fase 6 
        // Mutacao
        // i foi declarado com esse valor para que os individuos selecionados nao sejam alterados
        for (int i = (tamanhoPopulacao*porcentSelecionados)+1; i<tamanhoPopulacao; i++) // Percorrendo os individuos nao selecionados
        {
            for (int j = 0; j<sizeof(int)*8; j++) // Percorrendo cada bit do inteiro
            {
                if (rand() % 100 < taxaMutacao) // Vendo se o bit vai ser invertido
                {
                    // Vamos inverter o bit utilizando a operacao XOR pois caso uma das entradas do xor seja 1, o resultado é o inverso da outra entrada
                    // Precisamos apenas gerar uma potencia de 2, que em binario possui um único "1", para que assim apenas um bit seja invertido
                    // Esse bit vai ser o bit "j", e podemos gerar sua forma em decimal por 2^j
                    populacaoAtual[i] = populacaoAtual[i] ^ (int)pow(2, j);
                    populacaoAtual[i] %= LIMITE; 
                }
            }
        }
    }

    // Imprimindo a melhor solucao
    printf("Melhor Individuo: %d\n", melhorIndividuo);
    printf("Resultado: %d\n", resultadoMelhorInd);
    
    return 0;
}
