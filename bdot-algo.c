#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float dotter(float b, float b1, float b2, float b3, float b4, float h){
    float bdot;
    bdot = ((3 * b4) - (16 * b3) + (36 * b2) - (48 * b1) + (25 * b))/(h*12);
    return bdot;
}

float bdotter(float Bxh[], float Byh[], float Bzh[], float h, float Bdot[1][3]){
    Bdot[1][0] = dotter(Bxh[0], Bxh[1], Bxh[2], Bxh[3], Bxh[4], h);
    Bdot[1][1] = dotter(Byh[0], Byh[1], Byh[2], Byh[3], Byh[4], h);
    Bdot[1][2] = dotter(Bzh[0], Bzh[1], Bzh[2], Bzh[3], Bzh[4], h);
    return Bdot;
}

float bdotcap(float Bdot[1][3]){
    float bcap = [3][1];
    bcap[0][0] = Bdot[1][0] / sqrt(pow(Bdot[1][0], 2) + pow(Bdot[1][1], 2) + pow(Bdot[1][2], 2));
    bcap[1][0] = Bdot[1][1] / sqrt(pow(Bdot[1][0], 2) + pow(Bdot[1][1], 2) + pow(Bdot[1][2], 2));
    bcap[2][0] = Bdot[1][2] / sqrt(pow(Bdot[1][0], 2) + pow(Bdot[1][1], 2) + pow(Bdot[1][2], 2));
    return bcap;
}


float determinant(float Bcap[3][3]) {
    return (Bcap[0][0]*(Bcap[1][1]*Bcap[2][2] - Bcap[1][2]*Bcap[2][1])
         - Bcap[0][1]*(Bcap[1][0]*Bcap[2][2] - Bcap[1][2]*Bcap[2][0])
         + Bcap[0][2]*(Bcap[1][0]*Bcap[2][1] - Bcap[1][1]*Bcap[2][0]));
}

// Function to get cofactor of element (p, q)
void getCofactor(float Bcap[3][3], float temp[3][3], int p, int q) {
    int i = 0, j = 0;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (row != p && col != q) {
                temp[i][j++] = Bcap[row][col];
                if (j == 3 - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

// Function to find the adjoint of Bcap
void adjoint(float Bcap[3][3], float adj[3][3]) {
    float temp[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            getCofactor(Bcap, temp, i, j);
            float sign = ((i + j) % 2 == 0) ? 1.0f : -1.0f;

            adj[j][i] = sign * (temp[0][0]*temp[1][1] - temp[0][1]*temp[1][0]);
        }
    }
}

// Function to compute the inverse of Bcap
float inverseBcap(float Bcap[3][3]) {
    float invBcap[3][3]
    float det = determinant(Bcap);
    if (det == 0.0f) {
        printf("Matrix Bcap is singular and cannot be inverted.\n");
        return 0;
    }

    float adj[3][3];
    adjoint(Bcap, adj);

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            invBcap[i][j] = adj[i][j] / det;

    return invBcap;
}

float wtildedotproduct(float invBcap[3][3], float bcap[3][1]) {
    float wtilde[3][1];
    wtilde[0][0] = invBcap[0][0] * bcap[0][0] + invBcap[0][1] * bcap[1][0] + invBcap[0][2] * bcap[2][0];
    wtilde[1][0] = invBcap[1][0] * bcap[0][0] + invBcap[1][1] * bcap[1][0] + invBcap[1][2] * bcap[2][0];
    wtilde[2][0] = invBcap[2][0] * bcap[0][0] + invBcap[2][1] * bcap[1][0] + invBcap[2][2] * bcap[2][0];
    return wtilde;
}

float add(Bcap[3][3], epsilon[3][3]){
    float winverse[3][3];
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            w[i][j] = Bcap[i][j] + epsilon[i][j];
        }
    }
    return winverse
}

float crossProduct(float B[3][1], float wtilde[3][1]) {
    float Mraw[3];
    Mraw[0] = B[1][0]*wtilde[2][0] - B[2][0]*wtilde[1][0];
    Mraw[1] = B[2][0]*wtilde[0][0] - B[0][0]*wtilde[2][0];
    Mraw[2] = B[0][0]*wtilde[1][0] - B[1][0]*wtilde[0][0];
    return Mraw;
}

int main{
    float E = 0.000001;
    float h; // sampling rate
    float Bxh[5]; // array to store the last 5 values of Bx with Bx-4h stored at Bxh[0]
    float Byh[5]; // array to store the last 5 values of By
    float Bzh[5]; // array to store the last 5 values of Bz

    float B[3][1] = {{Bx[4]}, {By[4]}, {Bz[4]}};

    float epsilon[3][3];
    float Bdot[1][3];
    
    float Bcap[3][3] = {
                    {E,           E - B[2][1],   E + B[1][1]},
                    {E + B[2][1], E,             E - B[0][1]},
                    {E - B[1][1], E + B[0][1],   E}
                };
                                                                 
    Bdot = bdotter(float Bxh[], float Byh[], float Bzh[], float h, float Bdot[1][3]);
    float bcap[3][1] = bdotcap(float Bdot[1][3]);
    
    float winverse[3][3] = add(Bcap, epsilon);
    float invBcap[3][3] = inverseBcap(winverse);

    float wtilde[3][1] = wtildedotproduct(invBcap, bcap);

    float Mraw[3] = crossProduct(B, wtilde);
    float kw;
    printf("Mx = %f, My = %f, Mz = %f\n", (-1*kw*Mraw[0]), (-1*kw*Mraw[1]), (-1*kw*Mraw[2]));

}


