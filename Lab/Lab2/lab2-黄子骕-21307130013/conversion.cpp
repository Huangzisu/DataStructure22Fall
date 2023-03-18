#include<iostream>
#include<string>
#include<stack>

using namespace std;

string convert(string A){
    int n = A.size();
    stack<int> stack;
    string B;
    for(int i = 0; i < n; i++){
        char now = A[i];
        if(now >= '0' && now <= '9'){//now为数字
            B += now;
        }else if(now == '+' || now == '-'){
            if(stack.empty() || stack.top() == '('){
                stack.push(now);
                continue;
            }
            while(!(stack.empty()) && stack.top() != '('){
                B+=stack.top();
                stack.pop();
            }
            stack.push(now);
        }else if(now == '*' || now == '/' || now == 'm'){
            if(stack.empty() || stack.top() == '('){
                stack.push(now);
                continue;
            }
            if(stack.top() == '*' || stack.top() == '/' || stack.top() == 'm'){
                B+=stack.top();
                stack.pop();
            }
            stack.push(now);
        }else if(now == '('){
            stack.push(now);
        }else if(now == ')'){
            while(stack.top() != '('){
                B+=stack.top();
                stack.pop();
            }
            stack.pop();
        }
    }
    while(!(stack.empty())){
        B+=stack.top();
        stack.pop();
    }
    return B;
}

int main(){
    string A;
    cin >> A;
    cout << convert(A);
}