#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "HashMap.h"

#define HASH_SIZE 50
#define NUM_OF_HUSH_ELEMENTS 25

typedef enum
{
	PASSED,
	FAILED
} Result;

/*----------------user defined functions------------------*/

void destroyer(void* _data)
{
	free((int*)_data);
}

int PrintFunc(const void* _key, void* _value, void* _dummy)
{
	printf("key: %d value: %d |", *((int*)_key), *((int*)_value));
	return 1;
}

size_t hashFunc(const void* _key)
{
	return *((size_t*)_key);
}

int EqualFunction(const void* _firstKey, const void* _secondKey)
{
	return (*((int*)_firstKey) == *((int*)_secondKey));
}
/*-------------helper functions-------------------*/

static void PrintTest(Result _result, char* _testName)
{
	switch(_result)
	{
		case PASSED:
			printf("PASSED %s\n", _testName);
			break;
			
		case FAILED:
			printf("FAILED %s\n", _testName);
			break;
	}
}


HashMap* buildHash(size_t _capacity)
{
	size_t i;
	HashMap* map;
	int* key;
	int* value;
	
	map = HashMapCreate(HASH_SIZE, hashFunc, EqualFunction);
	
	for(i = 0; i < _capacity; ++i)
	{
		key = malloc(sizeof(int));
		value = malloc(sizeof(int));
		
		*value = (int)i;
		*key = (int)i;
		
		HashMapInsert(map, key, value);
	}
	
	return map;
}
/*-----------------main test functions-----------------*/

void TestCreateHash()
{
	char testName[] = "create hash";
	HashMap* hash;
	hash = HashMapCreate(HASH_SIZE, hashFunc, EqualFunction);
	
	if(NULL != hash)
	{
		PrintTest(PASSED, testName);
	}
	else
	{
		PrintTest(FAILED, testName);
	}
	HashMapDestroy(&hash, destroyer, destroyer);
}

void TestCreateHashNull()
{
	char testName[] = "create with null function";
	HashMap* hash;
	hash = HashMapCreate(0, NULL, EqualFunction);
	
	if(NULL == hash)
	{
		PrintTest(PASSED, testName);
	}
	else
	{
		PrintTest(FAILED, testName);
	}
}

void TestInsert()
{
	char testName[] = "insert hash";
	HashMap* hash;
	Result result = PASSED;
	int* key;
	int* value;
	int i;
	hash = HashMapCreate(HASH_SIZE, hashFunc, EqualFunction);
	
	for(i = 0; i < HASH_SIZE/2; ++i)
	{
		key = malloc(sizeof(int));
		value = malloc(sizeof(int));
		
		*value = 1;
		*key = i;
		if(MAP_SUCCESS != HashMapInsert(hash, key, value))
		{
			result = FAILED;
			break;
		}
	}
	/*HashMapForEach(hash, PrintFunc, NULL);*/
	PrintTest(result, testName);

	HashMapDestroy(&hash, destroyer, destroyer);
}

void TestInsertNullHash()
{
	char testName[] = "insert null hash";
	int key = 1;
	int value = 1;
	
	if(MAP_UNINITIALIZED_ERROR == HashMapInsert(NULL, &key, &value))
	{
		PrintTest(PASSED, testName);
	}
	else
	{
		PrintTest(FAILED, testName);
	}
}

void TestInsertNullKey()
{
	char testName[] = "insert null key";
	HashMap* hash;
	int* key = NULL;
	int value = 1;
	
	hash = HashMapCreate(10, hashFunc, EqualFunction);
	if(MAP_KEY_NULL_ERROR == HashMapInsert(hash, key, &value))
	{
		PrintTest(PASSED, testName);
	}
	else
	{
		PrintTest(FAILED, testName);
	}
}

void TestInsertDuplicate()
{
	char testName[] = "insert hash duplicate key";
	HashMap* hash;
	int* key;
	int* value;
	hash = HashMapCreate(HASH_SIZE, hashFunc, EqualFunction);
	
	key = malloc(sizeof(int));
	*key = 1;
	value = malloc(sizeof(int));
	*value = 1;
	
	HashMapInsert(hash, key, value);
	
	if(MAP_KEY_DUPLICATE_ERROR == HashMapInsert(hash, key, value))
	{
		PrintTest(PASSED, testName);
	}
	else
	{
		PrintTest(FAILED, testName);
	}
	HashMapDestroy(&hash, destroyer, destroyer);
}

void TestFind()
{
	char testName[] = "find by key";
	HashMap* hash;
	int *value1, *value2;
	MapResult error1, error2;
	int keyExist = 1;
	int keyNotExist = 14;
	
	hash = buildHash(NUM_OF_HUSH_ELEMENTS);
	error1 = HashMapFind(hash, &keyExist, (void**)(&value1));
	error2 = HashMapFind(hash, &keyNotExist, (void**)(&value2));
	if(MAP_SUCCESS == error1 && MAP_KEY_NOT_FOUND_ERROR == error2 && *value1 == 1)
	{
		PrintTest(PASSED, testName);
	}
	else
	{
		PrintTest(FAILED, testName);
	}
	HashMapDestroy(&hash, destroyer, destroyer);
}

void TestFindNullKey()
{
	char testName[] = "find null key";
	HashMap* hash;
	int value;
	
	hash = buildHash(NUM_OF_HUSH_ELEMENTS);
	if(MAP_KEY_NULL_ERROR == HashMapFind(hash, NULL, (void**)(&value)))
	{
		PrintTest(PASSED, testName);
	}
	else
	{
		PrintTest(FAILED, testName);
	}
	HashMapDestroy(&hash, destroyer, destroyer);
}

void TestFindNullHash()
{
	char testName[] = "find null hash";
	HashMap* hash;
	int* value;
	int key = 5;
	
	hash = buildHash(NUM_OF_HUSH_ELEMENTS);
	if(MAP_UNINITIALIZED_ERROR == HashMapFind(NULL, &key, (void**)(&value)))
	{
		PrintTest(PASSED, testName);
	}
	else
	{
		PrintTest(FAILED, testName);
	}
	HashMapDestroy(&hash, destroyer, destroyer);
}

void TestRemove()
{
	char testName[] = "remove";
	int keys[] = {62,45,1,2,87,4,6,15,8,7,11,79,33,377,90363,357978};
	int *key, *value;
	Result result = PASSED;
	HashMap* hash;
	MapResult error;
	size_t i;
	
	hash = HashMapCreate(HASH_SIZE, hashFunc, EqualFunction);
	
	for(i = 0; i < sizeof(keys) / sizeof(int); ++i)
	{
		key = malloc(sizeof(int));
		value = malloc(sizeof(int));
		*key = keys[i];
		*value = keys[i];
		HashMapInsert(hash, key, value);
	}
	for(i = 0; i < sizeof(keys) / sizeof(int); ++i)
	{
		 error = HashMapRemove(hash, &(keys[i]), (void**)(&key), (void**)(&value));
		 if(MAP_SUCCESS != error || *key != keys[i])
		 {
		 	result = FAILED;
		 	break;
		 }
		 free(key);
		 free(value);
	}
	PrintTest(result, testName);
	HashMapDestroy(&hash, destroyer, destroyer);
}

void TestRemoveDuplicate()
{
	char testName[] = "remove duplicate";
	int keys[] = {62,45,1,2,87,4,6,15,8,7,11,79,33,377,90363,357978};
	int *key, *value;
	HashMap* hash;
	MapResult error;
	size_t i;
	
	hash = HashMapCreate(HASH_SIZE, hashFunc, EqualFunction);
	
	for(i = 0; i < sizeof(keys) / sizeof(int); ++i)
	{
		key = malloc(sizeof(int));
		value = malloc(sizeof(int));
		*key = keys[i];
		*value = keys[i];
		HashMapInsert(hash, key, value);
	}
	
	HashMapRemove(hash, &(keys[0]), (void**)(&key), (void**)(&value));
	free(key);
	free(value);
	
	error = HashMapRemove(hash, &(keys[0]), (void**)(&key), (void**)(&value));
	if(MAP_KEY_NOT_FOUND_ERROR == error)
	{
		PrintTest(PASSED, testName);
	}
	else
	{
		PrintTest(FAILED, testName);
	}
	HashMapDestroy(&hash, destroyer, destroyer);
}

void TestRemoveKeyNotFound()
{
	char testName[] = "remove key not found";
	int keys[] = {62,45,1,2,87,4,6,15,8,7,11,79,33,377,90363,357978};
	int otherKey = 3;
	int *key, *value;
	HashMap* hash;
	MapResult error;
	size_t i;
	
	hash = HashMapCreate(HASH_SIZE, hashFunc, EqualFunction);
	
	for(i = 0; i < sizeof(keys) / sizeof(int); ++i)
	{
		key = malloc(sizeof(int));
		value = malloc(sizeof(int));
		*key = keys[i];
		*value = keys[i];
		HashMapInsert(hash, key, value);
	}
	
	error = HashMapRemove(hash, &otherKey, (void**)(&key), (void**)(&value));
	if(MAP_KEY_NOT_FOUND_ERROR == error)
	{
		PrintTest(PASSED, testName);
	}
	else
	{
		PrintTest(FAILED, testName);
	}
	HashMapDestroy(&hash, destroyer, destroyer);
}

void TestRemoveNullHash()
{
	char testName[] = "remove null hash map";
	int otherKey = 3;
	int *key, *value;
	MapResult error;
	
	error = HashMapRemove(NULL, &otherKey, (void**)(&key), (void**)(&value));
	if(MAP_UNINITIALIZED_ERROR == error)
	{
		PrintTest(PASSED, testName);
	}
	else
	{
		PrintTest(FAILED, testName);
	}
}

void TestRehash()
{
	char testName[] = "rehash";
	int keys[] = {62,45,1,2,87,4,6,15,8,7,11,79,33,377,90363,357978};
	int *key, *value;
	Result result = PASSED;
	HashMap* hash;
	MapResult error;
	size_t i;
	
	hash = HashMapCreate(10, hashFunc, EqualFunction);
	
	for(i = 0; i < sizeof(keys) / sizeof(int); ++i)
	{
		key = malloc(sizeof(int));
		value = malloc(sizeof(int));
		*key = keys[i];
		*value = keys[i];
		HashMapInsert(hash, key, value);
	}
	
	HashMapRehash(hash, 20);
	
	for(i = 0; i < sizeof(keys) / sizeof(int); ++i)
	{
		 error = HashMapRemove(hash, &(keys[i]), (void**)(&key), (void**)(&value));
		 if(MAP_SUCCESS != error || *key != keys[i])
		 {
		 	result = FAILED;
		 	break;
		 }
		 free(key);
		 free(value);
	}
	PrintTest(result, testName);
	HashMapDestroy(&hash, destroyer, destroyer);
}

void TestRehashSmallerSize()
{
	char testName[] = "rehash smaller size";
	int keys[] = {62,45,1,2,87,4,6,15,8,7,11,79,33,377,90363,357978};
	int *key, *value;
	Result result = PASSED;
	HashMap* hash;
	MapResult error;
	size_t i;
	
	hash = HashMapCreate(10, hashFunc, EqualFunction);
	
	for(i = 0; i < sizeof(keys) / sizeof(int); ++i)
	{
		key = malloc(sizeof(int));
		value = malloc(sizeof(int));
		*key = keys[i];
		*value = keys[i];
		HashMapInsert(hash, key, value);
	}
	
	HashMapRehash(hash, 5);
	
	for(i = 0; i < sizeof(keys) / sizeof(int); ++i)
	{
		 error = HashMapRemove(hash, &(keys[i]), (void**)(&key), (void**)(&value));
		 if(MAP_SUCCESS != error || *key != keys[i])
		 {
		 	result = FAILED;
		 	break;
		 }
		 free(key);
		 free(value);
	}
	PrintTest(result, testName);
	HashMapDestroy(&hash, destroyer, destroyer);
}

void TestReahshNullHashMap()
{
	char testName[] = "rehash null hash map";
	if(MAP_UNINITIALIZED_ERROR == HashMapRehash(NULL, 5))
	{
		PrintTest(PASSED, testName);
	}
	else
	{
		PrintTest(FAILED, testName);
	}
}

void TestCountItems()
{
	char testName[] = "count items";
	HashMap* hash;
	
	hash = buildHash(NUM_OF_HUSH_ELEMENTS);
	if(NUM_OF_HUSH_ELEMENTS == HashMapSize(hash))
	{
		PrintTest(PASSED, testName);
	}
	else
	{
		PrintTest(FAILED, testName);
	}
	HashMapDestroy(&hash, destroyer, destroyer);
}

void TestCountItemsNull()
{
	char testName[] = "count items null hash map";
	
	if(0 == HashMapSize(NULL))
	{
		PrintTest(PASSED, testName);
	}
	else
	{
		PrintTest(FAILED, testName);
	}
}

#ifndef NDEBUG
void TestStats()
{
	char testName[] = "stats";
	int keys[] = {62,45,1,2,87,4,6,15,8,7,11,79,33,377,90363,357978};
	int *key, *value;
	HashMap* hash;
	size_t i;
	MapStats* stats;
	
	hash = HashMapCreate(10, hashFunc, EqualFunction);
	
	for(i = 0; i < sizeof(keys) / sizeof(int); ++i)
	{
		key = malloc(sizeof(int));
		value = malloc(sizeof(int));
		*key = keys[i];
		*value = keys[i];
		HashMapInsert(hash, key, value);
	}
	
	stats =  HashMapGetStatistics(hash);
	if(stats->m_pairs == 16 && stats->m_collisions == 7 && stats->m_buckets == 10 && stats->m_chains == 9 && 
	   3 == stats->m_maxChainLength)
	{
		PrintTest(PASSED, testName);
	}
	else
	{
		PrintTest(FAILED, testName);
	}
	HashMapDestroy(&hash, destroyer, destroyer);
	free(stats);
}

#endif /* NDEBUG */

int main()
{
	TestCreateHash();
	TestCreateHashNull();
	
	TestInsert();
	TestInsertNullHash();
	TestInsertNullKey();
	TestInsertDuplicate();
	
	TestFind();
	TestFindNullKey();
	TestFindNullHash();
	
	TestRemove();
	TestRemoveDuplicate();
	TestRemoveKeyNotFound();
	TestRemoveNullHash();
	
	TestRehash();
	TestRehashSmallerSize();
	TestReahshNullHashMap();
	
	TestCountItems();
	TestCountItemsNull();
	
	#ifndef NDEBUG
	TestStats();
	#endif /* NDEBUG */
	
	return 1;
}
