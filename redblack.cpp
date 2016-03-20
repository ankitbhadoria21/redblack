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
void leftrotate(node_ptr *root, node_ptr x) {
node_ptr y = x->right;
x->right = y->left;
if (y->left != &NIL)
y->left->p = x;
y->p = x->p;
if (x->p == &NIL)
*root = y;
else if (x->p->left == x)
x->p->left = y;
else
x->p->right = y;
y->left = x;
x->p = y;
}

//Rigth Rotate Function
void rightrotate(node_ptr *root, node_ptr y) {
node_ptr x = y->left;
y->left = x->right;
if (x->right != &NIL)
x->right->p = y;
x->p = y->p;
if (y->p == &NIL)
*root = x;
else if (y->p->left == y)
y->p->left = x;
else
y->p->right = x;
x->right = y;
y->p = x;
}

//Red Black FixUp Function
void rbinsertfixup(node_ptr *root, node_ptr z) {
while (z->p->color == RED) {
if (z->p == z->p->p->left) {
node_ptr y = z->p->p->right;
if (y->color == RED) {
z->p->color = BLACK;
y->color = BLACK;
z->p->p->color = RED;
z = z->p->p;
}
else {
if (z == z->p->right) {
z = z->p;
leftrotate(root,z);
}
z->p->color = BLACK;
z->p->p->color = RED;
rightrotate(root,z->p->p);
}
}
else {
node_ptr y = z->p->p->left;
if (y->color == RED) {
z->p->color = BLACK;
y->color = BLACK;
z->p->p->color = RED;
z = z->p->p;
}
else {
if (z == z->p->left) {
z = z->p;
rightrotate(root,z);
}
z->p->color = BLACK;
z->p->p->color = RED;
leftrotate(root,z->p->p);
}
}
}
(*root)->color = BLACK;
}

//Red Black Insert Function
void rbinsert(node_ptr *root, node z) {
node_ptr tmp = (node_ptr) new node;
tmp->key = z.key;
tmp->count = z.count;
node_ptr y = &NIL;
node_ptr x = *root;
while (x != &NIL) {
y = x;
if (tmp->key < x->key)
x = x->left;
else
x = x->right;
}
tmp->p = y;
if (y == &NIL)
*root = tmp;
else if (tmp->key < y->key)
y->left = tmp;
else
y->right = tmp;
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
