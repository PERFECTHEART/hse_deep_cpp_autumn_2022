#include <Python.h>
#include <stdbool.h>

#define MAX_KEY 512
#define MAX_VAL 512

#define MAX_KEYS 100000
#define MAX_RET 5000000

typedef enum JsonTokenType
{
	None = 0,
	Number,
	String
} t_jtype;

typedef struct JsonRecord
{
	char	key[MAX_KEY];
	t_jtype	type;
	char	str[MAX_VAL];
	size_t	num;
} t_jrec;

// Don't count symbol inside quotes
size_t countSymbol(char c, char*str)
{
	size_t	n;
	bool	quote = false;

	for(size_t i = n = 0; str[i] != '\0'; i++)
	{
		if(str[i] == '"')
			quote = !quote;
		else if(str[i] == c && !quote)
			++n;
	}
	return n;
}

size_t parseJson(char *str, t_jrec *data, size_t nrec)
{
	// Читаем символ. Если кавычка - это ключ. Читам ключ до конца, ищем двоеточие - дальше значение.
	// Если первый символ значения кавычка - это строка, иначе - число
	size_t n = 0;
	size_t j;

	for(size_t i = 0; str[i] != '\0'; ) {
		if(str[i] == '"') {
			++i; // Пропускаем кавычку и начинаем читать ключ
			for(j = 0; str[i] != '"'; ++i)
				data[n].key[j++] = str[i];
			data[n].key[j++] = '\0';
			while(str[i] != ':') // Пропускаем симоволы до двоеточия
				i++;
			i++; // Пропускаем символ двоеточия и переходим к значению
			while(isspace(str[i]))
				i++;
			if(str[i] == '[' || str[i] == ']' || str[i] == '{' || str[i] == '}' || str[i] == ',') {
				data[n].type = None;
				i++;
				continue;
			}
			if(str[i] == '"') { // Это строковое значение
				data[n].type = String;
				i++; // Пропускаем кавычку
				for(j = 0; str[i] != '"'; ++i)
					data[n].str[j++] = str[i];
				i++; // Пропускаем замыкающую кавычку
				data[n].str[j++] = '\0';
			}
			else if(isdigit(str[i])) {
				data[n].type = Number;
				data[n].num = atoi(str+i*sizeof(char));
				while(isdigit(str[i]))
					i++;
			}
			// Не число и не строка, пропускаем
			if(data[n].type == Number || data[n].type == String) {
				n++;
				data[n].type = None;
				data[n].num = 0;
				data[n].str[0]='\0';
			}
			else
				data[n].type = None; // Тип данных не поддерживается
		}
		else
			i++;
	}
	return n; // Количество пар ключ + значение в массиве
}

static PyObject* utils_loads(PyObject *self, PyObject *args)
{
	//PyObject* list_obj;
	char *command;
	static t_jrec D[MAX_KEYS];
	size_t nrec;	// Длина массива
	size_t recs;	// Число записей в массиве
	PyObject *ret = PyDict_New();
	PyObject *o = NULL;

	if (!PyArg_ParseTuple(args, "s", &command)) {
		printf("ERROR: Can't loads() data\n");
		return NULL;
	}
	nrec = countSymbol(':', command);
	recs = parseJson(command, D, nrec);
	for(size_t i = 0; i < recs; i++) {
		if(D[i].type == Number)
			o = PyLong_FromSize_t(D[i].num);
		else if(D[i].type == String)
			o = PyUnicode_FromString(D[i].str);
		else
			o = NULL;
		PyDict_SetItem(ret, PyUnicode_FromString(D[i].key), o);
		Py_DECREF(PyUnicode_FromString(D[i].key));
		Py_DECREF(o);
	}

	return ret;
}

static PyObject* utils_dumps(PyObject *self, PyObject *args)
{
	PyObject *dict;
	Py_ssize_t len; // Размер переданного словаря
	PyObject *key = NULL, *value = NULL;
	Py_ssize_t pos = 0;
	char ret[MAX_RET];
	size_t retpos = 0;
	size_t spos;
	char *s;
	char buf[256];
	bool is_num = false;
	PyObject *o;

	if (!PyArg_ParseTuple(args, "O", &dict)) {
		printf("ERROR: Can't get data for dumps()\n");
		return NULL;
	}
	len = PyDict_Size(dict);
	ret[retpos++]='{';
	while (PyDict_Next((PyObject *)dict, &pos, &key, &value)) {
//		Py_INCREF(key);
//        	Py_INCREF(value);
		ret[retpos++]='"';
		s = PyBytes_AsString(PyUnicode_AsUTF8String(key));
		spos = 0;
		while(s[spos]!='\0')
			ret[retpos++] = s[spos++];
		ret[retpos++]='"';
		ret[retpos++]=':';
		ret[retpos++]=' ';
		o = PyObject_Str(value);
		if( o != NULL )
			o = PyUnicode_AsUTF8String(value);
		if(o == NULL) {
			is_num = true;
			sprintf(buf, "%ld", PyLong_AsLong(value));
			s = buf;
		}
		else {
			is_num = false;
			s = PyBytes_AsString(o);
		}
		spos = 0;
		// Кавычки вокруг чисел не нужны
		if(!is_num)
			ret[retpos++]='"';
		while(s[spos]!='\0')
			ret[retpos++] = s[spos++];
		if(!is_num)
			ret[retpos++]='"';
		// После последней пары key:value не нужна запятая
		if(pos < len) {
			ret[retpos++]=',';
			ret[retpos++]=' ';
		}
//		Py_CLEAR(key);
//        	Py_CLEAR(value);
        }
	ret[retpos++] = '}';
	ret[retpos++] = '\0';

	//printf("%s\n",ret);
	// При работе с числами на этапе преобразования типов возникает ошибка, от которой
	// Python падает по прерыванию. Снимаем флаг ошибки и работаем дальше.
	PyErr_Clear();
	return Py_BuildValue("s", ret);
}

static PyMethodDef methods[] = {
	{ "loads", utils_loads, METH_VARARGS, "build dict data from json string"},
	{ "dumps", utils_dumps, METH_VARARGS, "build json string from dict data"},
	{ NULL, NULL, 0, NULL}
};

static struct PyModuleDef utilsmodule = {
	PyModuleDef_HEAD_INIT, "utils_sum",
	NULL, -1, methods
};

PyMODINIT_FUNC PyInit_utils(void)
{
	return PyModule_Create(&utilsmodule);
}
