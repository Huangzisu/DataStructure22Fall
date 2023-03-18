#ifndef _NAVIGATOR_H
#define _NAVIGATOR_H

#include<string>
#include<queue>
#include<vector>
#include<limits.h>
#include<algorithm>
#include<iostream>
#include<fstream>
#include<time.h>

using namespace std;

struct edge{
    bool is_access = false;
    int bus_time = 0;
    int walk_time = 0;
};

struct node{
    string name;
    int number;
    int distance;
    bool operator<(const node &x) const{
        return distance > x.distance;
    }
};

class navigator{
public:
    navigator(){}
    void init_map(string file_name);
    bool find_shortest_path_walk(string start, string end);
    bool find_shortest_path_bus(string start, string end);
    bool find_shortest_path_nolimits(string start, string end);
    void decrease_dis(int number, vector<node> &heap, int new_dis);
    void print_path(vector<node> pre, int start_num, int end_num);
    int find_number(string name);
    int find_node(int number, vector<node> heap);
    void print_nodes();
    void print_map();
private:
    vector< vector<edge> > map;
    vector<node> nodes;
};



#endif