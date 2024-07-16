# nos
Printing, tracing and loging library.

# Printing

## Основные функции:
Функция ```print```. ```print``` может объекты различных типов:
```c++
nos::print("Hello");
nos::print(42);
nos::print(0.45);
//output: Hello420.45
```

При указании нескольких аргументов - выводит их через пробел.
```c++
nos::print("Hello", 42); //output: Hello 42
```

Функция ```println```. Поведение аналогично ```print```. В конце выводится "\r\n":
```c++
nos::println("Hello", 42); //output: Hello 42\r\n
```

## Переопределение вывода типов.
Существует множество способов подружить nos с кастомными типами.

1. Перегрузка оператора вывода std::ostream.  
nos умеет выводить на печать стандартные типы. Форматирование ввода-вывода также поддерживается.

```c++
struct A 
{
	int a = 42;
	friend std::ostream& operator << ( std::ostream& os, const A&);
};

std::ostream& operator << ( std::ostream& os, const A& a) 
{
	os << std::hex << a.a << "A!!!"; 
}

...

nos::println(A()); //2aA!!!
```

2. Перегрузка функции ```int nos_print ( nos::ostream& os, const A& a )```.
```c++
struct A 
{
	int a = 42;
};

int nos_print ( nos::ostream& os, const A& a ) 
{
	int ret = 0;
	ret += os.print("A(");
	ret += os.print(a.a); 
	ret += os.print(")");
	return ret;
}

...

nos::println(A()); //A(42)
```

2. Определение метода ```int print_to (nos::ostream& os)``` const для класса.
```c++
struct A 
{
	int a = 42;

	int print_to (nos::ostream& os) const 
	{
		int ret = 0;
		ret += os.print("A(");
		ret += os.print(a.a); 
		ret += os.print(")");
		return ret;
	}
};

...

nos::println(A()); //A(42)
```

3. Специализация структуры ```nos::print_implementation``` для типа.
```c++
struct A 
{
	int a = 42;
};

namespace nos 
{
	template <> struct print_implementation<A>
	{
		static int print_to ( nos::ostream& os, const A& a ) 
		{
			int ret = 0;
			ret += os.print("A(");
			ret += os.print(a.a); 
			ret += os.print(")");
			return ret;
		}
	};
}

...

nos::println(A()); //A(42)
```

# Потоки ввода-вывода.

# Форматированный вывод

## Основные функции:

Функция ```fprint(const char * fmt, const Args& ... args)```.
Функция осуществляет форматированный вывод:

```c++
nos::fprint("How are you? {}. How old are you? {}.", "I'm fine", 42);
//output: How are you? I'm fine. How old are you? 42.
```

Использование нумерации аргументов:
```c++
nos::fprint("{0} + {1} = {1} + {0}", "a", "b");
//output: a + b = b + a
```

Поддерживаются именованные аргументы.
```c++
nos::fprint("{a} + {b} = {1} + {0}", "a"_a="42", "b"_a="24");
//output: 42 + 24 = 24 + 42
```

Опции форматирования:
```c++
nos::fprint("{>8f_}", "Foo");
//output: _____Foo
```

# Трассировка 

# Логирование
