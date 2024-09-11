// ------------------------------------------- Bibliotecas -------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>

// ------------------------------------------- Structs -------------------------------------------

typedef struct Professor{
    int id;
    char nome[20];
    char email[50];
    char senha[50];
}__attribute__((packed)) Professor;

typedef struct Turma{
    int idTurma;
    int idProf; // Chave para professor
    char nomeTurma[50];
    char nomeDisciplina[50];
    int quantidadeDeAulas;
    int quantidadeDeAulasConlcuidas;

}__attribute__((packed)) Turma;

typedef struct Disciplina{
    char nomeDisciplina[50];
    char nomeTurma[50];
    float notas[4];
    float media;
    float frequencia;
    int faltas;
}__attribute__((packed)) Disciplina;

typedef struct Aluno{
    int matricula;
    int idProf; // Chave para professor
    char nome[50];
    Disciplina disciplinas[6];
    int quantidadeDisciplinas;

}__attribute__((packed)) Aluno;

typedef struct Registro{
    int idProfessor;
    char nome[50];
    char data[11];
    struct Registro *proxNo;
}__attribute__((packed)) Registro;


// ------------------------------------------- Assinatura de funções -------------------------------------------


// As. Telas ---------
void menuPrincipal();
void telaInicial(Professor *);
void telaTurma(int);
void telaAluno(int);
void telaNotasFreq(int);
void telaBoletins(int);

// As. Funções Auxiliares ---------
int ultimoIdPlus1Professor();
int ultimoIdPlus1Turma();
int ultimoIdPlus1Aluno();
int strVazia(const char *);
int ehDigitos(int);
int ehCaracter(char *);
Professor* buscarProfessor(const char *, const char *);
Turma procurarTurma(int, char *, char *);
int procurarDisciplinaTurma(char *, char *, int);
void excluirTurmaAlunos(int, char *, char *);
void verAlunosTurmaStrParametro(int, char *, char *);
float mediaTurma(Turma*);
char* dataAtual();
int verificarEmail(char *);

// As. Funções Principais ---------
void criarProfessor();
void verProfessores();
void loginProfessor();
void criarTurma(int);
void editarTurma(int);
void excluirTurma(int);
void verTurmas(int);
void verAlunosTurma(int);
void criarAluno(int);
//tratament o feito daqui pra cima
void editarAluno(int);
void excluirAluno();
void verAlunos(int);
void verAlunoMatricula(int);
void verAlunoNome(int);
void lancarFreq(int);
void lancarNotas(int);
void boletimAluno(int);
void boletimTurma(int);

// As. Funções Principais (Lista Encadeada) ---------
Registro* criarRegistro(int, char *, char *);
void adicionarListaRegistro(Registro **, int, char *, char *);
void criarRegistroTxt(Registro *);
Registro* receberLista();
void liberarMemLista(Registro *);
void verLista(Registro *, int);


// ------------------------------------------- FUNÇÕES -------------------------------------------


// Funcao que identifica o maior id e o retorna + 1
int ultimoIdPlus1Professor(){
    FILE *arquivoPtr;
    Professor professor;
    int maiorId = 1;

    arquivoPtr = fopen("professor.txt", "rb");
    if(arquivoPtr == NULL){
        printf("Nao foi possivel acessar o professor");

    }else{
        while(fread(&professor, sizeof(Professor), 1, arquivoPtr) == 1){
            if(professor.id > maiorId){
                maiorId = professor.id;
            }
        }
        maiorId++;
    }
    fclose(arquivoPtr);
    return maiorId;
}

// Funcao que identifica o maior id e o retorna + 1
int ultimoIdPlus1Turma(){
    FILE *arquivoPtr;
    Turma turma;
    int maiorId = 1;

    arquivoPtr = fopen("turma.txt", "rb");
    if(arquivoPtr == NULL){
        printf("Nao foi possivel acessar a turma");

    }else{
        while(fread(&turma, sizeof(Turma), 1, arquivoPtr) == 1){
            if(turma.idTurma > maiorId){
                maiorId = turma.idTurma;
            }
        }
        maiorId++;
    }
    fclose(arquivoPtr);
    return maiorId;
}

// Funcao que identifica o maior id e o retorna + 1
int ultimoIdPlus1Aluno(){
    FILE *arquivoPtr;
    Aluno aluno;
    int maiorId = 2024001;

    arquivoPtr = fopen("aluno.txt", "rb");
    if(arquivoPtr == NULL){
        printf("Nao foi possivel acessar o aluno");

    }else{
        while(fread(&aluno, sizeof(Aluno), 1, arquivoPtr) == 1){
            if(aluno.matricula > maiorId){
                maiorId = aluno.matricula;
            }
        }
        maiorId++;
    }
    fclose(arquivoPtr);
    return maiorId;
}

// Função que verifica se a string é vazia
int strVazia(const char *str) {
    return str == NULL || *str == '\0';
}

// Função que procura uma turma a partir dos nomes e a retorna
Turma procurarTurma(int idProfessor, char *nomeTurma, char *nomeDisciplina){
    FILE *arquivoPtr;
    Turma turma;
    Turma turmaNaoEncontrada = {-1, -1, "", "", -1, -1};

    arquivoPtr = fopen("turma.txt", "rb");
    if(arquivoPtr == NULL){
        printf("\nErro ao acessar o arquivo turma.txt");
        return turmaNaoEncontrada;
    }else{
        while(fread(&turma, sizeof(Turma), 1, arquivoPtr) == 1){
            if(strcmp(turma.nomeTurma, nomeTurma) == 0 && strcmp(turma.nomeDisciplina, nomeDisciplina) == 0 && turma.idProf == idProfessor){
                fclose(arquivoPtr);
                return turma;
            }
        }

        fclose(arquivoPtr);
        return turmaNaoEncontrada;
    }
}

// Função para cadastro de professores no arquivo professor.txt
void criarProfessor(){
    FILE *arquivoPtr;
    Professor professor;
    char inputBuffer[100]; // Utilizado na Leitura do teclado (forma segura e eficiente)
    
    arquivoPtr = fopen("professor.txt", "ab"); // Criando o arquivo que irá armazenar os tipos de dados professor / tipo ab, binário que aloca novos dados no fim do arquivo
    if(arquivoPtr == NULL){ // verificando se foi possivel abrir o arquivo
        printf("\nErro ao acessar o arquivo\n");

        printf("\nPressione qualquer tecla para continuar...");
        getch();
        return ;
    }else{
        // Verificando se é o primeiro registro de professor
        fseek(arquivoPtr, 0, SEEK_END); // Posiciona no final do arquivo
        long tamanho = ftell(arquivoPtr);

        memset(&professor, 0, sizeof(professor)); // garantindo que nao seja lido dados indesejaveis 

        // A partir da verificacao do tamanho do arquivo pelo ponteiro, é atribuido o id do professor
        if(tamanho == 0){
            professor.id = 1;
        }else{
            professor.id = ultimoIdPlus1Professor();
        }

        while(1){
            printf("\nDigite o nome: ");
            fgets(inputBuffer, sizeof(inputBuffer), stdin); 
            sscanf(inputBuffer, "%19[^\n]", professor.nome);  // Ler até a quebra de linha
            
            if(ehCaracter(professor.nome) == 0){
                printf("Informe um nome valido para o sistema.\n");
            }
            else{
                break;
            }
        }
        
        while(1){
            printf("\nDigite o email: ");
            fgets(inputBuffer, sizeof(inputBuffer), stdin); 
            sscanf(inputBuffer, "%49[^\n]", professor.email); 

            if(strVazia(professor.email) == 1){
                printf("Informe um email valido para o sistema.\n");
            }
            else{
                if(verificarEmail(professor.email) == 0){
                    printf("O email informado ja esta sendo utilizado no sistema, informe outro.\n");
                }
                else{
                    break;
                }
            }
        }

        while(1){
            printf("\nDigite a senha: ");
            fgets(inputBuffer, sizeof(inputBuffer), stdin); 
            sscanf(inputBuffer, "%49[^\n]", professor.senha); 

            if (!strVazia(professor.senha)) break;
            printf("\nEntrada invalida! Por favor tente novamente");
        }
        
        fwrite(&professor, sizeof(Professor), 1, arquivoPtr); // alocando todos os dados lidos em 1 linha do arquivo
        
        fclose(arquivoPtr); // fechando o arquivo posteriormente a toda a leitura

        printf("\nDados inseridos com sucesso!\n");

        printf("\nPressione qualquer tecla para continuar...");
        getch();
    }
}

// Função para vizualizar os professores criados e armazenados no arquivo professor.txt
void verProfessores(){
    FILE *arquivoPtr;
    Professor professor;
    
    arquivoPtr = fopen("professor.txt", "rb"); // Reabrir arquivo para leitura
    if(arquivoPtr == NULL){
        printf("\nErro ao acessar o arquivo\n");

        printf("\nPressione qualquer tecla para continuar...");
        getch();
        return ;
    } else {
        printf("Dados do arquivo:\n");
        while(fread(&professor, sizeof(Professor), 1, arquivoPtr) == 1){
            printf("\n---------------------------\n");
            printf("ID: %d\n", professor.id);
            printf("Nome: %s\n", professor.nome);
            printf("Email: %s\n", professor.email);
            printf("Senha: %s\n", professor.senha);
        
        }
        fclose(arquivoPtr);

        printf("\nPressione qualquer tecla para continuar...");
        getch();
    }
}

// Funcao busca e retorna um professor por um nome || LOGIN
Professor* buscarProfessor(const char *emailLog, const char *senhaLog){
    FILE *arquivoPtr;
    static Professor professor;
    
    arquivoPtr = fopen("professor.txt", "rb"); // Reabrir arquivo para leitura
    if(arquivoPtr == NULL){
        printf("\nErro ao acessar o arquivo\n");

        printf("\nPressione qualquer tecla para continuar...");
        getch();
        return NULL;
    } else {
        while(fread(&professor, sizeof(Professor), 1, arquivoPtr) == 1){ // percorrendo o arquivo
            if(strcmp(emailLog, professor.email) == 0 && strcmp(senhaLog, professor.senha) == 0){ // caso exista uma linha de arquivo (um professor) com o mesmo login e senha digitados pelo usuario é retornado o objeto professor encontrado
                fclose(arquivoPtr);
                return &professor;
            }
        }
        fclose(arquivoPtr); // caso nao encontre
        return NULL;
    }
}

// Função login do professor
void loginProfessor(){
    char inputBuffer[100];
    char emailLogin[50];
    char senhaLogin[50];

    // Lendo os campos que serão analisados no login
    printf("\n---------------------------------------------------------\n");
    while(1){
         printf("\nDigite o email: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin); 
        sscanf(inputBuffer, "%49[^\n]", emailLogin);

        if (!strVazia(emailLogin)) break;
        printf("\nEntrada invalida! Por favor tente novamente");
    }
   
   while(1){
        printf("\nDigite a senha: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin); 
        sscanf(inputBuffer, "%49[^\n]", senhaLogin);

        if (!strVazia(senhaLogin)) break;
        printf("\nEntrada invalida! Por favor tente novamente");
    }
    
    // Caso o professor exista e os campos esteja corretamente preenchidos será retornado o professor logado, caso não, NULL
    Professor* professorLogin = buscarProfessor(emailLogin, senhaLogin);

    if(professorLogin == NULL){
        printf("\nFalha ao logar");
        printf("\n\nPressione qualquer tecla para continuar...");
        getch();
    }else{
        printf("Login efetuado com sucesso\n");
        telaInicial(professorLogin);
    }
}

// Função para cadastro de turmas no arquivo turma.txt
void criarTurma(int idProfessor){
    FILE *arquivoPtr;
    Turma turma;
    Turma turmaExistente;
    char inputBuffer[256]; // Utilizado na Leitura do teclado (forma segura e eficiente)
    
    arquivoPtr = fopen("turma.txt", "ab"); // Criando o arquivo que irá armazenar os tipos de dados turma / tipo ab, binário que aloca novos dados no fim do arquivo
    if(arquivoPtr == NULL){ // verificando se foi possivel abrir o arquivo
        printf("\nErro ao acessar o arquivo\n");
        printf("\nPressione qualquer tecla para continuar...");
        getch();
        return ;
    }else{
        // Verificando se é o primeiro registro de professor
        fseek(arquivoPtr, 0, SEEK_END); // Posiciona no final do arquivo
        long tamanho = ftell(arquivoPtr); // estdr
        
        memset(&turma, 0, sizeof(turma)); // garantindo que nao seja lido dados indesejaveis 
        
        if(tamanho == 0){
            turma.idTurma = 1;
        }else{
            turma.idTurma = ultimoIdPlus1Turma();
        }

        turma.idProf = idProfessor;

        while(1){
            printf("\nDigite o nome da turma: ");
            fgets(inputBuffer, sizeof(inputBuffer), stdin); 
            sscanf(inputBuffer, "%49[^\n]", turma.nomeTurma);  // Ler até a quebra de linha

            if (!strVazia(turma.nomeTurma)) break;
            printf("\nEntrada invalida! Por favor tente novamente");
        }

        while(1){
            printf("\nDigite o nome da disciplina: ");
            fgets(inputBuffer, sizeof(inputBuffer), stdin); 
            sscanf(inputBuffer, "%49[^\n]", turma.nomeDisciplina);

            if (!strVazia(turma.nomeDisciplina)) break;
            printf("\nEntrada invalida! Por favor tente novamente");
        }

        // Verificando se ja existe a turma cadastrada no sistema
        turmaExistente = procurarTurma(idProfessor, turma.nomeTurma, turma.nomeDisciplina);
        if(turmaExistente.idProf != -1){
            printf("\nTurma ja cadastrada !!\n");

            fclose(arquivoPtr);
            printf("\nPressione qualquer tecla para continuar...");
            getch();
            return ;
        }

        while(1){
            printf("\nDigite a quantidade de aulas totais: ");
            fgets(inputBuffer, sizeof(inputBuffer), stdin); 
            sscanf(inputBuffer, "%d", &turma.quantidadeDeAulas);

            if(turma.quantidadeDeAulas > 0 && turma.quantidadeDeAulas <= 60) break;
            printf("\nEntrada invalida! Por favor tente novamente");
        }

        turma.quantidadeDeAulasConlcuidas = 0;

        fwrite(&turma, sizeof(Turma), 1, arquivoPtr); // alocando todos os dados lidos em 1 linha do arquivo
        
        fclose(arquivoPtr); // fechando o arquivo posteriormente a toda a leitura
        printf("\nDados inseridos com sucesso!\n");
        
        printf("\nPressione qualquer tecla para continuar...");
        getch();
    }
}

// Função para edição de dados de turmas no arquivo turma.txt
void editarTurma(int idProfessor){
    FILE *arquivoPtr;
    Turma turma;
    char nomeTurmaProcurada[50];
    char nomeDisciplinaProcurada[50];
    char inputBuffer[256];
    
    arquivoPtr = fopen("turma.txt", "r+b"); // Reabrir arquivo para leitura
    if(arquivoPtr == NULL){
        printf("\nErro ao acessar o arquivo turma.txt\n");
        printf("\nPressione qualquer tecla para continuar...");
        getch();
        return ;
    }else{
        int encontrouTurma = 0;

        while(1){
            printf("\nDigite o nome da turma: ");
            fgets(inputBuffer, sizeof(inputBuffer), stdin); 
            sscanf(inputBuffer, "%49[^\n]", nomeTurmaProcurada);

            if(strVazia(nomeTurmaProcurada) == 1){
                printf("\nEntrada invalida! Por favor tente novamente");
            }
            else{
                fflush(stdin);
                break;
            }
        }

        while(1){
            printf("\nDigite o nome da disciplina: ");
            fgets(inputBuffer, sizeof(inputBuffer), stdin); 

            if(sscanf(inputBuffer, "%49[^\n]", nomeDisciplinaProcurada)==0){
                printf("\nEntrada invalida! Por favor tente novamente");
            }
            else{
                break;
            }

        }

        // rewind(arquivoPtr); // Reposiciona o ponteiro no inicio do arquivo, isso para ele conseguir ler caso o usuario procure outro aluno
        printf("\nDados do arquivo:\n\n");
        while(fread(&turma, sizeof(Turma), 1, arquivoPtr) == 1){
            if(strcmp(turma.nomeTurma, nomeTurmaProcurada) == 0 && strcmp(turma.nomeDisciplina, nomeDisciplinaProcurada) == 0 && turma.idProf == idProfessor){
                encontrouTurma = 1;

                printf("---------------------------\n");
                printf("Turma: %s\n\n", turma.nomeTurma);

                while(1){
                    printf("\nDigite o novo nome da turma: ");
                    fgets(inputBuffer, sizeof(inputBuffer), stdin);

                    if(sscanf(inputBuffer, "%49[^\n]", turma.nomeTurma)==0){
                        printf("\nEntrada invalida! Por favor tente novamente");
                    }
                    else{
                        break;
                    }

                }

                while(1){
                    printf("\nDigite o novo nome da disciplina: ");
                    fgets(inputBuffer, sizeof(inputBuffer), stdin);

                    if(sscanf(inputBuffer, "%49[^\n]", turma.nomeDisciplina)==0){
                        printf("\nEntrada invalida! Por favor tente novamente");
                    }
                    else{
                        break;
                    }

                }

                while(1){
                    printf("\nDigite a nova quantidade de aulas totais: ");
                    fgets(inputBuffer, sizeof(inputBuffer), stdin);

                    if(sscanf(inputBuffer, "%d", &turma.quantidadeDeAulas)==0){
                         printf("\nEntrada invalida! Por favor tente novamente -");
                    }
                    else{
                        if(turma.quantidadeDeAulas > 0 && turma.quantidadeDeAulas <= 60 && turma.quantidadeDeAulas >= turma.quantidadeDeAulasConlcuidas){
                            break;
                        }
                        else{
                            printf("Quantidade de aulas invalido, informe um valor valido.\n");
                        }
                    }
                }
                
                // Posiciona o ponteiro do arquivo para o início do registro da turma
                fseek(arquivoPtr, -sizeof(Turma), SEEK_CUR);

                fwrite(&turma, sizeof(Turma), 1, arquivoPtr);

                fflush(arquivoPtr);

                printf("\nDados inseridos com sucesso!");
            }
        }

        if(!encontrouTurma){
            printf("\nNenhuma turma encontrada!\n");
        }

        fclose(arquivoPtr);

        printf("\n\nPressione qualquer tecla para continuar...");
        getch();
    }
}

// Função que auxilia a exclusão de turmas, garantindo a exclusão das turmas nas listas dos alunos que a tivera cadastrada
void excluirTurmaAlunos(int idProfessor, char *nomeTurmaProcurada, char *nomeDisciplinaProcurada){
    FILE *arquivoPtr;
    Aluno aluno;
    int encontrouAluno = 0;

    arquivoPtr = fopen("aluno.txt", "r+b");
    if(arquivoPtr == NULL){
        printf("\nErro ao abrir o arquivo de alunos");
        printf("\nPressione qualquer tecla para continuar...");
        getch();
        return;
    }else{
        while(fread(&aluno, sizeof(Aluno), 1, arquivoPtr) == 1){ 
            for(int i = 0; i < aluno.quantidadeDisciplinas; i++){
                if(strcmp(aluno.disciplinas[i].nomeTurma, nomeTurmaProcurada) == 0 && strcmp(aluno.disciplinas[i].nomeDisciplina, nomeDisciplinaProcurada) == 0 && aluno.idProf == idProfessor){
                    encontrouAluno = 1;
                    for(int j = i; j < aluno.quantidadeDisciplinas-1; j++){
                        aluno.disciplinas[j] = aluno.disciplinas[j+1];
                    }
                    aluno.quantidadeDisciplinas--;
                    // memset(&aluno.disciplinas[i], 0, sizeof(aluno.disciplinas[i]));
                    
                    fseek(arquivoPtr, -sizeof(Aluno), SEEK_CUR);

                    fwrite(&aluno, sizeof(Aluno), 1, arquivoPtr);

                    fflush(arquivoPtr);
                    break;
    
                }
            }
        } 
        if(!encontrouAluno){
            printf("\nNenhum aluno com a turma cadastrada encontrado");
        }
        fclose(arquivoPtr);
    }

}

// Função para exclusao de turma por nome da turma e disciplina
void excluirTurma(int idProf){
    FILE *arquivoPtr;
    FILE *arquivoTempPtr;
    Turma turma;
    int encontrouTurma = 0;
    char nomeTurmaProcurada[50];
    char nomeDisciplinaProcurada[50];
    char inputBuffer[100];

    arquivoPtr = fopen("turma.txt", "rb");
    if(arquivoPtr == NULL){
        printf("\nErro ao abrir o arquivo de turmas");
        printf("\nPressione qualquer tecla para continuar...");
        getch();
        return;
    }else{
        arquivoTempPtr = fopen("temp.txt", "wb");
        if(arquivoTempPtr == NULL){
            printf("\nErro ao criar o arquivo temporario");
            printf("\nPressione qualquer tecla para continuar...");
            getch();
            fclose(arquivoPtr);
            return;
        }else{

            // Recebendo a matricula do aluno que será excluído
            while(1){
                printf("\nDigite o nome da turma: ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                if(sscanf(inputBuffer, "%49[^\n]", nomeTurmaProcurada)==0){  // Ler até a quebra de linha
                    printf("\nEntrada invalida! Por favor tente novamente");
                }
                else{
                    break;
                }
            }

            while(1){
                printf("\nDigite o nome da disciplina: ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                if(sscanf(inputBuffer, "%49[^\n]", nomeDisciplinaProcurada)==0){
                    printf("\nEntrada invalida! Por favor tente novamente");
                }
                else{
                    break;
                }
            }
            
            // Percorrendo o arquivo aluno.txt original passando todos os alunos, exceto o que possui a matricula escolhida, para o novo txt
            while(fread(&turma, sizeof(Turma), 1, arquivoPtr) == 1){
                if(strcmp(turma.nomeTurma, nomeTurmaProcurada) == 0 && strcmp(turma.nomeDisciplina, nomeDisciplinaProcurada) == 0 && turma.idProf == idProf){
                    encontrouTurma = 1;
                }else{
                    fwrite(&turma, sizeof(Turma), 1, arquivoTempPtr);
                }
            } 

            // Fechando arquivos
            fclose(arquivoPtr);
            fclose(arquivoTempPtr);

            // Excluindo o antigo txt (incluindo o aluno escolhido) e renomeando o novo txt (sem o aluno escolhido) para aluno.txt
            remove("turma.txt");
            rename("temp.txt", "turma.txt");

            if(!encontrouTurma){
                printf("\nTurma não encontrada para exclusão");
            }else{
                excluirTurmaAlunos(idProf, nomeTurmaProcurada, nomeDisciplinaProcurada);
                printf("\nTurma excluida com sucesso");
            }

            printf("\nPressione qualquer tecla para continuar...");
            getch();
        }
    }
}

// Função para vizualizar todas as turmas do professor
void verTurmas(int idProfessor){
    FILE *arquivoPtr;
    Turma turma;
    
    arquivoPtr = fopen("turma.txt", "rb"); // Reabrir arquivo para leitura
    if(arquivoPtr == NULL){
        printf("\nErro ao acessar o arquivo\n");

        printf("\nPressione qualquer tecla para continuar...");
        getch();
        return ;
    } else {
        printf("Dados do arquivo:\n");
        while(fread(&turma, sizeof(Turma), 1, arquivoPtr) == 1){
            if(turma.idProf == idProfessor){
                printf("\n---------------------------\n");
                printf("ID: %d\n", turma.idTurma);
                printf("Nome: %s\n", turma.nomeTurma);
                printf("Disciplina: %s\n", turma.nomeDisciplina);
                printf("Aulas totais: %d\n", turma.quantidadeDeAulas);
                printf("Aulas ministradas: %d\n", turma.quantidadeDeAulasConlcuidas);
            }
        }
        
        fclose(arquivoPtr);

        printf("\nPressione qualquer tecla para continuar...");
        getch();
    }
}

// Função para vizualizar todos os alunos de uma turma
void verAlunosTurma(int idProfessor){
    FILE *arquivoAlunosPtr;
    Aluno aluno;
    Turma turma;
    char turmaProcurada[50], disciplinaProcurada[50];
    char inputBuffer[100];

    while(1){
        printf("\nDigite o nome da turma: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin); 
        sscanf(inputBuffer, "%49[^\n]", turmaProcurada);

        if(!strVazia(turmaProcurada)) break;
        printf("\nEntrada invalida! Por favor tente novamente");
    }

    while(1){
        printf("\nDigite o nome da disciplina: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin); 
        if(sscanf(inputBuffer, "%49[^\n]", disciplinaProcurada)==0){
            printf("\nEntrada invalida! Por favor tente novamente");
        }
        else{
            break;
        }
    }

    turma = procurarTurma(idProfessor, turmaProcurada, disciplinaProcurada);

    if(turma.idProf == -1){
        printf("\nTurma nao encontrada !!\n");
        printf("\nPressione qualquer tecla para continuar...");
        getch();

        return;
    }

    arquivoAlunosPtr = fopen("aluno.txt", "rb");
    if(arquivoAlunosPtr == NULL){
        printf("\nErro ao acessar o arquivo aluno.txt\n");

        printf("\nPressione qualquer tecla para continuar...");
        getch();
        return ;
    }else{
        int encontrouAluno = 0;
        
        printf("\nDados do arquivo:\n");

        while(fread(&aluno, sizeof(Aluno), 1, arquivoAlunosPtr) == 1){
            for(int k = 0; k < aluno.quantidadeDisciplinas; k++){
                if(strcmp(turma.nomeTurma, aluno.disciplinas[k].nomeTurma) == 0 && strcmp(turma.nomeDisciplina, aluno.disciplinas[k].nomeDisciplina) == 0 && aluno.idProf == idProfessor){
                    encontrouAluno = 1;
                    printf("\n---------------------------\n");
                    printf("Matricula: %d\n", aluno.matricula);
                    printf("Nome: %s\n", aluno.nome);
                    printf("Disciplina: %s\n", aluno.disciplinas[k].nomeDisciplina);
                    printf("Turma: %s\n", aluno.disciplinas[k].nomeTurma);
                    
                    for(int j = 0; j < 4; j++){
                        printf("Nota %d: %.2f\n", j+1, aluno.disciplinas[k].notas[j]);
                    }
                    printf("Media: %f\n", aluno.disciplinas[k].media);
                    printf("Frequencia: %f\n", aluno.disciplinas[k].frequencia);
                    
                    break;
                }
            }
        }

        if(encontrouAluno == 0){
            printf("\nNenhum aluno encontrado !!");
        }
        
        fclose(arquivoAlunosPtr);

        printf("\nPressione qualquer tecla para continuar...");
        getch();
    }
}

// retorna 0 se n encontrar, 1 se encontrar'
int procurarDisciplinaTurma(char *disciplinaProcurada, char *turmaProcurada, int idProfessor){
    FILE *arquivoPtr;
    Turma turma;
    
    arquivoPtr = fopen("turma.txt", "rb"); // Reabrir arquivo para leitura
    if(arquivoPtr == NULL){
        printf("\nErro ao acessar o arquivo\n");
        return 0;
    } else {
        while(fread(&turma, sizeof(Turma), 1, arquivoPtr) == 1){
            if(strcmp(disciplinaProcurada, turma.nomeDisciplina) == 0 && strcmp(turmaProcurada, turma.nomeTurma) == 0 && turma.idProf == idProfessor){
                fclose(arquivoPtr);
                return 1;
            }
        }
        fclose(arquivoPtr);
        return 0;
    }

}

// Função para cadastro de alunos no arquivo aluno.txt
void criarAluno(int idProfessor){
    FILE *arquivoPtr;
    Aluno aluno;
    char inputBuffer[100]; 
    
    arquivoPtr = fopen("aluno.txt", "ab"); // Criando o arquivo que irá armazenar os tipos de dados aluno / tipo ab, binário que aloca novos dados no fim do arquivo
    if(arquivoPtr == NULL){ 
        printf("\nErro ao acessar o arquivo\n");

        printf("\nPressione qualquer tecla para continuar...");
        getch();
        return ;
    }else{
        fseek(arquivoPtr, 0, SEEK_END); 
        long tamanho = ftell(arquivoPtr);
        
        memset(&aluno, 0, sizeof(aluno)); 
    
        if(tamanho == 0){
            aluno.matricula = 2024001;
        }else{
            aluno.matricula = ultimoIdPlus1Aluno();
        }

        aluno.idProf = idProfessor;

        while(1){
            printf("\nDigite o nome do aluno: ");
            fgets(inputBuffer, sizeof(inputBuffer), stdin); 
            sscanf(inputBuffer, "%49[^\n]", aluno.nome);

            if(ehCaracter(aluno.nome) == 0){
                printf("\nEntrada invalida! Por favor tente novamente");
            }
            else if(strVazia(aluno.nome) == 1){
                printf("\nEntrada invalida! Por favor tente novamente\n");
            }
            else{
                break;
            }
        }

        int i = 0;
        char opcaoDisciplina;
        aluno.quantidadeDisciplinas = 0;
        while(aluno.quantidadeDisciplinas < 6){ // lendo as disciplinas
            opcaoDisciplina = 's';

            memset(&aluno.disciplinas[i], 0, sizeof(aluno.disciplinas[i]));

            while(1){
                printf("\nDigite o nome da turma: ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                sscanf(inputBuffer, "%49[^\n]", aluno.disciplinas[i].nomeTurma); 

                if(!strVazia(aluno.disciplinas[i].nomeTurma)) break;
                printf("\nEntrada invalida! Por favor tente novamente\n");
            }

            while(1){
                printf("\nDigite o nome da disciplina: ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                sscanf(inputBuffer, "%49[^\n]", aluno.disciplinas[i].nomeDisciplina);

                if(!strVazia(aluno.disciplinas[i].nomeDisciplina)) break;
                printf("\nEntrada invalida! Por favor tente novamente\n");
            }
            
            // Função que procura disciplina no arquivo txt
            int chamadaFuncaoVerificacao = procurarDisciplinaTurma(aluno.disciplinas[i].nomeDisciplina, aluno.disciplinas[i].nomeTurma, aluno.idProf);
            
            if(chamadaFuncaoVerificacao == 0){ // Verificando se existe a disciplina digitada
                printf("\nErro ao encontrar a disciplina! ");
                
                printf("\nDeseja cadastrar uma nova disciplina? (S para continuar)");
                fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                sscanf(inputBuffer, " %c", &opcaoDisciplina);

                if(opcaoDisciplina == 's' || opcaoDisciplina == 'S'){
                    continue;
                }else{
                    memset(&aluno.disciplinas[i], 0, sizeof(aluno.disciplinas[i]));
                    break;
                }
               

            }else if(chamadaFuncaoVerificacao == 1){ 

                // Verificando se a disciplina ja foi inserida no aluno
                int verificadorDisciplinaCadastrada = 0;
                // Percorrendo a lista de disciplinas atual
                for(int n = 0; n < i; n++){
                    if(strcmp(aluno.disciplinas[n].nomeDisciplina, aluno.disciplinas[i].nomeDisciplina) == 0 && strcmp(aluno.disciplinas[n].nomeTurma, aluno.disciplinas[i].nomeTurma) == 0){
                        verificadorDisciplinaCadastrada = 1;
                        break;
                    }
                }

                if(verificadorDisciplinaCadastrada == 1){
                    printf("\nDisciplina ja cadastrada!");

                    printf("\nDeseja cadastrar uma nova disciplina? (S para continuar)");
                    fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                    sscanf(inputBuffer, " %c", &opcaoDisciplina);

                    if(opcaoDisciplina == 's' || opcaoDisciplina == 'S'){
                        continue;
                    }else{
                        memset(&aluno.disciplinas[i], 0, sizeof(aluno.disciplinas[i]));
                        break;
                    }
                }

                // Caso a disciplia não tenha sido ainda cadastrada no aluno:
                aluno.quantidadeDisciplinas += 1;
                aluno.disciplinas[i].media = 0;
                aluno.disciplinas[i].notas[0] = 0;
                aluno.disciplinas[i].notas[1] = 0;
                aluno.disciplinas[i].notas[2] = 0;
                aluno.disciplinas[i].notas[3] = 0;
                aluno.disciplinas[i].frequencia = 100;
                aluno.disciplinas[i].faltas = 0;
                i++;

                printf("\nDeseja continuar cadastrando disciplinas? (S para continuar)");
                fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                sscanf(inputBuffer, " %c", &opcaoDisciplina);
                
                if(opcaoDisciplina == 's' || opcaoDisciplina == 'S'){
                    continue;
                }else{
                    break;
                }
            }
        }

        if(aluno.quantidadeDisciplinas >= 6){
            printf("\nMaximo de disciplinas cadastradas!\n");
        }

        fwrite(&aluno, sizeof(Aluno), 1, arquivoPtr); // alocando todos os dados lidos em 1 linha do arquivo~

        printf("\nMatricula Gerada: %d\n", aluno.matricula);
        
        fclose(arquivoPtr); // fechando o arquivo posteriormente a toda a leitura

        printf("\nDados inseridos com sucesso!\n");

        printf("\nPressione qualquer tecla para continuar...");
        getch();
    }
}

// Função para edição de dados de alunos no arquivo aluno.txt
void editarAluno(int idProfessor){
    FILE *arquivoPtr;
    Aluno aluno;
    int matriculaProcurada;
    int encontrouAluno;
    char inputBuffer[100];
    
    arquivoPtr = fopen("aluno.txt", "r+b"); // Reabrir arquivo para leitura
    if(arquivoPtr == NULL){
        printf("\nErro ao acessar o arquivo\n");
        printf("\nPressione qualquer tecla para continuar...");
        getch();
        return ;
    } else {
        encontrouAluno = 0;

        // Recebendo a matricula do aluno que será editado
        while(1){
            printf("\nDigite a matricula do aluno: ");
            fgets(inputBuffer, sizeof(inputBuffer), stdin); 
            sscanf(inputBuffer, "%d", &matriculaProcurada);
            
            if(matriculaProcurada<2024001 || inputBuffer[0] == '\n'){
                printf("\nEntrada invalida! Por favor tente novamente\n");
            }
            else{
                break;
            }
        }

        // rewind(arquivoPtr); // Reposiciona o ponteiro no inicio do arquivo, isso para ele conseguir ler caso o usuario procure outro aluno
        while(fread(&aluno, sizeof(Aluno), 1, arquivoPtr) == 1){
            if(aluno.matricula == matriculaProcurada && aluno.idProf == idProfessor){
                encontrouAluno = 1;

                printf("\n---------------------------\n");
                printf("Aluno: %s\n", aluno.nome);

                char opcaoEdicao = '0';

                printf("\n---------------------------------------------------------\n");
                printf("(1) EDITAR NOME\n");
                printf("(2) EDITAR NOTAS\n");
                printf("(3) ADICIONAR DISCIPLINA \n");
                printf("(4) SAIR \n");
                
                fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                sscanf(inputBuffer, "%c", &opcaoEdicao);
                
                switch(opcaoEdicao){
                    case '1':
                        while(1){
                            printf("Digite o novo nome do aluno: ");
                            fgets(inputBuffer, sizeof(inputBuffer), stdin);

                            if(sscanf(inputBuffer, "%49[^\n]", aluno.nome)==0){
                                printf("\nEntrada invalida! Por favor tente novamente\n");
                            }
                            else if(ehCaracter(aluno.nome) == 0){
                                printf("\nEntrada invalida! Por favor tente novamente\n");
                            }
                            else{
                                break;
                            }
                        }
                        
                        break;

                    case '2': ;
                        int opcaoTurmaEdicao = 0;
                        printf("\nTurmas e Disciplinas do aluno [%s]\n", aluno.nome);
                        for(int i = 0; i < aluno.quantidadeDisciplinas; i++){
                            printf("\n(%d) Turma: %-10s Disciplina: %-10s \n", i+1, aluno.disciplinas[i].nomeTurma, aluno.disciplinas[i].nomeDisciplina);
                        }

                        while(1){
                            printf("\n\nDigite o numero da turma que deseja editar: ");
                            fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                            sscanf(inputBuffer, "%d", &opcaoTurmaEdicao);

                            if(inputBuffer[0] == '\n'){
                                printf("\nEntrada invalida! Por favor tente novamente\n");
                            }
                            else if(opcaoTurmaEdicao < 1 || opcaoTurmaEdicao > aluno.quantidadeDisciplinas+1){
                                printf("\nEntrada invalida! Por favor tente novamente\n");
                            }
                            else{
                                break;
                            }
                        }

                        char opcaoEdicaoDisciplina;
                        while(opcaoEdicaoDisciplina != '5'){
                            system("cls");
                            printf("\n%-5s | %-5s:\n", aluno.disciplinas[opcaoTurmaEdicao-1].nomeTurma, aluno.disciplinas[opcaoTurmaEdicao-1].nomeDisciplina);
        
                            for(int j = 0; j < 4; j++){
                                printf("\n %d nota: %.2f", j+1, aluno.disciplinas[opcaoTurmaEdicao-1].notas[j]);
                            }
                            // printf("\nMedia: %.2f", aluno.disciplinas[i].media);
                            printf("\n\nQual nota deseja alterar ? (1) (2) (3) (4) (5 para sair)\n");
                            fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                            sscanf(inputBuffer, "%c", &opcaoEdicaoDisciplina);
                            
                            if(opcaoEdicaoDisciplina == '1' || opcaoEdicaoDisciplina == '2' || opcaoEdicaoDisciplina == '3' || opcaoEdicaoDisciplina == '4'){

                                float novaNota = 0;
                                while(1){
                                    printf("\nDigite a nova nota: ");
                                    fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                                    sscanf(inputBuffer, "%f", &novaNota);
                                    if(inputBuffer[0] == '\n'){
                                        printf("\nEntrada invalida! Por favor tente novamente\n");
                                    }
                                    else if(ehCaracter(inputBuffer)==1){
                                        printf("\nOpcao invalida! Insira novamente uma opcao\n");
                                    }
                                    else if(novaNota<0||novaNota>10){
                                        printf("\nEntrada invalida! Por favor tente novamente\n");
                                    }
                                    else{
                                        break;
                                    }
                                }

                                switch(opcaoEdicaoDisciplina){
                                case '1':
                                    aluno.disciplinas[opcaoTurmaEdicao-1].notas[0] = novaNota;
                                    // atualizarMedia(aluno.matricula, i);
                                    break;
                                case '2':
                                    aluno.disciplinas[opcaoTurmaEdicao-1].notas[1] = novaNota;
                                    // atualizarMedia(aluno.matricula, i);
                                    break;
                                case '3':
                                    aluno.disciplinas[opcaoTurmaEdicao-1].notas[2] = novaNota;
                                    // atualizarMedia(aluno.matricula, i);
                                    break;
                                case '4':
                                    aluno.disciplinas[opcaoTurmaEdicao-1].notas[3] = novaNota;
                                    // atualizarMedia(aluno.matricula, i);
                                    break;
                                case '5':
                                    break;
                                default:
                                    printf("\nOpcao invalida! Insira novamente uma opcao\n");
                                    break;
                                }
                                aluno.disciplinas[opcaoTurmaEdicao-1].media = (aluno.disciplinas[opcaoTurmaEdicao-1].notas[0] + aluno.disciplinas[opcaoTurmaEdicao-1].notas[1] + aluno.disciplinas[opcaoTurmaEdicao-1].notas[2] + aluno.disciplinas[opcaoTurmaEdicao-1].notas[3])/4;
                                
                            }
                            else if(opcaoEdicaoDisciplina == '5'){
                                
                            }else{
                                printf("\nOpcao invalida! Insira novamente uma opcao\n");
                            }
                        }
                        break; 

                    case '3': ;

                        if(aluno.quantidadeDisciplinas >= 6){
                            printf("\nMaximo de Disciplinas cadastradas no aluno!\n");
                        }else{
                            int i = aluno.quantidadeDisciplinas;
                            char opcaoDisciplina;
                            while(aluno.quantidadeDisciplinas < 6){ // lendo as disciplinas
                                opcaoDisciplina = 's';

                                memset(&aluno.disciplinas[i], 0, sizeof(aluno.disciplinas[i]));

                                while(1){
                                    printf("\nDigite o nome da turma: ");
                                    fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                                    sscanf(inputBuffer, "%49[^\n]", aluno.disciplinas[i].nomeTurma); 

                                    if(!strVazia(aluno.disciplinas[i].nomeTurma)) break;
                                    printf("\nEntrada invalida! Por favor tente novamente\n");
                                }

                                while(1){
                                    printf("\nDigite o nome da disciplina: ");
                                    fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                                    sscanf(inputBuffer, "%49[^\n]", aluno.disciplinas[i].nomeDisciplina);

                                    if(!strVazia(aluno.disciplinas[i].nomeDisciplina)) break;
                                    printf("\nEntrada invalida! Por favor tente novamente\n");
                                }
                                
                                // Função que procura disciplina no arquivo txt
                                int chamadaFuncaoVerificacao = procurarDisciplinaTurma(aluno.disciplinas[i].nomeDisciplina, aluno.disciplinas[i].nomeTurma, aluno.idProf);
                                
                                if(chamadaFuncaoVerificacao == 0){ // Verificando se existe a disciplina digitada
                                    printf("\nErro ao encontrar a disciplina! ");

                                    printf("\nDeseja cadastrar uma nova disciplina? (S para continuar)");
                                    fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                                    sscanf(inputBuffer, " %c", &opcaoDisciplina);

                                    if(opcaoDisciplina == 's' || opcaoDisciplina == 'S'){
                                        continue;
                                    }else{
                                        memset(&aluno.disciplinas[i], 0, sizeof(aluno.disciplinas[i]));
                                        break;
                                    }

                                }else if(chamadaFuncaoVerificacao == 1){ 

                                    // Verificando se a disciplina ja foi inserida no aluno
                                    int verificadorDisciplinaCadastrada = 0;
                                    // Percorrendo a lista de disciplinas atual
                                    for(int n = 0; n < i; n++){
                                        if(strcmp(aluno.disciplinas[n].nomeDisciplina, aluno.disciplinas[i].nomeDisciplina) == 0 && strcmp(aluno.disciplinas[n].nomeTurma, aluno.disciplinas[i].nomeTurma) == 0){
                                            verificadorDisciplinaCadastrada = 1;
                                            break;
                                        }
                                    }

                                    if(verificadorDisciplinaCadastrada == 1){
                                        printf("\nDisciplina ja cadastrada!");

                                        printf("\nDeseja cadastrar uma nova disciplina? (S para continuar)");
                                        fgets(inputBuffer, sizeof(inputBuffer), stdin);
                                        sscanf(inputBuffer, " %c", &opcaoDisciplina);

                                        if(opcaoDisciplina == 's' || opcaoDisciplina == 'S'){
                                            continue;
                                        }else{
                                            memset(&aluno.disciplinas[i], 0, sizeof(aluno.disciplinas[i]));
                                            break;
                                        }
                                    }

                                    // Caso a disciplia não tenha sido ainda cadastrada no aluno:
                                    aluno.quantidadeDisciplinas += 1;
                                    aluno.disciplinas[i].media = 0;
                                    aluno.disciplinas[i].notas[0] = 0;
                                    aluno.disciplinas[i].notas[1] = 0;
                                    aluno.disciplinas[i].notas[2] = 0;
                                    aluno.disciplinas[i].notas[3] = 0;
                                    aluno.disciplinas[i].frequencia = 100;
                                    aluno.disciplinas[i].faltas = 0;
                                    i++;

                                    printf("\nDeseja continuar cadastrando disciplinas? (S para continuar)");
                                    fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                                    sscanf(inputBuffer, " %c", &opcaoDisciplina);
                                    
                                    if(opcaoDisciplina == 's' || opcaoDisciplina == 'S'){
                                        continue;
                                    }else{
                                        break;
                                    }
                                }
                            }
                        }
                        break;

                    case '4':
                        break;

                    default:
                        printf("\nOpcao invalida!\n");
                }
                
                // Posiciona o ponteiro do arquivo para o início do registro da turma
                fseek(arquivoPtr, -sizeof(Aluno), SEEK_CUR);

                fwrite(&aluno, sizeof(Aluno), 1, arquivoPtr);

                fflush(arquivoPtr);

                printf("\nEdicao concluida!\n");
            }
        }

        if(!encontrouAluno){
            printf("\nNenhum aluno encontrado!\n");
        }

        fclose(arquivoPtr);

        printf("\nPressione qualquer tecla para continuar...");
        getch();
    }
}

// Função para exclusao de aluno por matricula
void excluirAluno(){
    FILE *arquivoPtr;
    FILE *arquivoTempPtr;
    Aluno aluno;
    int matriculaProcurada;
    int encontrouAluno = 0;
    char inputBuffer[100];

    arquivoPtr = fopen("aluno.txt", "rb");
    if(arquivoPtr == NULL){
        printf("\nErro ao abrir o arquivo de alunos");
        printf("\nPressione qualquer tecla para continuar...");
        getch();
        return;
    }else{
        arquivoTempPtr = fopen("temp.txt", "wb");
        if(arquivoTempPtr == NULL){
            printf("\nErro ao criar o arquivo temporario");
            printf("\nPressione qualquer tecla para continuar...");
            getch();
            fclose(arquivoPtr);
            return;
        }else{

            // Recebendo a matricula do aluno que será excluído
            while(1){
                printf("\nDigite a matricula do aluno a ser excluido: ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                sscanf(inputBuffer, "%d", &matriculaProcurada);

                if(matriculaProcurada >= 2024001 && inputBuffer[0] != '\n') break;
                printf("\nEntrada invalida! Por favor tente novamente\n");
            }
            
            // Percorrendo o arquivo aluno.txt original passando todos os alunos, exceto o que possui a matricula escolhida, para o novo txt
            while(fread(&aluno, sizeof(Aluno), 1, arquivoPtr) == 1){
                if(aluno.matricula != matriculaProcurada){
                    fwrite(&aluno, sizeof(Aluno), 1, arquivoTempPtr);
                }else{
                    encontrouAluno = 1;
                }
            } 

            // Fechando arquivos
            fclose(arquivoPtr);
            fclose(arquivoTempPtr);

            // Excluindo o antigo txt (incluindo o aluno escolhido) e renomeando o novo txt (sem o aluno escolhido) para aluno.txt
            remove("aluno.txt");
            rename("temp.txt", "aluno.txt");

            if(!encontrouAluno){
                printf("\nAluno nao encontrado para exclusao");
            }else{
                printf("\nAluno excluido com sucesso");
            }

            printf("\nPressione qualquer tecla para continuar...");
            getch();
        }
    }
}

// Função para vizualizar todos os alunos do professor
void verAlunos(int idProfessor){
    FILE *arquivoPtr;
    Aluno aluno;
    
    arquivoPtr = fopen("aluno.txt", "rb"); // Reabrir arquivo para leitura
    if(arquivoPtr == NULL){
        printf("\nErro ao acessar o arquivo\n");
        printf("\nPressione qualquer tecla para continuar...");
        getch();
        return ;
    }else{
        printf("\nDados do arquivo:");
        while(fread(&aluno, sizeof(Aluno), 1, arquivoPtr) == 1){
            if(aluno.idProf == idProfessor){
                printf("\n\n---------------------------\n");
                printf("Matricula: %d\n", aluno.matricula);
                printf("Nome: %s\n", aluno.nome);
                for(int i = 0; i < aluno.quantidadeDisciplinas; i++){
                    printf("\nTurma: %s\n", aluno.disciplinas[i].nomeTurma);
                    printf("Disciplina: %s\n", aluno.disciplinas[i].nomeDisciplina);
                    for(int j = 0; j < 4; j++){
                        printf("Nota %d: %.2f\n", j+1, aluno.disciplinas[i].notas[j]);
                    }
                    printf("Media: %.2f\n", aluno.disciplinas[i].media);
                    printf("Frequencia: %.0f\n", aluno.disciplinas[i].frequencia);
                    printf("Faltas: %d\n", aluno.disciplinas[i].faltas);
                }
            }
        }
        fclose(arquivoPtr);

        printf("\nPressione qualquer tecla para continuar...\n");
        getch();
    }
}

// Função para vizualizar um aluno pela matricula
void verAlunoMatricula(int idProfessor){
    FILE *arquivoPtr;
    Aluno aluno;
    int matriculaProcurada;
    char inputBuffer[100];
    
    arquivoPtr = fopen("aluno.txt", "rb"); // Reabrir arquivo para leitura
    if(arquivoPtr == NULL){
        printf("\nErro ao acessar o arquivo\n");
        printf("\nPressione qualquer tecla para continuar...");
        getch();
        return ;
    }else{
        while(1){
            printf("\nDigite a matricula do aluno: "); 
            fgets(inputBuffer, sizeof(inputBuffer), stdin);
            sscanf(inputBuffer, "%d", &matriculaProcurada);
            
            if(matriculaProcurada < 2024001 || inputBuffer[0] == '\n'){
                printf("\nOpcao invalida! Insira novamente uma opcao\n");
            }else{
                break;
            }
            
        }

        // rewind(arquivoPtr); // Posiciona o ponteiro no inicio do arquivo
        int encontrouAluno = 0;
        while(fread(&aluno, sizeof(Aluno), 1, arquivoPtr) == 1){
            if(aluno.matricula == matriculaProcurada && aluno.idProf == idProfessor){
                encontrouAluno = 1;
                printf("\n\n---------------------------\n");
                printf("Matricula: %d\n", aluno.matricula);
                printf("Nome: %s\n", aluno.nome);
                for(int i = 0; i < aluno.quantidadeDisciplinas; i++){
                    printf("\nTurma: %s\n", aluno.disciplinas[i].nomeTurma);
                    printf("Disciplina: %s\n", aluno.disciplinas[i].nomeDisciplina);
                    for(int j = 0; j < 4; j++){
                        printf("Nota %d: %.2f\n", j+1, aluno.disciplinas[i].notas[j]);
                    }
                    printf("Media: %.2f\n", aluno.disciplinas[i].media);
                    printf("Frequencia: %.0f\n", aluno.disciplinas[i].frequencia);
                    printf("Faltas: %d\n", aluno.disciplinas[i].faltas);
                }
            }
        }

        fclose(arquivoPtr);

        if(!encontrouAluno){
            printf("\nNenhum aluno encontrado!\n");
        }

        printf("\nPressione qualquer tecla para continuar...\n");
        getch();
    }
}

// Função para vizualizar alunos pelo nome
void verAlunoNome(int idProfessor){
    FILE *arquivoPtr;
    Aluno aluno;
    char nomeProcurado[50];
    char inputBuffer[256];
    
    arquivoPtr = fopen("aluno.txt", "rb"); // Reabrir arquivo para leitura
    if(arquivoPtr == NULL){
        printf("\nErro ao acessar o arquivo\n");
        printf("\nPressione qualquer tecla para continuar...\n");
        getch();
        return ;
    } else {
    
        while(1){
            printf("\nDigite o nome do aluno: ");
            fgets(inputBuffer, sizeof(inputBuffer), stdin); 
            sscanf(inputBuffer, "%49[^\n]", nomeProcurado);
            
            if(inputBuffer[0] == '\n'){
                printf("\nOpcao invalida! Insira novamente uma opcao\n");
            }
            else if(ehCaracter(nomeProcurado) == 0){
                printf("\nOpcao invalida! Insira novamente uma opcao\n");
            }
            else{
                break;
            }
        }

        printf("\nDados do arquivo:\n");
        while(fread(&aluno, sizeof(Aluno), 1, arquivoPtr) == 1){
            if(strcmp(aluno.nome, nomeProcurado) == 0 && aluno.idProf == idProfessor){
                printf("\n---------------------------\n");
                printf("Matricula: %d\n", aluno.matricula);
                printf("Nome: %s\n", aluno.nome);
                for(int i = 0; i < aluno.quantidadeDisciplinas; i++){
                    printf("\nTurma: %s\n", aluno.disciplinas[i].nomeTurma);
                    printf("Disciplina: %s\n", aluno.disciplinas[i].nomeDisciplina);
                    for(int j = 0; j < 4; j++){
                        printf("Nota %d: %.2f\n", j+1, aluno.disciplinas[i].notas[j]);
                    }
                    printf("Media: %.2f\n", aluno.disciplinas[i].media);
                    printf("Frequencia: %.0f\n", aluno.disciplinas[i].frequencia);
                    printf("Faltas: %d\n", aluno.disciplinas[i].faltas);
                }
            }
        }
        fclose(arquivoPtr);

        printf("\nPressione qualquer tecla para continuar...");
        getch();
    }
}

// Função que cadastra aulas e controla a frequência
void lancarFreq(int idProfessor){
    FILE *arquivoPtr;
    FILE *arquivoAlunoPtr;
    Aluno aluno;
    Turma turma;
    char nomeTurmaProcurada[50];
    char nomeDisciplinaProcurada[50];
    int encontrouTurma;
    char inputBuffer[100];

    arquivoPtr = fopen("turma.txt", "r+b");

    if(arquivoPtr == NULL){
        printf("\nErro ao acessar o arquivo\n");
    }else{
        encontrouTurma = 0;
        while(1){
            printf("\nDigite o nome da turma: ");
            fgets(inputBuffer, sizeof(inputBuffer), stdin); 
            if(sscanf(inputBuffer, "%49[^\n]", nomeTurmaProcurada)==0){
                printf("\nOpcao invalida! Insira novamente uma opcao\n");
            }
            else{
                break;
            }
            
        }

        while(1){
            printf("\nDigite o nome da disciplina: ");
            fgets(inputBuffer, sizeof(inputBuffer), stdin); 
            if(sscanf(inputBuffer, "%49[^\n]", nomeDisciplinaProcurada)==0){
                printf("\nOpcao invalida! Insira novamente uma opcao\n");
            }
            else{
                break;
            }

        }
        
        while(fread(&turma, sizeof(Turma), 1, arquivoPtr) == 1){
            if(strcmp(turma.nomeDisciplina, nomeDisciplinaProcurada) == 0 && strcmp(turma.nomeTurma, nomeTurmaProcurada) == 0 && turma.idProf == idProfessor){
                encontrouTurma = 1;
                // Recebendo do usuario a quantidade de faltas a serem registradas
                if(turma.quantidadeDeAulasConlcuidas == turma.quantidadeDeAulas){
                    printf("\nLimite de aulas atingido!\n");
                    break;
                }

                int quantidadeAulasTemp;
                printf("\nQuantidade de aulas concluidas: %d", turma.quantidadeDeAulasConlcuidas);
                while(1){
                    printf("\nDigite quantas aulas serao registradas: ");
                    fgets(inputBuffer, sizeof(inputBuffer), stdin);
                    sscanf(inputBuffer, "%d", &quantidadeAulasTemp);

                    if((quantidadeAulasTemp > 1 && quantidadeAulasTemp <= turma.quantidadeDeAulas - turma.quantidadeDeAulasConlcuidas) && inputBuffer[0] != '\n') break;
                    printf("\nEntrada invalida! Por favor tente novamente\n");
                    printf("\nDigite um valor entre 1 e %d !", turma.quantidadeDeAulas - turma.quantidadeDeAulasConlcuidas);

                }
                turma.quantidadeDeAulasConlcuidas += quantidadeAulasTemp;

                // Posiciona o ponteiro do arquivo para o início do registro da turma
                fseek(arquivoPtr, -sizeof(Turma), SEEK_CUR);

                fwrite(&turma, sizeof(Turma), 1, arquivoPtr);

                fflush(arquivoPtr);

                rewind(arquivoPtr);

                arquivoAlunoPtr = fopen("aluno.txt", "r+b");
                if(arquivoAlunoPtr == NULL){
                    printf("Erro ao acessar o arquivo aluno");
                }else{
                    // Imprimindo os Alunos que possuem essa turma e disciplina
                    while(fread(&aluno, sizeof(Aluno), 1, arquivoAlunoPtr) == 1){
                        for(int i = 0; i < aluno.quantidadeDisciplinas; i++){
                            if(strcmp(aluno.disciplinas[i].nomeDisciplina, nomeDisciplinaProcurada) == 0 && strcmp(aluno.disciplinas[i].nomeTurma, nomeTurmaProcurada) == 0 && aluno.idProf == idProfessor){
                                    printf("\n-----------------------");
                                    printf("\nMatricula: %-10d Nome: %-10s\n", aluno.matricula, aluno.nome);
                            }
                        }
                    }

                    // Perguntando ao usuario se ele deseja registrar pelo menos 1 falta
                    int matriculaAlunoFalta;
                    char continuarFalta = 's';
                    int encontrouAluno, encontrouDisciplina;

                    while(1){
                        printf("\nDeseja registrar faltas? (S ou N)");
                        fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                        sscanf(inputBuffer, "%c", &continuarFalta);

                        if(inputBuffer[0] != '\n') break;
                        printf("\nEntrada invalida! Por favor tente novamente\n");
                    }

                    // Aplicando faltas
                    while(continuarFalta == 's' || continuarFalta == 'S'){
                        encontrouAluno = 0;
                        encontrouDisciplina = 0;

                        while(1){
                            printf("\nDigite a matricula do aluno: ");
                            fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                            sscanf(inputBuffer, "%d", &matriculaAlunoFalta);

                            if(matriculaAlunoFalta < 2024001 || inputBuffer[0] == '\n'){
                                printf("\nOpcao invalida! Insira novamente uma opcao\n");
                            }
                            else{
                                break;
                            }
                        }
                        
                        rewind(arquivoAlunoPtr); // reinicia o ponteiro para o início do arquivo
                        while(fread(&aluno, sizeof(Aluno), 1, arquivoAlunoPtr) == 1){
                            if(aluno.matricula == matriculaAlunoFalta && aluno.idProf == idProfessor){
                                encontrouAluno = 1;
                                for(int i = 0; i < aluno.quantidadeDisciplinas; i++){
                                    if(strcmp(aluno.disciplinas[i].nomeDisciplina, nomeDisciplinaProcurada) == 0 && strcmp(aluno.disciplinas[i].nomeTurma, nomeTurmaProcurada) == 0){
                                        encontrouDisciplina = 1;
                                        aluno.disciplinas[i].faltas += quantidadeAulasTemp;
                                        aluno.disciplinas[i].frequencia = ((turma.quantidadeDeAulasConlcuidas - aluno.disciplinas[i].faltas)*100)/turma.quantidadeDeAulasConlcuidas;

                                        // Posiciona o ponteiro do arquivo para o início do registro da turma
                                        fseek(arquivoAlunoPtr, -sizeof(Aluno), SEEK_CUR);

                                        fwrite(&aluno, sizeof(Aluno), 1, arquivoAlunoPtr);

                                        fflush(arquivoAlunoPtr);

                                        printf("\nFaltas inserida com sucesso!");

                                        break;
                                    }
                                }
                                break;
                            }
                        }

                        if(encontrouAluno != 1){
                            printf("\nO aluno nao foi encontrado");
                        }else if(encontrouDisciplina != 1){
                            printf("\nO aluno informado nao eh integrante da turma");
                        }

                        while(1){
                            printf("\nDeseja cadastrar faltas para mais algum aluno? ");
                            fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                            sscanf(inputBuffer, "%c", &continuarFalta);

                            if(inputBuffer[0] != '\n') break;
                            printf("\nEntrada invalida! Por favor tente novamente\n");
                        }
        
                    } 

                    fclose(arquivoAlunoPtr);
                }
            }
            if(encontrouTurma == 1){
                break;
            }
        }
        if(encontrouTurma != 1){
            printf("Nenhuma turma encontrada");
        }

        fclose(arquivoPtr);
    }
}

// Função que busca os alunos de uma turma/disciplina e imprime seu nome e matrícula
void verAlunosTurmaStrParametro(int idProfessor, char *turmaProcurada, char *disciplinaProcurada){
    FILE *arquivoPtr;
    FILE *arquivoAlunosPtr;
    Aluno aluno;
    Turma turma;
    
    arquivoPtr = fopen("turma.txt", "rb"); // Reabrir arquivo para leitura
    if(arquivoPtr == NULL){
        printf("\nErro ao acessar o arquivo turmas\n");
    }else{
        // rewind(arquivoPtr); // Posiciona o ponteiro no inicio do arquivo
        printf("\nAlunos cadastrados:\n");
        while(fread(&turma, sizeof(Turma), 1, arquivoPtr) == 1){
            if(strcmp(turma.nomeTurma, turmaProcurada) == 0 && strcmp(turma.nomeDisciplina, disciplinaProcurada) == 0 && turma.idProf == idProfessor){
                arquivoAlunosPtr = fopen("aluno.txt", "rb");
                if(arquivoAlunosPtr == NULL){
                    printf("\nErro ao acessar o arquivo alunos\n");
                }else{
                    while(fread(&aluno, sizeof(Aluno), 1, arquivoAlunosPtr) == 1){
                        for(int k = 0; k < aluno.quantidadeDisciplinas; k++){
                            if(strcmp(turma.nomeTurma, aluno.disciplinas[k].nomeTurma) == 0 && strcmp(turma.nomeDisciplina, aluno.disciplinas[k].nomeDisciplina) == 0 && aluno.idProf == idProfessor){
                                printf("\n---------------------------\n");
                                printf("Matricula: %d\n", aluno.matricula);
                                printf("Nome: %s\n", aluno.nome);
                    
                                break;
                            }
                        }
                        
                    }
                    fclose(arquivoAlunosPtr);
                }
            }
        }
        fclose(arquivoPtr);
    }

}

// Função que cadastra notas em alunos
void lancarNotas(int idProfessor){
    FILE *arquivoPtr;
    Aluno aluno;
    int matriculaProcurada;
    int encontrouAluno = 0;
    char turmaProcurada[50], disciplinaProcurada[50];
    char inputBuffer[100];

    while(1){
        printf("\nDigite o nome da turma: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin); 
        if(sscanf(inputBuffer, "%49[^\n]", turmaProcurada)==0){
            printf("\nOpcao invalida! Insira novamente uma opcao\n");
        }
        else{
            break;
        }

    }

    while(1){
        printf("\nDigite o nome da disciplina: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin); 
        if(sscanf(inputBuffer, "%49[^\n]", disciplinaProcurada)==0){
            printf("\nOpcao invalida! Insira novamente uma opcao\n");
        }
        else{
            break;
        }

    }
    
    verAlunosTurmaStrParametro(idProfessor, turmaProcurada, disciplinaProcurada); // imprimindo os aluno da disciplina na tela

    arquivoPtr = fopen("aluno.txt", "r+b");
    if(arquivoPtr == NULL){
        printf("Erro ao acessar o arquivo aluno");
        printf("\nPressione qualquer tecla para continuar...\n");
        getch();
        return;
    }else{
        while(1){
            printf("\nDigite a matricula do aluno: ");
            fgets(inputBuffer, sizeof(inputBuffer), stdin); 
            sscanf(inputBuffer, "%d", &matriculaProcurada);

            if(matriculaProcurada<2024001 || inputBuffer[0] == '\n'){
                printf("Informe apenas numeros validos para matricula(Ex: 2024001).\n");
            }
            else{
                break;
            }
        }
        
        rewind(arquivoPtr);
        while(fread(&aluno, sizeof(Aluno), 1, arquivoPtr) == 1){
            if(aluno.matricula == matriculaProcurada && aluno.idProf == idProfessor){
                encontrouAluno = 1;
                printf("---------------------------\n");
                printf("Aluno: %s\n\n", aluno.nome);

                for(int i = 0; i < aluno.quantidadeDisciplinas; i++){
                    if(strcmp(aluno.disciplinas[i].nomeDisciplina, disciplinaProcurada) == 0 && strcmp(aluno.disciplinas[i].nomeTurma, turmaProcurada) == 0 && aluno.idProf == idProfessor){
                        char opcaoEdicaoDisciplina;
                        while(opcaoEdicaoDisciplina != '5'){
                            printf("\n%s | %s\n", aluno.disciplinas[i].nomeTurma, aluno.disciplinas[i].nomeDisciplina);

                            for(int j = 0; j < 4; j++){
                                printf("\n %d nota: %.2f", j+1, aluno.disciplinas[i].notas[j]);
                            }

                            printf("\n\n Qual nota deseja alterar ? (1) (2) (3) (4) (5 para sair)");
                            fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                            sscanf(inputBuffer, "%c", &opcaoEdicaoDisciplina);

                            if(opcaoEdicaoDisciplina == '1' || opcaoEdicaoDisciplina == '2' || opcaoEdicaoDisciplina == '3' || opcaoEdicaoDisciplina == '4'){
                                float novaNota = 0;
                                while(1){
                                    printf("Digite a nova nota: ");
                                    fgets(inputBuffer, sizeof(inputBuffer), stdin); 
                                    sscanf(inputBuffer, "%f", &novaNota);
                                    
                                    if(inputBuffer[0] == '\n'){
                                        printf("\nOpcao invalida! Insira novamente uma opcao\n");
                                    }
                                    else if(ehCaracter(inputBuffer) == 1){
                                        printf("\nOpcao invalida! Insira novamente uma opcao\n");
                                    }
                                    else if(novaNota < 0 || novaNota > 10){
                                        printf("\nOpcao invalida! Insira novamente uma opcao\n");
                                    }
                                    else{
                                        break;
                                    }
                                }
                                
                                switch(opcaoEdicaoDisciplina){
                                case '1':
                                    aluno.disciplinas[i].notas[0] = novaNota;
                                    // atualizarMedia(aluno.matricula, i);
                                    break;
                                case '2':
                                    aluno.disciplinas[i].notas[1] = novaNota;
                                    // atualizarMedia(aluno.matricula, i);
                                    break;
                                case '3':
                                    aluno.disciplinas[i].notas[2] = novaNota;
                                    // atualizarMedia(aluno.matricula, i);
                                    break;
                                case '4':
                                    aluno.disciplinas[i].notas[3] = novaNota;
                                    // atualizarMedia(aluno.matricula, i);
                                    break;
                                default:
                                    printf("\nOpcao invalida! Insira novamente uma opcao\n");
                                }

                                aluno.disciplinas[i].media = (aluno.disciplinas[i].notas[0] + aluno.disciplinas[i].notas[1] + aluno.disciplinas[i].notas[2] + aluno.disciplinas[i].notas[3])/4;
                
                            }
                            else if(opcaoEdicaoDisciplina == '5'){
                                
                            }else{
                                printf("\nOpcao invalida! Insira novamente uma opcao\n");
                            }
                        }
                        break; // sair do loop de procura da turma e disciplina
                    }
                }
            }
            // Posiciona o ponteiro do arquivo para o início do registro da turma
            fseek(arquivoPtr, -sizeof(Aluno), SEEK_CUR);

            fwrite(&aluno, sizeof(Aluno), 1, arquivoPtr);

            fflush(arquivoPtr);
        }

        if(!encontrouAluno){
            printf("Nenhum aluno encontrado");
        }

        fclose(arquivoPtr);
    }
}

// Função que calcula a media de uma turma
float mediaTurma(Turma* turmaSelecionada){
    FILE *arquivoPtr;
    Aluno aluno;
    float mediaTurma = 0, contadorAlunos = 0;
    int encontrouAluno = 0;
    
    arquivoPtr = fopen("aluno.txt", "rb");
    if(arquivoPtr == NULL){
        printf("\nErro ao acessar o arquivo aluno.txt");
        return -1;
    }else{
        while(fread(&aluno, sizeof(Aluno), 1, arquivoPtr) == 1){
            for(int i = 0; i < aluno.quantidadeDisciplinas; i++){
                if(strcmp(aluno.disciplinas[i].nomeTurma, turmaSelecionada->nomeTurma) == 0 && strcmp(aluno.disciplinas[i].nomeDisciplina, turmaSelecionada->nomeDisciplina) == 0 && turmaSelecionada->idProf == aluno.idProf){
                    mediaTurma += aluno.disciplinas[i].media;
                    contadorAlunos++;
                    encontrouAluno = 1;
                    break;
                }
            }
        }

        fclose(arquivoPtr);
        
        mediaTurma = mediaTurma/contadorAlunos;
        if(encontrouAluno){
            return mediaTurma;
        }else{
            return -1;
        }
    }
}

// Função que apresenta o boletim do aluno 
void boletimAluno(int idProfessor){
    FILE *arquivoPtr;
    Aluno aluno;
    Turma turma;
    float mediaTur;
    int matriculaProcurada;
    char inputBuffer[100];
    
    arquivoPtr = fopen("aluno.txt", "rb"); // Reabrir arquivo para leitura
    if(arquivoPtr == NULL){
        printf("\nErro ao acessar o arquivo");
        printf("\nPressione qualquer tecla para continuar...");
        getch();
        return ;
    }else{
        while(1){
            printf("\nDigite a matricula do aluno: "); 
            fgets(inputBuffer, sizeof(inputBuffer), stdin);
            sscanf(inputBuffer, " %d", &matriculaProcurada);

            if(matriculaProcurada < 2024001 || inputBuffer[0] == '\n' || ehCaracter(inputBuffer) == 1){
                printf("\nOpcao invalida! Insira novamente uma opcao\n");
            }
            else{
                break;
            }
        }

        system("cls");
        while(fread(&aluno, sizeof(Aluno), 1, arquivoPtr) == 1){
            if(aluno.matricula == matriculaProcurada && aluno.idProf == idProfessor){
                printf(" -----------------------------------------\n");
                printf("                   BOLETIM\n");
                printf(" -----------------------------------------\n\n");
                printf(" Aluno: %s     Matricula: %d\n\n", aluno.nome, aluno.matricula);
                char string1[] = "FREQUENCIA (%)";

                printf(" %-15s %-15s %-10s %-14s %-10s %-10s %-10s %-10s %-15s %-15s %-15s\n", "TURMA", "DISCIPLINA", "FALTAS", string1, "NOTA 1", "NOTA 2", "NOTA 3", "NOTA 4", "MEDIA DO ALUNO", "MEDIA DA TURMA", "SITUACAO");
                
                for(int i = 0; i < aluno.quantidadeDisciplinas; i++){

                    turma = procurarTurma(idProfessor, aluno.disciplinas[i].nomeTurma, aluno.disciplinas[i].nomeDisciplina);
                    mediaTur = mediaTurma(&turma);

                    // Possivel erro nas aberturas de arquivos
                    if(turma.idProf == -1){
                        printf("\nTurma inexistente!");
                        fclose(arquivoPtr);
                        return ;
                    }

                    if(mediaTur == -1){
                        printf("\nErro");
                        fclose(arquivoPtr);
                        return ;
                    }

                    printf(" %-15s %-15s %-10d %-15.0f", aluno.disciplinas[i].nomeTurma, aluno.disciplinas[i].nomeDisciplina, aluno.disciplinas[i].faltas, aluno.disciplinas[i].frequencia);
                    for(int j = 0; j < 4; j++){
                        printf("%-10.2f ", aluno.disciplinas[i].notas[j]);
                    }
                    printf("%-15.2f %-15.2f ", aluno.disciplinas[i].media, mediaTur);

                    if(aluno.disciplinas[i].media >= 7 && aluno.disciplinas[i].frequencia >= 75){
                        printf("%-15s\n", "Aprovado");
                    }else{
                        printf("%-15s\n", "Reprovado");
                    }

                }
            }
        }

        printf("\nPressione qualquer tecla para continuar...");
        getch();
        fclose(arquivoPtr);
    }
}

// Função que apresenta o boletim de uma turma
void boletimTurma(int idProfessor){
    FILE *arquivoPtr;
    Aluno aluno;
    Turma turma;
    float mediaTur;
    char inputBuffer[100];

    char nomeTurmaProcurada[50];
    char nomeDisciplinaProcurada[50];

    while(1){
        printf("\nDigite o nome da turma: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin); 
        if(sscanf(inputBuffer, "%49[^\n]", nomeTurmaProcurada)==0){
            printf("\nOpcao invalida! Insira novamente uma opcao\n");
        }
        else{
            break;
        }

    }
    
    while(1){
        printf("\nDigite o nome da disciplina: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin); 
        if(sscanf(inputBuffer, "%49[^\n]", nomeDisciplinaProcurada)==0){
            printf("\nOpcao invalida! Insira novamente uma opcao\n");
        }
        else{
            break;
        }
    }

    system("cls");

    turma = procurarTurma(idProfessor, nomeTurmaProcurada, nomeDisciplinaProcurada);

    if(turma.idProf == -1){
        printf("\nTurma inexistente!");
    }else{
        arquivoPtr = fopen("aluno.txt", "rb"); // Reabrir arquivo para leitura
        if(arquivoPtr == NULL){
            printf("\nErro ao acessar o arquivo");
            return ;
        }else{
            mediaTur = mediaTurma(&turma);

            if(mediaTur == -1){
                printf("\nErro");
                fclose(arquivoPtr);
                return ;
            }

            printf(" -----------------------------------------\n");
            printf("               BOLETIM TURMA\n");
            printf(" -----------------------------------------\n\n");

            printf(" Turma: %-10s Disciplina: %-10s\n\n", nomeTurmaProcurada, nomeDisciplinaProcurada);

            printf(" C.H: %-10d Total de aulas: %-10d\n\n", turma.quantidadeDeAulas, turma.quantidadeDeAulasConlcuidas);
            char string1[] = "FREQUENCIA (%)";
            printf(" %-12s %-15s %-10s %-14s %-10s %-10s %-10s %-10s %-15s %-15s\n", "MATRICULA", "ALUNO", "FALTAS", string1, "NOTA 1", "NOTA 2", "NOTA 3", "NOTA 4", "MEDIA DO ALUNO", "SITUACAO");


            // rewind(arquivoPtr); // Posiciona o ponteiro no inicio do arquivo
            while(fread(&aluno, sizeof(Aluno), 1, arquivoPtr) == 1){
                for(int i = 0; i < aluno.quantidadeDisciplinas; i++){
                    if(strcmp(aluno.disciplinas[i].nomeTurma, nomeTurmaProcurada) == 0 && strcmp(aluno.disciplinas[i].nomeDisciplina, nomeDisciplinaProcurada) == 0 && aluno.idProf == idProfessor){
                        printf(" %-12d %-15s %-10d %-15.0f", aluno.matricula, aluno.nome, aluno.disciplinas[i].faltas, aluno.disciplinas[i].frequencia);

                        for(int j = 0; j < 4; j++){
                            printf("%-10.2f ", aluno.disciplinas[i].notas[j]);
                        }

                        printf("%-15.2f ", aluno.disciplinas[i].media);

                        if(aluno.disciplinas[i].media >= 7 && aluno.disciplinas[i].frequencia >= 75){
                            printf("%-15s\n", "Aprovado");
                        }else{
                            printf("%-15s\n", "Reprovado");
                        }
                        break;
                    }
                }
            }
        }
    
        printf("\nPressione qualquer tecla para continuar...");
        getch();
        fclose(arquivoPtr);
    }
}

// Função que aloca um espaço na memoria e gera a partir dos parametros um novo no de registro (ainda não alocado na lista encadeada)
Registro* criarRegistro(int idProf, char *nomeRegistro, char *dataRegistro){ 

    Registro* registroPtr = (Registro*)malloc(sizeof(Registro));

    if(registroPtr == NULL){ // Caso ocorra erro na alocação de memoria, retorna null
        printf("Erro na alocaçao de memoria");
        return registroPtr;

    }else{ // atribuindo os valores ao novo nó, além de atribuir null ao proximo
        strcpy(registroPtr -> nome, nomeRegistro);
        strcpy(registroPtr -> data, dataRegistro);
        registroPtr -> idProfessor = idProf;
        registroPtr -> proxNo = NULL;
        return registroPtr;
    }
}

// Funcao que chama a criação do no e que o aloca no final da lista encadeada
void adicionarListaRegistro(Registro **inicioLista, int idProfessor, char *nomeRegistro, char *nomeData){
    Registro *registroPtr;
    registroPtr = criarRegistro(idProfessor, nomeRegistro, nomeData);

    if(*inicioLista != NULL){ // Caso ja existam valores na lista
        Registro *percorrerPtr = *inicioLista;
        while(percorrerPtr->proxNo != NULL){ // percorrendo toda a lista ate encontrar o atual ultimo elemento
            percorrerPtr = percorrerPtr->proxNo;
        }
        percorrerPtr->proxNo = registroPtr; // alocando o novo registro no fim da lista

    }else{ 
        *inicioLista = registroPtr; // caso não existam nós na lista, o novo registro assume o inicio (cabeça)

    }
}

// Criando ou salvando a lista encadeada em um arquivo txt para persistir no disco
void criarRegistroTxt(Registro *inicioLista){
    FILE *arquivoPtr;
    arquivoPtr = fopen("registro.txt", "a");
    if(arquivoPtr == NULL){ 
        printf("Erro ao abrir o arquivo registro.txt");
        return;
    }else{
        Registro *percorrerPtr = inicioLista;
        while(percorrerPtr != NULL){
            fprintf(arquivoPtr, "%d %s %s\n", 
            percorrerPtr->idProfessor, percorrerPtr->data, percorrerPtr->nome); 
            percorrerPtr = percorrerPtr->proxNo;
        }
        fclose(arquivoPtr);
    }
}

// Funcão que carrega os dados armazenados no txt para a lista encadeada
Registro* receberLista(){
    FILE *arquivoPtr;
    arquivoPtr = fopen("registro.txt", "r");

    if(arquivoPtr == NULL){
        return NULL;
    }else{
        Registro *inicioLista = NULL;
        char nomeRegistro[50];
        char dataRegistro[11];
        int id;

        while(fscanf(arquivoPtr, "%d %10s %49[^\n]", &id, dataRegistro, nomeRegistro) != EOF){
            adicionarListaRegistro(&inicioLista, id, nomeRegistro, dataRegistro);
        }

        fclose(arquivoPtr);
        
        return inicioLista;
    }
}

// Função que libera a memória utilizada pela lista encadeada
void liberarMemLista(Registro *inicioLista){
    Registro *percorrerPtr;
    while(inicioLista != NULL){
        percorrerPtr = inicioLista;
        inicioLista = percorrerPtr->proxNo;
        free(percorrerPtr);
    }
}

// Função que todos os dados da lista encadeada
void verLista(Registro *inicioLista, int idProf){
    Registro *percorrerPtr = inicioLista;
    printf("Registros de atividades: \n\n");
    printf("Data:      Registro:\n");
    while(percorrerPtr != NULL){
        if(percorrerPtr->idProfessor == idProf){
            printf("%s %s\n", percorrerPtr->data, percorrerPtr->nome);
        }
        percorrerPtr = percorrerPtr->proxNo;
    }
    getch();
}

// Função que obtém a data atual e retorna a string
char* dataAtual(){
    static char dataStr[100]; // buffer para retornar a string da data
    
    time_t agora = time(NULL); // obtém a data e hora em tempo real
    
    struct tm *tempoLocal = localtime(&agora); // Converte o tempo lido agora para o tempo local
    
    strftime(dataStr, sizeof(dataStr), "%d/%m/%Y", tempoLocal); // Formatando a data para %d/%m/%Y (dia, mês, ano) dentro do buffer 
    
    return dataStr;
}

int ehDigitos(int entrada){

    if(isdigit(entrada) == 0){
        return 0;
    }
    else{
        return 1;
    }
}

int ehCaracter(char *entrada){
    int i = 0;
    int verificador = 1;
    while(entrada[i] != '\0'){
        if(isalpha(entrada[i]) == 0 && entrada[i] != ' '){
            verificador = 0;
            break;

        }
        i++;
    }
    return verificador;
}

int verificarEmail(char *entradaEmail){
    int verificador = 1;
    Professor professor;
    FILE* arquivoProfessor;

    arquivoProfessor = fopen("professor.txt", "r");

    if(arquivoProfessor == NULL){
        printf("Erro ao abrir arquivo.\n");
        printf("Pressione qualquer tecla para continuar...");
        getch();
    }
    else{
        while(fread(&professor, sizeof(Professor), 1, arquivoProfessor) == 1){
            if(strcmp(entradaEmail, professor.email) == 0){
                verificador = 0;
                break;
            }
        }
    }
    fclose(arquivoProfessor);
    return verificador;
}

// ------------------------------------------- MENUS -------------------------------------------


// Menu da tela inicial (login efetuado)
void telaInicial(Professor *professorPtr){
    Registro *lista; // lista encadeada de registros

    char inputBuffer[100];
    char opcaoTelaInicial;
    
    while(opcaoTelaInicial != '6'){
        system("cls");
        lista = NULL;
        lista = receberLista(); // carregando lista de registros

        printf("\n-----------------------------------------------");
        printf("\nBem-vindo, %s!", professorPtr->nome);
        printf("\n-----------------------------------------------\n\n");
        printf("(1) TURMAS\n");
        printf("(2) ALUNOS\n");
        printf("(3) NOTAS E FREQUENCIA\n");
        printf("(4) BOLETIM\n");
        printf("(5) REGISTROS\n");
        printf("(6) SAIR \n");

        fgets(inputBuffer, sizeof(inputBuffer), stdin); 
        sscanf(inputBuffer, "%c", &opcaoTelaInicial); 

        switch (opcaoTelaInicial){
        case '1':
            system("cls");
            telaTurma(professorPtr->id);
            break;

        case '2':
            system("cls");
            telaAluno(professorPtr->id);
            break;
        
        case '3':
            system("cls");
            telaNotasFreq(professorPtr->id);
            break;

        case '4':
            system("cls");
            telaBoletins(professorPtr->id);
            break;

        case '5':
            system("cls");
            verLista(lista, professorPtr->id);
            break;

        case '6':
            break;
        
        default:
            break;
        }

        // Liberando a memoria da lista encadeada de leitura
        liberarMemLista(lista);
    } 
}

// Tela com as funcionalidades envolvendo Turmas
void telaTurma(int idProf){
    Registro *lista = NULL; // lista encadeada de registros

    char inputBuffer[100];
    char opcaoTelaTurma;
    
    while(opcaoTelaTurma != '6'){
        system("cls");
        printf("\n---------------------------------------------------------\n");
        printf("(1) CADASTRAR TURMA\n");
        printf("(2) EDITAR TURMA\n");
        printf("(3) EXCLUIR TURMA\n");
        printf("(4) VIZUALIZAR TURMAS\n");
        printf("(5) VIZUALIZAR ALUNOS EM TURMA \n");
        printf("(6) SAIR \n");

        fgets(inputBuffer, sizeof(inputBuffer), stdin); 
        sscanf(inputBuffer, "%c", &opcaoTelaTurma); 

        switch (opcaoTelaTurma){
        case '1':
            system("cls");
            adicionarListaRegistro(&lista, idProf, "Cadastro de turma", dataAtual()); // Adicionando à lista encadeada um registro
            criarTurma(idProf);
            break;

        case '2':
            system("cls");
            adicionarListaRegistro(&lista, idProf, "Edicao de turma", dataAtual()); // Adicionando à lista encadeada um registro
            editarTurma(idProf);
            break;
        
        case '3':
            system("cls");
            adicionarListaRegistro(&lista, idProf, "Exclusao de turma", dataAtual()); // Adicionando à lista encadeada um registro
            excluirTurma(idProf);
            break;

        case '4':
            system("cls");
            adicionarListaRegistro(&lista, idProf, "Visualizacao de turma", dataAtual()); // Adicionando à lista encadeada um registro
            verTurmas(idProf);
            break;

        case '5':
            system("cls");
            adicionarListaRegistro(&lista, idProf, "Visualizacao de alunos em turma", dataAtual()); // Adicionando à lista encadeada um registro
            verAlunosTurma(idProf);
            break;
        
        case '6':
            break;

        default:
            break;
        }
    }

    // Registrando os dados adicionados da lista encadeada no arquivo txt, e liberando a memória
    criarRegistroTxt(lista); 
    liberarMemLista(lista);
}

// Tela com as funcionalidades envolvendo Alunos
void telaAluno(int idProf){
    Registro *lista = NULL; // lista encadeada de registros

    char inputBuffer[100];
    char opcaoTelaAlunos;
    
    while(opcaoTelaAlunos != '7'){
        system("cls");

        printf("\n---------------------------------------------------------\n");
        printf("(1) CADASTRAR ALUNO\n");
        printf("(2) EDITAR ALUNO\n");
        printf("(3) EXCLUIR ALUNO\n");
        printf("(4) VIZUALIZAR ALUNOS\n");
        printf("(5) VIZUALIZAR ALUNO (Matricula)\n");
        printf("(6) VIZUALIAR ALUNO (Nome)\n");
        printf("(7) SAIR \n");

        fgets(inputBuffer, sizeof(inputBuffer), stdin); 
        sscanf(inputBuffer, "%c", &opcaoTelaAlunos); 

        switch (opcaoTelaAlunos){
        case '1':
            system("cls");
            adicionarListaRegistro(&lista, idProf, "Cadastro de aluno", dataAtual());
            criarAluno(idProf);
            break;

        case '2':
            system("cls");
            adicionarListaRegistro(&lista, idProf, "Edicao de aluno", dataAtual());
            editarAluno(idProf);
            break;
        
        case '3':
            system("cls");
            adicionarListaRegistro(&lista, idProf, "Exclusao de aluno", dataAtual());
            excluirAluno();
            break;

        case '4':
            system("cls");
            adicionarListaRegistro(&lista, idProf, "Visualizacao de alunos", dataAtual());
            verAlunos(idProf);
            break;

        case '5':
            system("cls");
            adicionarListaRegistro(&lista, idProf, "Visualizacao de aluno (M)", dataAtual());
            verAlunoMatricula(idProf);
            break;

        case '6':
            system("cls");
            adicionarListaRegistro(&lista, idProf, "Visualizacao de aluno (N)", dataAtual());
            verAlunoNome(idProf);
            break;
        
        case '7':
            break;

        default:
            break;
        }
    }

    // Registrando os dados adicionados da lista encadeada no arquivo txt, e liberando a memória
    criarRegistroTxt(lista); 
    liberarMemLista(lista);
}

// Tela com as funcionalidades de registro de Aulas e Notas
void telaNotasFreq(int idProfessor){
    Registro *lista = NULL; // lista encadeada de registros

    char inputBuffer[100];
    char opcaoTelaNotasFreq;
    
    while(opcaoTelaNotasFreq != '3'){
        system("cls");

        printf("\n---------------------------------------------------------\n");
        printf("(1) CADASTRAR AULA\n");
        printf("(2) LANCAR NOTAS\n");
        printf("(3) SAIR \n");

        fgets(inputBuffer, sizeof(inputBuffer), stdin); 
        sscanf(inputBuffer, "%c", &opcaoTelaNotasFreq); 

        switch (opcaoTelaNotasFreq){
        case '1':
            system("cls");
            adicionarListaRegistro(&lista, idProfessor, "Cadastro de aula", dataAtual());
            lancarFreq(idProfessor);
            break;

        case '2':
            system("cls");
            adicionarListaRegistro(&lista, idProfessor, "Insercao de nota", dataAtual());
            lancarNotas(idProfessor);
            break;
        
        case '3':
            break;
        
        default:
            break;
        }
    }

    // Registrando os dados adicionados da lista encadeada no arquivo txt, e liberando a memória
    criarRegistroTxt(lista); 
    liberarMemLista(lista);
}

// Tela com as funcionalidades de exibição de Boletins
void telaBoletins(int idProf){
    Registro *lista = NULL; // lista encadeada de registros

    char inputBuffer[100];
    char opcaoTelaBoletins;
    
    while(opcaoTelaBoletins != '3'){
        system("cls");
        printf("\n---------------------------------------------------------\n");
        printf("(1) BOLETIM ALUNO\n");
        printf("(2) BOLETIM TURMA\n");
        printf("(3) SAIR \n");

        fgets(inputBuffer, sizeof(inputBuffer), stdin); 
        sscanf(inputBuffer, "%c", &opcaoTelaBoletins); 
        
        switch (opcaoTelaBoletins){
        case '1':
            system("cls");
            adicionarListaRegistro(&lista, idProf, "Boletim aluno", dataAtual());
            boletimAluno(idProf);
            break;

        case '2':
            system("cls");
            adicionarListaRegistro(&lista, idProf, "Boletim turma", dataAtual());
            boletimTurma(idProf);
            break;
        
        case '3':
            break;
        
        default:
            break;
        }
    }

    // Registrando os dados adicionados da lista encadeada no arquivo txt, e liberando a memória
    criarRegistroTxt(lista); 
    liberarMemLista(lista);
}

// Função principal do programa
int main() {
    // Buffer e opcaoMenu que serao utilizados na leitura de dados
    char inputBuffer[100];
    char opcaoMenu;
    
    while(opcaoMenu != '4') {
        system("cls");

        printf("\n---------------------------------------------------------\n");
        printf("(1) Cadastrar novo professor\n");
        printf("(2) Ver professores\n");
        printf("(3) Login\n");
        printf("(4) Sair \n");

        fgets(inputBuffer, sizeof(inputBuffer), stdin); 
        sscanf(inputBuffer, "%c", &opcaoMenu); 

        switch (opcaoMenu){
            case '1':
                system("cls");
                criarProfessor();
                break;
            
            case '2':
                system("cls");
                verProfessores();
                break;

            case '3':
                system("cls");
                loginProfessor();
            
            case '4':
                break;

            default:
                break;

        }
    }
    return 0;
}