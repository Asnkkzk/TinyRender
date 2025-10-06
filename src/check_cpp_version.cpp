#include <iostream>
using namespace std;

int main() {
    cout << "当前C++标准版本: ";
    #ifdef __cplusplus
        cout << __cplusplus << endl;
    #endif
    
    // 检查具体的C++标准
    cout << "C++标准检测:" << endl;
    #if __cplusplus == 202002L
        cout << "C++20" << endl;
    #elif __cplusplus == 201703L
        cout << "C++17" << endl;
    #elif __cplusplus == 201402L
        cout << "C++14" << endl;
    #elif __cplusplus == 201103L
        cout << "C++11" << endl;
    #elif __cplusplus == 199711L
        cout << "C++98" << endl;
    #else
        cout << "预标准C++" << endl;
    #endif
    
    return 0;
}