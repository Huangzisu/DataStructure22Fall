#include<iostream>
#include<vector>
#include<memory>
using namespace std;

struct node{
    int key = 0;
    int value = 0;
    node *parent = nullptr;
    node *left = nullptr;
    node *right = nullptr;
};

void insert_sub(node *&T, node *parent, node *&z){
    if(!T){
        z->parent = parent;
        T = z;
        return;
    }
    if(z->key < T->key){
        insert_sub(T->left, T, z);
    }else if(z->key > T->key){
        insert_sub(T->right, T, z);
    }else{
        cout << "插入失败";
        return;
    }
    return;
}

void insert(node *&T, node *&z){
    if(!T){
        T = z;
        return;
    }
    if(z->key < T->key){
        insert_sub(T->left, T, z);
    }else if(z->key > T->key){
        insert_sub(T->right, T, z);
    }else{
        cout << "插入失败";
        return;
    }
    return;
}

void build_BST(node *&root){
    int key, val;
    while(cin >> key >> val){
        node *temp = new node();
        temp->key = key;
        temp->value = val;
        insert(root, temp);
    }
}

void inorder_treewalk(node *T){
    if(T){
        inorder_treewalk(T->left);      
        cout << T->value;
        inorder_treewalk(T->right);
    }
    return;
}

int minimum(node *x){
    while(x->left){
        x = x->left;
    }
    return x->value;
}

int maximum(node *x){
    while(x->right){
        x = x->right;
    }
    return x->value;
}

int search_recursion(node *x, int k){
    if(!x){
        return -1;
    }
    if(x->key == k){
        return x->value;
    }else if(x->key > k){
        return search_recursion(x->left, k);
    }else{
        return search_recursion(x->right, k);
    }
}

int search_iterative(node *x, int k){
    while(x->key != k){
        if(!x){
            return -1;
        }
        if(k > x->key){
            x = x->right;
        }else{
            x = x->left;
        }
    }
    return x->value;
}

int successor(node *x){
    if(x->right != nullptr){
        return minimum(x->right);
    }
    node *y = x->parent;
    while(y != nullptr && x == y->right){
        x = y;
        y = x->parent;
    }
    return y->value;
}

node* successor_node(node *x){
    if(x->right != nullptr){
        x = x->right;
        while(x->left){
            x = x->left;
        }
        return x;
    }
    node *y = x->parent;
    while(y != nullptr && x == y->right){
        x = y;
        y = x->parent;
    }
    return y;
}

int predecessor(node *x){
    if(x->left != nullptr){
        return maximum(x->left);
    }
    node *y = x->parent;
    while(y != nullptr && x == y->left){
        x = y;
        y = x->parent;
    }
    return y->value;
}

void _delete(node *&root, node *&z){
    node *y;
    if(z->left == nullptr || z->right == nullptr){
        y = z;
    }else{
        y = successor_node(z); 
    }
    node *x;
    if(y->left){
        x = y->left;
    }else{
        x = y->right;
    }
    if(x){
        x->parent = y->parent;
    }
    if(!y->parent){
        root = x;
    }else if(y == y->parent->left){
        y->parent->left = x;
    }else{
        y->parent->right = x;
    }
    if(y != z){
        z->key = y->key;
    }
}

void preorder_treewalk(node *T){
    if(T){
        cout << T->value;
        preorder_treewalk(T->left);      
        preorder_treewalk(T->right);
    }
    return;
}

void postorder_treewalk(node *T){
    if(T){
        postorder_treewalk(T->left);     
        postorder_treewalk(T->right);
        cout << T->value;
    }
    return;
}

void traversal(node *root){
    preorder_treewalk(root);
    cout << endl;
    inorder_treewalk(root);
    cout << endl;
    postorder_treewalk(root);
}


int main(){
    //建立BST，输入用空格隔开的数字对，前一个代表key值，后一个代表value
    //如：
    //2 3 1 9 6 7
    node *rootp = new node();
    rootp->key = 4;//此为根结点数据，可以随意设置
    rootp->value = 6;
    build_BST(rootp);
    //测试maximum、minimum
    cout << "minimum is: " << minimum(rootp) << endl << "maximum is: " << maximum(rootp);
    //测试search
    cout << endl  << "result of search_recursion: " <<  search_recursion(rootp, 4) << endl << "result of search_iterative: " << search_iterative(rootp, 4);
    //测试sucessor与predecessor
    cout << endl << "the value of its successor is: " << successor(rootp) << endl << "the value of its predecessor is: " << predecessor(rootp);
    //测试insert与delete
    node *new_node = new node();
    new_node->key = 5;
    new_node->value = 10;
    insert(rootp, new_node);
    cout << endl << search_recursion(rootp, new_node->key);
    _delete(rootp, new_node);
    cout << endl << search_recursion(rootp, new_node->key);
    //测试遍历
    cout << endl;
    traversal(rootp);
}