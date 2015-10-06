typedef struct leaf {
    int unsigned number;
    int balancing_factor;
    struct leaf *father;
    struct leaf *left_leaf;
    struct leaf *right_leaf;
}Leaf;
    
typedef struct tree {
    Leaf *root;
}Tree;

Leaf *init();
Leaf *search(Leaf *leaf, int number);
void pos_order(Leaf *leaf);
void pre_order(Leaf *leaf);
void in_order(Leaf *leaf);
void rotate_right_left(Leaf *root, Tree *tree);
void rotate_left_right(Leaf *root, Tree *tree);
void rotate_left(Leaf *root, Tree *tree);
void rotate_right(Leaf *root, Tree *tree);
void balancing_right(Leaf *leaf);
void balancing_left(Leaf *leaf);
void update_balancing_factor(Leaf *leaf, Tree *tree);
void update_balancing_factor_top_down(Leaf *leaf, Tree *tree);
Leaf *remove_leaf(Leaf *leaf, Tree *tree, int number);
int deep(Leaf *runs);
int balancing_factor(Leaf *leaf);
int insert_leaf(Tree *tree);


Leaf *init(int number) {
    Leaf *new_leaf = (Leaf*)malloc(sizeof(Leaf));

    new_leaf->number = number;
    new_leaf->balancing_factor = 0;
    new_leaf->right_leaf = NULL;
    new_leaf->left_leaf = NULL;
    new_leaf->father = new_leaf;

    return new_leaf;
}

int insert_leaf(Tree *tree, int number){
    Leaf *leaf = init(number);
    Leaf *father;
    Leaf *runs = tree->root;  

    while(runs) {
        father = runs; 

        if(leaf->number < runs->number) {

            runs = runs->left_leaf;

        }else if(leaf->number > runs->number) {

            runs = runs->right_leaf;

        }else { 
            return 1;
        }
    }

    if(!tree->root) {
        tree->root = leaf;
    } else if(leaf->number < father->number) {
        father->left_leaf = leaf;
        leaf->father = father;
    	update_balancing_factor(leaf, tree);
    } else {
        father->right_leaf = leaf;
        leaf->father = father;
    	update_balancing_factor(leaf, tree);
    }

}

Leaf *remove_leaf(Leaf *leaf, Tree *tree, int number){
		
	if(leaf == NULL){
		return NULL;
	}else if(number < leaf->number){
		leaf->left_leaf = remove_leaf(leaf->left_leaf, tree, number);
	}else if(number > leaf->number){
		leaf->right_leaf = remove_leaf(leaf->right_leaf, tree, number);
	}else{
		if(leaf->left_leaf == NULL  && leaf->right_leaf == NULL){
			free(leaf);
			leaf = NULL;
		}else if(leaf->right_leaf == NULL){
			Leaf *temp = leaf;
			leaf = leaf->left_leaf;
			leaf->father = temp->father;
			free(temp);
		}else if(leaf->left_leaf == NULL){
			Leaf *temp = leaf;
			leaf = leaf->right_leaf;
			leaf->father = temp->father;
			free(temp);
		}else{
			Leaf *temp = leaf->left_leaf;

			while(temp->right_leaf != NULL){
				temp = temp->right_leaf;
			}

			leaf->number = temp->number;
			temp->number = number;
			leaf->left_leaf = remove_leaf(leaf->left_leaf, tree, number);
		}	
	}
	return leaf;
}

void balancing_left(Leaf *leaf, Tree *tree){
	if(leaf->right_leaf->balancing_factor < 0){
        rotate_right_left(leaf, tree);
    }else{
        rotate_left(leaf, tree);
    }
}

void balancing_right(Leaf *leaf, Tree *tree){
    if(leaf->left_leaf->balancing_factor > 0){
        rotate_left_right(leaf, tree);
    }else{
        rotate_right(leaf, tree);
    }	
}

void update_balancing_factor(Leaf *leaf, Tree *tree){

    leaf->balancing_factor = balancing_factor(leaf);
        
    if(leaf->balancing_factor == 2){
    	balancing_left(leaf, tree);
    }else if(leaf->balancing_factor == (-2)){
    	balancing_right(leaf, tree);
    }else if(leaf != tree->root){
        update_balancing_factor(leaf->father, tree);
    }
}

void update_balancing_factor_top_down(Leaf *leaf, Tree *tree){
	leaf->balancing_factor = balancing_factor(leaf);
  
    if(leaf->balancing_factor == 2){
    	balancing_left(leaf, tree);
    }else if(leaf->balancing_factor == (-2)){
    	balancing_right(leaf, tree);
    }

    leaf->balancing_factor = balancing_factor(leaf);

    if(leaf->left_leaf != NULL){ 
        update_balancing_factor_top_down(leaf->left_leaf, tree);
    }

    if(leaf->right_leaf != NULL){ 
        update_balancing_factor_top_down(leaf->right_leaf, tree);
    }
}

int balancing_factor(Leaf *leaf){
    return deep(leaf->right_leaf) - deep(leaf->left_leaf);
}

int deep(Leaf *runs){
    int count_right;
    int count_left;

    if(runs == NULL){
        return 0;
    }

    count_left = 1 + deep(runs->left_leaf);
    count_right =1 + deep(runs->right_leaf); 

    return count_right > count_left ? count_right : count_left;
}

void rotate_right(Leaf *root, Tree *tree) {

    Leaf *new_root;
    Leaf *temp;

    new_root = root->left_leaf;
    temp = new_root->right_leaf;
    root->left_leaf = temp;

    if(root->left_leaf){
        root->left_leaf->father = root;
    } 

    new_root->right_leaf = root;

    if(root != tree->root){ 
        new_root->father = root->father;
        root->father->right_leaf = new_root;
    }else{
        new_root->father = new_root;
        tree->root  = new_root;
    }

    root->father = new_root;
    root = new_root;

    root->right_leaf->balancing_factor = balancing_factor(root->right_leaf);
    root->balancing_factor = balancing_factor(root);
}

void rotate_left(Leaf *root, Tree *tree) {
    Leaf *new_root;
    Leaf *temp;

    new_root = root->right_leaf;
    temp = new_root->left_leaf;
    root->right_leaf = temp;

    if(root->right_leaf){
        root->right_leaf->father = root;
    }

    new_root->left_leaf = root;

    if(root != tree->root){ 
        new_root->father = root->father;
        root->father->left_leaf = new_root;
    }else{
        new_root->father = new_root;
        tree->root = new_root;
    }

    root->father = new_root;
    root = new_root;

    root->left_leaf->balancing_factor = balancing_factor(root->left_leaf);
    root->balancing_factor = balancing_factor(new_root);
}

void rotate_left_right(Leaf *root, Tree *tree){
    rotate_left(root->left_leaf, tree);
    rotate_right(root, tree);
}

void rotate_right_left(Leaf *root, Tree *tree){
    rotate_right(root->right_leaf, tree);
    rotate_left(root, tree);
}

void in_order(Leaf *leaf){
    if(leaf != NULL){
        in_order(leaf->left_leaf);
        printf("Folha: %d  Pai: %d       ", leaf->number, leaf->father->number);
        if(leaf->left_leaf != NULL) {
            printf("Esquerdo: %d ", leaf->left_leaf->number);
        }

		if(leaf->right_leaf != NULL) {
            printf("Direito: %d ", leaf->right_leaf->number);
        }
		printf("  Balanceamento: %d\n", leaf->balancing_factor);
        in_order(leaf->right_leaf);
    }
}