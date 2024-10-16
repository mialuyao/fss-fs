#include <chrono>
#include "fss/include/fss-common.h"
#include "fss/include/fss-client.h"
#include "fss/include/fss-server.h"
// #include "common.h"

//32 bit
#define SCALING_FACTOR 33554432
#define FRACTION_BITS 25
#define INTEGER_BITS 18

// 64 bit
// #define SCALING_FACTOR 1048576ll
// #define FRACTION_BITS 20
// #define INTEGER_BITS 43


uint32_t floatToFixed(double value);
uint32_t dcf_fss(uint32_t lastx, uint32_t tol, uint32_t x);
uint32_t sqrtfss(uint32_t a, uint32_t tol, uint32_t value);
double fixedToFloat(uint32_t fixedValue);

uint32_t floatToFixed(double value) {
    return static_cast<uint32_t>(value * (1 << FRACTION_BITS));
}

double fixedToFloat(uint32_t fixedValue) {
    return static_cast<double>(fixedValue) / (1 << FRACTION_BITS);
}

// x<a, return b
uint32_t dcf_fss(uint32_t a, uint32_t b, uint32_t x){
    Fss fclient, fserver;
    ServerKeyLt k0;
    ServerKeyLt k1;

    initializeClient(&fclient, 32, 2); //2方
    generateTreeLt(&fclient, &k0, &k1, a, b); 
    initializeServer(&fserver, &fclient);

    uint32_t lt_ans0, lt_ans1, lt_fin;
    lt_ans0 = evaluateLt(&fserver, &k0, x); //x 要判断的值
    lt_ans1 = evaluateLt(&fserver, &k1, x);
    lt_fin = lt_ans0 - lt_ans1;
    return lt_fin;
}

uint32_t sqrtfss(uint32_t a, uint32_t tol, uint32_t initial_value){
    uint32_t current_x = initial_value; //当前计算结果
    uint32_t lastx = 0;  //上一次迭代值
    
    int i = 1;
    while(!(dcf_fss(tol, current_x, current_x-lastx))){
        i++;
        lastx = current_x;
        current_x = floatToFixed(0.5 * (fixedToFloat(lastx) + fixedToFloat(a) / fixedToFloat(lastx)));
        std::cout <<  "Iteration: " << i-1 << " "<< "current x :" << fixedToFloat(current_x) << std::endl;
    } 
    return current_x;
}

int main()
{    
    uint32_t a = floatToFixed(17.0); //待开方的值
    double toler = 1e-7; //误差
    double initial_value = floatToFixed(0.48); //初始值
    uint32_t result; 
    uint32_t tol;
    tol = floatToFixed(toler);

    result = sqrtfss(a, tol, initial_value);

    std::cout << "The square root of " << a << " is approximately: " << result << std::endl;

    return 0;
}