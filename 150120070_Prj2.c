// Semih Bað
// 150120070

#include<stdio.h>
#include <stdlib.h>
#include <math.h>


/// -AVL TREE IMPLEMANTATION- ///
int numberOfComparisonsForAvl = 0;
int numberOfSingleRotation_Left_Avl = 0;
int numberOfSingleRotation_Right_Avl = 0;
int numberOfDubleRotation_LeftRight_Avl = 0;
int numberOfDubleRotation_RightLeft_Avl = 0;

//avl node
struct nodeAvl{
	int data;
	int count;
	int height;
	struct nodeAvl *left;
	struct nodeAvl *right;
};
typedef struct nodeAvl avlNode;

//create node for avl
avlNode* createAvlNode(int data){
	avlNode *newNode = malloc(sizeof(avlNode));
	newNode->data = data;
	newNode->count = 1;
	newNode->height = 1;
	newNode->left = NULL;
	newNode->right = NULL;
	
	return newNode;
}

//find max number
int findMax(int a, int b){
	if(a>b){
		return a;
	}
	else if(b>a){
		return b;
	}
}

//get height
int getHeight(avlNode *node){
	if (node == NULL){
		return 0;
	}
	else{
		return node->height;
	}
}

//Rotations
//left rotation
avlNode* lRotationForAvl(avlNode *root){
	avlNode *temp = root->right;
	root->right = temp->left;
	temp->left = root;
	
	root->height = findMax(getHeight(root->left), getHeight(root->right)) + 1;
	temp->height = findMax(getHeight(temp->left), getHeight(temp->right)) + 1;
	
	return temp;
}

//right rotation
avlNode* rRotationForAvl(avlNode* root){
	avlNode *temp = root->left;
	root->left = temp->right;
	temp->right = root;
	
	root->height = findMax(getHeight(root->left), getHeight(root->right)) + 1;
	temp->height = findMax(getHeight(temp->left), getHeight(temp->right)) + 1;
	
	return temp;
}

//get balance value
int findBalance(avlNode *root){
	if(root == NULL){
		return 0;
	}
	return (getHeight(root->left) - getHeight(root->right));
}

//insert node to avl tree
avlNode* insertForAvl(avlNode *root, int data){
	//if the root is null
	if(root == NULL){
		return createAvlNode(data);
	}
	numberOfComparisonsForAvl++;
	//if the root is not null- find the correct place 
	if(data < root->data){
		root->left = insertForAvl(root->left, data);
	}
	else if(data > root->data){
		root->right = insertForAvl(root->right, data);
	}
	else if(data == root->data){
		root->count = root->count + 1;
		return root;
	}
	//find new height of node if it is necessary
	root->height = findMax(getHeight(root->left), getHeight(root->right)) + 1;
	
	//find balance between right and left node of currend node
	int balance = findBalance(root);
	
	//do proper rotation if it is necessary
	//single rotation - right rotation
	if((balance > 1) && (data < root->left->data)){
		numberOfSingleRotation_Right_Avl++;
		return rRotationForAvl(root);
	}
	
	//single rotation - left rotation
	if((balance < -1) && (data > root->right->data)){
		numberOfSingleRotation_Left_Avl++;
		return lRotationForAvl(root);
	}
	
	//double rotation - right/left rotation
	if((balance < -1) && (data < root->right->data)){
		numberOfDubleRotation_RightLeft_Avl++;
		root->right = rRotationForAvl(root->right);
		return lRotationForAvl(root);
	}
	
	//double rotation - left/right rotation
	if((balance > 1) && (data > root->left->data)){
		numberOfDubleRotation_LeftRight_Avl++;
		root->left = lRotationForAvl(root->left);
		return rRotationForAvl(root);
	}
	
	return root;
}

//print the tree preorder
void pprintAvlTree(avlNode *tree){
	if(tree == NULL){
		return;	
	}
	else{
		printf("%d ",tree->data);
		pprintAvlTree(tree->left);
		pprintAvlTree(tree->right);
	}
}

//calculate total time units for avl tree
int calculateTimeUnitForAvl(){
//	printf("\nThe cost component from the comparisons for avl tree : %d",numberOfComparisonsForAvl);
//	printf("\nThe number of the single left rotations for avl tree: %d",numberOfSingleRotation_Left_Avl);
//	printf("\nThe number of the single right rotations for avl tree: %d",numberOfSingleRotation_Right_Avl);
//	printf("\nThe number of the double left-right rotations for avl tree: %d",numberOfDubleRotation_LeftRight_Avl);
//	printf("\nThe number of the double right-left rotations for avl tree: %d",numberOfDubleRotation_RightLeft_Avl);
	int total = (numberOfComparisonsForAvl + numberOfSingleRotation_Left_Avl + numberOfSingleRotation_Right_Avl + numberOfDubleRotation_LeftRight_Avl*2 + numberOfDubleRotation_RightLeft_Avl*2);
	printf("\nTotal cost: %d",total);
	
	return total;
}







/// -SPLAY TREE IMPLEMANTATION- ///
int numberOfComparisonsForSplay = 0;
int numberOfZigRotations = 0;
int numberOfZigZagRotations = 0;
int numberOfZigZigRotations = 0;
int isProcessActive;

//stack for splay tree
// splay tree path node
struct nodeSplayPath{
	char direction;
	struct nodeSplayPath *next;
};
typedef struct nodeSplayPath splayPathNode;
splayPathNode *top = NULL;

//check if the stcak empty or not
int isEmpyt(){
	if(top != NULL){
		return 1;
	}
	else{
		return 0;
	}
}

//push function for stack
void push(char direction){
	if(top == NULL){
		splayPathNode *n = malloc(sizeof(splayPathNode));
		n->next = NULL;
		n->direction = direction;
		top = n;
	}
	else{
		splayPathNode *n = malloc(sizeof(splayPathNode));
		n->direction = direction;
		n->next = top;
		top = n;
	}	
}

//pop function for stack
void pop(){
	splayPathNode *temp;
	temp = top;
	top = top->next;
	free(temp);
}


// splay node
struct nodeSplay{
	int data;
	int count;
	struct nodeSplay *left;	
	struct nodeSplay *right;
	struct nodeSplay *parent;
};
typedef struct nodeSplay splayNode;

//this node always keep the last splay node
splayNode *lastSplaynode = NULL;

//create node for splay
splayNode* createSplayNode(splayNode *parent, int data){
	splayNode *newNode = malloc(sizeof(splayNode));
	newNode->data = data;
	newNode->count = 1;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->parent = parent;
	lastSplaynode = newNode;
	
	return newNode;
}

//Rotations
//left rotation
splayNode* lRotationForSplay(splayNode *lastNode){
	splayNode *parent = lastNode->parent;
	parent->right = lastNode->left;
	if(lastNode->left != NULL){
		lastNode->left->parent = parent; 
	}
	lastNode->left = parent;
	lastNode->parent = parent->parent;
	parent->parent = lastNode;
	return lastNode;
}

//right rotation
splayNode* rRotationForSplay(splayNode *lastNode){
	splayNode *parent = lastNode->parent;
	parent->left = lastNode->right;
	if(lastNode->right != NULL){
		lastNode->right->parent = parent;
	}
	lastNode->right = parent;
	lastNode->parent = parent->parent;
	parent->parent = lastNode;
	
	return lastNode;
}

//insert node to splay tree
splayNode* insertForSplay(splayNode *parent, splayNode *root, int data){
	//if the root is null
	if(root == NULL){
		return createSplayNode(parent, data);
	}
	numberOfComparisonsForSplay++;
	//if the root is not null- find the correct place 
	if(data < root->data){
		push('l');
		root->left = insertForSplay(root,root->left,data);
	}
	else if(data > root->data){
		push('r');
		root->right = insertForSplay(root,root->right,data);
	}
	else if(data == root->data){
		root->count = root->count + 1;
		lastSplaynode = root;	
		return root;
	}
	
	
	//do double rotation
	if(isProcessActive){
		if(top->direction != top->next->direction){
			//do zigzag
			numberOfZigZagRotations++;
			if(top->direction == 'r'){
				pop();
				root->left = lRotationForSplay(lastSplaynode);
				pop();
				isProcessActive = 0;
				return rRotationForSplay(lastSplaynode);
			}
			else if(top->direction == 'l'){
				pop();
				root->right = rRotationForSplay(lastSplaynode);
				pop();
				isProcessActive = 0;
				return lRotationForSplay(lastSplaynode);
			}
		}
		else if(top->direction == top->next->direction){
			//do zigzig
			numberOfZigZigRotations++;
			if(top->direction == 'r'){
				pop();
				pop();
				root = lRotationForSplay(lastSplaynode->parent);
				isProcessActive = 0;
				return lRotationForSplay(lastSplaynode);
			}
			if(top->direction == 'l'){
				pop();
				pop();
				root = rRotationForSplay(lastSplaynode->parent);
				isProcessActive = 0;
				return rRotationForSplay(lastSplaynode);
			}
		}
	}
	else if(lastSplaynode->parent == root){	
		//find the proper position for splay operations
		if(!isEmpyt()){
			//empty
			return root;
		}
		else{
			//not empty
			if(top->next == NULL){
				//zig operation
				numberOfZigRotations++;
				if(top->direction == 'r'){
					pop();
					return lRotationForSplay(lastSplaynode);
				}
				else if(top->direction == 'l'){
					pop();
					return rRotationForSplay(lastSplaynode);
				}
			}
			else if(top->direction != top->next->direction){
				//zig zag operation
				isProcessActive = 1;
				return;
			}
			else if(top->direction == top->next->direction){
				//zigzig operation
				isProcessActive = 1;
				return;
			}
		}
	}
	return root;
}

//print the three preorder
void pprintSplayTree(splayNode *tree){
	if(tree == NULL){
		return;	
	}
	else{
		printf("%d ",tree->data);
		pprintSplayTree(tree->left);
		pprintSplayTree(tree->right);
	}
}

int calculateTimeUnitForSplay(){
//	printf("\nThe cost component from the comparisons for splay tree : %d",numberOfComparisonsForSplay);
//	printf("\nThe number of the zig rotations for splay tree: %d",numberOfZigRotations);
//	printf("\nThe number of the zigzag rotations for splay tree: %d",numberOfZigZagRotations);
//	printf("\nThe number of the zigzig rotations for splay tree: %d",numberOfZigZigRotations);
	int total = (numberOfComparisonsForSplay + numberOfZigRotations + numberOfZigZagRotations*2 + numberOfZigZigRotations*2);	
	printf("\nTotal cost: %d",total);
	
	return total;
}


//this function reads data from given file and creats both avl and splay trees
void readDataAndCreateTrees(char *fileName, avlNode *avlRoot, splayNode *splayRoot){
    FILE *inputFile = fopen(fileName, "r");
	int data;
	if(inputFile == NULL){
		printf("No such file found");
	}
	else{
		while(!feof(inputFile)){	
			fscanf(inputFile,"%d ", &data);
			avlRoot = insertForAvl(avlRoot,data);
			splayRoot = insertForSplay(NULL,splayRoot,data);
		}
	}
	fclose(inputFile);
	
	//give output
	printf("AVL tree:\n");
	pprintAvlTree(avlRoot);
	int totalAvl = calculateTimeUnitForAvl();
	printf("\n\n");
	printf("Splay tree\n");
	pprintSplayTree(splayRoot);
	int totalSplay = calculateTimeUnitForSplay();
	printf("\n\n");
}


int main(int argc,char* argv[]){
	if(argc==1){
		printf("Error message!");
	}
    if(argc>=2)
    {
	
	avlNode *avlRoot = NULL;	
	splayNode *splayRoot = NULL;
	
	char *fileName = argv[1];
	readDataAndCreateTrees(fileName, avlRoot, splayRoot);
		
	return 0;
	}
}
