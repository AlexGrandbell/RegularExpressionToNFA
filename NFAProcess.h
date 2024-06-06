//
// Created by AlexGrandbell on 24-6-6.
//

#ifndef REGULAREXPRESSIONTONFA_NFAPROCESS_H
#define REGULAREXPRESSIONTONFA_NFAPROCESS_H

#include "NFAGroup.h"
#include <fstream>
#include <stack>
#include <string>

class NFAProcess {
private:
    //可以选择从文件中读取
    string readFilePath = "in.txt";
    string writeFilePath = "out.txt";

    string s;
    NFAGroup nfaGroup;

    void process0_Start();//第零阶段处理：合并两种事件

    void process1_AddConcat();//第一阶段处理：分隔元素
    void process2_ToPostfix();//第二阶段处理：转后缀表达式
    void process3_ToNFA();//第三阶段处理：转NFA
    void printNFA();//最后打印结果

    void printEdge(NFAEdge);//打印边

    bool isLetter(char c);//判断是否为字母
public:
    void autoTest();//读取文件自动测试
    void startProcess();//手动测试
};


#endif //REGULAREXPRESSIONTONFA_NFAPROCESS_H
