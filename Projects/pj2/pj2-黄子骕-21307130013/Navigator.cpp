#include "Navigator.h"

using namespace std;

void navigator::decrease_dis(int number, vector<node> &heap, int new_dis){
    heap[number].distance = new_dis;
    return;
}

bool navigator::find_shortest_path_walk(string start, string end){
    //开始计时
    clock_t start_time = clock();
    //确定起始、终点结点
    node sta_node, end_node;
    int flag1 = 0;
    int flag2 = 0;
    for(size_t i = 0; i < nodes.size(); i++){
        if(nodes[i].name == start){
            sta_node = nodes[i];
            flag1 = 1;
        }
        if(nodes[i].name == end){
            end_node = nodes[i];
            flag2 = 1;
        }
    }
    if(!flag1){
        cout << "There is no location: " << start << " in the map!" << endl;
        return false;
    }
    if(!flag2){
        cout << "There is no location: " << end << " in the map!" << endl;
        return false;
    }
    vector<node> pre(nodes.size());//记录每个结点的前驱
    //构造极小堆
    vector<node> heap = nodes;
    //将所有结点的dis置为正无穷，起始点置为0
    for(size_t i = 0; i < heap.size(); i++){
        if(heap[i].name == start){
            heap[i].distance = 0;
        }else{
            heap[i].distance = INT_MAX;
        }
    }    
    make_heap(heap.begin(), heap.end());
    int time;
    while(!heap.empty()){
        //取出堆顶元素
        node min = heap[0];
        if(min.number == end_node.number){
            time = min.distance;
            break;
        }
        //遍历堆，对于相邻结点进行堆的Decrease_key
        for(size_t i = 0; i < heap.size(); i++){
            if(map[min.number][heap[i].number].is_access){
                int sum = min.distance + map[min.number][heap[i].number].walk_time;
                if(heap[i].distance > sum){
                    decrease_dis(i, heap, sum);
                    //更新pre状态
                    pre[heap[i].number] = min;
                }
            }
        }
        //对更新过数据的堆重新heapify
        heap.erase(heap.begin());
        make_heap(heap.begin(), heap.end());
    }
    if(heap.empty()){
        cout << "You can not reach <" << end << "> from <" << start << ">" << endl;
        return false;
    }
    //输出路径以及所需时间
    print_path(pre, sta_node.number, end_node.number);
    cout << "Total time on foot is: " << time << " min" << endl;    
    //结束计时
    clock_t end_time = clock();
    //输出运行时间信息
    double running_time = double(end_time - start_time) / CLOCKS_PER_SEC;
    cout << "Running time is: " << running_time*1000 << " ms" << endl;
    return true;
}

bool navigator::find_shortest_path_bus(string start, string end){
    //开始计时
    clock_t start_time = clock();
    //确定起始、终点结点
    node sta_node, end_node;
    int flag1 = 0;
    int flag2 = 0;
    for(size_t i = 0; i < nodes.size(); i++){
        if(nodes[i].name == start){
            sta_node = nodes[i];
            flag1 = 1;
        }
        if(nodes[i].name == end){
            end_node = nodes[i];
            flag2 = 1;
        }
    }
    if(!flag1){
        cout << "There is no location: " << start << " in the map!" << endl;
        return false;
    }
    if(!flag2){
        cout << "There is no location: " << end << " in the map!" << endl;
        return false;
    }
    vector<node> pre(nodes.size());//记录每个结点的前驱
    //构造极小堆
    vector<node> heap = nodes;
    //将所有结点的dis置为正无穷，起始点置为0
    for(size_t i = 0; i < heap.size(); i++){
        if(heap[i].name == start){
            heap[i].distance = 0;
        }else{
            heap[i].distance = INT_MAX;
        }
    }    
    make_heap(heap.begin(), heap.end());
    int time;
    while(!heap.empty()){
        //取出堆顶元素
        node min = heap[0];
        if(min.number == end_node.number){
            time = min.distance;
            break;
        }
        //遍历堆，对于相邻结点进行堆的Decrease_key
        for(size_t i = 0; i < heap.size(); i++){
            if(map[min.number][heap[i].number].is_access){
                int sum = min.distance + map[min.number][heap[i].number].bus_time;
                if(heap[i].distance > sum){
                    decrease_dis(i, heap, sum);
                    //更新pre状态
                    pre[heap[i].number] = min;
                }
            }
        }
        //对更新过数据的堆重新heapify
        heap.erase(heap.begin());
        make_heap(heap.begin(), heap.end());
    }
    if(heap.empty()){
        cout << "You can not reach <" << end << "> from <" << start << ">" << endl;
        return false;
    }
    //输出路径以及所需时间
    print_path(pre, sta_node.number, end_node.number);
    cout << "Total time by bus is: " << time << " min" << endl;    
    //结束计时
    clock_t end_time = clock();
    //输出运行时间信息
    double running_time = double(end_time - start_time) / CLOCKS_PER_SEC;
    cout << "Running time is: " << running_time*1000 << " ms" << endl;
    return true;
}

bool navigator::find_shortest_path_nolimits(string start, string end){
    //开始计时
    clock_t start_time = clock();
    //确定起始、终点结点
    node sta_node, end_node;
    int flag1 = 0;
    int flag2 = 0;  
    for(size_t i = 0; i < nodes.size(); i++){
        if(nodes[i].name == start){
            sta_node = nodes[i];
            flag1 = 1;
        }
        if(nodes[i].name == end){
            end_node = nodes[i];
            flag2 = 1;
        }
    }
    if(!flag1){
        cout << "There is no location: " << start << " in the map!" << endl;
        return false;
    }
    if(!flag2){
        cout << "There is no location: " << end << " in the map!" << endl;
        return false;
    }
    vector<node> pre(nodes.size());//记录每个结点的前驱
    //构造极小堆
    vector<node> heap = nodes;
    //将所有结点的dis置为正无穷，起始点置为0
    for(size_t i = 0; i < heap.size(); i++){
        if(heap[i].name == start){
            heap[i].distance = 0;
        }else{
            heap[i].distance = INT_MAX;
        }
    }    
    make_heap(heap.begin(), heap.end());
    int time;
    while(!heap.empty()){
        //取出堆顶元素
        node min = heap[0];
        if(min.number == end_node.number){
            time = min.distance;
            break;
        }
        //遍历堆，对于相邻结点进行堆的Decrease_key
        for(size_t i = 0; i < heap.size(); i++){
            if(map[min.number][heap[i].number].is_access){
                int sum = min.distance + std::min(map[min.number][heap[i].number].bus_time, map[min.number][heap[i].number].walk_time);
                if(heap[i].distance > sum){
                    decrease_dis(i, heap, sum);
                    //更新pre状态
                    pre[heap[i].number] = min;
                }
            }
        }
        //对更新过数据的堆重新heapify
        heap.erase(heap.begin());
        make_heap(heap.begin(), heap.end());
    }
    if(heap.empty()){
        cout << "You can not reach <" << end << "> from <" << start << ">" << endl;
        return false;
    }
    //输出路径以及所需时间
    print_path(pre, sta_node.number, end_node.number);
    cout << "Total time is: " << time << " min" << endl;    
    //结束计时
    clock_t end_time = clock();
    //输出运行时间信息
    double running_time = double(end_time - start_time) / CLOCKS_PER_SEC;
    cout << "Running time is: " << running_time*1000 << " ms" << endl;
    return true;  
}

void navigator::print_path(vector<node> pre, int start_num, int end_num){
    vector<node> route;
    route.push_back(nodes[end_num]);
    int i = end_num;
    while(pre[i].number != start_num){
        route.push_back(pre[i]);
        i = pre[i].number;
    }
    route.push_back(nodes[start_num]);
    cout << "Your best route: ";
    for(auto iter = route.end() - 1; iter != route.begin(); iter--){
        cout << iter->name << " ---> ";
    }
    cout << route[0].name << endl;
    return;
}

void navigator::init_map(string file_name){
    ifstream file;
    file.open(file_name, ios::in);
    if(!file.is_open()){
        cout << "Failed to open the file !" << endl;
        return;
    }
    //先读取地点信息
    string line;
    int i = 0;
    while(getline(file, line)){
        if(line == "nodes:"){
            continue;
        }
        if(line == "edges:"){
            break;
        }
        node temp;
        temp.name = line;
        temp.number = i;
        nodes.push_back(temp);
        i++;
    }
    //初始化map大小
    map.resize(nodes.size());
    for(size_t i = 0; i < nodes.size(); i++){
        map[i].resize(nodes.size());
    }
    //读取边的信息
    while(getline(file, line)){
        //从文件中取得信息
        auto st = 0;
        st = line.find(" ");
        string origin = line.substr(0, st);
        vector<string> save;
        save.push_back(origin);
        size_t end;
        st++;    
        end = line.find(" ", st);
        while(end != string::npos){
            string temp = line.substr(st, end - st);
            save.push_back(temp);
            st = end + 1;
            end = line.find(" ", st);
        }
        string last = line.substr(st);
        save.push_back(last);
        //存入map中
        //先找到出发点编号
        int origin_num = find_number(origin);
        size_t i = 1;
        while(i < save.size()){
            int destination_num = find_number(save[i]);
            int walk_time = stoi(save[i+1]);
            int bus_time = stoi(save[i+2]);
            map[origin_num][destination_num].is_access = true;
            map[origin_num][destination_num].walk_time = walk_time;
            map[origin_num][destination_num].bus_time = bus_time;
            i = i + 3;
        }
    }
    
}

int navigator::find_number(string name){
    int number = 0;
    for(size_t i = 0; i < nodes.size(); i++){
        if(nodes[i].name == name){
            number = nodes[i].number;
        }
    }
    return number;
}

void navigator::print_nodes(){
    for(int i = 0; i < nodes.size(); i++){
        cout << nodes[i].name << " ";
    }
    cout << endl;
    return;
}

void navigator::print_map(){
    for(int i = 0; i < nodes.size(); i++){
        for(int j = 0; j < nodes.size(); j++){
            if(map[i][j].is_access){
                cout << map[i][j].walk_time << " " << map[i][j].bus_time << " " << nodes[i].name << " " << nodes[j].name << endl;
            }else{
                cout << "no edge!" << endl;
            }
        }
    }
    return;
}

int navigator::find_node(int number, vector<node> heap){
    for(int i = 0; i < heap.size(); i++){
        if(number == heap[i].number){
            return i;
        }
    }
    return -1;
}
