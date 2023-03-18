#include "Navigator.h"

using namespace std;

int main(){
    //初始化地图
    navigator navigator;
    navigator.init_map("map.txt");
    string command, start, end, trip_mode;
    while(true){
        cout << "Please enter your command(1-quit , 2-navigator): " << endl;
        //退出或导航
        cin >> command;
        if(command == "1"){
            cout << "Quit!" << endl;
            break;
        }
        //输入导航信息
        cout << "Please enter your start point: ";
        cin >> start;
        cout << "Please enter your end point: ";
        cin >> end;
        cout << "Please decide your trip mode(1-on foot, 2-by bus, 3-no limits): ";
        cin >> trip_mode;
        //进行导航操作
        if(trip_mode == "1"){
            navigator.find_shortest_path_walk(start, end);
        }else if(trip_mode == "2"){
            navigator.find_shortest_path_bus(start, end);
        }else if(trip_mode == "3"){
            navigator.find_shortest_path_nolimits(start, end);
        }else{
            cout << "Invalid trip mode!" << endl;
        }
    }
}