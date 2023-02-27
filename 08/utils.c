#include <Python.h>
#include <stdbool.h>

#define MAX_KEY 256	// Максимальная длина ключа
#define MAX_VAL 128	// Максимальная длина строки

#define MAX_KEYS 100000	// Максимальное число пар "ключ": "значение"
#define MAX_RET 5000000	// Максимальная длина возвращаемой строки dumps()

// Типы данных JSON
typedef enum JsonTokenType
{
	None = 0,
	Number,
	Float,
	String,
	Bool
} t_jtype;

// Хранение данных типа JSON
typedef struct JsonRecord
{
	char	key[MAX_KEY];
	t_jtype	type;
	char	str[MAX_VAL];
	long	num;
	bool	logic;
	double	numf;
} t_jrec;

// Подсчёт числа символа вне кавычек
size_t countSymbol(char c, char*str)
{
	size_t	n;
	bool	quote = false;

	for(size_t i = n = 0; str[i] != '\0'; ++i)
	{
		if(str[i] == '"')
		{
			quote = !quote;
		}
		else if(str[i] == c && !quote)
		{
			++n;
		}
	}
	return n;
}


// Разбираем строку и заносим данные в массив данных типа JSON
size_t parseJson(char *str, t_jrec *data, size_t nrec)
{
	// Читаем символ. Если кавычка - это ключ. Читам ключ до конца, ищем двоеточие - дальше значение.
	// Если первый символ значения кавычка - это строка
	size_t n = 0;
	size_t j;

	// Значение i изменяем внутри цикла, поэтому здесь это поле пустое
	for(size_t i = 0; str[i] != '\0'; )
	{
		if(str[i] == '"')
		{
			// В этом месте тип данных ещё не определён
			data[n].type = None;
			++i; // Пропускаем кавычку и начинаем читать ключ
			for(j = 0; str[i] != '"' && str[i] != '\0'; ++i)
			{
				data[n].key[j++] = str[i];
			}
			data[n].key[j++] = '\0';
			while(str[i] != ':' && str[i] != '\0') // Пропускаем симоволы до двоеточия
			{
				++i;
			}
			++i; // Пропускаем символ двоеточия и переходим к значению
			while(isspace(str[i]) && str[i] != '\0')
			{
				++i;
			}
			if(str[i] == '[' || str[i] == ']' || str[i] == '{' ||
				str[i] == '}' || str[i] == ',' || str[i] == '\0')
			{
				continue;
			}
			if(str[i] == '"')
			{ // Это строковое значение
				data[n].type = String;
				i++; // Пропускаем кавычку
				for(j = 0; str[i] != '"' && str[i] != '\0'; ++i)
				{
					data[n].str[j++] = str[i];
				}
				++i; // Пропускаем замыкающую кавычку
				data[n].str[j++] = '\0';
			}
			// Тип данных - Float
			else if(isdigit(str[i]) || str[i] == '-')
			{
				j = 1;
				while(isdigit(str[i+j]) && str[i] != '\0')
				{
					++j;
				}
				if(str[i+j] == '.')
				{
					data[n].type = Float;
					/*
					++j;
					while(isdigit(str[i+j]) && str[i] != '\0')
					{
						++j;
					}
					strncpy(data[n].str, str + i, j);
					*/
					sscanf(str + i, "%lf", &data[n].numf);
				}
				else
				{
					data[n].type = Number;
					data[n].num = atol(str+i*sizeof(char));
				}
				++i; // Пропустим первый символ, чтобы больше не проверять символ "минус"
				while((isdigit(str[i]) || str[i]=='.') && str[i] != '\0')
				{
					++i;
				}
			}
			// Тип данных - bool
			else if(strncmp(str+i, "true", 4) == 0 || strncmp(str+i, "false", 5) == 0)
			{
				data[n].type = Bool;
				data[n].logic = strncmp(str + i, "true", 4) == 0 ? true : false;
			}
			// Не число и не строка, пропускаем
			if(data[n].type != None)
			{
				n++;
			}
		}
		else
		{
			i++;
		}
	}
	return n; // Количество пар ключ + значение в массиве
}

// Читаем данные из строки и возврщаем в Python словарь
static PyObject* utils_loads(PyObject *self, PyObject *args)
{
	//PyObject* list_obj;
	char *command;
	static t_jrec D[MAX_KEYS]; // Массив для хранения словаря
	size_t nrec;	// Длина массива
	size_t recs;	// Число записей в массиве
	PyObject *ret = PyDict_New(); // Объект Python, который возвращает функция
	PyObject *o = NULL;

	if (!PyArg_ParseTuple(args, "s", &command))
	{
		printf("ERROR: Can't loads() data\n");
		return NULL;
	}
	nrec = countSymbol(':', command);	// Выясняем число ключей в строке
	recs = parseJson(command, D, nrec);	// Парсим строку
	for(size_t i = 0; i < recs; i++)	// Формируем возвращаемое значние
	{
		// Формируем ссылку на объект в зависимости от типа значения
		switch(D[i].type)
		{
			case Number:
				o = PyLong_FromLong(D[i].num);
				break;
			case Float: // Float храним в строке для точности
				//o = PyFloat_FromString(PyBytes_FromString(D[i].str));
				o = PyFloat_FromDouble(D[i].numf);
				break;
			case String:
				o = PyUnicode_FromString(D[i].str);
				break;
			case Bool:
				o = D[i].logic ? Py_True : Py_False;
				break;
			default: // Такого быть не должно, но перестрахуемся
				o = NULL;
		}
		PyDict_SetItem(ret, PyUnicode_FromString(D[i].key), o);
	}

	return ret;
}

// Получаем на вход объект типа dict
// конвертируем всё в строку
static PyObject* utils_dumps(PyObject *self, PyObject *args)
{
	PyObject *dict;
	Py_ssize_t len;		// Размер переданного словаря
	PyObject *key = NULL, *value = NULL;
	Py_ssize_t pos = 0;	// Индекс в словаре
	char ret[MAX_RET];	// Эту строку возвращает функция
	size_t retpos = 0;	// Индекс в возвращаемой строке
	size_t spos;		// Индекс в строке
	char *s;
	char buf[MAX_VAL];	// Буфер для строки
	bool is_str;		// Признак того, что значение надо выводить в кавычках
	PyObject *o;

	// Проверяем, что на вход поступил объект dict
	if (!PyArg_ParseTuple(args, "O", &dict))
	{
		printf("ERROR: Can't get data for dumps()\n");
		return NULL;
	}
	len = PyDict_Size(dict); // Получаем длину словаря
	// Начинаем строку с фигурной скобки
	ret[retpos++] = '{';
	while(PyDict_Next((PyObject *) dict, &pos, &key, &value))
	{
		ret[retpos++] = '"';
		s = PyBytes_AsString(PyUnicode_AsUTF8String(key));
		spos = 0;
		while(s[spos] != '\0')
		{
			ret[retpos++] = s[spos++];
		}
		ret[retpos++] = '"';
		ret[retpos++] = ':';
		ret[retpos++] = ' ';
		is_str = false; // Надо ли ставить кавычки вокруг значения выясняем в процессе
		if(PyLong_CheckExact(value))
		{
			sprintf(buf, "%ld", PyLong_AsLong(value));
			s = buf;
		}
		else if(PyFloat_CheckExact(value))
		{
			sprintf(buf, "%lf", PyFloat_AsDouble(value));
			s = buf;
		}
		else if(PyBool_Check(value))
		{
			if(PyObject_IsTrue(value))
			{
				s = "true";
			}
			else {
				s = "false";
			}
		}
		else {
			o = PyUnicode_AsUTF8String(value);
			if(o != NULL)
			{
				s = PyBytes_AsString(o);
			}
			is_str = true; // Значение типа строка, кавычки нужны
		}
		spos = 0;
		// Кавычки вокруг значения нужны только для строк
		if(is_str)
		{
			ret[retpos++] = '"';
		}
		while(s[spos] != '\0')
		{
			ret[retpos++] = s[spos++];
		}
		if(is_str)
		{
			ret[retpos++] = '"';
		}
		// После последней пары key:value не нужна запятая
		if(pos < len)
		{
			ret[retpos++]=',';
			ret[retpos++]=' ';
		}
	}
	// Завершаем строку фигурной скобкой
	ret[retpos++] = '}';
	ret[retpos++] = '\0';

	// Если при преобразования типов в Python программе будет возникать ошибка,
	// её можно подавить, вызвав функцию
	// PyErr_Clear();
	return Py_BuildValue("s", ret);
}

static PyMethodDef methods[] = {
	{"loads", utils_loads, METH_VARARGS, "Build dict data from json string"},
	{"dumps", utils_dumps, METH_VARARGS, "Build json string from dict data"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef utilsmodule = {
	PyModuleDef_HEAD_INIT, "utils_sum",
	NULL, -1, methods
};

PyMODINIT_FUNC PyInit_utils(void)
{
	return PyModule_Create(&utilsmodule);
}
