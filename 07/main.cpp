#include "avl_array.hpp"
#include <iostream>
#include <gtest/gtest.h>
#include <cstdlib> 

// Тест метода at
TEST(Lab7, test_at_method)
{
	avl_array<int, int, int, 10> avl;

	for(int i=0; i<100; i+=10) {
		avl.insert(i, i + 999);
		ASSERT_EQ(*avl.at(i), i + 999);
		ASSERT_EQ(avl.at(i).key(), i);
	}
}

// Тест прерывания при обращении к несуществующему элементу
TEST(Lab7, test_at_throw)
{
	avl_array<int, int, int, 10> avl;

	for(int i=0; i<100; i+=10) {
		avl.insert(i, i);
		ASSERT_THROW(avl.at(i+1).val(), std::out_of_range);
		ASSERT_THROW(avl.at(i+1).key(), std::out_of_range);
	}
}

// Дважды вставляем элемент, проверяем, что значение изменилось
TEST(Lab7, insert_twice)
{
	avl_array<int, int, int, 100> avl;

	for(int i=0; i<100; i++) {
		ASSERT_TRUE(avl.insert(i, i));
		ASSERT_EQ(avl[i], i);
		ASSERT_TRUE(avl.insert(i, i+1));
		ASSERT_EQ(avl[i], i+1);
	}
}

// Тест ошибки при попытке вставить элемент в заполненный массив
TEST(Lab7, insert_in_full_array)
{
	avl_array<int, int, int, 1> avl; // Ёмкость массива - 1 элемент

	avl.insert(10, 10);
	ASSERT_FALSE(avl.insert(20, 20));
}


// Удаление элементов в прямом и обратном направлении
TEST(Lab7, erase_valid)
{
	avl_array<int, int, int, 100> avl;

	for(int i=0; i<100; i++)
		avl.insert(i, i);
	for(int i=0; i<100; i++)
		ASSERT_TRUE(avl.erase(i));
	for(int i=0; i<100; i++)
		avl.insert(i, i);
	for(int i=99; i>=0; i--)
		ASSERT_TRUE(avl.erase(i));
}

// Тест ошибки при удалении несуществующего элемента
TEST(Lab7, erase_invalid)
{
	avl_array<int, int, int, 1> avl;

	avl.insert(10, 10);
	ASSERT_FALSE(avl.erase(20));
}

// Поиск узла по ключу
TEST(Lab7, find_valid)
{
	avl_array<int, int, int, 100> avl;

	for(int i=0; i<100; i++)
		avl.insert(i, i);
	for(int i=0; i<100; i++)
		ASSERT_EQ(*avl.find(i), i);
}

// Поиск несуществующего ключа
TEST(Lab7, find_invalid)
{
	avl_array<int, int, int, 1> avl;

	avl.insert(10, 10);
	ASSERT_NE(*avl.find(20), 20);
}

// Тест метода contains при наличии элемента
TEST(Lab7, contains_valid)
{
	avl_array<int, int, int, 100> avl;

	int a;
	for(int i=0; i<100; i++)
		avl.insert(i, i);
	for(int i=0; i<100; i++)
		ASSERT_TRUE(avl.contains(i, a));
}

// Тест метода contains при отсутствии элемента
TEST(Lab7, contains_invalid)
{
	avl_array<int, int, int, 1> avl;
	int a;

	avl.insert(10, 10);
	ASSERT_FALSE(avl.contains(20, a));
}

// Тест того, что массив пуст
TEST(Lab7, empty_valid)
{
	avl_array<int, int, int, 1> avl;

	ASSERT_TRUE(avl.empty());
}

// Тест того, что массив непуст
TEST(Lab7, empty_invalid)
{
	avl_array<int, int, int, 1> avl;

	avl.insert(10, 10);
	ASSERT_FALSE(avl.empty());
}

// Проверка степени заполнения массива в цикле
TEST(Lab7, test_size_changes)
{
	avl_array<int, int, int, 100> avl;

	for(int i=0; i<90; i++) {
		ASSERT_EQ(avl.size(), i);
		avl.insert(i, i);
		ASSERT_EQ(avl.size(), i+1);
	}
	ASSERT_EQ(avl.capacity(), 100);
}

// Тест очистки массива
TEST(Lab7, test_clear_method)
{
	avl_array<int, int, int, 100> avl;

	for(int i=0; i<1000; i+=10)
		avl.insert(i, i);
	ASSERT_EQ(avl.size(), 100);
	avl.clear();
	ASSERT_EQ(avl.size(), 0);
}

// Тест end при пустом массиве
TEST(Lab7, test_end_on_empty_data)
{
	avl_array<int, int, std::uint16_t, 2048> avl;
	avl_array<int, int, std::uint16_t, 2048>::iterator it = avl.begin();
	ASSERT_EQ(it , avl.end());
	ASSERT_EQ(it, avl.end());
}

// Тест присваивания и проверки значения через []
TEST(Lab7, test_brackets_usage)
{
	avl_array<int, int, int, 10> avl;

	for(int i=10; i<100; i+=10){
		avl[i] = i+1;		// Присваивание
		ASSERT_EQ(avl[i], i+1); // Получение значения
		ASSERT_EQ(*avl.at(i), i+1);
	}
}

// Самопорверка avl дерева
TEST(Lab7, test_data_integrity)
{
	avl_array<int, int, int, 10> avl;

	for(int i=0; i<10; i++){
		avl.insert(i, rand() % 1000);
	}
	ASSERT_TRUE(avl.check());
}

// Тест цикла begin() end()
TEST(Lab7, test_begin_end)
{
	avl_array<int, int, int, 12> avl;
	int x = 10;

	for(int i=10; avl.size() < 12; i+=10) {
		avl.insert(i, i);
	}
	for (auto it = avl.begin(); it != avl.end(); it++) {
		ASSERT_EQ(*it, x);
		x += 10;
	}
}

// Тест цикла rbegin() rend()
TEST(Lab7, test_rbegin_rend)
{
	avl_array<int, int, int, 12> avl;
	int x = 120;

	for(int i=10; avl.size() < 12; i+=10) {
		avl.insert(i, i);
	}
	for (auto it = avl.rbegin(); it != avl.rend(); it++) {
		ASSERT_EQ(*it, x);
		x -= 10;
	}
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
