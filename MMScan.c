/*/////////////////////////////////////////////////////////////////////////////
//
// File name : MMScan.c
// Author    : Sanjay Rajopdhye
// Date      : 2019/Sept/16
// Desc      : Finds the prefix product of an array of BxB matrices
//
/////////////////////////////////////////////////////////////////////////////*/

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <sys/time.h>
#include <sys/errno.h>
#include <omp.h>

#define max(x, y)   ((x)>(y) ? (x) : (y))
#define min(x, y)   ((x)>(y) ? (y) : (x))

void MMScan(float ***X, float ***Y, long start, long end, long size) {
    long n, i, j, k;
    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            Y[start][i][j] = X[start][i][j];
        }
    }
#ifdef FAUX  // incorrect parallelization 
#pragma omp parallel for
#endif // incorrect parallelization 
    for (n = start+1; n <= end; ++n) {
        for (i = 0; i < size; ++i) {
            for (j = 0; j < size; ++j) {
                float acc = 0;
                for (k = 0; k < size; ++k) {
                    acc = acc + Y[n-1][i][k] * X[n][k][j];
                }
                Y[n][i][j] = acc;
            }
        }
    }
}

void MMScanDNC(float ***X, float ***Y, float ***T, long start, long end, long size, long aux, long p) {
    /*  		                                                                 */
    /*  		THE CODE BELOW IS IDENTICAL TO THE ONE FOR MMSCAN ABOVE          */
    /*  		                                                                 */
    /*  Your job is to replace it with a recursive (divide and conquer)
     * algorithm that first exposes parallelism, and then exploits it in different
     * ways. You will write a single function, but with conditional compilation
     * flags. Your Makefile should produce different executables for the different
     * parallel versions we require.
     */
    
    long n, i, j, k;

    if (start == end) {
        for (i = 0; i < size; ++i) {
            for (j = 0; j < size; ++j) {
                Y[start][i][j] = X[start][i][j];
            }
        }
        return;
    } else {
        long mid = (start + end) / 2;
        
        for (n = mid+1; n <= end; ++n) {
            for (i = 0; i < size; ++i) {
                for (j = 0; j < size; ++j) {
                    float acc = 0;
                    for (k = 0; k < size; ++k) {
                        acc = acc + (Y[mid][i][k] * Y[n][k][j]);
                    }
                    T[n][i][j] = acc;
                }
            }
        }
        for (n = mid+1; n <= end; ++n) {
            for (i = 0; i < size; ++i) {
                for (j = 0; j < size; ++j) {
                    Y[n][i][j] = T[n][i][j];
                }
            }
        } 
        return;
    }
}
