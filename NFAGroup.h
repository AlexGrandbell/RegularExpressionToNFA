//
// Created by AlexGrandbell on 24-6-6.
//

#ifndef REGULAREXPRESSIONTONFA_NFAGROUP_H
#define REGULAREXPRESSIONTONFA_NFAGROUP_H

#include "NFAEdge.h"

//NFA组，包括一个起点和一个终点、许多边
class NFAGroup {
public:
    vector<NFAEdge> edges;
    int stateNum;//状态数量（编号）
    int startState;//起始状态
    int endState;//终止状态

    NFAGroup();//初始化一个起点和一个终点
};


#endif //REGULAREXPRESSIONTONFA_NFAGROUP_H
