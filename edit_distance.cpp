#include "char_dist.h"

int matrix[128][128];
int distance(char *s1, char *s2) {
    int x, y, s1len, s2len;
    
    s1len = strlen(s1);
    s2len = strlen(s2);
    
    
    matrix[0][0] = 0;

    for (x = 1; x <= s2len; x++) {
        matrix[x][0] = matrix[x-1][0] + 1;    	
    }

    for (y = 1; y <= s1len; y++) {
        matrix[0][y] = matrix[0][y-1] + 1;    	
    }

    for (x = 1; x <= s2len; ++x) {
        for (y = 1; y <= s1len; ++y) {
            matrix[x][y] = matrix[x-1][y] + 10; // ignore a character from the first string

            int tmp = matrix[x][y-1] + 10; // ignore a character from the second string
            if (tmp < matrix[x][y]) {
            	matrix[x][y] = tmp;
            }

            tmp = matrix[x-1][y-1] + cost_for(s1[y], s2[x]); // add the cost of matching based on keyboard

            if (tmp < matrix[x][y]) {
            	matrix[x][y] = tmp;
            }
        }
    }
 
    return matrix[s2len][s1len];
}