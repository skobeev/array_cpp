#pragma once
#ifndef FUNC_H
#define FUNC_H
using namespace std;
enum fail { ok, NoOpen, F_Memory, Is_Empty, F_Elem, NoPr, NoSum/*не прямоугольная*/ };
typedef double** PtrMas;
typedef double* PtrLine;
PtrMas memory(double**& , int& , int& );  //если тут сделать fail, то увидит ли главная функция его.

PtrLine memory(double*& , int& ); // выделяем память под массив столбцов. хватит ли памяти для массива суммы стобцов
fail func(string, int&, int&, double**&, string&);
fail zadanie(double**, int, int, double*&, int&, int&);

void printf(double**&, int&, int&);
void DelMatr(double**, int); //удаление


void CopyMatr(double**, int, int, double**&);


string ConvPtrMatrtoStr(double**, int, int);

#endif
