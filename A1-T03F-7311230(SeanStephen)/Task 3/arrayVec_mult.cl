__kernel void arrayVec_mult(int number,
                          __global int* vector) {
   

   int arrayIndex = get_global_id(0);
   int arrayValue = vector[arrayIndex];
   int incrementValue = number * (arrayIndex);
   arrayValue += incrementValue;
   vector[arrayIndex] = arrayValue;

}

