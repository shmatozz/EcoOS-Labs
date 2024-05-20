/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"

#include "IdEcoLab1.h"
#include "IdEcoLab1Stable.h"

#include "IdEcoList1.h"
#include "CEcoLab1Sink.h"
#include "IEcoConnectionPointContainer.h"
#include "IEcoLab1Events.h"

#include <stdio.h>
#include <stdlib.h>

// X
#include "IEcoCalculatorX.h"
// Y 
#include "IEcoCalculatorY.h"
//A B D E
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"

int __cdecl compInts(const void *a_ptr, const void *b_ptr) {
    const int a = *(int *)a_ptr;
    const int b = *(int *)b_ptr;

    if (a > b)       return  1;
    else if (a == b) return  0;
    else             return -1;
}


int __cdecl compChars(const void *a_ptr, const void *b_ptr) {
    char a = *(char *)a_ptr;
    char b = *(char *)b_ptr;

    if (a > b)       return  1;
    else if (a == b) return  0;
    else             return -1;
}


int getInt(const char *src) {
    return *(int *) src;
}


int getChar(const char *src) {
    return *(char *) src;
}

void printIntArray(void *arr, size_t size) {
    int i;
    int *array = (int *) arr;

    for (i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void printCharArray(void *arr, size_t size) {
    int i;
    char *array = (char *) arr;

    for (i = 0; i < size; ++i) {
        printf("%c ", array[i]);
    }
    printf("\n");
}

void *createIntArray(IEcoMemoryAllocator1 *pIMem, size_t size) {
    size_t i;
    int *arr = (int *) pIMem->pVTbl->Alloc(pIMem, size * sizeof(int));

    arr[0] = (rand() % 30000);
    for (i = 1; i < size; i++) {
        arr[i] = (rand() % 30000);
    }

    return arr;
}

void *createBigIntArray(IEcoMemoryAllocator1 *pIMem, size_t size) {
    size_t i;
    int *arr = (int *) pIMem->pVTbl->Alloc(pIMem, size * sizeof(int));

    arr[0] = (rand() % 10000) * (rand() %  3000);
    for (i = 1; i < size; i++) {
        arr[i] = (rand() % 10000) * (rand() % 3000);
    }

    return arr;
}

void *createCharArray(IEcoMemoryAllocator1 *pIMem, size_t size) {
    size_t i;
    char *arr = (char *) pIMem->pVTbl->Alloc(pIMem, size * sizeof(char));

    arr[0] = rand() % 26 + 'a';
    for (i = 1; i < size; i++) {
        arr[i] = rand() % 26 + 'a';
    }

    return arr;
}

void deleteArray(IEcoMemoryAllocator1 *pIMem, void *arr_ptr, size_t size) {
    pIMem->pVTbl->Free(pIMem, arr_ptr);
}

void *createCopyArray(IEcoMemoryAllocator1 *pIMem, void *src, uint32_t byte_count) {
    void *copy_array = pIMem->pVTbl->Alloc(pIMem, byte_count);
    pIMem->pVTbl->Copy(pIMem, copy_array, src, byte_count);
    return copy_array;
}

void testSorting(
        IEcoMemoryAllocator1 *pIMem,
        IEcoLab1 *ecoLab1,
        IEcoLab1 *ecoLab1Stable,
        FILE *results,
        size_t elem_size,
        void *createArray(IEcoMemoryAllocator1 *, size_t),
        void printArray(void *arr, size_t),
        int __cdecl comp(const void*, const void*),
        int get(const char *)
) {
    void* arr;
    clock_t start, end;
    double elapsed_time;
    size_t size;

    printf("Counting Sort\n");
    //fprintf(results, "Counting Sort\n");
    for (size = 65536; size <= 8388608; size *= 2) {
        arr = createArray(pIMem, size);

        start = clock();
        ecoLab1->pVTbl->qsort(ecoLab1, arr, size, elem_size, comp, get);
        end = clock();
        elapsed_time = (double) (end - start) / CLOCKS_PER_SEC;
        printf("size: %7d, time: %lf\n", size, elapsed_time);
        //fprintf(results, "%d,%lf\n", size, elapsed_time);

        deleteArray(pIMem, arr, size);
    }
    printf("\n");
    //fprintf(results, "\n");

    printf("Stable Counting Sort\n");
    //fprintf(results, "Stable Counting Sort\n");
    for (size = 65536; size <= 8388608; size *= 2) {
        arr = createArray(pIMem, size);

        start = clock();
        ecoLab1Stable->pVTbl->qsort(ecoLab1, arr, size, elem_size, comp, get);
        end = clock();
        elapsed_time = (double) (end - start) / CLOCKS_PER_SEC;
        printf("size: %7d, time: %lf\n", size, elapsed_time);
        //fprintf(results, "%d,%lf\n", size, elapsed_time);

        deleteArray(pIMem, arr, size);
    }
    printf("\n");
    //fprintf(results, "\n");

    printf("Standard QSort\n");
    //fprintf(results, "Standard QSort\n");
    for (size = 65536; size <= 8388608; size *= 2) {
        arr = createArray(pIMem, size);

        start = clock();
        qsort(arr, size, elem_size, comp);
        end = clock();
        elapsed_time = (double) (end - start) / CLOCKS_PER_SEC;
        printf("size: %7d, time: %lf\n", size, elapsed_time);
        //fprintf(results, "%d,%lf\n", size, elapsed_time);

        deleteArray(pIMem, arr, size);
    }
    printf("\n");
    //fprintf(results, "\n");
}

void testLab2(
		IEcoMemoryAllocator1 *pIMem,
		IEcoLab1 *pIEcoLab1,
        IEcoLab1 *pIEcoLab1Stable,
		int output) {
	IEcoCalculatorX* pIX = 0;
	IEcoCalculatorY* pIY = 0;
	int* arr;
	int16_t result;
	int16_t total_result = 0;

	result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void **) &pIX);
	total_result += result;
	if (result == 0 && output) {
		printf("X from EcoLab1:\n");
		printf("  %d + %d = %d\n", 100, 1111, pIX->pVTbl->Addition(pIX, 100, 1111));
		printf("  %d - %d = %d\n\n", 2111, 111, pIX->pVTbl->Subtraction(pIX, 2111, 111));
	}

    result = pIX->pVTbl->QueryInterface(pIX, &IID_IEcoCalculatorY, (void **) &pIY);
	total_result += result;
	if (result == 0 && output) {
		printf("Y from X (from EcoLab1)\n");
		printf("  %d * %d = %d\n", 2, 3, pIY->pVTbl->Multiplication(pIY, 2, 3));
		printf("  %d / %d = %d\n\n", 9, 3, pIY->pVTbl->Division(pIY, 9, 3));
	}

	result = pIEcoLab1Stable->pVTbl->QueryInterface(pIEcoLab1Stable, &IID_IEcoCalculatorX, (void **) &pIX);
	total_result += result;
	if (result == 0 && output) {
		printf("X from EcoLab1Stable\n");
		printf("  %d + %d = %d\n", 6, 2, pIX->pVTbl->Addition(pIX, 6, 2));
		printf("  %d - %d = %d\n\n", 11, 55, pIX->pVTbl->Subtraction(pIX, 11, 55));
	}

    result = pIX->pVTbl->QueryInterface(pIX, &IID_IEcoCalculatorY, (void **) &pIY);
	total_result += result;
	if (result == 0 && output) {
		printf("Y from X (from EcoLab1Stable)\n");
		printf("  %d * %d = %d\n", 15, 3, pIY->pVTbl->Multiplication(pIY, 15, 3));
		printf("  %d / %d = %d\n\n", 100, 10, pIY->pVTbl->Division(pIY, 100, 10));
	}

	result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void **) &pIY);
	total_result += result;
	if (result == 0 && output) {
		printf("Y from EcoLab1\n");
		printf("  %d * %d = %d\n", 11, 11, pIY->pVTbl->Multiplication(pIY, 11, 11));
		printf("  %d / %d = %d\n\n", 123, 123, pIY->pVTbl->Division(pIY, 123, 123));
	}

    result = pIY->pVTbl->QueryInterface(pIY, &IID_IEcoCalculatorX, (void **) &pIX);
	total_result += result;
	if (result == 0 && output) {
		printf("X from Y (from EcoLab1)\n");
		printf("  %d + %d = %d\n", 1, 8, pIX->pVTbl->Addition(pIX, 1, 8));
		printf("  %d - %d = %d\n\n", 14, 3, pIX->pVTbl->Subtraction(pIX, 14, 3));
	}

    result = pIEcoLab1Stable->pVTbl->QueryInterface(pIEcoLab1Stable, &IID_IEcoCalculatorY, (void **) &pIY);
	total_result += result;
	if (result == 0 && output) {
		printf("Y from EcoLab1Stable\n");
		printf("  %d * %d = %d\n", 100, 5, pIY->pVTbl->Multiplication(pIY, 100, 5));
		printf("  %d / %d = %d\n\n", 500, 4, pIY->pVTbl->Division(pIY, 500, 4));
	}

    result = pIY->pVTbl->QueryInterface(pIY, &IID_IEcoCalculatorX, (void **) &pIX);
	total_result += result;
	if (result == 0 && output) {
		printf("X from Y (from EcoLab1Stable)\n");
		printf("  %d + %d = %d\n", -2, 2, pIX->pVTbl->Addition(pIX, -2, 2));
		printf("  %d - %d = %d\n\n", 10, 111, pIX->pVTbl->Subtraction(pIX, 10, 111));
	}

	if (total_result == 0) {
		printf("Lab2 check: OK\n");
	} else {
		printf("Lab2 check: ERROR\n");
	}
}

// Функция EcoMain - точка входа
int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    /* Указатель на системный интерфейс */
    IEcoSystem1* pISys = 0;
    /* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoInterfaceBus1* pIBus = 0;
    /* Указатель на интерфейс работы с памятью */
    IEcoMemoryAllocator1* pIMem = 0;
    /* Указатель на тестируемый интерфейс */
    IEcoLab1* pIEcoLab1 = 0;
    IEcoLab1 *pIEcoLab1Stable = 0;
	/* Указатель на интерфейс контейнера точек подключения */
    IEcoConnectionPointContainer* pICPC = 0;
    /* Указатель на интерфейс точки подключения */
    IEcoConnectionPoint* pICP = 0;
	/* Указатель на обратный интерфейс */
    IEcoLab1Events* pIEcoLab1Sink = 0;
    IEcoUnknown* pISinkUnk = 0;
    uint32_t cAdvise = 0;
	// Вспомогательные переменные
	int* arr;
	int i;
	int arr2[10] = {2, 3, 1, 2, 10, 9, 6, 2, 7, 7};
	int arr3[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    //FILE *results;

    /* Проверка и создание системного интрефейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem1, (void **)&pISys);
        if (result != 0 && pISys == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
            goto Release;
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoList1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_53884AFC93C448ECAA929C8D3A562281);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#endif
    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 || pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        goto Release;
    }

    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }

    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1Stable, 0, &IID_IEcoLab1, (void**) &pIEcoLab1Stable);
    if (result != 0 || pIEcoLab1 == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }

	/* Проверка поддержки подключений обратного интерфейса */
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoConnectionPointContainer, (void **)&pICPC);
    if (result != 0 || pICPC == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }

	/* Запрос на получения интерфейса точки подключения */
    result = pICPC->pVTbl->FindConnectionPoint(pICPC, &IID_IEcoLab1Events, &pICP);
    if (result != 0 || pICP == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }
    /* Освобождение интерфейса */
    pICPC->pVTbl->Release(pICPC);

    /* Создание экземпляра обратного интерфейса */
    result = createCEcoLab1Sink(pIMem, (IEcoLab1Events**)&pIEcoLab1Sink);

    if (pIEcoLab1Sink != 0) {
        result = pIEcoLab1Sink->pVTbl->QueryInterface(pIEcoLab1Sink, &IID_IEcoUnknown,(void **)&pISinkUnk);
        if (result != 0 || pISinkUnk == 0) {
            /* Освобождение интерфейсов в случае ошибки */
            goto Release;
        }
        /* Подключение */
        result = pICP->pVTbl->Advise(pICP, pISinkUnk, &cAdvise);
        /* Проверка */
        if (result == 0 && cAdvise == 1) {
            /* Сюда можно добавить код */
        }
        /* Освобождение интерфейса */
        pISinkUnk->pVTbl->Release(pISinkUnk);
    }

    srand(time(0));

    //fopen_s(&results, "results.csv", "w");
	printf("Test #1: Random generated array.\n");
	arr = createIntArray(pIMem, 10);
	printf("Initial array:\n");
	printf("  "); printIntArray(arr, 10);
	pIEcoLab1->pVTbl->qsort(pIEcoLab1, arr, 10, sizeof(int), compInts, getInt);
	printf("Sorted array:\n");
	printf("  "); printIntArray(arr, 10); printf("\n");
	deleteArray(pIMem, arr, 10);
	
	printf("Test #2: Array with dublicates.\n");
	printf("Initial array:\n");
	printf("  "); printIntArray(&arr2, 10);
	pIEcoLab1->pVTbl->qsort(pIEcoLab1, &arr2, 10, sizeof(int), compInts, getInt);
	printf("Sorted array:\n");
	printf("  "); printIntArray(&arr2, 10); printf("\n");
	
	printf("Test #3: Array of one number.\n");
	printf("Initial array:\n");
	printf("  "); printIntArray(&arr3, 10);
	pIEcoLab1->pVTbl->qsort(pIEcoLab1, &arr3, 10, sizeof(int), compInts, getInt);
	printf("Sorted array:\n");
	printf("  "); printIntArray(&arr3, 10); printf("\n");

	testLab2(pIMem, pIEcoLab1, pIEcoLab1Stable, 0); // 0 - check without print, 1 - check with print 

	getchar();
    //fclose(results);

	if (pIEcoLab1Sink != 0) {
        /* Отключение */
        result = pICP->pVTbl->Unadvise(pICP, cAdvise);
        pIEcoLab1Sink->pVTbl->Release(pIEcoLab1Sink);
        pICP->pVTbl->Release(pICP);
    }

Release:

    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Освобождение интерфейса работы с памятью */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Освобождение тестируемого интерфейса */
    if (pIEcoLab1 != 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
    }

    /* Освобождение тестируемого интерфейса */
    if (pIEcoLab1Stable != 0) {
        pIEcoLab1Stable->pVTbl->Release(pIEcoLab1Stable);
    }


    /* Освобождение системного интерфейса */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}

