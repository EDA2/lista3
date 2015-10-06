#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

void menu(){
	printf("1 - Insira um número: \n");
	printf("2 - Imprima os números ordenados: \n");
	printf("3 - Exclua uma folha: \n");
	printf("0 - Sair: ");
}

int main (){
	char option = '0';
	int new_number = 0, delete_number = 0;
	Tree *tree = (Tree*)malloc(sizeof(Tree));

	do{
		menu();
		scanf("%1s", &option);

		if (option == '1') {
			printf("Digite o número que deseja inserir: ");
   			scanf("%d", &new_number);				
			insert_leaf(tree, new_number);
		}

		if (option == '2') {
			in_order(tree->root);
		}	
			
		if (option == '3') {
			printf("Digite o número que deseja remover da árvore: ");
			scanf("%d", &delete_number);
			tree->root = remove_leaf(tree->root, tree,  delete_number);
			update_balancing_factor_top_down(tree->root, tree);				
		}

		printf("\n");
	}while(option != '0');

	return 0;
}