/*
 * File      : main.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2015-07-29     Arda.Fu      first implementation
 */
#include <rtthread.h>
#include <board.h>

#include <iostream>
#include <vector>
#include <list>
#include <map>

using namespace std;

int main(void)
{
    vector<int> vec;

    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);

    cout << "?????:" << vec.size() << endl;

    // ????????vector
    for (int i = 0; i < vec.size(); i++)
        cout << vec[i] << " ";
    cout << endl;

    // ???????vector
    vector<int>::iterator itor = vec.begin();
    for (; itor != vec.end(); itor++)
        cout << *itor << " ";
    cout << endl;

    return 0;
}




