# organizador acadêmico

este projeto implementa um sistema de gerenciamento acadêmico em linguagem c. ele é projetado para auxiliar professores na administração de turmas, alunos e disciplinas, fornecendo funcionalidades como:  

## funcionalidades
- **cadastro de professores, turmas e alunos**  
- **gerenciamento de disciplinas e notas**: registro e edição de notas dos alunos.  
- **frequência e controle de aulas**: acompanhamento das aulas ministradas e faltas dos alunos.  
- **exibição de boletins e médias**: apresentação de informações detalhadas de desempenho individual e por turma.  

## estrutura do projeto  
o código utiliza um conjunto de `structs` para representar entidades como professores, alunos, turmas e disciplinas. todas as operações principais são realizadas por meio de funções que manipulam arquivos binários para armazenamento persistente dos dados.  

## como executar  
1. compile o arquivo `organizador.c` utilizando um compilador c compatível.  
2. configure o ambiente de execução com base no arquivo `launch.json`, caso utilize um ambiente de depuração como o vs code.  
3. execute o programa e navegue pelos menus interativos para acessar as funcionalidades.  

## melhorias futuras  
- implementação de interface gráfica.  
- suporte para integração com plataformas online.  
- otimizações na manipulação de arquivos para maior desempenho.  
