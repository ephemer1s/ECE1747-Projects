#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <mutex>
#include <unistd.h>

using namespace std;

int main(int argc, char** argv) {
    cout << sizeof(int) << endl;
    cout << sizeof(char) << endl;
    cout << sizeof(char*) << endl;
    cout << sizeof(pthread_t) << endl;
    cout << sizeof(stack<char>) << endl;
    cout << sizeof(mutex) << endl;
    return 0;
}
