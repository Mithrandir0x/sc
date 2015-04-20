#include <iostream>
#include <omp.h>

#define NUM_RECTS 1000000

int main()
{
  int i;
  double mid, height, width, sum = 0.0;
  double area;

  width = 1.0 / (double) NUM_RECTS;
  
#pragma omp parallel for private(mid, height) reduction(+:sum) 
  for(i = 0; i < NUM_RECTS; i++) {
    mid = (i + 0.5) * width;
    height = 4.0 / (1.0 + mid * mid);
    sum += height;
  }
  area  = width * sum;
  std::cout << "pi = " << area << std::endl;

  return 0;
}


