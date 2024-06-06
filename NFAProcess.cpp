//
// Created by AlexGrandbell on 24-6-6.
//

#include "NFAProcess.h"

//自动读取
void NFAProcess::autoTest() {
    ifstream inFile(readFilePath);
    inFile >> s;
    process0_Start();
}

//手动输入
void NFAProcess::startProcess() {
    cout<<"请输入正规式：";
    cin>>s;
    process0_Start();
}

//第零阶段处理：合并两种事件
void NFAProcess::process0_Start() {
    nfaGroup = NFAGroup();
    //转小写
    for (int i = 0; i < s.length(); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] - 'A' + 'a';
        }
    }
    cout<<"输入的正规式为："<<s<<endl;
    if (s==""){
        cout<<"输入为空！"<<endl;
        return;
    }
    //将输入的正规式添加+号
    process1_AddConcat();
    cout<<"添加加号后的正规式为："<<s<<endl;

    //转后缀
    process2_ToPostfix();
    cout<<"转换后的正规式为："<<s<<endl;

    //转NFA
    process3_ToNFA();
    printNFA();//打印结果
}

//判断是否为字母
bool NFAProcess::isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

//第一阶段处理：分隔元素
void NFAProcess::process1_AddConcat() {
    //储存添加+后的字符串
    string newS = "";
    //括号匹配检查
    stack<char> checkStack;

    for (int i = 0; i < s.length(); i++) {
        if(isLetter(s[i])) {//字母
            newS += s[i];
            //如果后面是字母或者左括号，添加+
            if (i < s.length() - 1 && (isLetter(s[i + 1]) || s[i + 1] == '(')) {
                newS += '+';
            }
        } else if (s[i]=='|'){
            newS += '|';
        } else if(s[i]=='*'){
            newS += '*';
            //如果后面是字母或者左括号，添加+
            if (i<s.length()-1 && (isLetter(s[i+1]) || s[i+1]=='(')){
                newS += '+';
            }
        } else if(s[i]=='('){
            newS += '(';
            checkStack.push('(');
        } else if(s[i]==')'){
            newS += ')';
            //如果后面是字母或者左括号，添加+
            if (i<s.length()-1 && (isLetter(s[i+1]) || s[i+1]=='(')){
                newS += '+';
            }
            //括号匹配检测
            if (checkStack.empty()){
                cout<<"括号匹配错误！"<<endl;
                s="";
                throw exception();
            } else{
                checkStack.pop();
            }
        }
    }
    //最后的括号匹配检测
    if (!checkStack.empty()){
        cout<<"括号匹配错误！"<<endl;
        s="";
        throw exception();
    }
    s = newS;//更新s
}

//第二阶段处理：转后缀表达式
void NFAProcess::process2_ToPostfix() {
    //储存转换后的字符串
    string newS = "";
    //操作符栈
    stack<char> opStack;
    //操作符优先级
    int priority[128];
    priority['|'] = 1;
    priority['+'] = 2;
    priority['*'] = 3;
    priority['('] = 0;
    priority[')'] = 0;

    for (int i = 0; i < s.length(); i++) {
        if (isLetter(s[i])) {//字母
            newS += s[i];
        } else {
            if (s[i] == '(' || s[i]=='*') {//左括号或*
                opStack.push(s[i]);
            }else if (s[i] == ')') {//右括号
                while (opStack.top() != '(') {
                    newS += opStack.top();
                    opStack.pop();
                }
                opStack.pop();
            }else {//操作符
                while (!opStack.empty() && priority[opStack.top()] >= priority[s[i]]) {
                    newS += opStack.top();
                    opStack.pop();
                }
                opStack.push(s[i]);
            }
        }
    }
    while (!opStack.empty()) {
        newS += opStack.top();
        opStack.pop();
    }
    s = newS;//更新s
}

//第三阶段处理：转NFA
void NFAProcess::process3_ToNFA() {
    //状态栈
    stack<NFAGroup> stateStack;
    //状态编号
    int stateNum = 0;
    //操作符优先级
    int priority[128];
    priority['|'] = 1;
    priority['+'] = 2;
    priority['*'] = 3;

    for (int i = 0; i < s.length(); i++) {
        if (isLetter(s[i])) {//字母
            NFAGroup nfaGroup = NFAGroup();
            NFAEdge nfaEdge = NFAEdge(0, 1, s[i]);
            nfaGroup.edges.push_back(nfaEdge);
            stateStack.push(nfaGroup);
        } else {
            if (s[i] == '+') {//连接
                NFAGroup nfaGroup2 = stateStack.top();
                stateStack.pop();
                NFAGroup nfaGroup1 = stateStack.top();
                stateStack.pop();

                //连接两个图
                NFAGroup nfaGroup = nfaGroup1;
                nfaGroup.stateNum = nfaGroup1.stateNum + nfaGroup2.stateNum - 1;
                nfaGroup.startState = 0;
                nfaGroup.endState = nfaGroup.stateNum - 1;

                for (int i = 0; i < nfaGroup2.edges.size(); ++i) {
                    NFAEdge nfaEdge = nfaGroup2.edges[i];
                    nfaEdge.start += nfaGroup1.stateNum - 1;
                    nfaEdge.end += nfaGroup1.stateNum - 1;
                    nfaGroup.edges.push_back(nfaEdge);
                }
                stateStack.push(nfaGroup);
            } else if (s[i] == '|') {//或
                NFAGroup nfaGroup2 = stateStack.top();
                stateStack.pop();
                NFAGroup nfaGroup1 = stateStack.top();
                stateStack.pop();
                NFAGroup nfaGroup = NFAGroup();
                nfaGroup.stateNum = nfaGroup1.stateNum + nfaGroup2.stateNum - 2;
                nfaGroup.startState = 0;
                nfaGroup.endState = nfaGroup.stateNum - 1;

                for (int i = 0; i < nfaGroup1.edges.size(); i++) {
                    NFAEdge nfaEdge = nfaGroup1.edges[i];
                    if (nfaEdge.end == nfaGroup1.endState) {
                        nfaEdge.end = nfaGroup.endState;
                    }
                    nfaGroup.edges.push_back(nfaEdge);
                }
                for (int i = 0; i < nfaGroup2.edges.size(); i++) {
                    NFAEdge nfaEdge = nfaGroup2.edges[i];
                    if (nfaEdge.end == nfaGroup2.endState) {
                        nfaEdge.end = nfaGroup.endState;
                    }else{
                        nfaEdge.end += nfaGroup1.stateNum - 2;
                    }
                    if (nfaEdge.start != 0){
                        nfaEdge.start += nfaGroup1.stateNum - 2;
                    }
                    nfaGroup.edges.push_back(nfaEdge);
                }
                stateStack.push(nfaGroup);
            } else if (s[i] == '*') {//闭包
                NFAGroup nfaGroup1 = stateStack.top();
                stateStack.pop();
                NFAGroup nfaGroup = NFAGroup();
                if (nfaGroup1.startState != nfaGroup1.endState) {
                    nfaGroup.stateNum = nfaGroup1.stateNum + 1;
                } else {
                    nfaGroup.stateNum = nfaGroup1.stateNum + 2;
                }
                nfaGroup.endState = nfaGroup.stateNum - 1;
                nfaGroup.startState = 0;
                //图内的边编号加1
                for (int i = 0; i < nfaGroup1.edges.size(); i++) {
                    NFAEdge nfaEdge = nfaGroup1.edges[i];
                    nfaEdge.start += 1;
                    if (nfaEdge.end == nfaGroup1.endState) {
                        nfaEdge.end = nfaEdge.start;
                    } else {
                        nfaEdge.end += 1;
                    }
                    nfaGroup.edges.push_back(nfaEdge);
                }
                //添加闭包边
                NFAEdge nfaEdge1 = NFAEdge(0, 1, '~');
                NFAEdge nfaEdge2 = NFAEdge( 1, nfaGroup.endState, '~');
                nfaGroup.edges.push_back(nfaEdge1);
                nfaGroup.edges.push_back(nfaEdge2);
                stateStack.push(nfaGroup);
            }
        }
    }
    nfaGroup = stateStack.top();
}

//打印边
void NFAProcess::printEdge(NFAEdge edge) {
    char start = edge.start==0?'X':edge.start-1+'0';
    char end = edge.end==nfaGroup.stateNum-1?'Y':edge.end-1+'0';
    cout<<start<<"-"<<edge.transChar<<"->"<<end<<" ";
}

//打印NFA
void NFAProcess::printNFA() {
    cout<<"X ";
    for (int j = 0; j < nfaGroup.edges.size(); ++j) {
        if (nfaGroup.edges[j].start==0){
            printEdge(nfaGroup.edges[j]);
        }
    }
    cout<<endl;
    cout<<"Y"<<endl;

    for(int i=1;i<nfaGroup.stateNum-1;i++){
        cout<<i-1<<" ";
        for (int j = 0; j < nfaGroup.edges.size(); ++j) {
            if (nfaGroup.edges[j].start==i){
                printEdge(nfaGroup.edges[j]);
            }
        }
        cout<<endl;
    }

}