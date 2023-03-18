#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<istream>

using namespace std;

struct stu{
    string id = "";
    string name = "";
    string major = "";
    string gender = "";
    int key = 0;
};

struct hashSlot{
    stu inf;
    int key = 0;
    int times2find = 0;
    bool flag = true;
};

void read_data(vector<stu> &stuinf){
    string x;
    stu temp;
    int count = 1;
    while(cin >> x){
        if(count % 4 == 1)  temp.id = x;
        if(count % 4 == 2)  temp.name = x;
        if(count % 4 == 3)  temp.major = x;
        if(count % 4 == 0){
            temp.gender = x;
            stuinf.push_back(temp);
        }
        count++;
        cin.clear();
    }
}


int generate_key(string name){
    int sum = 0;
    for(int i =0; i < name.size(); i++){
        sum += toascii(name[i]);
    }
    return sum;
}

void add_key(vector<stu> &stuinf){
    for(int i = 0; i < stuinf.size(); i++){
        stuinf[i].key = generate_key(stuinf[i].name);
    }
}

int hash_func(int key){
    return (key % 97);
}

void hash_insert(stu x, vector<hashSlot> &hashmap){
    int idx = hash_func(x.key);
    int times = 1;
    int cur_idx = idx;
    if(hashmap[cur_idx].times2find != 0 || hashmap[cur_idx].flag == false){
        while(hashmap[cur_idx].times2find != 0 || hashmap[cur_idx].flag == false){
            cur_idx = (cur_idx+1) % 100;
            times++;
            if(idx == cur_idx){
                cout << "Hash table is full!";
                return;
            }
        }
    }
    hashmap[cur_idx].inf = x;
    hashmap[cur_idx].key = x.key;
    hashmap[cur_idx].times2find = times;
}

vector<hashSlot> init_hashmap(vector<stu> stu_info, vector<hashSlot> &hashmap){
    hashSlot init_h;
    for(int i = 0; i < 100; i++){
        hashmap.push_back(init_h);
    }
    for(int j = 0; j < stu_info.size(); j++){
        hash_insert(stu_info[j], hashmap);
    }
    return hashmap;
}

stu hash_search(string name, vector<hashSlot> hashmap){
    stu null;
    int name_key = generate_key(name);
    int idx = hash_func(name_key);
    int cur_idx = idx;
    // if(hashmap[idx].times2find == 0 && hashmap[idx].flag != false){
    //     cout << "Can not find!";
    // }
    while((hashmap[cur_idx].key != name_key) || (hashmap[cur_idx].inf.name != name)){
        cur_idx = (cur_idx+1)%100;
        if(idx == cur_idx){
            cout << "Can not find" << endl;
            return null;
        }
    }
    if(hashmap[cur_idx].flag == false){
        cout << "Has been deleted!" << endl; 
        return null;
    } 
    return hashmap[cur_idx].inf;
}

void hash_delete(string name, vector<hashSlot> &hashmap){
    int name_key = generate_key(name);
    int idx = hash_func(name_key);
    if(hashmap[idx].times2find == 0 && hashmap[idx].flag != false){
        cout << "Can not find!";
    }
    while((hashmap[idx].key != name_key) || (hashmap[idx].inf.name != name)){
        idx = (idx+1) % 100;
    }
    hashmap[idx].flag = false;
    hashmap[idx].times2find = 0;
}

float evaluate(vector<hashSlot> hashmap){
    int sum;
    int len = hashmap.size();
    int count = 0;
    for(int i = 0; i < hashmap.size(); i++){
        if(hashmap[i].times2find != 0)  count++;
        sum += hashmap[i].times2find;
    }
    return (float)sum / count;
}

int main(){
    //将数据读入stu_info中，并计算key值
    vector<stu> stu_info;
    read_data(stu_info);
    add_key(stu_info);
    //初始化并将stu_info中数据插入hashmap
    vector<hashSlot> hashmap;
    init_hashmap(stu_info, hashmap);
    //测试hash_search
    stu result = hash_search("huangzisu", hashmap);
    cout << result.id << " " << result.name << " " << result.major << " " << result.gender << endl;
    //测试hash_insert
    stu temp;
    temp.name = "test_name";
    temp.gender = "test_gender";
    temp.id = "test_id";
    temp.key = generate_key(temp.name);
    temp.major = "test_major";
    hash_search(temp.name, hashmap);
    hash_insert(temp, hashmap);
    result = hash_search(temp.name, hashmap);
    cout << result.id << " " << result.name << " " << result.major << " " << result.gender << endl;
    //测试hash_delete
    hash_delete(temp.name, hashmap);
    hash_search(temp.name, hashmap);
    //测试evaluate
    cout << evaluate(hashmap) << endl;

    return 0;
}