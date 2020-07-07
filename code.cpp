/* В файле дана матрица. Если в матрицы есть 2 столбца с одинаковой суммой чисел, то поменять их местами.
Предусмотрены следующие проверки:
1.	Открытие файла
2.	Выделение памяти
3.	Прямоугольность матрицы, если не прямоугольная, то в какой строке ошибка
4.	Если прямоугольная, то все ли значения числовые, если нет, то вывести неправильную последовательность и индекс, где она находится. 
Файл ввести с расширением!
*/

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include "Func.h"
using namespace std;


int main()
{
	setlocale(LC_ALL, "Russian");
	string name, elem;
	int count_stolb, count_strok, st1, st2;
	double **mas;
	double *sum;
	fail temp;
	
	cout << "Введите название файла: ";
	getline(cin, name);
	while (func(name, count_stolb, count_strok, mas, elem) == NoOpen)
	{
		cout << "\nФайл с таким именем не найдет\n ";
		cout << "Введите другое название(для выходна из программы наберите ?): ";
		getline(cin, name);
		if (name == "?")
			return 0;
	}
	cout << "Файл открыт\n";
	temp = func(name, count_stolb, count_strok, mas, elem);
	switch (temp)
	{
	case F_Memory: cout << "\nНе удалось выделить память\n";
		system("pause");
		return 0;

	case Is_Empty: cout << "\nФайл пуст\n";
		system("pause");
		return 0;

	case F_Elem: cout << "\nНеверный формат элемента матрицы.\n";
		cout << "Проверьте элемент \"" << elem << "\" с индексом [" << count_strok << "][" << count_stolb << "]\n";
		system("pause");
		return 0;

	case NoPr: cout << "\nМатрица не прямоугольная.Ошибка в строке №" << ++count_strok << endl;
		system("pause");
		return 0;

	case ok: cout << "\nМатрица успешно считана.\n";
	}
	//cout << ConvPtrMatrtoStr(mas, count_strok, count_stolb);
	printf(mas, count_stolb, count_strok);

	temp = zadanie(mas, count_stolb, count_strok, sum, st1, st2);
	switch (temp)
	{
	case ok: cout << "Столбцы с индексами [" << st1 << "] и [" << st2 << "] имеют одинаковую сумму." << endl;
			cout << "Новая матрица:\n";
			printf(mas, count_stolb, count_strok);
			delete[] sum;
			break;
	case F_Memory: cout << "Недостаточно памяти";
		return 0;
	case NoSum: cout << "Столбцов с одинаковой суммой нет!\n";
		break;
	}
	
	DelMatr(mas, count_strok);
	system("pause");
	return 0;
}

PtrMas memory(double**& mas, int& count_strok, int& count_stolb)  //если тут сделать fail, то увидит ли главная функция его.
{
	int i;

	try
	{
		mas = new double*[count_strok];
	}
	catch (bad_alloc)
	{
		return nullptr;
	}

	try
	{
		for (i = 0; i < count_strok; i++)
			mas[i] = new double[count_stolb];
	}
	catch (bad_alloc)
	{
		int j = i - 1;
		while (j >= 0)
		{
			delete[] mas[j];
			j--;
		}
		delete[] mas;
		return nullptr;
	}
	return mas;
}
PtrLine memory(double*& sum, int& count_stolb) // выделяем память под массив столбцов. хватит ли памяти для массива суммы стобцов
{
	try
	{
		sum = new double[count_stolb];
	}
	catch (bad_alloc)
	{
		return nullptr;
	}
	return sum;
}
fail func(string name, int& count_stolb, int& count_strok, double**& mas, string &elem) // открытие, считывание, прямоугольность формат данных, выделение памяти
{
	ifstream fin;
	fin.open(name);//открытие файла
	if (!fin.is_open())
	{
		return NoOpen;
	}

	int pos;
	double temp; //переменная, куда будет считывать числа
	bool work = true; // работа внешнего цикла
	char symbol = 0; //считывание символов
	int count_elem = 0; // число элементов
	count_stolb = 0; // присваиваем счетчикам столбцов и сторок 0
	count_strok = 0;

	fin >> ws;// пропускаем разделительные символы
	if (fin.peek() == EOF) // если конец файла
	{
		fin.close();//закрытие файла
		return Is_Empty;
	}
	while (work) //переменная которая отвечает за данный цикл
	{
		pos = fin.tellg();
		if (fin >> temp)
		{
			count_stolb++;

			//Выявление ошибок
			while (symbol != EOF && symbol != '\n')
			{
				symbol = fin.get();// передвигаем стрелку на один символ вправо. тоже самое, что fin.peek() fin.seekg(1, ios::cur)
				switch (symbol)
				{
				case ' ': break;
				case '\t': break;

				case EOF:
					work = false; //выходим из цикла

				case '\n':


					if (count_elem != count_stolb*count_strok) // проверка на прямоугольность
					{
						return NoPr;
					}
					count_strok++;
					count_elem += count_stolb;
					count_stolb = 0;
					fin >> ws;
					if (fin.eof())
					{
						symbol = EOF;
						work = false;
						break;
					}
					break;
				default: //дошли до числа
					fin.seekg(-1, ios::cur); ///     или попробовать так fin.unget(); // возвращение на символ назад

					symbol = EOF;//для выхода из внутреннего цикла
				}
			}
			symbol = 0; // надо войти в цикл, обнуляем
		}
		else
		{
			fin.clear();
			fin.seekg(pos, ios::beg);
			fin.seekg(-1, ios::cur);
			symbol = fin.peek();
			if (symbol != '\n' || symbol != '\t' || symbol != ' ') // спросить на счет одного случая.
			{
				count_stolb--; // если неправильный элемнт начинается с числа, то надо уменьшить счетчик.
			}
			while (symbol != '\n' && symbol != '\t' && symbol != ' ')
			{
				fin.seekg(-1, ios::cur);
				symbol = fin.peek();
			}
			fin >> ws;
			fin >> elem;
			return F_Elem;
		}
	}


	count_stolb = count_elem / count_strok; /*при переходе на другую строку, мы обнуляли счетчик стобцов.
											Значит если пройдена проверка на прямоугольность, то можно найти их.*/
											/*cout << count_strok;
											cout << count_stolb; */  // проверить, правильно ли мы считали столбы и строки.


	if (memory(mas, count_strok, count_stolb) == nullptr)
	{
		return F_Memory;
	}

	fin.clear();
	fin.seekg(0, ios::beg);


	for (int i = 0; i < count_strok; i++)
	{
		for (int j = 0; j < count_stolb; j++)
		{
			fin >> mas[i][j];
		}
	}

	fin.close();

	return ok;

}

void printf(double** &mas, int &count_stolb, int &count_strok) //вывод
{
	for (int i = 0; i < count_strok; i++)
	{
		for (int j = 0; j < count_stolb; j++)
			cout << setw(6) << mas[i][j];
		cout << endl;
	}
}

fail zadanie(double** mas, int count_stolb, int count_strok, double*& sum, int& st1, int& st2)
{
	double k = 0;
	if (memory(sum, count_stolb) == nullptr)
	{
		return F_Memory;
	}

	for (int i = 0; i < count_stolb; i++)
	{
		sum[i] = 0;
		for (int j = 0; j < count_strok; j++)
		{

			sum[i] = sum[i] + mas[j][i];

		}
		/*cout << "сумма " << i + 1 << "-го столбца " << sum[i] << "\t"; использовал для проверки правильности поиска суммы */
	}

	for (st1 = 0; st1 < count_stolb; st1++)
	{
		for (st2 = 0; st2 < count_stolb; st2++)
		{
			if ((sum[st1] == sum[st2]) && (st1 != st2))
			{
				for (int x = 0; x < count_strok; x++)
				{
					k = mas[x][st1];
					mas[x][st1] = mas[x][st2];
					mas[x][st2] = k;
				}
				return ok;
			}

		}
	}
	return NoSum;
}

void DelMatr(double** mas, int count_strok) //удаление
{
	for (int i = 0; i < count_strok; i++)
		delete[] mas[i];
	delete[] mas;
}

void CopyMatr(double** mas, int count_strok, int count_stolb, double**& masCopy)
{
	memory(masCopy, count_strok, count_stolb);
	for (int i = 0; i < count_strok; i++)
		for (int j = 0; j < count_stolb; j++)
			masCopy[i][j] = mas[i][j];

}

string ConvPtrMatrtoStr(double** mas, int count_strok, int count_stolb)
{
	//memory(mas, count_strok, count_stolb);

	stringstream ss;
	for (int i = 0; i < count_strok; i++)
	{
		for (int j = 0; j < count_stolb; j++)
			ss << setw(5) << mas[i][j];
		ss << "\n";
	}
	string StrPtrMatr = ss.str();
	return StrPtrMatr;

}
