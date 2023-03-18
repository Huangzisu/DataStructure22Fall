#ifndef _RBT_H
#define _RBT_H

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#define black 1
#define red 0

using namespace std;

//////////////////////////////////////////////////////////定义结点
struct rbt_node{
    rbt_node* left = nullptr;
    rbt_node* right = nullptr;
    rbt_node* parent = nullptr;
    string wordNpos;
    string value;
    int colour = red;//0时为红，1时为黑,且初始化节点为红
};
//////////////////////////////////////////////////////////定义rbt类
class rbt{
public:
    vector<string> split(string x);
    rbt_node* find_successor(rbt_node* x);
    rbt(): root(nullptr){}
    void update_root(rbt_node* x);
    rbt_node* get_root() const; 
    void left_rotate(rbt_node* x);
    void right_rotate(rbt_node* x);
    bool insert(rbt_node* x);
    bool insert_by_command(string x);
    bool insert_by_file(string file_name);
    void dump(rbt_node* x) const;
    bool initialize(string file_name);
    bool delete_by_command(string wordNpos);
    bool delete_by_file(string file_name);
    rbt_node* search(string wordNpos) const;
    bool update_by_command(string oldone_wordNpos, string newone);
    bool test() const;
    bool sub_test(rbt_node* x, int sum, int black_count) const;
private:
    rbt_node* root;
};


#endif