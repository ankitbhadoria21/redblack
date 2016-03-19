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
struct node NIL={0};
node_ptr NILPTR = &NIL;

void inorder(node_ptr root) {
if (root != NILPTR) {
inorder(root->left);
cout<<root->key<<" "<<root->count<<"\n";
inorder(root->right);
}
}

//Doing normal binary search, if the element is less than root search left else search right
node_ptr search(node_ptr root, int k) {
if (root == NILPTR || root->key == k)
return root;
if (k < root->key)
return search(root->left, k);
else
return search(root->right, k);
}

node_ptr tree_minimum(node_ptr root) {
while (root->left != NILPTR)
root = root->left;
return root;
}

node_ptr tree_maximum(node_ptr root) {
while (root->right != NILPTR)
root = root->right;
return root;
}

void leftrotate(node_ptr *root, node_ptr x) {
node_ptr y = x->right;
x->right = y->left;
if (y->left != NILPTR)
y->left->p = x;
y->p = x->p;
if (x->p == NILPTR)
*root = y;
else if (x->p->left == x)
x->p->left = y;
else
x->p->right = y;
y->left = x;
x->p = y;
}

void rightrotate(node_ptr *root, node_ptr y) {
node_ptr x = y->left;
y->left = x->right;
if (x->right != NILPTR)
x->right->p = y;
x->p = y->p;
if (y->p == NILPTR)
*root = x;
else if (y->p->left == y)
y->p->left = x;
else
y->p->right = x;
x->right = y;
y->p = x;
}

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

void rbinsert(node_ptr *root, node z) {
node_ptr tmp = (node_ptr) new node;
tmp->key = z.key;
tmp->count = z.count;
node_ptr y = NILPTR;
node_ptr x = *root;
while (x != NILPTR) {
y = x;
if (tmp->key < x->key)
x = x->left;
else
x = x->right;
}
tmp->p = y;
if (y == NILPTR)
*root = tmp;
else if (tmp->key < y->key)
y->left = tmp;
else
y->right = tmp;
tmp->left = NILPTR;
tmp->right = NILPTR;
tmp->color = RED;
rbinsertfixup(root,tmp);
}

void rbtransplant(node_ptr *root, node_ptr u, node_ptr v) {
if (u->p == NILPTR)
*root = v;
else if (u == u->p->left)
u->p->left = v;
else
u->p->right = v;
v->p = u->p;
}

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

void rbdelete(node_ptr *root, int z) {
node_ptr tmp = search(*root, z);
if (tmp == NILPTR) {
return;
}
node_ptr y = tmp;
int y_orig_color = y->color;
node_ptr x;
if (tmp->left == NILPTR) {
x = tmp->right;
rbtransplant(root,tmp,tmp->right);
}
else if (tmp->right == NILPTR) {
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

void increase(node_ptr root,int id,int value) {
node_ptr tmp=search(root,id);
node val={0};
val.key=id;
val.count=value;

if(tmp == NILPTR) {
rbinsert(&root,val);
cout << value << "\n";
}
else {
tmp->count += value;
cout << tmp->count<<"\n";
}

}

void reduce(node_ptr root,int id,int value) {
node_ptr tmp = search(root,id);
if(tmp == NILPTR) {
cout << 0 << "\n";
return;
}
tmp->count-=value;
if(tmp->count <= 0) {
rbdelete(&root,id);
cout << 0 << "\n";
}
cout<< tmp->count << "\n";
}

void count(node_ptr root,int id) {
node_ptr tmp=search(root,id);
if(tmp == NILPTR) {
cout << 0 << "\n";
}
else {
cout << tmp->count << "\n";
}

}

void next(node_ptr root, int id) {
node_ptr temp = search(root, id);

if (temp == NILPTR) {
cout <<0 << " " << 0 << "\n";
return;
}
if (temp->right != NILPTR)
{
node_ptr tmp;
tmp = tree_minimum(temp->right);
cout << tmp->key <<" "<< tmp->count << endl;
return;
}
node_ptr y = temp->p;
while (y != NILPTR && temp == y->right) {
temp = y;
y = y->p;
}
cout << y->key <<" "<< y->count << "\n";
}

void previous(node_ptr root, int id) {
node_ptr temp = search(root, id);
if (temp == NILPTR) {
cout << 0 << " " << 0 << "\n";
return;
}
if (temp->left != NILPTR) {
node_ptr tmp = tree_maximum(temp->left);
cout << tmp->key <<" "<< tmp->count << "\n";
return;
}
node_ptr y = temp->p;
while (y != NILPTR && temp == y->left) {
temp = y;
y = y->p;
}
cout << y->key <<" "<< y->count << "\n";
}

void inrange(node_ptr root,int id1,int id2,int &count) {
if(root==NULL) return;
if(id1 < root->key)
inrange(root->left,id1,id2,count);

if(id1 <= root->key && id2 >= root->key) {
count+=root->count;
}

if(id2 > root->key)
inrange(root->right,id1,id2,count);
}

void rangewrapper(node_ptr root,int id1,int id2) {
int count=0;
if(id1>id2) {
int tmp=id1;
id1=id2;
id2=tmp;
}
inrange(root,id1,id2,count);
cout << count << "\n";
}

void color_tree(node *root,int level) {
if(root == &NIL) return;
if(level&1) root->color=RED;
color_tree(root->left,level+1);
color_tree(root->right,level+1);
}

node *convert(node *arr,int start,int end,node *root)
{
if(start > end) return &NIL;

int mid=start+((end-start)>>1);

node* tmp = &arr[mid];
tmp->color = BLACK;
tmp->p = root;

tmp->left = convert(arr,start,mid-1,tmp);

tmp->right = convert(arr,mid+1,end,tmp);

return tmp;

}

node* arraytobst(node* nums,int n) {
if(n == 0) return NULL;

node *root = convert(nums,0,n-1,NULL);
color_tree(root,0);
return root;
}

int main(int argc,char *arv[]) {
int n;
ifstream inp;
inp.open(arv[1],ifstream::in);
inp>>n;
node *nums=new node[n];
node_ptr root=NULL;
for(int i=0;i<n;++i) {
inp>>nums[i].key>>nums[i].count;
}
inp.close();
root=arraytobst(nums,n);
cout<<"Tree Built\n";
/*
increase (root,350 ,100);
reduce (root,350, 50);
count (root,350);
rangewrapper(root, 300, 1000);
rangewrapper(root,200, 299);
rangewrapper(root,200 ,1000);
rangewrapper(root,300, 349);
rangewrapper(root,350, 350);
rangewrapper(root,349, 350);
next (root,300);
next (root,349);
next (root,360);
previous (root,360);
previous (root,350);
previous (root,0);
reduce (root,271, 6);
previous (root,350);
reduce (root,271, 3);
previous (root,350);
previous (root,150);
*/
string command;
while(1) {
cin>>command;
if(command=="quit") {
break;
}
else if(command=="increase") {
node tmp;
int in1,in2;
cin>>in1>>in2;
increase(root,in1,in2);
}
else if(command=="reduce") {
int in1,in2;
cin>>in1>>in2;
reduce(root,in1,in2);
}
else if(command=="count") {
int in1;
cin>>in1;
count(root,in1);
}
else if(command=="inrange") {
int in1,in2;
cin>>in1>>in2;
rangewrapper(root,in1,in2);
}
else if(command=="next") {
int in1;
cin>>in1;
next(root,in1);
}
else if(command=="previous") {
int in1;
cin>>in1;
previous(root,in1);
}

}
return 0;
}
