//
// Created by AlexGrandbell on 24-6-6.
//

#ifndef REGULAREXPRESSIONTONFA_NFAEDGE_H
#define REGULAREXPRESSIONTONFA_NFAEDGE_H

#include <iostream>
#include <vector>

using namespace std;

//NFA边，包括起点、终点和转换字符
class NFAEdge {
public:
    int start;
    int end;
    char transChar;
    NFAEdge(int start, int end, char transChar) : start(start), end(end), transChar(transChar) {}
};


#endif //REGULAREXPRESSIONTONFA_NFAEDGE_H
