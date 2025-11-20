#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "field.h"
#include "snark_sim.h"
int main(void){
 field_t F={.p=2147483647ULL};
 uint64_t A[7],B[7],C[7],t,a,b,w;

 /*if(scanf("{\"p\":%*u,\"crs\":{\"A_eval\":[%"SCNu64",%"SCNu64",%"SCNu64",%"SCNu64",%"SCNu64",%"SCNu64",%"SCNu64"],\"B_eval\":[%"SCNu64",%"SCNu64",%"SCNu64",%"SCNu64",%"SCNu64",%"SCNu64",%"SCNu64"],\"C_eval\":[%"SCNu64",%"SCNu64",%"SCNu64",%"SCNu64",%"SCNu64",%"SCNu64",%"SCNu64"],\"t_eval\":%"SCNu64"},\"instance\":{\"a\":%"SCNu64",\"b\":%"SCNu64",\"w\":%"SCNu64"}}",
 &A[0],&A[1],&A[2],&A[3],&A[4],&A[5],&A[6],&B[0],&B[1],&B[2],&B[3],&B[4],&B[5],&B[6],&C[0],&C[1],&C[2],&C[3],&C[4],&C[5],&C[6],&t,&a,&b,&w)!=25){   // CHANGED FROM 22 TO 25, INCORRECT # OF ASSIGNED FIELDS
  fprintf(stderr,"Parse error\n");return 2;}
  */

    // *** REPLACED PROVIDED SCAN WITH THIS ONE ***
 
    int newscan = scanf(  // ignore all non-digit values and scan #s in order
                       // order: A[0-6],B[0-6],C[0-6],t,a,b,w
                       // return # of assigned items (7*3+4=25) NOT 22 <-------
        "%*[^0-9]"  "%*u"      // p (suppressed)
        "%*[^0-9]"  "%"SCNu64  // A[0] 
        "%*[^0-9]"  "%"SCNu64  // A[1]
        "%*[^0-9]"  "%"SCNu64  // A[2]
        "%*[^0-9]"  "%"SCNu64  // A[3]
        "%*[^0-9]"  "%"SCNu64  // A[4]
        "%*[^0-9]"  "%"SCNu64  // A[5]
        "%*[^0-9]"  "%"SCNu64  // A[6]

        "%*[^0-9]"  "%"SCNu64  // B[0]
        "%*[^0-9]"  "%"SCNu64  // B[1]
        "%*[^0-9]"  "%"SCNu64  // B[2]
        "%*[^0-9]"  "%"SCNu64  // B[3]
        "%*[^0-9]"  "%"SCNu64  // B[4]
        "%*[^0-9]"  "%"SCNu64  // B[5]
        "%*[^0-9]"  "%"SCNu64  // B[6]

        "%*[^0-9]"  "%"SCNu64  // C[0]
        "%*[^0-9]"  "%"SCNu64  // C[1]
        "%*[^0-9]"  "%"SCNu64  // C[2]
        "%*[^0-9]"  "%"SCNu64  // C[3]
        "%*[^0-9]"  "%"SCNu64  // C[4]
        "%*[^0-9]"  "%"SCNu64  // C[5]
        "%*[^0-9]"  "%"SCNu64  // C[6]

        "%*[^0-9]"  "%"SCNu64  // t_eval 
        "%*[^0-9]"  "%"SCNu64  // a 
        "%*[^0-9]"  "%"SCNu64  // b 
        "%*[^0-9]"  "%"SCNu64  // w 
        ,
        &A[0], &A[1], &A[2], &A[3], &A[4], &A[5], &A[6],
        &B[0], &B[1], &B[2], &B[3], &B[4], &B[5], &B[6],
        &C[0], &C[1], &C[2], &C[3], &C[4], &C[5], &C[6],
        &t, &a, &b, &w
    );
if(newscan !=25){
    fprintf(stderr, "Parse Error\n");
    return 2;
}

// *** EVERYTHING ELSE IS THE SAME ***

 uint64_t c,s,y;build_witness(F,a,b,w,&c,&s,&y);
 uint64_t z[7]={1,a,b,w,c,s,y};
 uint64_t A_tau=lincombo(F,A,z),B_tau=lincombo(F,B,z),C_tau=lincombo(F,C,z);
 uint64_t H_tau=compute_H_tau(F,A_tau,B_tau,C_tau,t);
 printf("{\"A_tau\":%"PRIu64",\"B_tau\":%"PRIu64",\"C_tau\":%"PRIu64",\"H_tau\":%"PRIu64",\"y\":%"PRIu64"}\n",A_tau,B_tau,C_tau,H_tau,y);
 return 0;}