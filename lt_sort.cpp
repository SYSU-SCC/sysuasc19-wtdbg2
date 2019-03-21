#include <algorithm>
// #include <iostream>
#include <stdio.h>
// using namespace std;
#include <functional>
#include <array>
// #include "kbm.h"

extern "C" void lt_print(void* buffer,size_t size,int rank);

void lt_print(void* buffer,size_t size,int rank){
    // for(int c1=0;c1<size;c1++){
    //     cout<<buffer[c1].mat<<" ";
    // }
    // std::cout<<"lttest"<<std::endl;
    // printf("lttest:%d\n",size);

    typedef struct {
	uint32_t qidx:31, qdir:1;  // u4i
	uint32_t tidx:31, tdir:1;
	unsigned long long cgoff:40, cglen:24;
	int qb, qe, tb, te;
	int mat, cnt, aln, gap; // gap is counted in BINs
    } temp_kbm_map_t;
    temp_kbm_map_t* tbuffer = (temp_kbm_map_t*)buffer;
 
    // sort using a lambda expression 
    std::sort(tbuffer, tbuffer + size, [](const temp_kbm_map_t& a, const temp_kbm_map_t& b) {
        return b.mat > a.mat;   
    });

}

// bool com(const kbm_map_t& a, const kbm_map_t& b){
//         return b.mat > a.mat;
// }

// void ltsort(kbm_map_t* buffer,size_t size, bool bigger){
//     cout<<"begin to sort"<<endl;
//     sort(buffer, buffer+size, com);
    
//     lt_print(buffer, size);
// }



