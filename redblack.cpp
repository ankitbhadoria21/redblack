#include <iostream>
#include <cstdlib>
#include <fstream>
#define RED	1
#define BLACK	0

using namespace std;

//node struct having required fields like key,color,count etc
struct node {
int key;
struct node *left, *right, *p;
int color;
int count;
};

typedef struct node *node_ptr;
//Terminal Node each internal node will point to
struct node NIL={0};

//Doing normal binary search, if the element is less than root search left else search right
node_ptr search(node_ptr root, int k) {
if (root == &NIL || root->key == k)
return root;
if (k < root->key)
return search(root->left, k);
else
return search(root->right, k);
}

//find minimum element in the tree
node_ptr tree_minimum(node_ptr root) {
while (root->left != &NIL)
root = root->left;
return root;
}

//find maximum element in the tree
node_ptr tree_maximum(node_ptr root) {
while (root->right != &NIL)
root = root->right;
return root;
}

//Psuedo Code from Introduction to Algorithms Thomas Corman Edition 3 Chapter 13
//is used as reference for insert, delete operations related to  RedBlack Tree.

//Left Rotate
void leftrotate(node_ptr *root, node_ptr par) {
node_ptr right = par->right;
par->right = right->left;
if (right->left != &NIL)
right->left->p = par;
right->p = par->p;
if (par->p == &NIL)
*root = right;
else if (par->p->left == right )
par->p->left = right;
else
par->p->right = right;
right->left = par;
par->p = right;
}

//Rigth Rotate Function
void rightrotate(node_ptr *root, node_ptr par) {
node_ptr left = par->left;
par->left = left->right;
if (left->right != &NIL)
left->right->p = par;
left->p = par->p;
if (par->p == &NIL)
*root = left;
else if (par->p->left == par )
par->p->left = left;
else
par->p->right = left;
left->right = par;
par->p = left;
}

//Red Black FixUp Function
void rbinsertfixup(node_ptr *root, node_ptr new_node) {
while (new_node->p->color == RED) {
if (new_node->p == new_node->p->p->left) {
node_ptr uncle = new_node->p->p->right;
if (uncle->color == RED) {
new_node->p->color = BLACK;
uncle->color = BLACK;
new_node->p->p->color = RED;
new_node = new_node->p->p;
}
else {
if (new_node == new_node->p->right) {
new_node = new_node->p;
leftrotate(root,new_node);
}
new_node->p->color = BLACK;
new_node->p->p->color = RED;
rightrotate(root,new_node->p->p);
}
}
else {
node_ptr uncle = new_node->p->p->left;
if (uncle->color == RED) {
new_node->p->color = BLACK;
uncle->color = BLACK;
new_node->p->p->color = RED;
new_node = new_node->p->p;
}
else {
if (new_node == new_node->p->left) {
new_node = new_node->p;
rightrotate(root,new_node);
}
new_node->p->color = BLACK;
new_node->p->p->color = RED;
leftrotate(root,new_node->p->p);
}
}
}
(*root)->color = BLACK;
}

//Red Black Insert Function
void rbinsert(node_ptr *root, node new_node) {
node_ptr tmp = (node_ptr) new node;
tmp->key = new_node.key;
tmp->count = new_node.count;
node_ptr x_old = &NIL;
node_ptr par = *root;
while (par != &NIL) {
x_old = par;
if (tmp->key < par->key)
par = par->left;
else
par = par->right;
}
tmp->p = x_old;
if (x_old == &NIL)
*root = tmp;
else if (tmp->key < x_old->key)
x_old->left = tmp;
else
x_old->right = tmp;
tmp->left = &NIL;
tmp->right = &NIL;
tmp->color = RED;
rbinsertfixup(root,tmp);
}

void rbtransplant(node_ptr *root, node_ptr u, node_ptr v) {
if (u->p == &NIL)
*root = v;
else if (u == u->p->left)
u->p->left = v;
else
u->p->right = v;
v->p = u->p;
}

//Delete FixUp Function as given in Introduction to Algorithms
void rbdeletefixup(node_ptr *root, node_ptr x) {
while (x != *root && x->color == BLACK) {
if (x == x->p->left) {
node_ptr w = x->p->right;
if (w->color == RED) {
w->color = BLACK;
x->p->color = RED;
leftrotate(root,x->p);
w = x->p->right;
}
if (w->left->color == BLACK && w->right->color == BLACK) {
w->color = RED;
x = x->p;
}
else {
if (w->right->color == BLACK) {
w->left->color = BLACK;
w->color = RED;
rightrotate(root,w);
w = x->p->right;
}
w->color = x->p->color;
x->p->color = BLACK;
w->right->color = BLACK;
leftrotate(root,x->p);
x = *root;
}
}
else {
node_ptr w = x->p->left;
if (w->color == RED) {
w->color = BLACK;
x->p->color = RED;
rightrotate(root,x->p);
w = x->p->left;
}
if (w->left->color == BLACK && w->right->color == BLACK) {
w->color = RED;
x = x->p;
}
else {
if (w->left->color == BLACK) {
w->right->color = BLACK;
w->color = RED;
leftrotate(root,w);
w = x->p->left;
}
w->color = x->p->color;
x->p->color = BLACK;
w->left->color = BLACK;
rightrotate(root,x->p);
x = *root;
}
}
}
x->color = BLACK;
}

//Red Black Delete function
void rbdelete(node_ptr *root, int z) {
node_ptr tmp = search(*root, z);
if (tmp == &NIL) {
return;
}
node_ptr y = tmp;
int y_orig_color = y->color;
node_ptr x;
if (tmp->left == &NIL) {
x = tmp->right;
rbtransplant(root,tmp,tmp->right);
}
else if (tmp->right == &NIL) {
x = tmp->left;
rbtransplant(root,tmp,tmp->left);
}
else {
y = tree_minimum(tmp->right);
y_orig_color = y->color;
x = y->right;
if (y->p == tmp)
x->p = y;
else {
rbtransplant(root,y,y->right);
y->right = tmp->right;
y->right->p = y;
}
rbtransplant(root,tmp,y);
y->left = tmp->left;
y->left->p = y;
y->color = tmp->color;
}
if (y_orig_color == BLACK)
rbdeletefixup(root,x);
}

//Increase the value of entry with key 'id' by 'value'
void increase(node_ptr root,int id,int value) {
node_ptr tmp=search(root,id);
node val={0};
val.key=id;
val.count=value;

if(tmp == &NIL) {
rbinsert(&root,val);
cout << value << "\n";
}
else {
tmp->count += value;
cout << tmp->count<<"\n";
}

}

//Reduces the value of entry with key 'id' by 'value'
void reduce(node_ptr root,int id,int value) {
node_ptr tmp = search(root,id);
if(tmp == &NIL) {
cout << 0 << "\n";
return;
}
tmp->count-=value;
if(tmp->count <= 0) {
rbdelete(&root,id);
cout << 0 << "\n";
return ;
}
cout<< tmp->count << "\n";
}

//Function returns the count of entry with key 'id', basically a get function
void count(node_ptr root,int id) {
node_ptr tmp = search(root,id);
if(tmp == &NIL) {
cout << 0 << "\n";
}
else {
cout << tmp->count << "\n";
}

}

//Function to find the value greater than provided id
node_ptr next(node_ptr root,int id) {
node_ptr prev = NULL;
while(root != &NIL) {
if(root->key > id) {
//record the last max value
prev=root;
root=root->left;
}
else {
root=root->right;
}
}
return prev;
}

//Wrapper Function to find the value greater than provided id
void nextwrapper(node_ptr root,int id) {
node_ptr res = NULL;
res = next(root,id);
if(res)
cout << res->key << " "<< res->count << "\n";
else
cout << 0 << " " << 0 << "\n";
}


//Function to find the value less than provided id
node_ptr previous(node_ptr root,int id) {
node_ptr prev = NULL;
while(root != &NIL) {
if(root->key >= id) {
root = root->left;
}
else {
//record last min value
prev = root;
root = root->right;
}
}
return prev;
}

//Wrapper Function to find the value less than provided id
void prevwrapper(node_ptr root,int id) {
node_ptr res = NULL;
res = previous(root,id);
if(res)
cout << res->key << " " << res->count << "\n";
else
cout << 0 <<" "<< 0 <<"\n";
}

//Function to return the total count of values between id1 and id2
void inrange(node_ptr root,int id1,int id2,int &count) {
if(root == &NIL) return;
if(id1 < root->key)
inrange(root->left,id1,id2,count);

if(id1 <= root->key && id2 >= root->key) {
count+=root->count;
}

if(id2 > root->key)
inrange(root->right,id1,id2,count);
}

//Wrapper Function for finding range between id1 and id2
void rangewrapper(node_ptr root,int id1,int id2) {
int count=0;
if(id1 > id2) {
int tmp = id1;
id1 = id2;
id2 = tmp;
}
inrange(root,id1,id2,count);
cout << count << "\n";
}

void color_tree(node *root,int level) {
if(root == &NIL) return;
//color every odd level as Red
if(level&1) root->color = RED;
//Go Left
color_tree(root->left,level+1);
//Go Right
color_tree(root->right,level+1);
}

//Function which converts array to binary search tree
node *convert(node *arr,int start,int end,node *root)
{
if(start > end) return &NIL;

int mid=start+((end-start)>>1);
//Initialize the root with mid element
node* tmp = &arr[mid];
tmp->color = BLACK;
tmp->p = root;

//Do the same for left subarray
tmp->left = convert(arr,start,mid-1,tmp);
//Do the same for right subarray
tmp->right = convert(arr,mid+1,end,tmp);

return tmp;

}
//Wrapper Function to convert array to balance binary search tree
node* arraytobst(node* nums,int n) {
if(n == 0) return NULL;

node *root = convert(nums,0,n-1,NULL);
color_tree(root,0);
return root;
}

int main(int argc,char *arv[]) {
int n;
ifstream inp;
//Opening file given for reading
inp.open(arv[1],ifstream::in);
//get total number of Key Value Pairs
inp >> n;
node *nums = new node[n];
//Root node for the RB Tree
node_ptr root = NULL;
//Reading Key Value pairs from file
for(int i=0;i<n;++i) {
inp >> nums[i].key >> nums[i].count;
}
inp.close();
//Make a balanced binary tree from input file
root = arraytobst(nums,n);
if(!root) {
cout << "Error in Building Tree\n";
return -1;
}
string command;
//Read command.txt file
while(1) {
cin >> command;
if(command == "quit") {
break;
}
else if(command == "increase") {
node tmp;
int in1,in2;
cin >> in1 >> in2;
increase(root,in1,in2);
}
else if(command == "reduce") {
int in1,in2;
cin >> in1 >> in2;
reduce(root,in1,in2);
}
else if(command == "count") {
int in1;
cin >> in1;
count(root,in1);
}
else if(command == "inrange") {
int in1,in2;
cin >> in1 >> in2;
rangewrapper(root,in1,in2);
}
else if(command == "next") {
int in1;
cin >> in1;
nextwrapper(root,in1);
}
else if(command == "previous") {
int in1;
cin >> in1;
prevwrapper(root,in1);
}

}

return 0;
}
