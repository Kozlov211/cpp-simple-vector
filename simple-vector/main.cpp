#include <cassert>
#include <iostream>
#include <numeric>
#include <string>

#include "test.h"

using namespace std;

int main() {
    TestTemporaryObjConstructor();
    TestTemporaryObjOperator();
    TestNamedMoveConstructor();
    TestNamedMoveOperator();
    TestNoncopiableMoveConstructor();
    TestNoncopiablePushBack();
    TestNoncopiableInsert();
    TestNoncopiableErase();
    return 0;
}
