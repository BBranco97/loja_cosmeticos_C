//BIBLIOTECAS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<locale.h>
#include <time.h>
#include<ctype.h>//coloca o char do genero em min�sculo 

//CONSTANTES

//clientes
#define MAX_CHAR_NOME 100
#define MAX_EMAILS 10
#define MAX_CHAR_EMAILS 256
#define MAX_TELEFONES 10
#define MAX_CHAR_TELEFONES 14
#define MAX_CLIENTES 100 //definido com base no tamanho max que o arquivo pode ter 
//produtos
#define MAX_CHAR_CODIGO 5
#define MAX_CHAR_DESCRICAO 200
#define MAX_PRODUTOS 100 //definido com base no tamanho max que o arquivo pode ter 
//compra/vendas
#define MAX_COMPRAVENDAS 100 //definido com base no tamanho max que o arquivo pode ter 


//ESTRUTURAS

struct hora{
	int hora;
	int minuto;
};

struct Cliente {
    char cpf[15];
    char nome[MAX_CHAR_NOME];
   	struct tm dataNascimento;
    char sexo;
    float salario;
    int qntEmails;
    char emails[MAX_EMAILS][MAX_CHAR_EMAILS];
    int qntTelefones;
    char telefones[MAX_TELEFONES][MAX_CHAR_TELEFONES];
};

struct Produto {
    char codigo[MAX_CHAR_CODIGO];
    char descricao[MAX_CHAR_DESCRICAO];
    int qnt;
    float preco;
    int desconto; // porcentagem
    struct tm validade;
};

struct CompraVenda {

    char cpfCliente[15];
    char codigoProduto[MAX_CHAR_CODIGO];
    struct tm dataTransacao; 
    struct hora horaTransacao;
    double valor;
};


//FUN��ES DE USO GERAL




void Menu(){
	printf("\n_________________________\n");
	printf("\n\t MENU \t\n");
	printf("\n_________________________\n");
	printf("\nEscolha uma op��o");
	printf("\n1. Submenu de Clientes");
	printf("\n2. Submenu de Produtos");
	printf("\n3. Submenu de Compra/Venda");
	printf("\n4. Submenu de Relat�rios");
	printf("\n5. Sair");
	printf("\n_________________________\n");
}

void Submenu(){
	printf("\n_________________________\n");
	printf("\n1. Listar Todos");
	printf("\n2. Listar Um");
	printf("\n3. Incluir (sem repeti��o)");
	printf("\n4. Alterar");
	printf("\n5. Excluir");
	printf("\n6. Voltar ao menu principal");
	printf("\n_________________________\n");
}

void limparBuffer(){
    int c;
    while ((c = getchar()) != '\n');
}



//CLIENTES



//arquivos

void abreArqClientes(struct Cliente **cadastroClientes, int *contClientes) {
    FILE *arquivo;
    int numClientes;

    arquivo = fopen("clientes.bin", "rb"); // modo de leitura 
    
	if (arquivo == NULL) {	
	
        // cria um novo arquivo vazio    
        arquivo = fopen("clientes.bin", "wb"); 
        
        if (arquivo == NULL) {
            printf("Erro ao criar o arquivo de clientes.\n");
            return;
        }


        numClientes = 0;
        *contClientes = numClientes;
        // nao precisa alocar memoria 
        
    } 
    
	else {
        // o arquivo existe
        
		// pega a quantidade de clientes no arquivo
        fseek(arquivo, 0, SEEK_END);
        long fileSize = ftell(arquivo);
        rewind(arquivo);
        numClientes = fileSize / sizeof(struct Cliente);
        *contClientes = numClientes;

        // aloca mem�ria para o vetor de clientes
        *cadastroClientes = (struct Cliente *)malloc(numClientes * sizeof(struct Cliente));
        if (*cadastroClientes == NULL) {
            printf("Erro ao alocar mem�ria para o vetor de clientes\n");
            fclose(arquivo);
            return;
        }

        // le os dados do arquivo e coloca no vetor de clientes
        fread(*cadastroClientes, sizeof(struct Cliente), numClientes, arquivo);
    }

    fclose(arquivo);
}

void salvarArqClientes(struct Cliente **cadastroClientes, int *contClientes) {
    FILE *arquivo;
    
    arquivo = fopen("clientes.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de clientes para escrita\n");
        return;
    }
    
    // escreve os dados do vetor de clientes no arquivo
    fwrite(*cadastroClientes, sizeof(struct Cliente), *contClientes, arquivo);
    
    fclose(arquivo);
}


//imprimir

void imprimirCliente(struct Cliente **cadastroClientes, int i) {
    char dataNascimentoStr[20]; // string para armazenar a data

    printf("CLIENTE %d\n", i + 1);
    printf("Nome: %s\n", (*cadastroClientes)[i].nome);
    printf("CPF: %s\n", (*cadastroClientes)[i].cpf);

    // converte struct tm para string
    strftime(dataNascimentoStr, sizeof(dataNascimentoStr), "%Y-%m-%d", &(*cadastroClientes)[i].dataNascimento);
    printf("Data de Nascimento: %s\n", dataNascimentoStr);

    printf("Sexo: %c\n", (*cadastroClientes)[i].sexo);
    printf("Sal�rio: %.2f\n", (*cadastroClientes)[i].salario);
    printf("Emails:\n");
    for (int j = 0; j < (*cadastroClientes)[i].qntEmails; j++) {
        printf("-> %s\n", (*cadastroClientes)[i].emails[j]);
    }
    printf("Telefones:\n");
    for (int j = 0; j < (*cadastroClientes)[i].qntTelefones; j++) {
        printf("-> %s\n", (*cadastroClientes)[i].telefones[j]);
    }
    printf("\n_________________________\n");
}


//buscar

int buscarCliente(struct Cliente **cadastroClientes, int cont, char cpf[]) {
    int i;
    
    for (i = 0; i < cont; i++) {
        if (strcmp((*cadastroClientes)[i].cpf, cpf) == 0) {
            return i; // cliente encontrado
        }
    }
    
    return -1; // cliente n�o encontrado
}

//listar todos

void listarTodosClientes(struct Cliente **cadastroClientes, int cont) {
    int i, j;
	
    printf("\nLISTA DE CLIENTES:\n");
	
    for (i = 0; i < cont; i++) {
        imprimirCliente(cadastroClientes, i);
    }
}


//listar-um

void listarUmCliente(struct Cliente **cadastroClientes, int cont) {
    int i;
    char cpf[15]; //para armazenar o CPF

    printf("Insira o CPF: ");
    scanf("%s", cpf);
    limparBuffer();
    system("cls");

    i = buscarCliente(cadastroClientes, cont, cpf);
    if (i != -1) {
        imprimirCliente(cadastroClientes, i);
    } else {
        printf("\nCliente n�o encontrado\n");
    }
}

//incluir

void incluirCliente(struct Cliente **cadastroClientes, int *qntClientes) {
    struct Cliente novoCliente;
    char cpf[15];
    char dataNascimentoStr[11]; // para armazenar a data temporariamente
    int ano, mes, dia; // para dividir a data
    int i;
    int realocacao;
    
    if (*qntClientes == MAX_CLIENTES) {
        printf("Limite de cadastro de clientes atingido\n");
        return;
    }
    
    realocacao = *qntClientes;
    realocacao++;
    
    // realoca��o de mem�ria
    *cadastroClientes = (struct Cliente *)realloc(*cadastroClientes, realocacao * sizeof(struct Cliente));
    if (*cadastroClientes == NULL) {
        printf("Erro na realoca��o de mem�ria\n");
        return;
    }
    
    printf("Insira o CPF: ");
    scanf("%s", cpf);
    limparBuffer();

    i = buscarCliente(cadastroClientes, *qntClientes, cpf);
    if (i != -1) {
        printf("Cliente j� cadastrado.\n");
        return;
    }

    strcpy(novoCliente.cpf, cpf);

    printf("Insira o nome: ");
    scanf("%s", novoCliente.nome);
    limparBuffer();

    printf("Insira a data de nascimento (formato: AAAA-MM-DD): ");
    scanf("%10s", dataNascimentoStr); // armazena na string
    limparBuffer();

    sscanf(dataNascimentoStr, "%d-%d-%d", &ano, &mes, &dia); // divide a string

    novoCliente.dataNascimento.tm_year = ano - 1900; // atualizacoes para a biblioteca
    novoCliente.dataNascimento.tm_mon = mes - 1;
    novoCliente.dataNascimento.tm_mday = dia;

    printf("Insira o sexo (M/F/O): ");
    scanf("%c", &novoCliente.sexo);
    novoCliente.sexo = tolower(novoCliente.sexo);

    printf("Insira o sal�rio: ");
    scanf("%f", &novoCliente.salario);

    printf("Insira a quantidade de e-mails: ");
    scanf("%d", &novoCliente.qntEmails);

    for (i = 0; i < novoCliente.qntEmails; i++) {
        printf("Insira o e-mail %d: ", i + 1);
        scanf("%s", novoCliente.emails[i]);
        limparBuffer();
    }

    printf("Insira a quantidade de telefones: ");
    scanf("%d", &novoCliente.qntTelefones);

    for (i = 0; i < novoCliente.qntTelefones; i++) {
        printf("Insira o telefone %d: ", i + 1);
        scanf("%s", novoCliente.telefones[i]);
        limparBuffer();
    }

    (*cadastroClientes)[*qntClientes] = novoCliente;
    (*qntClientes)++;
} 


//alterar

void alterarCadastroCliente(struct Cliente **cadastroClientes, int cont) {
    int i;
    char cpf[15];
    char dataNascimentoStr[11]; // para armazenar a data de nascimento
    int ano, mes, dia; // divisao da data

    printf("Insira o CPF: ");
    scanf("%s", cpf);
    limparBuffer();

    i = buscarCliente(cadastroClientes, cont, cpf);

    if (i == -1) {
        printf("Cliente n�o encontrado.\n");
        return;
    }

    struct Cliente *cliente = &(*cadastroClientes)[i]; // faz um ponteiro que aponta para o cliente selecionado

    printf("Novo nome: ");
    scanf("%s", cliente->nome);
    limparBuffer();

    //data de nascimento
    printf("Nova data de nascimento (formato: AAAA-MM-DD): ");
    scanf("%10s", dataNascimentoStr); // le como uma string
    limparBuffer();

    //converte para 3 int
    sscanf(dataNascimentoStr, "%d-%d-%d", &ano, &mes, &dia);

    //coloca na struct
    cliente->dataNascimento.tm_year = ano - 1900; // ano ajustado para a biblioteca
    cliente->dataNascimento.tm_mon = mes - 1; // mes ajustado para a biblioteca
    cliente->dataNascimento.tm_mday = dia;

    printf("Novo sexo (M/F/O): ");
    scanf(" %c", &cliente->sexo);
    cliente->sexo = tolower(cliente->sexo); // altera para min�sculo
    limparBuffer();

    printf("Novo sal�rio: ");
    scanf("%f", &cliente->salario);
    limparBuffer();

    printf("Nova quantidade de e-mails: ");
    scanf("%d", &cliente->qntEmails);
    limparBuffer();

    for (int i = 0; i < cliente->qntEmails; i++) {
        printf("Novo e-mail %d: ", i + 1);
        scanf("%s", cliente->emails[i]);
        limparBuffer();
    }

    printf("Nova quantidade de telefones: ");
    scanf("%d", &cliente->qntTelefones);
    limparBuffer();

    for (int i = 0; i < cliente->qntTelefones; i++) {
        printf("Novo telefone %d: ", i + 1);
        scanf("%s", cliente->telefones[i]);
        limparBuffer();
    }
}


//excluir

void excluirCadastroCliente(struct Cliente **cadastroClientes, int *qntClientes) {
    int i, indice, realocacao;
    char cpf[15];

    printf("Insira o CPF: ");
    scanf("%s", cpf);
    limparBuffer();

    indice = buscarCliente(cadastroClientes, *qntClientes, cpf);

    if (indice == -1) {
        printf("Cliente n�o encontrado.\n");
        return;
    }

    // movendo os clientes para preencher o espa�o vazio
    for (i = indice; i < *qntClientes - 1; i++) {
        (*cadastroClientes)[i] = (*cadastroClientes)[i + 1];
    }

    // reduz o tamanho do vetor em 1
    realocacao = *qntClientes;
    realocacao--;
    *cadastroClientes = (struct Cliente *)realloc(*cadastroClientes, realocacao * sizeof(struct Cliente));

    (*qntClientes)--;
}
	
//submenu

void SubmenuClientes(struct Cliente **cadastroClientes, int *qntClientes){
    int opcao, cont;
    
    do{
        printf("\n_________________________\n");
        printf("\n\t CLIENTES \t\n");
        Submenu();
    
        scanf("%d", &opcao);
        
        // usado para verificar se houve altera��o no n�mero (confirmar exclus�o e inclus�o)
        cont = *qntClientes;
        
        switch(opcao){
            case 1: // listar todos
                system("cls");
                if(cont == 0){
                    printf("\nN�o h� clientes cadastrados");
                }
                else{
                    listarTodosClientes(cadastroClientes, cont);
                }
            
                break;
            
            case 2: // listar um
                system("cls");
                if(cont == 0){
                    printf("\nN�o h� clientes cadastrados");
                }
                else{
                    listarUmCliente(cadastroClientes, cont);
                }
                
                break;
            
            case 3: // incluir
                system("cls");
                incluirCliente(cadastroClientes, qntClientes);
                
                if((cont + 1) == (*qntClientes)){
                    printf("\nCliente cadastrado com sucesso");
                }
                else{
                    printf("\nN�o foi poss�vel adicionar o cliente");
                }
                
                break;
            
            case 4: // alterar
                system("cls");
                if(cont == 0){
                    printf("\nN�o h� clientes cadastrados");
                }
                else{
                    alterarCadastroCliente(cadastroClientes, cont);
                    printf("\nCadastro Alterado com sucesso");
                }
                
                break;
            
            case 5: // excluir
                system("cls");
                if(cont == 0){
                    printf("\nN�o h� clientes cadastrados");
                }
                else{
                    excluirCadastroCliente(cadastroClientes, qntClientes);
                    if((cont - 1) == *qntClientes){
                        printf("\nCliente exclu�do com sucesso");
                    }
                    else{
                        printf("\nN�o foi poss�vel excluir o cliente");
                    }
                }
                    
                break;
            
            case 6: // volta ao menu principal
                system("cls");
                printf("\nVoltando ao menu principal");
                
                break;
            
            default: // op��o inv�lida
                system("cls");
                printf("\nOp��o Inv�lida");
                
                break;
        }
        
    } while(opcao != 6);
}	
	


//PRODUTOS



//arquivos

void abreArqProdutos(struct Produto **cadastroProdutos, int *contProdutos) {
    FILE *arquivo;
    int numProdutos;

    arquivo = fopen("produtos.bin", "rb"); // modo de leitura 
    
    if (arquivo == NULL) {
        // cria um arquivo
        
        arquivo = fopen("produtos.bin", "wb"); 
        if (arquivo == NULL) {
            printf("Erro ao criar o arquivo de produtos.\n");
            return;
        }

        numProdutos = 0;
        *contProdutos = numProdutos;

    } 
	
	else {
        // quantidade de produtos no arquivo
        
        fseek(arquivo, 0, SEEK_END);
        long fileSize = ftell(arquivo);
        rewind(arquivo);
        numProdutos = fileSize / sizeof(struct Produto);
        *contProdutos = numProdutos;

        // aloca mem�ria para o vetor de produtos
        *cadastroProdutos = (struct Produto *)malloc(numProdutos * sizeof(struct Produto));
        if (*cadastroProdutos == NULL) {
            printf("Erro ao alocar mem�ria para o vetor de produtos\n");
            fclose(arquivo);
            return;
        }

        // le os dados do arquivo e coloca no vetor 
        fread(*cadastroProdutos, sizeof(struct Produto), numProdutos, arquivo);
    }

    fclose(arquivo);
}

void salvarArqProdutos(struct Produto **cadastroProdutos, int *contProdutos) {
    FILE *arquivo;
    
    arquivo = fopen("produtos.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de produtos para escrita\n");
        return;
    }
    
    // escreve os dados do vetor de produtos no arquivo
    fwrite(*cadastroProdutos, sizeof(struct Produto), *contProdutos, arquivo);
    
    fclose(arquivo);
}


//imprimir

void imprimirProduto(struct Produto **cadastroProdutos, int i) {
    char dataValidadeStr[20];

    printf("PRODUTO %d\n", i + 1);
    printf("C�digo: %s\n", (*cadastroProdutos)[i].codigo);
    printf("Descri��o: %s\n", (*cadastroProdutos)[i].descricao);
    printf("Quantidade: %d\n", (*cadastroProdutos)[i].qnt);
    printf("Pre�o: %.2f\n", (*cadastroProdutos)[i].preco);
    printf("Desconto: %d%%\n", (*cadastroProdutos)[i].desconto);

    // converte para string
    strftime(dataValidadeStr, sizeof(dataValidadeStr), "%Y-%m-%d", &(*cadastroProdutos)[i].validade);
    printf("Validade: %s\n", dataValidadeStr);

    printf("\n_________________________\n");
}

//buscar

int buscarProduto(struct Produto **cadastroProdutos, int cont, char codigo[]) {
    int i;

    for (i = 0; i < cont; i++) {
        if (strcmp((*cadastroProdutos)[i].codigo, codigo) == 0) { // compara strings
            return i; // produto encontrado
        }
    }
    return -1; // produto n�o encontrado
}

//listar todos

void listarTodosProdutos(struct Produto **cadastroProdutos, int cont) {
    int i;

    printf("\nLISTA DE PRODUTOS:\n");

    for (i = 0; i < cont; i++) {
        imprimirProduto(cadastroProdutos, i);
    }
}

//listar-um

void listarUmProduto(struct Produto **cadastroProdutos, int cont) {
    int i;
    char codigo[MAX_CHAR_CODIGO]; // para armazenar o c�digo

    printf("Insira o c�digo do produto: ");
    scanf("%s", codigo);
    limparBuffer();
    system("cls");

    i = buscarProduto(cadastroProdutos, cont, codigo);
    if (i != -1) {
        imprimirProduto(cadastroProdutos,i);
    } else {
        printf("\nProduto n�o encontrado\n");
    }
}

//incluir

void incluirProduto(struct Produto **cadastroProdutos, int *qntProdutos) {
    struct Produto novoProduto;
    char codigo[MAX_CHAR_CODIGO];
    char dataValidadeStr[11]; // para armazenar a data temporariamente
    int dia, mes, ano; // para dividir a data
    int i;
    int realocacao;

    if (*qntProdutos == MAX_PRODUTOS) {
        printf("Limite de produtos atingido\n");
        return;
    }

    printf("Insira o c�digo: ");
    scanf("%s", codigo);
    limparBuffer();

    i = buscarProduto(cadastroProdutos, *qntProdutos, codigo);
    if (i != -1) {
        printf("Produto j� cadastrado.\n");
        return;
    }

    realocacao = *qntProdutos;
    realocacao++;

    // realoca��o de memoria
    *cadastroProdutos = (struct Produto *)realloc(*cadastroProdutos, realocacao * sizeof(struct Produto));
    if (*cadastroProdutos == NULL) {
        printf("Erro na realoca��o de mem�ria\n");
        return;
    }

    strcpy(novoProduto.codigo, codigo);

    printf("Insira a descri��o: ");
    scanf("%s", novoProduto.descricao);
    limparBuffer();

    printf("Insira a quantidade: ");
    scanf("%d", &novoProduto.qnt);

    printf("Insira o pre�o: ");
    scanf("%f", &novoProduto.preco);

    printf("Insira o desconto (%%): ");
    scanf("%d", &novoProduto.desconto);

    printf("Insira a data de validade (formato: AAAA-MM-DD): ");
    scanf("%10s", dataValidadeStr); // armazena na string
    limparBuffer();

    sscanf(dataValidadeStr, "%d-%d-%d", &ano, &mes, &dia); // divide a string

    novoProduto.validade.tm_year = ano - 1900; // atualiza��es para a biblioteca
    novoProduto.validade.tm_mon = mes - 1;
    novoProduto.validade.tm_mday = dia;

    (*cadastroProdutos)[*qntProdutos] = novoProduto;
    (*qntProdutos)++;
}

//alterar

void alterarCadastroProduto(struct Produto **cadastroProdutos, int cont) {
    int i;
    char codigo[MAX_CHAR_CODIGO];
    char dataValidadeStr[11]; // para armazenar a data de validade em string
    int ano, mes, dia; // para a divis�o da data

    printf("Insira o c�digo do produto: ");
    scanf("%s", codigo);

    i = buscarProduto(cadastroProdutos, cont, codigo);

    if (i == -1) {
        printf("Produto n�o encontrado.\n");
        return;
    }

    struct Produto *produto = &(*cadastroProdutos)[i]; // faz um ponteiro que aponta para o produto selecionado

    printf("Nova descri��o: ");
    scanf("%s", produto->descricao);
    limparBuffer();

    printf("Nova quantidade: ");
    scanf("%d", &produto->qnt);

    printf("Novo pre�o: ");
    scanf("%f", &produto->preco);

    printf("Novo desconto (%%): ");
    scanf("%d", &produto->desconto);

    printf("Nova data de validade (formato: AAAA-MM-DD): ");
    scanf("%10s", dataValidadeStr); // data de validade como string
    limparBuffer();

    sscanf(dataValidadeStr, "%d-%d-%d", &ano, &mes, &dia); // divide a string

    produto->validade.tm_year = ano - 1900; // atualiza para a biblioteca
    produto->validade.tm_mon = mes - 1;
    produto->validade.tm_mday = dia;
}

//excluir

void excluirCadastroProduto(struct Produto **cadastroProdutos, int *qntProdutos) {
    int i, indice, realocacao;
    char codigo[MAX_CHAR_CODIGO];

    printf("Insira o c�digo do produto: ");
    scanf("%s", codigo);
    limparBuffer();

    indice = buscarProduto(cadastroProdutos, *qntProdutos, codigo);

    if (indice == -1) {
        printf("Produto n�o encontrado.\n");
        return;
    }

    // movendo para preencher o espa�o vazio
    for (i = indice; i < *qntProdutos - 1; i++) {
        (*cadastroProdutos)[i] = (*cadastroProdutos)[i + 1];
    }

    // reduz o tamanho do vetor em 1
    realocacao = *qntProdutos;
    realocacao--;
    *cadastroProdutos = (struct Produto *)realloc(*cadastroProdutos, realocacao * sizeof(struct Produto));

    (*qntProdutos)--;
}
	
//submenu

void SubmenuProdutos(struct Produto **cadastroProdutos, int *qntProdutos) {
    int opcao, cont;

    do {
        printf("\n_________________________\n");
        printf("\n\t PRODUTOS \t\n");
        Submenu();

        scanf("%d", &opcao);
		
        // usado para verificar se houve altera��o no numero (confirmar exclus�o e inclus�o)
        cont = *qntProdutos;

        switch (opcao) {
        	
            case 1: // listar todos
				system("cls");
                if (cont == 0) {
                    printf("\nN�o h� produtos cadastrados");
                } else {
                    listarTodosProdutos(cadastroProdutos, cont);
                }

                break;

            case 2: // listar um
				system("cls");
                if (cont == 0) {
                    printf("\nN�o h� produtos cadastrados");
                } else {
                    listarUmProduto(cadastroProdutos, cont);
                }

                break;

            case 3: // incluir
				system("cls");
                incluirProduto(cadastroProdutos, qntProdutos);

                if ((cont + 1) == (*qntProdutos)) {
                    printf("\nProduto cadastrado com sucesso");
                } else {
                    printf("\nN�o foi poss�vel adicionar o produto");
                }

                break;

            case 4: // alterar
				system("cls");
                if (cont == 0) {
                    printf("\nN�o h� produtos cadastrados");
                } else {
                    alterarCadastroProduto(cadastroProdutos, cont);
                    printf("\nCadastro alterado com sucesso");
                }

                break;

            case 5: // excluir
				system("cls");
                if (cont == 0) {
                    printf("\nN�o h� produtos cadastrados");
                } else {
                    excluirCadastroProduto(cadastroProdutos, qntProdutos);
                    if ((cont - 1) == *qntProdutos) {
                        printf("\nProduto exclu�do com sucesso");
                    } else {
                        printf("\nN�o foi poss�vel excluir o produto");
                    }
                }

                break;

            case 6: // voltar ao menu principal
				system("cls");
                printf("\nVoltando ao menu principal");

                break;

            default: // op��o inv�lida
				system("cls");
                printf("\nOp��o Inv�lida");

                break;
        }

    } while (opcao != 6);
}


//COMPRA/VENDA


//arquivos

void abreArqCompraVendas(struct CompraVenda **cadastroCompraVendas, int *contCompraVendas) {
    FILE *arquivo;
    int numCompraVendas;

    arquivo = fopen("compras_vendas.bin", "rb"); // modo de leitura 
    
    if (arquivo == NULL) {
        //cria um novo arquivo vazio
        arquivo = fopen("compras_vendas.bin", "wb"); 
        
        if (arquivo == NULL) {
            printf("Erro ao criar o arquivo de compras e vendas.\n");
            return;
        }

        numCompraVendas = 0;
        *contCompraVendas = numCompraVendas;

        //pois nao ha dados para ler do arquivo vazio
        //nao precisa alocar a memoria    
    }
	
	 else {
	 	
        //pega a quantidade de compras e vendas no arquivo
        fseek(arquivo, 0, SEEK_END);
        long fileSize = ftell(arquivo);
        rewind(arquivo);
        numCompraVendas = fileSize / sizeof(struct CompraVenda);
        *contCompraVendas = numCompraVendas;

        // aloca mem�ria para o vetor
        *cadastroCompraVendas = (struct CompraVenda *)malloc(numCompraVendas * sizeof(struct CompraVenda));
        if (*cadastroCompraVendas == NULL) {
            printf("Erro ao alocar mem�ria para o vetor de compras e vendas\n");
            fclose(arquivo);
            return;
        }

        //coloca os dados do arquivo e no vetor
        fread(*cadastroCompraVendas, sizeof(struct CompraVenda), numCompraVendas, arquivo);
    }

    fclose(arquivo);
}


void salvarArqCompraVendas(struct CompraVenda **cadastroCompraVendas, int *contCompraVendas) {
    FILE *arquivo;
    
    arquivo = fopen("compras_vendas.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de compras e vendas para escrita.\n");
        return;
    }
    
    // escreve os dados do vetor no arquivo
    fwrite(*cadastroCompraVendas, sizeof(struct CompraVenda), *contCompraVendas, arquivo);
    
    fclose(arquivo);
}

//imprimir

void imprimirCompraVenda(struct CompraVenda **cadastroCompraVendas, int i) {
    char dataTransacaoStr[20];
    char horaTransacaoStr[8];

    printf("TRANSA��O %d\n", i + 1);
    printf("CPF Cliente: %s\n", (*cadastroCompraVendas)[i].cpfCliente);
    printf("C�digo Produto: %s\n", (*cadastroCompraVendas)[i].codigoProduto);

    // Converte struct tm para string
    strftime(dataTransacaoStr, sizeof(dataTransacaoStr), "%Y-%m-%d", &((*cadastroCompraVendas)[i].dataTransacao));
    printf("Data da Transa��o: %s\n", dataTransacaoStr);

	// Converte struct hora para string 
    sprintf(horaTransacaoStr, "%02d:%02d", (*cadastroCompraVendas)[i].horaTransacao.hora, (*cadastroCompraVendas)[i].horaTransacao.minuto);
    // armazena a sequ�ncia formatada em uma string
    printf("Hora da Transa��o: %s\n", horaTransacaoStr);

    printf("Valor: R$ %.2f\n", (*cadastroCompraVendas)[i].valor);

    printf("\n_________________________\n");
}


//buscar
//mandar data da transa��o como struct

int buscarCompraVenda(struct CompraVenda **cadastroCompraVendas, int cont, char cpfCliente[], char codigoProduto[], struct tm dataTransacao, struct hora horaTransacao) {
    int i;
    int dia, mes, ano, hora, minuto;

    ano = dataTransacao.tm_year;
    mes = dataTransacao.tm_mon;
    dia = dataTransacao.tm_mday;
    hora = horaTransacao.hora;
    minuto = horaTransacao.minuto;

    for (i = 0; i < cont; i++) {
        if (strcmp((*cadastroCompraVendas)[i].cpfCliente, cpfCliente) == 0 &&
            strcmp((*cadastroCompraVendas)[i].codigoProduto, codigoProduto) == 0 &&
            ((*cadastroCompraVendas)[i].dataTransacao.tm_year == ano) &&
            ((*cadastroCompraVendas)[i].dataTransacao.tm_mon == mes) &&
            ((*cadastroCompraVendas)[i].dataTransacao.tm_mday == dia) &&
            ((*cadastroCompraVendas)[i].horaTransacao.hora == hora) &&
            ((*cadastroCompraVendas)[i].horaTransacao.minuto == minuto)) {
                
            return i; // Compra/Venda encontrada
        }
    }

    return -1; // Compra/Venda n�o encontrada
}

//listar todos

void listarTodasCompraVendas(struct CompraVenda **cadastroCompraVendas, int cont) {
    int i;

    printf("\nLISTA DE TRANSA��ES:\n");

    for (i = 0; i < cont; i++) {
        imprimirCompraVenda(cadastroCompraVendas, i);
    }
}

//listar-um

void listarUmaCompraVenda(struct CompraVenda **cadastroCompraVendas, int cont) {
    int i;
    char codigoProduto[MAX_CHAR_CODIGO]; //para armazenar o c�digo
    char cpfCliente[15]; //para armazenar cpf
    char dataTransacaoStr[20];
    char horaTransacaoStr[8];
    struct tm dataTransacaoTemp; //para enviar a data
    struct hora horaTemp; //para enviar a hora
    int dia, mes, ano, hora, minuto; //para separar a data e hora

    printf("\nInsira o CPF do cliente: ");
    scanf("%s", cpfCliente);
    limparBuffer();

    printf("\nInsira o c�digo do produto: ");
    scanf("%s", codigoProduto);
    limparBuffer();

    printf("\nInsira a data da transa��o (formato: AAAA-MM-DD): ");
    scanf("%16s", dataTransacaoStr);
    limparBuffer();

    printf("\nInsira a hora da transa��o (formato: HH:MM): ");
    scanf("%2d:%2d", &hora, &minuto);
    limparBuffer();

    system("cls");

    sscanf(dataTransacaoStr, "%d-%d-%d", &ano, &mes, &dia); // divide a string

    dataTransacaoTemp.tm_year = ano - 1900; // coloca na struct e faz atualiza��es para a biblioteca
    dataTransacaoTemp.tm_mon = mes - 1;
    dataTransacaoTemp.tm_mday = dia;
    
    horaTemp.hora = hora; //coloca na struct
    horaTemp.minuto = minuto;

    i = buscarCompraVenda(cadastroCompraVendas, cont, cpfCliente, codigoProduto, dataTransacaoTemp, horaTemp);
    if (i != -1) {
        imprimirCompraVenda(cadastroCompraVendas, i);
    } else {
        printf("\nTransa��o n�o encontrada\n");
    }
}

//incluir

void incluirCompraVenda(struct CompraVenda **cadastroCompraVendas, int *qntCompraVendas, struct Cliente **cadastroClientes, int *qntClientes, struct Produto **cadastroProdutos, int *qntProdutos) {

    struct CompraVenda novaCompraVenda;
    struct tm dataTransacaoTemp;
    struct hora horaTransacaoTemp;

    char cpf[15];
    char codigo[MAX_CHAR_CODIGO];
    char dataTransacaoStr[20]; // armazena data temporariamente
    char horaTransacaoStr[8]; // armazena hora temporariamente

    int dia, mes, ano; // divide a data
    int hora, minuto; // divide a hora
    int i;
    int realocacao;

    if (*qntCompraVendas == MAX_COMPRAVENDAS) {
        printf("Limite de transa��es atingido\n");
        return;
    }

    printf("Insira o CPF do cliente: ");
    scanf("%s", cpf);
    limparBuffer();

    i = buscarCliente(cadastroClientes, *qntClientes, cpf);

    if (i == -1) {
        printf("Cliente n�o cadastrado\n");
        return;
    }

    printf("Insira o c�digo do produto: ");
    scanf("%s", codigo);
    limparBuffer();

    i = buscarProduto(cadastroProdutos, *qntProdutos, codigo);

    if (i == -1) {
        printf("Produto n�o cadastrado\n");
        return;
    }

    printf("Insira a data da transa��o (formato: AAAA-MM-DD): ");
    scanf("%s", dataTransacaoStr); // copia para string
    sscanf(dataTransacaoStr, "%d-%d-%d", &ano, &mes, &dia); // separa em inteiros
    limparBuffer();

    dataTransacaoTemp.tm_year = ano - 1900; // atualiza para a biblioteca de tempo
    dataTransacaoTemp.tm_mon = mes - 1;
    dataTransacaoTemp.tm_mday = dia;

    printf("Insira a hora da transa��o (formato: HH:MM): ");
    scanf("%d:%d", &hora, &minuto);
    limparBuffer();

    horaTransacaoTemp.hora = hora;
    horaTransacaoTemp.minuto = minuto;

    i = buscarCompraVenda(cadastroCompraVendas, *qntCompraVendas, cpf, codigo, dataTransacaoTemp, horaTransacaoTemp);
    if (i != -1) {
        printf("Compra ou venda j� cadastrada.\n");
        return;
    }
    
    realocacao = *qntCompraVendas;
    realocacao++;

    // realoca��o de memoria
    *cadastroCompraVendas = (struct CompraVenda *)realloc(*cadastroCompraVendas, realocacao * sizeof(struct CompraVenda));
    if (*cadastroCompraVendas == NULL) {
        printf("Erro na realoca��o de mem�ria\n");
        return;
    }

    // deposi das verifica��es, os valores podem ser passados para a novaCompraVenda

    strcpy(novaCompraVenda.cpfCliente, cpf);
    strcpy(novaCompraVenda.codigoProduto, codigo);

    novaCompraVenda.dataTransacao = dataTransacaoTemp;
    novaCompraVenda.horaTransacao = horaTransacaoTemp;

    printf("Insira o valor da transa��o: ");
    scanf("%lf", &novaCompraVenda.valor);

    (*cadastroCompraVendas)[*qntCompraVendas] = novaCompraVenda; //insere no vetor
    (*qntCompraVendas)++;
}

//alterar

void alterarCadastroCompraVenda(struct CompraVenda **cadastroCompraVendas, int cont) {
    int i;
    
    char cpfCliente[15];
    char codigoProduto[MAX_CHAR_CODIGO];
    double novoValor;
    
    struct tm dataTransacaoTemp;
    struct hora horaTransacaoTemp;
    char dataTransacaoStr[20]; // armazena a data e hora temporariamente
    char horaTransacaoStr[8]; // armazena a hora temporariamente
    int dia, mes, ano; // divide a data
    int hora, minuto; // divide a hora

    printf("Insira o CPF do cliente: ");
    scanf("%s", cpfCliente);

    printf("Insira o c�digo do produto: ");
    scanf("%s", codigoProduto);
    
    printf("Insira a data da transa��o (formato: AAAA-MM-DD): ");
    scanf("%s", dataTransacaoStr); // copia para string
    sscanf(dataTransacaoStr, "%d-%d-%d", &ano, &mes, &dia); //separa em inteiros
    
    dataTransacaoTemp.tm_year = ano - 1900; // atualiza para a biblioteca de tempo
    dataTransacaoTemp.tm_mon = mes - 1;
    dataTransacaoTemp.tm_mday = dia;

    printf("Insira a hora da transa��o (formato: HH:MM): ");
    scanf("%d:%d", &hora, &minuto);
    
    horaTransacaoTemp.hora = hora;
    horaTransacaoTemp.minuto = minuto;
    
    i = buscarCompraVenda(cadastroCompraVendas, cont, cpfCliente, codigoProduto, dataTransacaoTemp, horaTransacaoTemp);
    if (i == -1) {
        printf("Transa��o n�o encontrada.\n");
        return;
    }

    struct CompraVenda *compraVenda = &(*cadastroCompraVendas)[i];

    printf("Novo valor da transa��o: ");
    scanf("%lf", &novoValor);

    compraVenda->valor = novoValor;
}


//excluir

void excluirCadastroCompraVenda(struct CompraVenda **cadastroCompraVendas, int *qntCompraVendas) {
    int i, indice;
    char cpfCliente[15];
    char codigoProduto[MAX_CHAR_CODIGO];
    char dataTransacaoStr[20];
    int dia, mes, ano, hora, minuto;
    struct tm dataTransacaoTemp;
    struct hora horaTransacaoTemp;
    int realocacao;
    
    printf("Insira o CPF do cliente: ");
    scanf("%s", cpfCliente);
    limparBuffer();
    
    printf("Insira o c�digo do produto: ");
    scanf("%s", codigoProduto);
    limparBuffer();
    
    printf("Insira a data da transa��o (formato: AAAA-MM-DD): ");
    scanf("%s", dataTransacaoStr); // copia para string
    sscanf(dataTransacaoStr, "%d-%d-%d", &ano, &mes, &dia); // separa em inteiros
    
    dataTransacaoTemp.tm_year = ano - 1900; // atualiza para a biblioteca de tempo
    dataTransacaoTemp.tm_mon = mes - 1;
    dataTransacaoTemp.tm_mday = dia;
    
    printf("Insira a hora da transa��o (formato: HH:MM): ");
    scanf("%d:%d", &hora, &minuto);
    
    horaTransacaoTemp.hora = hora;
    horaTransacaoTemp.minuto = minuto;
    
    indice = buscarCompraVenda(cadastroCompraVendas, *qntCompraVendas, cpfCliente, codigoProduto, dataTransacaoTemp, horaTransacaoTemp);
    if (indice == -1) {
        printf("Transa��o n�o encontrada.\n");
        return;
    } 
    
    // move para preencher o espa�o vazio
    for (i = indice; i < *qntCompraVendas - 1; i++) {
        (*cadastroCompraVendas)[i] = (*cadastroCompraVendas)[i + 1];
    }
    
    // reduz o tamanho do vetor em 1
    realocacao = *qntCompraVendas;
    realocacao--;
    *cadastroCompraVendas = (struct CompraVenda *)realloc(*cadastroCompraVendas, realocacao * sizeof(struct CompraVenda));
    
    (*qntCompraVendas)--;
}


//submenu

void SubmenuCompraVendas(struct CompraVenda **cadastroCompraVendas, int *qntCompraVendas, struct Cliente **cadastroClientes, int *qntClientes, struct Produto **cadastroProdutos, int *qntProdutos) {
    int opcao, cont;

    do {
        printf("\n_________________________\n");
        printf("\n\t TRANSA��ES \t\n");
        Submenu();

        scanf("%d", &opcao);
		
        // usado para confirmar exclus�o e inclus�o
        cont = *qntCompraVendas;

        switch (opcao) {
        	
            case 1: // listar todos
				system("cls");
                if (cont == 0) {
                    printf("\nN�o h� transa��es cadastradas");
                } else {
                    listarTodasCompraVendas(cadastroCompraVendas, cont);
                }

                break;

            case 2: // listar um
				system("cls");
                if (cont == 0) {
                    printf("\nN�o h� transa��es cadastradas");
                } else {
                    listarUmaCompraVenda(cadastroCompraVendas, cont);
                }

                break;

            case 3: // incluir
				system("cls");
                incluirCompraVenda(cadastroCompraVendas, qntCompraVendas, cadastroClientes, qntClientes, cadastroProdutos, qntProdutos);

                if ((cont + 1) == (*qntCompraVendas)) {
                    printf("\nTransa��o cadastrada com sucesso");
                } else {
                    printf("\nN�o foi poss�vel adicionar a transa��o");
                }

                break;

            case 4: // alterar
				system("cls");
                if (cont == 0) {
                    printf("\nN�o h� transa��es cadastradas");
                } else {
                    alterarCadastroCompraVenda(cadastroCompraVendas, cont);
                    printf("\nTransa��o alterada com sucesso");
                }

                break;

            case 5: // excluir
				system("cls");
                if (cont == 0) {
                    printf("\nN�o h� transa��es cadastradas");
                } else {
                    excluirCadastroCompraVenda(cadastroCompraVendas, qntCompraVendas);
                    if ((cont - 1) == *qntCompraVendas) {
                        printf("\nTransa��o exclu�da com sucesso");
                    } else {
                        printf("\nN�o foi poss�vel excluir a transa��o");
                    }
                }

                break;

            case 6: // voltar ao menu principal
				system("cls");
                printf("\nVoltando ao menu principal");

                break;

            default: // op��o inv�lida
				system("cls");
                printf("\nOp��o Inv�lida");

                break;
        }

    } while (opcao != 6);
}



//RELAT�RIOS



int comparaDatas(struct tm data1, struct tm data2) {
    if (data1.tm_year < data2.tm_year)
        return -1;
    else if (data1.tm_year > data2.tm_year)
        return 1;
    else {
        if (data1.tm_mon < data2.tm_mon)
            return -1;
        else if (data1.tm_mon > data2.tm_mon)
            return 1;
        else {
            if (data1.tm_mday < data2.tm_mday)
                return -1;
            else if (data1.tm_mday > data2.tm_mday)
                return 1;
            else
                return 0; 
        }
    }
    //retorna 0 quando a data � igual
    //retorna 1 quando a data 1 > data 2
    //retorna -1 quando a data 1 < data 2
}

//1

void relatorio1(struct Cliente **cadastroClientes, int contClientes) {
    int i, quantidade, ver = 0;
    FILE *relatorioTelefones;
    char dataNascimentoStr[20]; // string para armazenar a data

    printf("\nEntre com a quantidade de telefones: ");
    scanf("%d", &quantidade);

    relatorioTelefones = fopen("relatorioTelefones.txt", "a+");
    if (relatorioTelefones == NULL) {
        printf("\nN�o foi poss�vel abrir o arquivo\n");
        return;
    }

    for (i = 0; i < contClientes; i++) {
        if ((*cadastroClientes)[i].qntTelefones >= quantidade) {
            ver++;
            if (ver == 1) {
            	printf("\nClientes encontrados com %d ou mais telefones:\n", quantidade);
                fprintf(relatorioTelefones, "\nClientes encontrados com %d ou mais telefones:\n", quantidade);
            }
            
            // imprime no console  
            imprimirCliente(cadastroClientes, i);

            // coloca no arquivo
            fprintf(relatorioTelefones, "CLIENTE %d\n", i + 1);
            fprintf(relatorioTelefones, "Nome: %s\n", (*cadastroClientes)[i].nome);
            fprintf(relatorioTelefones, "CPF: %s\n", (*cadastroClientes)[i].cpf);

            strftime(dataNascimentoStr, sizeof(dataNascimentoStr), "%Y-%m-%d", &(*cadastroClientes)[i].dataNascimento);
            fprintf(relatorioTelefones, "Data de Nascimento: %s\n", dataNascimentoStr);

            fprintf(relatorioTelefones, "Sexo: %c\n", (*cadastroClientes)[i].sexo);
            fprintf(relatorioTelefones, "Sal�rio: %.2f\n", (*cadastroClientes)[i].salario);

            fprintf(relatorioTelefones, "Emails:\n");
            for (int j = 0; j < (*cadastroClientes)[i].qntEmails; j++) {
                fprintf(relatorioTelefones, "-> %s\n", (*cadastroClientes)[i].emails[j]);
            }

            fprintf(relatorioTelefones, "Telefones:\n");
            for (int j = 0; j < (*cadastroClientes)[i].qntTelefones; j++) {
                fprintf(relatorioTelefones, "-> %s\n", (*cadastroClientes)[i].telefones[j]);
            }
			
			printf("\n_________________________\n");
            fprintf(relatorioTelefones, "\n_________________________\n");
        }
    }

    if (ver == 0) {
    	printf("N�o foi encontrado nenhum cliente com mais de %d telefones\n", quantidade);
        fprintf(relatorioTelefones,"N�o foi encontrado nenhum cliente com mais de %d telefones\n", quantidade);
    }

    fclose(relatorioTelefones);
}


//2

void relatorio2(struct Produto **cadastroProdutos, int contProdutos) {
    int i, ver = 0;
    FILE *relatorioProdutosVencidos;
    char dataValidadeStr[20]; // armazenar a data

    // obtem o tempo atual em segundos
    time_t rawtime;
    time(&rawtime);

    // converte para ponteiro da struct
    struct tm *timeinfo;
    timeinfo = localtime(&rawtime);

    // Converte para struct
    struct tm agora = *timeinfo;

    relatorioProdutosVencidos = fopen("relatorioProdutosVencidos.txt", "a+");
    if (relatorioProdutosVencidos == NULL) {
        printf("\nNao foi possivel abrir o arquivo\n");
        return;
    }

    for (i = 0; i < contProdutos; i++) {
        if (comparaDatas((*cadastroProdutos)[i].validade, agora) < 0) {
            ver++;
            if (ver == 1) {
            	printf("Produtos Vencidos");
                fprintf(relatorioProdutosVencidos, "\nProdutos vencidos:\n");
            }
            
            //imprime no console
 			imprimirProduto(cadastroProdutos, i);

            // coloca no arquivo
            fprintf(relatorioProdutosVencidos, "PRODUTO %d\n", i + 1);
            fprintf(relatorioProdutosVencidos, "C�digo: %s\n", (*cadastroProdutos)[i].codigo);
            fprintf(relatorioProdutosVencidos, "Descri��o: %s\n", (*cadastroProdutos)[i].descricao);
            fprintf(relatorioProdutosVencidos, "Quantidade: %d\n", (*cadastroProdutos)[i].qnt);
            fprintf(relatorioProdutosVencidos, "Pre�o: %.2f\n", (*cadastroProdutos)[i].preco);
            fprintf(relatorioProdutosVencidos, "Desconto: %d%%\n", (*cadastroProdutos)[i].desconto);

			// donverte struct tm para string
            strftime(dataValidadeStr, sizeof(dataValidadeStr), "%Y-%m-%d", &(*cadastroProdutos)[i].validade);
            fprintf(relatorioProdutosVencidos, "Data de Validade: %s\n", dataValidadeStr);

			printf("\n_________________________\n");
            fprintf(relatorioProdutosVencidos, "\n_________________________\n");
        }
    }

    if (ver == 0) {
    	printf("N�o foi encontrado nenhum produto vencido\n");
        fprintf(relatorioProdutosVencidos, "N�o foi encontrado nenhum produto vencido\n");
    }

    fclose(relatorioProdutosVencidos);
}

//3

void relatorio3(struct CompraVenda **cadastroCompraVendas, int contCompraVendas) {
    char dataInicialStr[20], dataFinalStr[20];
    char dataTransacaoStr[20];
    char horaTransacaoStr[8];
    struct tm dataInicial;
    struct tm dataFinal;
    int i, ver = 0;
    int ano, mes, dia;

    printf("\nInsira a data inicial (formato: AAAA-MM-DD): ");
    scanf("%s", dataInicialStr); // copia para string

    sscanf(dataInicialStr, "%d-%d-%d", &ano, &mes, &dia); // separa em inteiros

    dataInicial.tm_year = ano - 1900; // atualiza para a biblioteca de tempo
    dataInicial.tm_mon = mes - 1;
    dataInicial.tm_mday = dia;

    printf("\nInsira a data final (formato: AAAA-MM-DD): ");
    scanf("%s", dataFinalStr); // copia para string

    sscanf(dataFinalStr, "%d-%d-%d", &ano, &mes, &dia); // separa em inteiros

    dataFinal.tm_year = ano - 1900; // atualiza para a biblioteca de tempo
    dataFinal.tm_mon = mes - 1;
    dataFinal.tm_mday = dia;

    FILE *relatorioTransacoes;

    relatorioTransacoes = fopen("relatorioTransacoesDatas.txt", "a+");
    if (relatorioTransacoes == NULL) {
        printf("\nNao foi possivel abrir o arquivo\n");
        return;
    }

    for (i = 0; i < contCompraVendas; i++) {
        if ((comparaDatas((*cadastroCompraVendas)[i].dataTransacao, dataInicial) >= 0) && (comparaDatas(dataFinal, (*cadastroCompraVendas)[i].dataTransacao) >= 0)) {
            ver++;
            if (ver == 1) {
                printf("\nVendas realizadas entre as datas:\n");
                fprintf(relatorioTransacoes, "\nVendas realizadas entre as datas:\n");
            }

            imprimirCompraVenda(cadastroCompraVendas, i);

            // coloca no arquivo
            fprintf(relatorioTransacoes, "TRANSACAO %d\n", i + 1);
            fprintf(relatorioTransacoes, "CPF do Cliente: %s\n", (*cadastroCompraVendas)[i].cpfCliente);
            fprintf(relatorioTransacoes, "C�digo do Produto: %s\n", (*cadastroCompraVendas)[i].codigoProduto);

            // converte struct tm para string
            strftime(dataTransacaoStr, sizeof(dataTransacaoStr), "%Y-%m-%d", &(*cadastroCompraVendas)[i].dataTransacao);
            fprintf(relatorioTransacoes, "Data da Transa��o: %s\n", dataTransacaoStr);

            // converte hora para string
            sprintf(horaTransacaoStr, "%02d:%02d", (*cadastroCompraVendas)[i].horaTransacao.hora, (*cadastroCompraVendas)[i].horaTransacao.minuto);
            fprintf(relatorioTransacoes, "Hora da Transa��o: %s\n", horaTransacaoStr);

            fprintf(relatorioTransacoes, "Valor: %.2f\n", (*cadastroCompraVendas)[i].valor);

            printf("\n_________________________\n");
            fprintf(relatorioTransacoes, "\n_________________________\n");
        }
    }

    if (ver == 0) {
        printf("N�o foi encontrada nenhuma transa��o entre as datas\n");
        fprintf(relatorioTransacoes,"N�o foi encontrada nenhuma transa��o entre as datas\n");
    }

    fclose(relatorioTransacoes);
}


//submenu

void submenuRelatorios (struct CompraVenda **cadastroCompraVendas, int *qntCompraVendas, struct Cliente **cadastroClientes, int *qntClientes, struct Produto **cadastroProdutos, int *qntProdutos){
	
	int opcao;
	int contClientes = *qntClientes;
	int contProdutos = *qntProdutos;
	int contCompraVendas = *qntCompraVendas;
	
	do{
		printf("\n_________________________\n");
		printf("\n\t RELAT�RIOS \t\n");
		printf("\n_________________________\n");
		printf("\n1. Mostrar os dados dos clientes com mais de X telefones");
		printf("\n2. Mostrar os dados dos produtos vencidos");
		printf("\n3. Mostrar os dados das vendas realizadas entre duas datas");
		printf("\n4. Voltar ao menu principal");
		printf("\n_________________________\n");
		
		scanf("%d", &opcao);
		
		switch(opcao){
			case 1: //relatorio 1
				system("cls");
				relatorio1(cadastroClientes, contClientes);
				
				break;
				
			case 2://relatorio 2
				system("cls");
				relatorio2(cadastroProdutos, contProdutos);
				
				break;
				
			case 3://relatorio 3
				system("cls");
				relatorio3(cadastroCompraVendas, contCompraVendas);
				
				break;
				
			case 4://volta para o menu
				system("cls");
				printf("\nVoltando ao menu principal");
				
				break;
				
			default: //opcao invalida
				system("cls");
                printf("\nOp��o Inv�lida");

                break;
		}
		
	}while (opcao != 'd');	
}


//FUN��O PRINCIPAL


int main() {
    setlocale(LC_ALL, "Portuguese");
    // permite caracteres do portugues

    int contClientes = 0, contProdutos = 0, contCompraVendas = 0;
    int opcao;

    // cria/preenche vetor clientes
    struct Cliente *cadastroClientes = NULL;
    abreArqClientes(&cadastroClientes, &contClientes);

    // cria/preenche vetor produtos
    struct Produto *cadastroProdutos = NULL;
    abreArqProdutos(&cadastroProdutos, &contProdutos);

    // cria/preenche vetor compra/vendas
    struct CompraVenda *cadastroCompraVendas = NULL;
    abreArqCompraVendas(&cadastroCompraVendas, &contCompraVendas);

    do {
        Menu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: //clientes
                system("cls");
                SubmenuClientes(&cadastroClientes, &contClientes);
                salvarArqClientes(&cadastroClientes, &contClientes);
                break;

            case 2: //produtos 
                system("cls");
                SubmenuProdutos(&cadastroProdutos, &contProdutos);
                salvarArqProdutos(&cadastroProdutos, &contProdutos);
                break;

            case 3: //compra e venda
                system("cls");
                SubmenuCompraVendas(&cadastroCompraVendas, &contCompraVendas, &cadastroClientes, &contClientes, &cadastroProdutos, &contProdutos);
                salvarArqCompraVendas(&cadastroCompraVendas, &contCompraVendas);
                break;

            case 4: // relatorios
                system("cls");
                submenuRelatorios(&cadastroCompraVendas, &contCompraVendas, &cadastroClientes, &contClientes, &cadastroProdutos, &contProdutos);
                break;

            case 5:// fim
                system("cls");
                printf("\nEncerrando o programa");
                break;

            default: //opcao invalida
                system("cls");
                printf("\nOp��o inv�lida");
                break;
        }
    } while (opcao != 5);

    // libera memoria alocada
    free(cadastroClientes);
    free(cadastroProdutos);
    free(cadastroCompraVendas);

    exit(0);
}

