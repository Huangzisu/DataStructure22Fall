#include "bt.h"
#include "rbt.h"

using namespace std;

int main(){
    int minimum_degree = 2;
    rbt rbt;
    bt bt(minimum_degree);
    string path;
    string raw_data;
    string key;
    char c;
    vector<string> splited_data;
    while(true){
        cout << "Please input your command: ";
        int option;
        cin >> option;
        switch(option){
        case 0:
            cin >> path;
            rbt.initialize(path);
            path.clear();
            break;
        case 1:
            c = getchar();
            getline(cin, raw_data);
            rbt.insert_by_command(raw_data);
            raw_data.clear();
            break;
        case 2:
            cin >> path;
            rbt.insert_by_file(path);
            path.clear();
            break;
        case 3:
            c = getchar();
            getline(cin, raw_data);
            splited_data = rbt.split(raw_data);
            rbt.delete_by_command(splited_data[0]);
            raw_data.clear();
            splited_data.clear();
            break;
        case 4:
            cin >> path;
            rbt.delete_by_file(path);
            path.clear();
            break;
        case 5:
            c = getchar();
            getline(cin, raw_data);
            splited_data = rbt.split(raw_data);
            rbt.update_by_command(splited_data[0], raw_data);
            raw_data.clear();
            splited_data.clear();
            break;
        case 6:
            c = getchar();
            getline(cin, key);
            rbt.search(key);
            key.clear();
            break;
        case 7:
            rbt.dump(rbt.get_root());
            break;
        case 10:
            cin >> path;
            bt.initialize(path);
            path.clear();
            break;
        case 11:
            c = getchar();
            getline(cin, raw_data);
            bt.insert_by_command(raw_data);
            raw_data.clear();
            break;
        case 12:
            cin >> path;
            bt.insert_by_file(path);
            path.clear();
            break;
        case 13:
            c = getchar();
            getline(cin, raw_data);
            splited_data = bt.split(raw_data);
            bt.delete_by_command(splited_data[0]);
            raw_data.clear();
            splited_data.clear();
            break;
        case 14:
            cin >> path;
            bt.delete_by_file(path);
            path.clear();
            break;
        case 15:
            c = getchar();
            getline(cin, raw_data);
            splited_data = bt.split(raw_data);
            bt.update_by_command(splited_data[0], raw_data);
            break;
        case 16:
            c = getchar();
            getline(cin, key);
            bt.search(key);
            break;
        case 17:
            bt.dump(bt.get_root());
            break;
        case 99:
            return 0;
        default:
            break;
        }
    }
}