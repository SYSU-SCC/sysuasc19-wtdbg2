#include <algorithm>
// #include <iostream>
#include <stdio.h>
// using namespace std;
#include <functional>
#include <array>
// #include "kbm.h"

extern "C" void lt_sort_kbm_map_t_mat(void* buffer,size_t size,int rank);
extern "C" void lt_sort_kbm_map_t_qb(void* buffer,size_t size,int rank);
extern "C" void lt_sort_kbm_map_t_tb(void* buffer,size_t size,int rank);
extern "C" void lt_sort_kbm_dpe_t(void* buffer,size_t size,int rank);
extern "C" void lt_sort_kbm_read_t_rdlen(void* buffer,size_t size,int rank);
extern "C" void lt_sort_kbm_tmp_bmer_t_bidx(void* buffer,size_t size,int rank);
extern "C" void lt_sort_kbm_ref_t_off(void* buffer,size_t size,int rank);
extern "C" void lt_sort_kbm_ref_t_qbidx(void* buffer,size_t size,int rank);


typedef struct {
	uint32_t qidx:31, qdir:1;
	uint32_t tidx:31, tdir:1;
	unsigned long long cgoff:40, cglen:24;
	int qb, qe, tb, te;
	int mat, cnt, aln, gap; // gap is counted in BINs
} lt_kbm_map_t;

// typedef struct {
//     uint32_t qidx:31, qdir:1;  // u4i
//     uint32_t tidx:31, tdir:1;
//     unsigned long long cgoff:40, cglen:24;
//     int qb, qe, tb, te;
//     int mat, cnt, aln, gap; // gap is counted in BINs
// } lt_temp_kbm_map_t;

typedef struct {
    unsigned long long mer:46, tot:17, flt:1;
} lt_kbm_kmer_t;

typedef struct {
    unsigned long long off:40, cnt:24;
} lt_kbm_kaux_t;

typedef struct {
    lt_kbm_kmer_t *mer;
    lt_kbm_kaux_t *aux;
    uint32_t kidx;
    uint32_t off:24, dir:1, pdir:1, fine:1, closed:1, extra_bits1:4;
    uint32_t qbidx;
    uint32_t poffs[2];
    unsigned long long bidx, boff, bend;
    //kbm_bmer_t *b, *end;
} lt_kbm_ref_t;

typedef struct {
    unsigned long long rdoff:40, bincnt:24;
    uint32_t rdlen, binoff;
    char *tag;
} lt_kbm_read_t;

typedef struct {
	uint8_t bidx; // u1i// bidx = (kbm_baux_t->bidx << 32 | kbm_bmer_t->bidx)  
	uint8_t dir:1, koff:7; // koff is the real (offset? >> 1), here offset is +0 or +1
} lt_kbm_baux_t;

typedef struct {
	unsigned long long bidx;
	lt_kbm_baux_t aux;
} lt_kbm_tmp_bmer_t;


typedef struct {
    uint32_t poff;
    uint32_t refidx;
    unsigned long long bidx:40, koff:24; // u8i
} lt_kbm_dpe_t;

void lt_sort_kbm_map_t_mat(void* buffer,size_t size,int rank){

    lt_kbm_map_t* tbuffer = (lt_kbm_map_t*)buffer;
 
    // sort using a lambda expression 
    std::sort(tbuffer, tbuffer + size, [](const lt_kbm_map_t& a, const lt_kbm_map_t& b) {
        return b.mat > a.mat;   
    });
}

void lt_sort_kbm_map_t_qb(void* buffer,size_t size,int rank){
    lt_kbm_map_t* tbuffer = (lt_kbm_map_t*)buffer;
    
    if(rank == 0){
        // sort using a lambda expression 
        std::sort(tbuffer, tbuffer + size, [](const lt_kbm_map_t& a, const lt_kbm_map_t& b) {
            return b.qb > a.qb;   
        });
    }else{
        std::sort(tbuffer, tbuffer + size, [](const lt_kbm_map_t& a, const lt_kbm_map_t& b) {
            return a.qb > b.qb;   
        });
    }
}

void lt_sort_kbm_map_t_tb(void* buffer,size_t size,int rank){
    lt_kbm_map_t* tbuffer = (lt_kbm_map_t*)buffer;
    
    if(rank == 0){
        // sort using a lambda expression 
        std::sort(tbuffer, tbuffer + size, [](const lt_kbm_map_t& a, const lt_kbm_map_t& b) {
            return b.tb > a.tb;   
        });
    }else{
        std::sort(tbuffer, tbuffer + size, [](const lt_kbm_map_t& a, const lt_kbm_map_t& b) {
            return a.tb > b.tb;   
        });
    }
}

void lt_sort_kbm_dpe_t(void* buffer,size_t size,int rank){
    lt_kbm_dpe_t* tbuffer = (lt_kbm_dpe_t*)buffer;
//  #define num_cmpgtx(a, b, c, d) (((a) > (b))? 1 : (((a) < (b))? 0 : (((c) > (d)))))
    if(rank ==0){
        std::sort(tbuffer, tbuffer + size, [](const lt_kbm_dpe_t& a, const lt_kbm_dpe_t& b) {
            return (((a.bidx) > (b.bidx))? 1 : (((a.bidx) < (b.bidx))? 0 : (((a.poff) > (b.poff)))));
        });
    }else{
        std::sort(tbuffer, tbuffer + size, [](const lt_kbm_dpe_t& a, const lt_kbm_dpe_t& b) {
            // return num_cmpgtx(a.bidx, b.bidx, a.poff, b.poff);   
            return (((b.bidx) > (a.bidx))? 1 : (((b.bidx) < (a.bidx))? 0 : (((b.poff) > (a.poff)))));
        });
    }
}

void lt_sort_kbm_read_t_rdlen(void* buffer,size_t size,int rank){
    lt_kbm_read_t* tbuffer = (lt_kbm_read_t*)buffer;
    // sort using a lambda expression 

    // printf("before:");
    // for(int c1=0;c1<size;c1++){
    //     printf("%d ",tbuffer[c1].rdlen);
    // }
    // printf("\n");

    if(rank ==0){
        std::sort(tbuffer, tbuffer + size, [](const lt_kbm_read_t& a, const lt_kbm_read_t& b) {
            // return num_cmpgtx(a.bidx, b.bidx, a.poff, b.poff);   
            return b.rdlen > a.rdlen;
        });
    }else{
        std::sort(tbuffer, tbuffer + size, [](const lt_kbm_read_t& a, const lt_kbm_read_t& b) {
            // return num_cmpgtx(a.bidx, b.bidx, a.poff, b.poff);   
            return a.rdlen > b.rdlen;
        });
    }
    // printf("after:");
    // for(int c1=0;c1<size;c1++){
    //     printf("%d ",tbuffer[c1].rdlen);
    // }
    // printf("\n");
    
}


void lt_sort_kbm_tmp_bmer_t_bidx(void* buffer,size_t size,int rank){

    lt_kbm_tmp_bmer_t* tbuffer = (lt_kbm_tmp_bmer_t*)buffer;
    if(rank == 0){
        std::sort(tbuffer, tbuffer + size, [](const lt_kbm_tmp_bmer_t& a, const lt_kbm_tmp_bmer_t& b) {
            // return num_cmpgtx(a.bidx, b.bidx, a.poff, b.poff);   
            return b.bidx > a.bidx;
        });
    }else{
        std::sort(tbuffer, tbuffer + size, [](const lt_kbm_tmp_bmer_t& a, const lt_kbm_tmp_bmer_t& b) {
            // return num_cmpgtx(a.bidx, b.bidx, a.poff, b.poff);   
            return a.bidx > b.bidx;
        });
    }
}

void lt_sort_kbm_ref_t_off(void* buffer,size_t size,int rank){

    lt_kbm_ref_t* tbuffer = (lt_kbm_ref_t*)buffer;
    if(rank == 0){
        std::sort(tbuffer, tbuffer + size, [](const lt_kbm_ref_t& a, const lt_kbm_ref_t& b) {
            // return num_cmpgtx(a.bidx, b.bidx, a.poff, b.poff);   
            return b.off > a.off;
        });
    }else{
        std::sort(tbuffer, tbuffer + size, [](const lt_kbm_ref_t& a, const lt_kbm_ref_t& b) {
            // return num_cmpgtx(a.bidx, b.bidx, a.poff, b.poff);   
            return a.off > b.off;
        });
    }
}

void lt_sort_kbm_ref_t_qbidx(void* buffer,size_t size,int rank){

    lt_kbm_ref_t* tbuffer = (lt_kbm_ref_t*)buffer;
    if(rank == 0){
        std::sort(tbuffer, tbuffer + size, [](const lt_kbm_ref_t& a, const lt_kbm_ref_t& b) {
// #define num_cmpgtx(a, b, c, d) (((a) > (b))? 1 : (((a) < (b))? 0 : (((c) > (d)))))
            return (((a.qbidx) > (b.qbidx))? 1 : (((a.qbidx) < (b.qbidx))? 0 : (((b.bend - b.boff) > (a.bend - a.boff)))));
        });
    }else{
        std::sort(tbuffer, tbuffer + size, [](const lt_kbm_ref_t& a, const lt_kbm_ref_t& b) {
            return (((b.qbidx) > (a.qbidx))? 1 : (((b.qbidx) < (a.qbidx))? 0 : (((a.bend - a.boff) > (b.bend - b.boff)))));
        });
    }

}