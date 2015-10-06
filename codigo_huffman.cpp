/*
*
*/
#include <iostream>
#include <queue>
#include <vector>
#include <list>
#include <string>

#define LEFT 0
#define RIGHT 1

using namespace std;

//estritura que gurarda o no, contendo referencia para o filho da esquerda
//o filho da direita e o pai, alem do caracter representado
struct Node {
	char character;
	Node *right, *left, *parent;
};

//estrutura que guarda a arvore de codificacao, seu pesso
//e a um codigo que é 1 (fiho da esquerda) ou 0 (filho da direita)
struct Tree{
	int weight;
	int code;
	Node tree;
};

//estrutura que guarda cada caracter, quantas vezes ele aparece e
//a referencia no arvore de codificacao
struct Character {
	char character;
	int amount;
	Tree *tree_code;
};

//classe definida para montagem da fila prioritaria de arvores
class CompareTree {
public:
    bool operator()(Tree& t1, Tree& t2)
    {
       if (t1.weight > t2.weight){ 
       		return true;
       }else{
                return false;
       }
    }
};

//funcao que cria uma arvore dado um caracter e seu peso
Tree make_tree (int weight, char character){
	Node n;
	n.character = character;
	n.left = NULL;
	n.right = NULL;
	n.parent = NULL;
	
	
	Tree t;
	t.weight = weight;
	t.tree = n;
	return t;
}

//funcao que define a arvore como filho da esquerda
void codify_tree_left (Tree t){
	t.code = LEFT;
}

//funcao que define a arvore como filho da direita
void codify_tree_right (Tree t){
	t.code = RIGHT;
}

//funcao que mescla duas arvores
Tree merge_tree( Tree t1, Tree t2){
	
	Tree t;
	t.weight = t1.weight + t2.weight;
	
	t1.tree.parent = &t.tree;
	t2.tree.parent = &t.tree;
	
	codify_tree_left (t1);
	t.tree.left = &t1.tree;
	
	codify_tree_right(t2);
	t.tree.right = &t2.tree;
	t.tree.character = '<';

	return t;
}


//funcao que procura um caracter na lista de contagem
//(se não encontra retorna -1)
int finde_character (vector <Character> count_char, char c){
	int index = -1;
	for (int i = 0; i < count_char.size(); i++){
		Character temp = count_char[i];
		if(temp.character == c){
			index = i;
		}
	}
	return index;
}

//funcao que adiciona um caracter no vetor de contagem ou incrementa a quantidade
//se ele ja existe 
vector <Character> count_character(vector <Character> count_char, char c){
	int index = 0;
	index = finde_character (count_char, c);
	if(index != -1){
		count_char[index].amount++;
	} else{
		Character new_char;
		new_char.character = c;
		new_char.amount = 1;
		new_char.tree_code = NULL;
		count_char.push_back(new_char);
	}
	return count_char;
}

int main()
{
	
	string mensage;
	mensage = "Como vai voce?";
	cout << "Mensagem a ser codificada:\n\"" << mensage << "\"\n";
	
	//Contandos cada caracter e adicionando no vetor de contagens
	vector <Character> count_char;
	for (int i = 0; i < mensage.length(); i++){
		count_char = count_character(count_char, mensage[i]);
	}

	priority_queue<Tree, vector<Tree>, CompareTree> trees;
	
	//Transformando as estruturas Character em arvores e
	//adicionado na fila ordenada 	
	for (int i = 0; i < count_char.size(); i++){
		Character temp = count_char[i];
		Tree tree;
		tree = make_tree (temp.amount, temp.character);
		count_char[i].tree_code = &tree;
		trees.push(tree);
		cout << temp.character << " - " << temp.amount <<"\n";
	}
	
	//Montando a arvore do código de Huffmam
	while ((trees.size()) != 1) {
		Tree t1 = trees.top();
		trees.pop();
		Tree t2 = trees.top();
		trees.pop();
		Tree temp = merge_tree(t1, t2);
		trees.push(temp);
		
	}
	
	//Extraindo os códigos de cada caracter
	for (int i = 0; i < count_char.size(); i++){
		list <int> code;
		Tree *tree = count_char[i].tree_code;
		do{
			int way = tree->code;
			code.push_front (way);
			tree = tree->tree.parent;
			//erro tree->tree.parent é um nó e o que se espera é
			//a arvore do pai
		}while(tree->tree.parent != NULL);
	}

  	
	

    return 0;
}
