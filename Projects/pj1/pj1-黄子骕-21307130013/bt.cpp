#include "bt.h"

/////////////////////////////////////////类成员函数实现
int bt::num(bt_node* cur){
    int sum = 0;
    if(cur->leaf){
        return cur->keys_num;
    }
    for(int i = 0; i <= cur->keys_num; i++){
        sum += num(cur->children[i]);
    }
    return sum; 
}

void bt::dump(bt_node* cur){
    if(cur->leaf){
        for(int i = 0; i < cur->keys_num; i++){
            cout << cur->pairs[i].wordNpos << " " << cur->pairs[i].value << endl;
        }
        return;
    }
    for(int i = 0; i < cur->keys_num; i++){
            dump(cur->children[i]);
            cout << cur->pairs[i].wordNpos << " " << cur->pairs[i].value << endl;
        }
    dump(cur->children[cur->keys_num]);
    return;
}

void bt::update_by_command(string oldone_wordNpos, string newone){
    //首先寻找是否有该结点
    pair<bt_node*, int> update = sub_search(get_root(), oldone_wordNpos);
    //报告提示并进行相应处理，若有则删除并插入新结点，若无则直接插入新结点
    if(update.first){
        cout << "Updating the key-value pair......" << endl;
        delete_by_command(oldone_wordNpos);
    }else{
        cout << "Inserting the key-value pair......" << endl;
    }
    insert_by_command(newone);
    cout << "Update done!" << endl;
    return;
}

void bt::delete_by_file(string file_name){
    ifstream file;
    file.open(file_name, ios::in);
    if(!file.is_open()){
        cout << "Fail to open the file!" << endl;
        return;
    }
    string raw_data;
    vector<string> splited_data;
    getline(file, raw_data);//吸收第一行的attribute
    raw_data.clear();
    while(getline(file, raw_data)){
        splited_data = split(raw_data);
        delete_by_command(splited_data[0]);
        raw_data.clear();
        splited_data.clear();
    }
    cout << "Delete_by_file: " << file_name << " completed!" << endl;
    return;        
}

void bt::delete_by_command(string wordNpos){
    if(sub_search(get_root(), wordNpos).first){
        _delete(get_root(), wordNpos);
        cout << "Successfully deleted wordNpos: " << wordNpos << "!" << endl;
    }else{
        cout << "key: " << wordNpos << "   missing!" << endl;
    }
}

void bt::_delete(bt_node* cur, string wordNpos){
    //找到大于等于key值的最小元素（或是目标本身）位置
    int t = get_minimum_degree();
    int pos = 0;
    for(; pos < cur->keys_num; pos++){
        if(cur->pairs[pos].wordNpos >= wordNpos){
            break;
        }
    }
    //目标key在当前结点
    if(cur->pairs[pos].wordNpos == wordNpos && pos < cur->keys_num){
        //case1：当前结点为叶节点，直接删除
        if(cur->leaf){
            cur->pairs.erase(cur->pairs.begin() + pos);
            cur->keys_num--;
            return;
        }    
        //case2：x为内结点
        bt_node* right_child = cur->children[pos+1];
        bt_node* left_child = cur->children[pos];
        //case2a：left_child中有至少t个key
        if(left_child->keys_num >= t){
            bt_node* predecessor = find_predecessor(cur, wordNpos);
            cur->pairs[pos] = predecessor->pairs[predecessor->keys_num - 1];
            _delete(left_child, predecessor->pairs[predecessor->keys_num - 1].wordNpos);
        }
        //case2b：right_child中至少有t个key
        else if(right_child->keys_num >= t){
            bt_node* successor = find_successor(cur, wordNpos);
            cur->pairs[pos] = successor->pairs[0];
            _delete(right_child, successor->pairs[0].wordNpos);
        }
        //case2c：left_child与right_child都只有t-1个key
        else if(left_child->keys_num == t - 1 && right_child->keys_num == t - 1){
            //将cur中key下移
            left_child->pairs.insert(left_child->pairs.begin() + left_child->keys_num, cur->pairs[pos]);
            //将right_child中key左移
            for(int i = 0; i < right_child->keys_num; i++){
                left_child->pairs.insert(left_child->pairs.begin() + t + i, right_child->pairs[i]);
            }
            //若有孩子则也需合并
            if(!left_child->leaf){
                for(int i = 0; i <= right_child->keys_num; i++){
                    left_child->children.insert(left_child->children.begin() + t + i, right_child->children[i]);
                }
            }
            //更新left_child中key数量
            left_child->keys_num = 2 * t - 1;
            //删除失效的数据
            cur->pairs.erase(cur->pairs.begin() + pos);
            cur->children.erase(cur->children.begin() + pos + 1);
            //更新cur中key数量
            cur->keys_num--;
            //若cur中key为0且为root，则更新root
            if(cur->keys_num == 0 && cur == get_root()){
                update_root(left_child);
            }
            //删除right_child
            delete right_child;
            //递归删除
            _delete(left_child, wordNpos);
        }
    }else{//case3：目标key不在当前结点
        //若当前结点为叶结点，则无此key
        if(cur->leaf){
            cout << "key: " << wordNpos << "  missing!" << endl;
            return;
        }
        bt_node* subtree = cur->children[pos];
        bt_node* right_sibling;
        bt_node* left_sibling;
        if(pos != 0){
            left_sibling = cur->children[pos - 1];
        }else{
            left_sibling = nullptr;
        }
        if(pos != cur->keys_num){
            right_sibling = cur->children[pos + 1];
        }else{
            right_sibling = nullptr;
        }
        if(subtree->keys_num == t - 1){
            //case3a：下一次寻找的结点有t-1个key，相邻兄弟至少一个有至少t个key
            //right_sibling有至少t个key
            if(right_sibling && right_sibling->keys_num >= t){
                //将cur中的key下移
                subtree->pairs.insert(subtree->pairs.begin() + subtree->keys_num, cur->pairs[pos]);
                subtree->keys_num++;
                //将right_sibling中一key上移
                cur->pairs[pos] = right_sibling->pairs[0];
                right_sibling->pairs.erase(right_sibling->pairs.begin());
                //移动right_sibling的children
                if(!subtree->leaf){
                    subtree->children.insert(subtree->children.begin() + subtree->keys_num, right_sibling->children[0]);
                    right_sibling->children.erase(right_sibling->children.begin());
                }
                right_sibling->keys_num--;
            }
            //left_sibling有至少t个key
            else if(left_sibling && left_sibling->keys_num >= t){
                //将cur中key下移
                subtree->pairs.insert(subtree->pairs.begin(), cur->pairs[pos - 1]);
                subtree->keys_num++;
                //将left_sibling中一key上移
                cur->pairs[pos - 1] = left_sibling->pairs[left_sibling->keys_num - 1];
                left_sibling->pairs.erase(left_sibling->pairs.begin() + left_sibling->keys_num - 1);
                //移动children
                if(!subtree->leaf){
                    subtree->children.insert(subtree->children.begin(), left_sibling->children[left_sibling->keys_num]);
                    left_sibling->children.erase(left_sibling->children.begin() + left_sibling->keys_num);
                }
                left_sibling->keys_num--;
            }else{//case3b：相邻兄弟都只有t-1个key
                //若有right_sibling，则与其合并
                if(right_sibling){
                    //将cur中key下移
                    subtree->pairs.insert(subtree->pairs.begin() + subtree->keys_num, cur->pairs[pos]);
                    //将right_sibling中key左移
                    for(int i = 0; i < right_sibling->keys_num; i++){
                        subtree->pairs.insert(subtree->pairs.begin() + t + i, right_sibling->pairs[i]);                        
                    }
                    if(!subtree->leaf){
                        for(int i = 0; i <= right_sibling->keys_num; i++){
                            subtree->children.insert(subtree->children.begin() + t + i, right_sibling->children[i]);
                        }
                    }
                    //更新subtree的key数量
                    subtree->keys_num += 1 + right_sibling->keys_num;
                    //更新数据
                    cur->pairs.erase(cur->pairs.begin() + pos);
                    cur->children.erase(cur->children.begin() + pos + 1);
                    cur->keys_num--;
                    //若cur为root且key数量降到0，则更新root
                    if(cur->keys_num == 0 && cur == get_root()){
                        update_root(subtree);
                    }
                }else{//与左兄弟合并
                    pos--;
                    //将cur中key下移
                    left_sibling->pairs.insert(left_sibling->pairs.begin() + left_sibling->keys_num, cur->pairs[pos]);
                    //将subtree中key左移
                    for(int i = 0; i < subtree->keys_num; i++){
                        left_sibling->pairs.insert(left_sibling->pairs.begin() + t + i, subtree->pairs[i]);                        
                    }
                    if(!left_sibling->leaf){
                        for(int i = 0; i <= subtree->keys_num; i++){
                            left_sibling->children.insert(left_sibling->children.begin() + t + i, subtree->children[i]);
                        }
                    }
                    //更新left_sibling的key数量
                    left_sibling->keys_num += 1 + subtree->keys_num;
                    //更新数据
                    cur->pairs.erase(cur->pairs.begin() + pos);
                    cur->children.erase(cur->children.begin() + pos + 1);
                    cur->keys_num--;
                    //若cur为root且key数量降到0，则更新root
                    if(cur->keys_num == 0 && cur == get_root()){
                        update_root(left_sibling);
                    }
                    subtree = left_sibling;     
                }
            }      
        }
        _delete(subtree, wordNpos);
    }
    return;
}

bool bt::test_internal_node(bt_node* cur, string wordNpos){
    for(int i = 0; i < cur->keys_num; i++){
        if(cur->pairs[i].wordNpos == wordNpos){
            return true;
        }
    }
    return false;
}

bt_node* bt::find_predecessor(bt_node* cur, string wordNpos){
    //找到当前key位置
    int i = 0;
    while(i < cur->keys_num && cur->pairs[i].wordNpos != wordNpos){
        i++;
    }
    cur = cur->children[i];
    //寻找predecessor
    while(!cur->leaf){
        cur = cur->children[cur->keys_num];
    }
    return cur;    
}

bt_node* bt::find_successor(bt_node* cur, string wordNpos){
    //找到当前key位置
    int i = 0;
    while(i < cur->keys_num && cur->pairs[i].wordNpos != wordNpos){
        i++;
    }
    cur = cur->children[i+1];
    //寻找successor
    while(!cur->leaf){
        cur = cur->children[0];
    }
    return cur;
}

bool bt::insert_by_file(string file_name){
    ifstream file;
    file.open(file_name, ios::in);
    if(!file.is_open()){
        cout << "Fail to open the file!" << endl;
        return false;
    }
    string raw_data;
    getline(file, raw_data);//吸收第一行的attribute
    raw_data.clear();
    while(getline(file, raw_data)){
        bt_node* temp = new(bt_node);
        insert_by_command(raw_data);
        raw_data.clear();
    }
    cout << "Insert_by_file: " << file_name << " completed!" << endl;
    return true;
}

bool bt::test(bt_node* x){
    //测试是否每一个结点符合升序
    for(int i = 0; i < x->keys_num - 1; i++){
        if(x->pairs[i].wordNpos > x->pairs[i+1].wordNpos){
            return false;
        }
    }
    //测试每个结点key个数
    if(x->keys_num > 2 * get_minimum_degree() - 1 || x->keys_num < 1){
        return false;
    }
    if(!x->leaf){
        for(int i = 0; i <= x->keys_num; i++){
            if(!x->children[i]){
                return false;
            }
        }           
    }
    if(x->leaf){
        return true;
    }
    for(int i = 0; i <= x->keys_num; i++){
        if(!test(x->children[i])){
            return false;
        }
    }
    return true;
}

bool bt::initialize(string file_name){
    ifstream file;
    file.open(file_name, ios::in);
    if(!file.is_open()){
        cout << "Fail to open the file!" << endl;
        return false;
    }
    string raw_data;
    getline(file, raw_data);//吸收第一行的attribute
    raw_data.clear();
    while(getline(file, raw_data)){
        insert_by_command(raw_data);
        raw_data.clear();
    }
    cout << "Initialize_by_file: " << file_name << " completed!" << endl;
    return true;
}

void bt::insert_by_command(string raw_data){
    vector<string> splited_data = split(raw_data);
    string wordNpos = splited_data[0];
    string value = splited_data[1];
    key_value pair;
    pair.wordNpos = wordNpos;
    pair.value = value;
    //若为空树则设其为根结点
    if(!get_root()){
        bt_node* new_node = new(bt_node);
        update_root(new_node);
        new_node->children.resize(1);
        new_node->leaf = true;
        new_node->pairs.insert(new_node->pairs.begin(), pair);
        new_node->keys_num++;
        cout << "Successfully inserted the key-value pair: " << raw_data << endl;
        return;
    }
    //首先查找树中是否有该wordNpos
    if(sub_search(get_root(), wordNpos).first){
        //若有，则提示发生conflict
        cout << "key: " << wordNpos << "  conflict!" << endl;
        return;
    }
    bt_node* cur = get_root();
    if(cur->keys_num == 2 * get_minimum_degree() - 1){
        bt_node* new_node = new(bt_node);
        update_root(new_node);
        new_node->leaf = false;
        new_node->pairs.resize(1);
        new_node->children.resize(1);
        new_node->children.insert(new_node->children.begin(), cur);
        split_child(new_node, 0);
        insert_nonfull(new_node, pair);
    }else{
        insert_nonfull(cur, pair);
    }
    cout << "Successfully inserted the key-value pair: " << raw_data << endl;
    return;
}

void bt::insert_nonfull(bt_node* cur, key_value pair){
    int i = 0;
    while(i < cur->keys_num && pair.wordNpos > cur->pairs[i].wordNpos){
        i++;
    }
    if(cur->leaf){
        cur->pairs.insert(cur->pairs.begin() + i, pair);
        cur->keys_num++;
    }else{
        //判断目标孩子是否满
        if(cur->children[i]->keys_num == 2 * get_minimum_degree() - 1){
            //若满则split
            split_child(cur, i);
            if(pair.wordNpos > cur->pairs[i].wordNpos){
                i++;
            }
        }
        insert_nonfull(cur->children[i], pair); 
    }
    return;
}

void bt::split_child(bt_node* cur, int i){
    //创建新结点
    bt_node* right_part = new(bt_node);
    right_part->children.resize(1);
    bt_node* left_part = cur->children[i];
    //对分裂的结点进行数据复制
    right_part->leaf = left_part->leaf;
    right_part->keys_num = get_minimum_degree() - 1;
    for(int j = 0; j < get_minimum_degree() - 1; j++){
        right_part->pairs.insert(right_part->pairs.begin() + j, left_part->pairs[j + get_minimum_degree()]);
    }
    //若两个part不是叶节点，则还需进行children的数据复制
    if(!left_part->leaf){
        for(int j = 0; j < get_minimum_degree(); j++){
        right_part->children.insert(right_part->children.begin() + j, left_part->children[j + get_minimum_degree()]);
        }
    }
    //更新left_part中key数量
    left_part->keys_num = get_minimum_degree() - 1;
    //在cur中插入分裂上来的数据
    cur->pairs.insert(cur->pairs.begin() + i, left_part->pairs[get_minimum_degree() - 1]);
    cur->children.insert(cur->children.begin() + i + 1, right_part);
    //更新cur中key数量
    cur->keys_num++;
    return;
}

pair<bt_node*, int> bt::sub_search(bt_node* cur, string wordNpos){
    int i = 0;
    while(i < cur->keys_num && wordNpos > cur->pairs[i].wordNpos){
        i++;
    }
    if(i < cur->keys_num && wordNpos == cur->pairs[i].wordNpos){
        return pair(cur, i);
    }else if(cur->leaf){
        return pair(nullptr, 0);
    }
    return sub_search(cur->children[i], wordNpos);
}

pair<bt_node*, int> bt::search(string wordNpos){
    pair<bt_node*, int> result = sub_search(get_root(), wordNpos);
    if(result.first){
        cout << "Successfully found the wordNpos: " << wordNpos << ", the corresponding frequency is: " << result.first->pairs[result.second].value << endl;
    }else{
        cout << "key " << wordNpos << " missing!" << endl;
    }
    return result;
}

bt_node* bt::get_root(){
    return root;
}

void bt::update_root(bt_node* x){
    root = x;
    return;
}

int bt::get_minimum_degree(){
    return minimum_degree;
}

vector<string> bt::split(string x){
    vector<string> result;
    int i = 0;
    while(x[i] != ' ' && x[i] != '\n'){
        ++i;
    }//遇到第一个空格符
    ++i;
    int start1 = i;
    while(x[i] != ' ' && x[i] != '\n'){
        ++i;
    }//遇到第二个空格符
    result.push_back(x.substr(0, i));//保存wordNpart
    ++i;
    result.push_back(x.substr(i));//保存frequency
    return result;
}