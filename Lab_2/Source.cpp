#include<math.h>
#include<iostream>
#include<windows.h>
#include<stdio.h>
#include<conio.h>
#include<complex>
#include<math.h>
#define PI 3.14159265    
using namespace std;


template<typename T>
struct Points
{
private:
	static double lambda;
public:
	T x, y;
	double& _lambda() { return lambda; }
	template<typename V> friend std::ostream& operator <<(std::ostream&, const Points<V>&);
};

template<typename T>
double Points<T>::lambda = 0.000001;

template<typename T>
std::ostream& operator <<(std::ostream& os, const Points<T>& p)
{
	return os << "(" << p.x << ", " << p.y << ")";
}

//Меню выбора типа данных
void choose_type()
{
	std::cout << "Choose the type of data you need:\n\n";
	std::cout << "1. int\n";
	std::cout << "2. float\n";
	std::cout << "3. double\n";
	std::cout << "4. std::complex<float>\n";
	std::cout << "5. std::complex<double>\n";
	std::cout << "\nEnter the number: ";
}

//Основное меню
void menu()
{
	std::cout << "1. Create a polyline\n";
	std::cout << "2. Get the vertices of the polyline\n";
	std::cout << "3. Get the length of the polyline\n";
	std::cout << "4. Add a vertex to the beginning of the polyline\n";
	std::cout << "5. Add a vertex to the end of the polyline\n";
	std::cout << "6. Create a polyline that is the 'sum' of two polylines\n";
	std::cout << "7. ADDITIONAL QUESTION: Create a polyline regular polygon\n";
	std::cout << "8. Change precision\n";
	std::cout << "9. Exit the program\n";
	std::cout << "\nEnter the number: ";
}

//----------------------------------------------------------------

//Класс ломанной
template<typename T>
class Broken
{
private:
	Points<T>* data;
	int cap, n;
	void memory();

public:
	Broken();
	Broken(const int cap);
	~Broken();
	Broken(const Broken<T>&);
	double len_broken() const;
	double len_broken_complex() const;
	Broken<T> operator +(const Broken<T>&) const;
	void operator =(const Broken<T>&);
	void operator +=(const Points<T>&);
	template<typename V> friend void operator +=(const Points<V>&, Broken<V>&);
	template<typename V> friend std::ostream& operator <<(std::ostream&, const Broken<V>&);
	Points<T>& operator [](int);
	int get_cap() const;
	int get_n() const;
	void set_n();
};

//Выделение памяти для массива вершин
template<typename T>
void Broken<T>::memory()
{
	cap += 2;
	Points<T>* tmp_data = new Points<T>[cap];

	for (int i = 0; i < n; i++) {
		if (i < cap) tmp_data[i] = data[i];
		else throw "Error! Access outside of allocated memory.\n\n";
	}

	delete[] data;
	data = tmp_data;
}

//Конструктор по умолчанию
template<typename T>
Broken<T>::Broken() :cap(0), n(0), data(NULL) {}

//Конструктор для выделения памяти для вершин
template<typename T>
Broken<T>::Broken(const int cap) : cap(cap), n(0)
{
	data = new Points<T>[cap];
}

//Деструктор
template<typename T>
Broken<T>::~Broken() { delete[] data; }

//Конструктор копирования
template<typename T>
Broken<T>::Broken(const Broken<T>& obj) : cap(obj.cap), n(obj.n)
{
	data = new Points<T>[cap];

	for (int i = 0; i < n; i++) {
		if (i < cap) data[i] = obj.data[i];
		else throw "Error! Access outside of allocated memory.\n\n";
	}
}

//Вычисление длины ломаной линии
template<typename T>
double Broken<T>::len_broken() const
{
	double sum = 0;

	for (int i = 0; i < n - 1; i++) {
		if (i >= 1 && i < n - 1 && data[i - 1] == data[i + 1]) continue;

		double x = 0, y = 0;

		x = pow(data[i].x - data[i + 1].x, 2);
		y = pow(data[i].y - data[i + 1].y, 2);

		sum += sqrt(x + y);
	}

	return sum;
}

//Вычисление длины ломаной линии для комплексных чисел
template<typename T>
double Broken<T>::len_broken_complex() const
{
	double sum = 0;

	for (int i = 0; i < n - 1; i++) {
		if (i >= 1 && i < n - 1 && data[i - 1] == data[i + 1]) continue;

		std::complex<double> x, y;

		x = data[i].x - data[i + 1].x;
		y = data[i].y - data[i + 1].y;

		sum += sqrt(pow(std::abs(x), 2) + pow(std::abs(y), 2)); // z = sqrt(a^2+b^2)
	}
	return sum;
}

//Оператор сложения двух ломаных линий
template<typename T>
Broken<T> Broken<T>:: operator +(const Broken<T>& obj) const
{
	Broken<T> tmp_obj(cap + obj.cap);
	int tmp_cap = cap + obj.cap;
	tmp_obj.n = n + obj.n;

	for (int i = 0; i < n; i++) {
		if (i < tmp_cap) tmp_obj.data[i] = data[i];
		else throw "Error! Access outside of allocated memory.\n\n";
	}

	for (int i = n; i < tmp_obj.n; i++) {
		if (i < tmp_cap) tmp_obj.data[i] = obj.data[i - n];
		else throw "Error! Access outside of allocated memory.\n\n";
	}

	return tmp_obj;
}

//Оператор присваивания
template<typename T>
void Broken<T>:: operator =(const Broken<T>& obj)
{
	delete[] data;
	data = new Points<T>[obj.cap];
	n = obj.n;
	cap = obj.cap;
	for (int i = 0; i < n; i++) {
		if (i < cap) data[i] = obj.data[i];
		else throw "Error! Access outside of allocated memory.\n\n";
	}
}

//Оператор добавдения вершины в конец ломанной
template<typename T>
void Broken<T>:: operator +=(const Points<T>& p)
{
	if (cap <= n) memory();
	data[n++] = p;
}

//Оператор для добавления вершины в начало
template<typename T>
void operator +=(const Points<T>& p, Broken<T>& obj)
{
	if (obj.cap <= obj.n) obj.memory();

	Points<T>* tmp_data = new Points<T>[obj.cap];

	for (int i = 0; i < obj.n; i++)
	{
		if (i < obj.cap) tmp_data[i + 1] = obj.data[i];
		else throw "Error! Access outside of allocated memory.";
	}

	tmp_data[0] = p;

	delete[] obj.data;
	obj.data = tmp_data;
	obj.n++;
}

//Перегрузка оператора вывода вершины
template<typename T>
std::ostream& operator <<(std::ostream& os, const Broken<T>& obj)
{
	for (int i = 0; i < obj.n; i++)
	{
		os << obj.data[i];
		if (i < obj.n - 1) os << " -> ";
	}
	return os;
}

//Оператор чтнения/изменения вектора
template<typename T>
Points<T>& Broken<T>:: operator [](const int i)
{
	if (i < n) return data[i];
	else throw "Error! Attempt to access by case index.\n\n";
}

//Получение размера массива вершин
template<typename T>
int Broken<T>::get_cap() const { return cap; }

//Получение количества вершин в массиве
template<typename T>
int Broken<T>::get_n() const { return n; }

//Увеличить на одну из вершин счетчика в массиве
template<typename T>
void Broken<T>::set_n() { n++; }

//----------------------------------------------------------

//Создать ломанную
template<typename T>
void create_polyline(Broken<T>* mas_obj, int* count)
{
	float n = 0;
	Points<T> p = { 0 , 0 };

	std::cout << "\n";
	do
	{
		std::cout << "The maximum number of vertices in a polyline: ";
		std::cin >> n;
	} while (n <= 0 || n != static_cast<int>(n));

	n = static_cast<int>(n);

	Broken<T> tmp(n);

	do
	{
		std::cout << "How many vertices do you want to add now?: ";
		std::cin >> n;
	} while (n <= 0 || n > tmp.get_cap()|| n != static_cast<int>(n));

	n = static_cast<int>(n);

	for (int i = 0; i < n; i++)
	{
		tmp.set_n();
		std::cout << "\nEnter x: ";
		std::cin >> p.x;
		std::cout << "Enter y: ";
		std::cin >> p.y;
		tmp[i] = p;
	}

	mas_obj[*count] = tmp;
	*count += 1;
}

//Отображение вершин ломаной линии
template<typename T>
void output_vertices(Broken<T>* mas_obj, int count)
{
	int n = 0;

	std::cout << "\n";
	do
	{
		std::cout << "Which polyline vertices do you want to get?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);


	std::cout << "\nVertices: ";
	std::cout << mas_obj[n];

	std::cout << "\n\n";
	system("pause");
}

//Получить длинну ломанной
template<typename T>
void len_poly(Broken<T>* mas_obj, int count)
{
	int n = 0;

	std::cout << "\n";
	do
	{
		std::cout << "Which polyline length do you want to calculate?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);

	std::cout << "The length of the polyline is " << mas_obj[n].len_broken();
	std::cout << "\n\n";
	system("pause");
}

//Добавить вершину в начало ломанной
template<typename T>
void ver_beg(Broken<T>* mas_obj, int count)
{
	int n = 0;
	Points<T> p;

	std::cout << "\n";
	do
	{
		std::cout << "In which polyline do I need to add a vertex to the beginning?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);

	std::cout << "\nEnter x: ";
	std::cin >> p.x;
	std::cout << "Enter y: ";
	std::cin >> p.y;

	p += mas_obj[n];
}

//Добавить вершину в конец ломанной
template<typename T>
void ver_back(Broken<T>* mas_obj, int count)
{
	int n = 0;
	Points<T> p;

	std::cout << "\n";
	do
	{
		std::cout << "In which polyline do I need to add a vertex to the beginning?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);

	std::cout << "\nEnter x: ";
	std::cin >> p.x;
	std::cout << "Enter y: ";
	std::cin >> p.y;

	mas_obj[n] += p;
}

//Создание новой ломанной, полученной путем соединения двух других ломанных
template<typename T>
void sum_ver(Broken<T>* mas_obj, int* count)
{
	int n = 0, m = 0;

	std::cout << "\n";
	do
	{
		std::cout << "Select the first vertex (counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= *count);

	do
	{
		std::cout << "Select the second vertex (counting from zero): ";
		std::cin >> m;
	} while (m < 0 || m >= *count);

	mas_obj[*count] = mas_obj[n] + mas_obj[m];
	*count += 1;
}

//Создать вектор правильный многоугольник
template<typename T>
void add_task(Broken<T>* mas_obj, int* count)
{
	Points<T> p = { 0, 0 };
	double a = 0;
	double len = 0;
	double angle = 0, result_x = 0, result_y = 0;

	do {
		std::cout << "\nEnter the number N - the number of vertices in the polyline (N>2): ";
		std::cin >> a;
		std::cout << "\nEnter the length of the polyline: ";
		std::cin >> len;
	} while (a <= 2);
	mas_obj[*count] += p;

	angle = ((a - 2) * 180) / a;
	mas_obj[*count] += p;

	for (int i = 1; i < a; i++) {
		result_x = (result_x + len * cos((i - 1) * ((180 - angle) * PI / 180)));
		result_y = (result_y + len * sin((i - 1) * ((180 - angle) * PI / 180)));
		p.x = result_x;
		p.y = result_y;

		mas_obj[*count] += p;
	}
	p.x = 0;
	p.y = 0;
	mas_obj[*count] += p;
	*count += 1; 
}

//---------------------------------------------------------

//Создать ломанную (std::complex<>)
template<typename S>
void create_polyline(Broken<std::complex<S>>* mas_obj, int* count)
{
	int n = 0;
	Points<std::complex<S>> p;
	S num = 0;

	std::cout << "\n";
	do
	{
		std::cout << "The maximum number of vertices in a polyline: ";
		std::cin >> n;
	} while (n <= 0);

	Broken<std::complex<S>> tmp(n);

	do
	{
		std::cout << "How many vertices do you want to add now?: ";
		std::cin >> n;
	} while (n <= 0 || n > tmp.get_cap());


	for (int i = 0; i < n; i++)
	{
		tmp.set_n();
		std::cout << "\nEnter x: ";

		std::cout << "\n\tEnter real: ";
		std::cin >> num;
		p.x.real(num);

		std::cout << "\tEnter imag: ";
		std::cin >> num;
		p.x.imag(num);


		std::cout << "\nEnter y: ";

		std::cout << "\n\tEnter real: ";
		std::cin >> num;
		p.y.real(num);

		std::cout << "\tEnter imag: ";
		std::cin >> num;
		p.y.imag(num);

		tmp[i] = p;
	}

	mas_obj[*count] = tmp;
	*count += 1;
}

//Добавить вектор в начало ломанной (std::complex<>)
template<typename S>
void ver_beg(Broken<std::complex<S>>* mas_obj, int count)
{
	int n = 0;
	Points<std::complex<S>> p;
	S num = 0;

	std::cout << "\n";
	do
	{
		std::cout << "In which polyline do I need to add a vertex to the beginning?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);

	std::cout << "\nEnter x: ";

	std::cout << "\n\tEnter real: ";
	std::cin >> num;
	p.x.real(num);

	std::cout << "\tEnter imag: ";
	std::cin >> num;
	p.x.imag(num);


	std::cout << "\nEnter y: ";

	std::cout << "\n\tEnter real: ";
	std::cin >> num;
	p.y.real(num);

	std::cout << "\tEnter imag: ";
	std::cin >> num;
	p.y.imag(num);

	p += mas_obj[n];
}

//Добавить вектор в конец ломанной(std::complex)
template<typename S>
void ver_back(Broken<std::complex<S>>* mas_obj, int count)
{
	int n = 0;
	Points<std::complex<S>> p;
	S num = 0;

	std::cout << "\n";
	do
	{
		std::cout << "In which polyline do I need to add a vertex to the beginning?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);

	std::cout << "\nEnter x: ";

	std::cout << "\n\tEnter real: ";
	std::cin >> num;
	p.x.real(num);

	std::cout << "\tEnter imag: ";
	std::cin >> num;
	p.x.imag(num);


	std::cout << "\nEnter y: ";

	std::cout << "\n\tEnter real: ";
	std::cin >> num;
	p.y.real(num);

	std::cout << "\tEnter imag: ";
	std::cin >> num;
	p.y.imag(num);

	mas_obj[n] += p;
}

//Получить длинну ломанной(std::complex<>)
template<typename S>
void len_poly(Broken<std::complex<S>>* mas_obj, int count)
{
	int n = 0;

	std::cout << "\n";
	do
	{
		std::cout << "Which polyline length do you want to calculate?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);

	std::cout << "The length of the polyline is " << mas_obj[n].len_broken_complex();
	std::cout << "\n\n";
	system("pause");
}

//Создать ломанную правильный многоугольник (std::complex)
template<typename S>
void add_task(Broken<std::complex<S>>* mas_obj, int* count)
{
	
	Points<std::complex<S>> p;
	double a = 0;
	double len = 0;
	double angle = 0, result_x = 0, result_y = 0, abs = 0, tmp = 0;

	do {
		std::cout << "\nEnter the number N - the number of vertices in the polyline (N>2): ";
		std::cin >> a;
		std::cout << "\nEnter the length of the polyline: ";
		std::cin >> len;
	} while (a <= 2);
	mas_obj[*count] += p;

	angle = ((a - 2) * 180) / a;

	for (int i = 1; i < a; i++) {
		abs = len * cos((i - 1) * ((180 - angle) * PI / 180));
		if (abs < 0) { tmp = sqrt(abs*abs / 2) * (-1); }
		else { tmp = sqrt(abs * abs / 2); }
		result_x = (result_x + tmp);

		abs = len * sin((i - 1) * ((180 - angle) * PI / 180));
		if (abs < 0) { tmp = sqrt(abs * abs / 2) * (-1); }
		else { tmp = sqrt(abs * abs / 2); }
		result_y = (result_y + tmp);

		p.x.real(result_x); p.x.imag(result_x);
		p.y.real(result_y); p.y.imag(result_y);

		mas_obj[*count] += p;
	}

	p.x.real(0); p.x.imag(0);
	p.y.real(0); p.y.imag(0);
	mas_obj[*count] += p;
	*count += 1;
}

//---------------------------------------------------------

//Изменение точности
template<typename T>
void change_precision()
{
	Points<T> p;
	double n = 0;

	std::cout << "Now the accuracy is " << p._lambda();
	std::cout << "\nEnter new precision: ";
	std::cin >> n;

	p._lambda() = n;
}

//Оператор сравнения точности 
template<typename T>
bool operator==(Points<T>& p1, Points<T>& p2)
{
	if (fabs(p1.x - p2.x) > p1._lambda()) return false;
	if (fabs(p1.y - p2.y) > p1._lambda()) return false;
	return true;
}

//Оператор на неравенство
template<typename T>
bool operator!=(Points<T>& p1, Points<T>& p2)
{
	if (fabs(p1.x - p2.x) < p1._lambda()) return false;
	if (fabs(p1.y - p2.y) < p1._lambda()) return false;
	return true;
}

//Оператор сравнения точности (std::complex<>)
template<typename S>
bool operator==(Points<std::complex<S>>& p1, Points<std::complex<S>>& p2)
{
	if (fabs(p1.x.real() - p2.x.real()) > p1._lambda()) return false;
	if (fabs(p1.x.imag() - p2.x.imag()) > p1._lambda()) return false;
	if (fabs(p1.y.real() - p2.y.real()) > p1._lambda()) return false;
	if (fabs(p1.y.imag() - p2.y.imag()) > p1._lambda()) return false;
	return true;
}

//Оператор на неравенство(std::complex<>)
template<typename S>
bool operator!=(Points<std::complex<S>>& p1, Points<std::complex<S>>& p2)
{
	if (fabs(p1.x.real() - p2.x.real()) < p1._lambda()) return false;
	if (fabs(p1.x.imag() - p2.x.imag()) < p1._lambda()) return false;
	if (fabs(p1.y.real() - p2.y.real()) < p1._lambda()) return false;
	if (fabs(p1.y.imag() - p2.y.imag()) < p1._lambda()) return false;
	return true;
}

//Оператор вывода
template<typename S>
std::ostream& operator <<(std::ostream& os, const std::complex<S>& c)
{
	return os << "(" << c.real() << ", " << c.imag() << ")";
}

//----------------------------------------------------------

//Основная функция
int main()
{
	int n = 0;
	int count = 0;

	choose_type();
	std::cin >> n;

	if (n == 1)
	{
		Broken<int> mas_obj[20];
		while (1)
		{
			try {
				system("CLS");
				menu();
				std::cin >> n;
				if (n == 1) create_polyline(mas_obj, &count);
				if (n == 2) output_vertices(mas_obj, count);
				if (n == 3) len_poly(mas_obj, count);
				if (n == 4) ver_beg(mas_obj, count);
				if (n == 5) ver_back(mas_obj, count);
				if (n == 6) sum_ver(mas_obj, &count);
				if (n == 7) add_task(mas_obj, &count);
				if (n == 8) change_precision<int>();
				if (n == 9) break;
			}
			catch (const char* ex)
			{
				std::cout << ex;
				system("pause");
				system("CLS");
			}
		}
	}

	else if (n == 2)
	{
		Broken<float> mas_obj[20];
		while (1)
		{
			try {
				system("CLS");
				menu();
				std::cin >> n;
				if (n == 1) create_polyline(mas_obj, &count);
				if (n == 2) output_vertices(mas_obj, count);
				if (n == 3) len_poly(mas_obj, count);
				if (n == 4) ver_beg(mas_obj, count);
				if (n == 5) ver_back(mas_obj, count);
				if (n == 6) sum_ver(mas_obj, &count);
				if (n == 7) add_task(mas_obj, &count);
				if (n == 8) change_precision<float>();
				if (n == 9) break;
			}
			catch (const char* ex)
			{
				std::cout << ex;
				system("pause");
				system("CLS");
			}
		}
	}

	else if (n == 3)
	{
		Broken<double> mas_obj[20];
		while (1)
		{
			try {
				system("CLS");
				menu();
				std::cin >> n;
				if (n == 1) create_polyline(mas_obj, &count);
				if (n == 2) output_vertices(mas_obj, count);
				if (n == 3) len_poly(mas_obj, count);
				if (n == 4) ver_beg(mas_obj, count);
				if (n == 5) ver_back(mas_obj, count);
				if (n == 6) sum_ver(mas_obj, &count);
				if (n == 7) add_task(mas_obj, &count);
				if (n == 8) change_precision<double>();
				if (n == 9) break;
			}
			catch (const char* ex)
			{
				std::cout << ex;
				system("pause");
				system("CLS");
			}
		}
	}

	else if (n == 4)
	{
		Broken<std::complex<float>> mas_obj[20];
		while (1)
		{
			try {
				system("CLS");
				menu();
				std::cin >> n;
				if (n == 1) create_polyline(mas_obj, &count);
				if (n == 2) output_vertices(mas_obj, count);
				if (n == 3) len_poly(mas_obj, count);
				if (n == 4) ver_beg(mas_obj, count);
				if (n == 5) ver_back(mas_obj, count);
				if (n == 6) sum_ver(mas_obj, &count);
				if (n == 7) add_task(mas_obj, &count);
				if (n == 8) change_precision<std::complex<float>>();
				if (n == 9) break;
			}
			catch (const char* ex)
			{
				std::cout << ex;
				system("pause");
				system("CLS");
			}
		}
	}

	else if (n == 5)
	{
		Broken<std::complex<double>> mas_obj[20];
		while (1)
		{
			try {
				system("CLS");
				menu();
				std::cin >> n;
				if (n == 1) create_polyline(mas_obj, &count);
				if (n == 2) output_vertices(mas_obj, count);
				if (n == 3) len_poly(mas_obj, count);
				if (n == 4) ver_beg(mas_obj, count);
				if (n == 5) ver_back(mas_obj, count);
				if (n == 6) sum_ver(mas_obj, &count);
				if (n == 7) add_task(mas_obj, &count);
				if (n == 8) change_precision<std::complex<double>>();
				if (n == 9) break;
			}
			catch (const char* ex)
			{
				std::cout << ex;
				system("pause");
				system("CLS");
			}
		}
	}

	else std::cout << "\n\nPlease enter the correct number next time.\n\n";
}
