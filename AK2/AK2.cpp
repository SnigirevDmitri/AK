#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <time.h>
#include <omp.h>
#include <stdint.h>

double pi_time_opt(double* pi, int64_t accuracy, const uint8_t& threads_number);

int main() 
{
   setlocale(LC_CTYPE, "Russian");
   double pi;
   int64_t accuracy;
   uint8_t threads_number = 0;
   printf_s("Введите точность вычисления числа Пи(количество членов ряда):\n");
   scanf_s("%lld", &accuracy);
   printf_s("Введите число потоков:\n");
   scanf_s("%hhu", &threads_number);
   double time = pi_time_opt(&pi, accuracy, threads_number);
   printf_s("Число Пи: %.30lf\n", pi);
   printf_s("Затраченное время: %lf\n", time);
   _getch();
   return 0;
}

double pi_time_opt(double* pi, int64_t accuracy, const uint8_t& threads_number)
{
   double pi_here = 1;
   int64_t i;
   clock_t start = clock();
   omp_set_num_threads(threads_number);
#pragma omp parallel for lastprivate(i) reduction (- : pi_here) shared(accuracy) schedule(guided)
   for (i = 1; i < (accuracy + 1) / 2; i++) {
      pi_here -= 2 / double(16 * i * i - 1);
   }
   if (accuracy % 2 == 0)
      pi_here -= 1 / double(2 * accuracy - 1);
   pi_here *= 4;
   *pi = pi_here;
   clock_t end = clock();
   return double(end - start) / CLK_TCK;
}