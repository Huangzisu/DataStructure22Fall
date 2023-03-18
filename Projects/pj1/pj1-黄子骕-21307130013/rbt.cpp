#include "rbt.h"

/////////////////////////////////////////////类成员函数实现
void rbt::dump(rbt_node* x) const{
    if(x){
        dump(x->left);
        cout << x->wordNpos << " " << x->value << endl;
        dump(x->right);
    }
}

rbt_node* rbt::get_root() const{
    return root;
}

void rbt::update_root(rbt_node* x){
    root = x;
}

void rbt::left_rotate(rbt_node* x){
    rbt_node* parent = x->parent;
    rbt_node* right_kid = x->right;
    rbt_node* rl_grandkid = right_kid->left;
    //进行关系的变化
    if(!parent){
        right_kid->parent = nullptr;
        update_root(right_kid);
    }else{
        if(x == parent->left){
            parent->left = right_kid;
            right_kid->parent = parent;
        }else{
            parent->right = right_kid;
            right_kid->parent = parent;
        }
    }
    
    right_kid->left = x;
    x->parent = right_kid;
    x->right = rl_grandkid;
    if(rl_grandkid){
        rl_grandkid->parent = x;
    }
}

void rbt::right_rotate(rbt_node* x){
    rbt_node* parent = x->parent;
    rbt_node* left_kid = x->left;
    rbt_node* lr_grandkid = left_kid->right;
    //进行关系的变化
    if(!parent){
        left_kid->parent = nullptr;
        update_root(left_kid);
    }else{
        if(x == parent->left){
            parent->left = left_kid;
            left_kid->parent = parent;
        }else{
            parent->right = left_kid;
            left_kid->parent = parent;
        }
    }
    x->left = lr_grandkid;
    if(lr_grandkid){
        lr_grandkid->parent = x;
    }
    left_kid->right = x;
    x->parent = left_kid;
}

bool rbt::insert(rbt_node* x){
    //若此时为空树，则将插入的node设置为root
    if(get_root() == nullptr){
        x->colour = black;//将该node变黑
        update_root(x);
        return true;
    }
    //若不为空树，则先将该node插入（与二叉查找树插入方法相同)
    rbt_node* cur = get_root();
    rbt_node* tmp_par;//记录每次比较时父节点，便于插入时将待插入结点与其对应父节点连接
    while(cur){
        if(x->wordNpos < cur->wordNpos){
            tmp_par = cur;
            cur = cur->left;
        }else if(x->wordNpos > cur->wordNpos){
            tmp_par = cur;
            cur = cur->right;
        }else{
            cout << "key " << x->wordNpos << " conflict!" << endl;
            return false;
        }
    }
    x->parent = tmp_par;
    if(x->wordNpos < tmp_par->wordNpos){
        tmp_par->left = x;
    }else{
        tmp_par->right = x;
    }
    //接着对红黑树进行调整以符合其性质
    //case1：父结点为黑，则无需调整
    //case2：父结点为红色,分为三个subcase（分父节点是左/右子节点讨论）
    rbt_node* parent = tmp_par;
    cur = x;
    while(parent->colour == red){
        rbt_node* gp = parent->parent;//用gf保存祖父结点
        //父节点为左孩子的情况
        if(parent == gp->left){
            rbt_node* uncle = gp->right;
            //subcase1：父结点为红、叔叔结点为红  
            if(uncle && uncle->colour == red){
                //变色，并向上继续进行处理
                parent->colour = black;
                uncle->colour = black;
                gp->colour = red;
                cur = gp;
                parent = cur->parent;
                if(!parent){
                    cur->colour = black;
                    break;
                }
                continue;
            }
            //subcase2 & subcase3：叔叔结点为黑
            if(!uncle || uncle->colour == black){
                int flag = 0;//用flag记录是否进行预旋转
                //subcase2：当前结点为右孩子，则先进行左旋，变为subcase3
                if(cur == parent->right){
                    left_rotate(parent);
                    flag = 1;
                }
                //subcase3：当前结点为左孩子，则进行右旋并变色
                right_rotate(gp);
                gp->colour = red;
                if(flag){
                    cur->colour = black;                    
                }else{
                    parent->colour = black;
                }
                break;
            }
        }
        //父节点为右孩子的情况
        else{
            rbt_node* uncle = gp->left;
            //subcase1：父结点为红，叔叔结点为红
            if(uncle && uncle->colour == red){
                //变色，并向上继续进行处理
                parent->colour =  black;      
                uncle->colour = black;
                gp->colour = red;
                cur = gp;
                parent = cur->parent;
                if(!parent){
                    cur->colour = black;
                    break;
                }
                continue;  
            }
            //subcase2 & subcase3：叔叔结点为黑
            if(!uncle || uncle->colour == black){
                int flag = 0;//用flag记录是否进行预旋转
                //subcase2：当前结点为左孩子，则先进行右旋，变为subcase3
                if(cur == parent->left){
                    right_rotate(parent);
                    flag = 1;
                }
                //subcase3：当前结点为右孩子，则进行左旋并变色
                left_rotate(gp);
                gp->colour = red;
                if(flag){
                    cur->colour = black;                    
                }else{
                    parent->colour = black;
                }
                break;
            }
        }
    }
    return true;
}

bool rbt::insert_by_command(string x){
    vector<string> data = split(x);
    rbt_node* new_node = new(rbt_node);
    new_node->wordNpos = data[0];
    new_node->value = data[1];
    if(insert(new_node)){
        cout << "Successfully inserted the key-value pair: " << x << endl;
        return true;
    }
    return false;
}

bool rbt::initialize(string file_name){
    ifstream file;
    file.open(file_name, ios::in);
    if(!file.is_open()){
        cout << "Fail to open the file!" << endl;
        return false;
    }
    string raw_data;
    vector<string> splited_data;
    getline(file, raw_data);//吸收第一行的attribute
    raw_data.clear();
    while(getline(file, raw_data)){
        rbt_node* temp = new(rbt_node);
        splited_data = split(raw_data);
        temp->wordNpos = splited_data[0];
        temp->value = splited_data[1];
        insert(temp);
        raw_data.clear();
        splited_data.clear();
    }
    cout << "Initialize_by_file: " << file_name << " completed!" << endl;
    return true;
}

bool rbt::insert_by_file(string file_name){
    ifstream file;
    file.open(file_name, ios::in);
    if(!file.is_open()){
        cout << "Fail to open the file!" << endl;
        return false;
    }
    string raw_data;
    vector<string> splited_data;
    getline(file, raw_data);//吸收第一行的attribute
    raw_data.clear();
    while(getline(file, raw_data)){
        rbt_node* temp = new(rbt_node);
        splited_data = split(raw_data);
        temp->wordNpos = splited_data[0];
        temp->value = splited_data[1];
        insert(temp);
        raw_data.clear();
        splited_data.clear();
    }
    cout << "Insert_by_file: " << file_name << " completed!" << endl;
    return true;
}

bool rbt::delete_by_command(string wordNpos){
    //首先查找待删除结点，若无该结点则报告并返回
    rbt_node* need_del = search(wordNpos);
    if(!need_del){
        return false;
    }
    //接着分为以下几个case处理
    //case1：待删除结点有两个非空子结点，则找到后继结点，转换为case2或case3
    string need_del_save_wordNpos = need_del->wordNpos;//保存原待删除结点的key
    if(need_del->left && need_del->right){
        rbt_node* real_del = find_successor(need_del);
        //用实际删除结点中数据替换原待删除结点数据
        need_del->wordNpos = real_del->wordNpos;
        need_del->value = real_del->value;
        need_del = real_del;//用实际删除结点替换原待删除结点进行处理
    }
    //case2：待删除结点只有一个子结点
    if(need_del->left && !need_del->right){//只有左孩子
        rbt_node* kid = need_del->left;
        rbt_node* parent = need_del->parent;
        if(!parent){
            update_root(kid);
        }
        if(need_del == parent->left){
            parent->left = kid;
            kid->parent = parent;
        }else{
            parent->right = kid;
            kid->parent = parent;
        }
        kid->colour = black;
        cout << "Successfully deleted wordNpos: " << need_del_save_wordNpos << " !" << endl;
        delete need_del;
        return true;
    }else if(!need_del->left && need_del->right){//只有右孩子
        rbt_node* kid = need_del->right;
        rbt_node* parent = need_del->parent;
        if(!parent){
            update_root(kid);
        }
        if(need_del == parent->left){
            parent->left = kid;
            kid->parent = parent;
        }else{
            parent->right = kid;
            kid->parent = parent;
        }
        kid->colour = black;
        cout << "Successfully deleted wordNpos: " << need_del_save_wordNpos << " !" << endl;
        delete need_del;
        return true;
    }
    //case3：待删除结点没有子结点
    if(!need_del->left && !need_del->right){
        rbt_node* parent = need_del->parent;
        //若待删除结点为root则将root置为nullptr
        if(!parent){
            update_root(nullptr);
            cout << "Successfully deleted wordNpos: " << need_del_save_wordNpos << " !" << endl;
            delete need_del;
            return true;
        }
        //subcase1：待删除结点为红，则直接删除
        if(need_del->colour == red){
            if(need_del == parent->left){
                parent->left = nullptr;
            }else{
                parent->right = nullptr;
            }
            cout << "Successfully deleted wordNpos: " << need_del_save_wordNpos << " !" << endl;
            delete need_del;
            return true;
        }
        //subcase2：待删除结点为黑，则分以下情况讨论进行调整
        rbt_node* cur = need_del;
        while(cur != get_root() && cur->colour == black){
            rbt_node* parent = cur->parent;
            //当待删除结点为左孩子时
            if(cur == parent->left){
                rbt_node* sibling = parent->right;
                //situation1：兄弟结点为红色，则左旋并变色
                if(sibling->colour == red){
                    sibling->colour = black;
                    parent->colour = red;                    
                    left_rotate(parent);
                }
                else if(sibling->colour == black){
                    //situation2：兄弟结点为黑色且其两个子结点为黑或空，则进行颜色调整，并继续向上调整
                    if((!sibling->left || sibling->left->colour == black) && (!sibling->right || sibling->right->colour == black)){
                        sibling->colour = red;
                        cur = parent;
                    }
                    //situation3：兄弟结点为黑色且其左子结点为红，右子结点为黑或空，则右旋并颜色调整
                    else if((sibling->left && sibling->left->colour == red) && (!sibling->right || sibling->right->colour == black)){
                        sibling->left->colour = black;
                        sibling->colour = red;
                        right_rotate(sibling);
                    }
                    //situation4:兄弟结点为黑色且其右子结点为红，则左旋并颜色调整
                    else if((sibling->right && sibling->right->colour == red)){
                        sibling->colour = parent->colour;
                        parent->colour = black;
                        sibling->right->colour = black;
                        left_rotate(parent);
                        break;
                    }                    
                }
            }
            //当待删除结点为右孩子时
            else{
                rbt_node* sibling = parent->left;
                //situation1：兄弟结点为红色，则右旋并变色
                if(sibling->colour == red){
                    sibling->colour = black;
                    parent->colour = red;
                    right_rotate(parent);
                }
                else if(sibling->colour == black){
                    //situation2：兄弟结点为黑色且其两个子结点为黑或空，则进行颜色调整，并继续向上调整
                    if((!sibling->left || sibling->left->colour == black) && (!sibling->right || sibling->right->colour == black)){
                        sibling->colour = red;
                        cur = parent;
                    }         
                    //situation3：兄弟结点为黑色且其右子结点为红，左子结点为黑或空，则左旋并颜色调整
                    else if((!sibling->left || sibling->left->colour == black) && (sibling->right && sibling->right->colour == red)){
                        sibling->right->colour = black;
                        sibling->colour = red;
                        left_rotate(sibling);
                    }
                    //situation4:兄弟结点为黑色且其左子结点为红，则右旋并颜色调整
                    else if((sibling->left && sibling->left->colour == red)){
                        sibling->colour = parent->colour;
                        parent->colour = black;
                        sibling->left->colour = black;
                        right_rotate(parent);
                        break;
                    }                                             
                }
            }
        }
        cur->colour = black;
        //在subcase2中进行实际删除
        if(need_del == parent->left){
            parent->left = nullptr;
        }else{
            parent->right = nullptr;
        }
        cout << "Successfully deleted wordNpos: " << need_del_save_wordNpos << " !" << endl;
        delete need_del;
    }
    return true;
}

bool rbt::delete_by_file(string file_name){
    ifstream file;
    file.open(file_name, ios::in);
    if(!file.is_open()){
        cout << "Fail to open the file!" << endl;
        return false;
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
    return true;    
}

rbt_node* rbt::search(string wordNpos) const{
    rbt_node* cur = get_root();
    while(cur){
        if(wordNpos > cur->wordNpos){
            cur = cur->right;
        }else if(wordNpos < cur->wordNpos){
            cur = cur->left;
        }else{
            cout << "Successfully found the wordNpos: " << wordNpos << ", the corresponding frequency is: " << cur->value << endl;
            return cur;
        }
    }
    cout << "Key: " << wordNpos << "   missing!" << endl;
    return nullptr;
}

bool rbt::update_by_command(string oldone_wordNpos, string newone){
    //首先寻找是否有该结点
    rbt_node* update = search(oldone_wordNpos);
    //报告提示并进行相应处理，若有则删除并插入新结点，若无则直接插入新结点
    if(update){
        cout << "Updating the key-value pair......" << endl;
        delete_by_command(oldone_wordNpos);
    }else{
        cout << "Inserting the key-value pair......" << endl;
    }
    insert_by_command(newone);
    return true;
}

bool rbt::test() const{
    if(get_root() == nullptr){//空树的情况，为红黑树
        cout << "Test passed!" << endl;
        return true;
    }
    if(get_root()->colour == red){//根结点为红色的情况，不为红黑树
        cout << "The root is red!" << endl;
        return false;
    }
    //判断每条路径黑色结点个数是否相同
    //先取定一条路径，得到一条路径上的黑色结点个数(此处取最右侧路径)
    rbt_node* cur = get_root();
    int black_count = 0;
    while(cur){
        if(cur->colour == black){
            ++black_count;
        }
        cur = cur->right;
    }
    //接着判断其他路径黑色结点个数是否相同以及是否有连续红色结点（调用sub_test递归实现）
    if(sub_test(get_root(), 0, black_count)){
        cout << "Test passed!" << endl;
        return true;
    }
    return false;
}

bool rbt::sub_test(rbt_node* x, int sum, int black_count) const{
    if(!x){
        return true;
    }
    //若当前为黑色结点，则累积
    if(x->colour == black){
        sum++;
    }
    //判断是否走完一条路径，若走完则比较黑色结点数量
    if(!x->left && !x->right){
        if(sum != black_count){
            cout << "Different numbers of black nodes!" << endl;
            return false;
        }
    }
    //判断是否出现连续红色结点，若出现则不是红黑树
    if(x->colour == red && ((x->left && x->left->colour == red) || (x->right && x->right->colour == red))){
        cout << "Red nodes appear successively!" << endl;
        return false;
    }
    return sub_test(x->left, sum, black_count) && sub_test(x->right, sum, black_count);
}

vector<string> rbt::split(string x){
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

rbt_node* rbt::find_successor(rbt_node* x){
    if(x->right){
        x = x->right;
        while(x->left){
            x = x->left;
        }
        return x;
    }
    rbt_node* y = x->parent;
    while(y && x == y->right){
        x = y;
        y = x->parent;
    }
    return y;
}

