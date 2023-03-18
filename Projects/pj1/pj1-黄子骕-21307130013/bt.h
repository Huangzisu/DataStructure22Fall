#ifndef _BT_H
#define _BT_H

#include<string>
#include<vector>
#include<iostream>
#include<fstream>

using namespace std;

/////////////////////////////////////////定义键值对、结点
struct key_value{
    string wordNpos;
    string value;  
};

struct bt_node{
    int keys_num = 0;
    vector<key_value> pairs;
    bool leaf = true;
    vector<bt_node*> children;
};
/////////////////////////////////////////定义类
class bt{
public:
    vector<string> split(string x);
    bt(int x): root(nullptr), minimum_degree(x){}
    bt_node* get_root();
    void update_root(bt_node* x);
    int get_minimum_degree();
    void insert_by_command(string raw_data);
    pair<bt_node*, int> search(string wordNpos);
    pair<bt_node*, int> sub_search(bt_node* cur, string wordNpos);
    void split_child(bt_node* cur, int i);
    void insert_nonfull(bt_node* cur, key_value pair);
    bool initialize(string file_name);
    bool test(bt_node* x);
    bool insert_by_file(string file_name);
    void delete_by_command(string wordNpos);
    bt_node* find_successor(bt_node* cur, string wordNpos);
    bt_node* find_predecessor(bt_node* cur, string wordNpos);
    bool test_internal_node(bt_node* cur, string wordNpos);
    void _delete(bt_node* cur, string wordNpos);
    void delete_by_file(string file_name);
    void update_by_command(string oldone_wordNpos, string newone);
    void dump(bt_node* cur);
    int num(bt_node* cur);
private:
    bt_node* root;
    int minimum_degree;
};

#endif