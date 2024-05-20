#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoLab1Stable.h"

// A B D E
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"


// Функция QueryInterface для интерфейса IEcoLab1
int16_t ECOCALLMETHOD CEcoLab1Stable_QueryInterface(/* in */ struct IEcoLab1* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1Stable * pCMe = (CEcoLab1Stable*)me;
    if (me == 0 || ppv == 0) {
        return -1;
    }
    if ( IsEqualUGUID(riid, &IID_IEcoLab1) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
	else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorX) ) {
		if (pCMe->m_pInnerUnknown != 0) {
			// Запрашиваем интерфейс IEcoCalculatorX внутреннего компонента
			return pCMe->m_pInnerUnknown->pVTbl->QueryInterface(pCMe->m_pInnerUnknown, riid, ppv);
		}
		else {
			*ppv = &pCMe->m_pVTblIX;
		}
		pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
	}
	else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorY) ) {
		*ppv = &pCMe->m_pVTblIY;
		pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else {
        *ppv = 0;
        return -1;
    }
    return 0;
}

// Функция AddRef для интерфейса IEcoLab1
uint32_t ECOCALLMETHOD CEcoLab1Stable_AddRef(/* in */ struct IEcoLab1* me) {
    CEcoLab1Stable* pCMe = (CEcoLab1Stable*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    return ++pCMe->m_cRef;
}

// Функция Release для интерфейса IEcoLab1
uint32_t ECOCALLMETHOD CEcoLab1Stable_Release(/* in */ struct IEcoLab1* me) {
    CEcoLab1Stable* pCMe = (CEcoLab1Stable*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

	/* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        if ( pCMe->m_pInnerUnknown != 0 ) {
            // Предотвращение рекурсивного вызова
            if (pCMe->m_pInnerUnknown->pVTbl->Release(pCMe->m_pInnerUnknown) == 0) {
                pCMe->m_pInnerUnknown = 0;
            }
            else {
                pCMe->m_cRef = 1;
            }
        }
        if ( pCMe->m_cRef == 0 ) {
            deleteCEcoLab1Stable((IEcoLab1*)pCMe);
        }
        return 0;
    }
    return pCMe->m_cRef;
}

void copy_bytes(char *src, char *dst, size_t count) {
    char *end = src + count;
    while (src < end) {
        *(dst++) = *(src++);
    }
}

void inc(char *p) {
    int *pp = (int *) p;
    ++*pp;
}

void counting_sort(
    int *count_buffer,
    char *array,
    char *temp_array,
    char *temp,
    size_t elem_count,
    size_t elem_size,
    const char* max,
    int __cdecl comp(const void *, const void *),
    int get(const char *)
) {
    int i;

    for (i = 0; i < elem_count; ++i) {
        ++count_buffer[get(array + i * elem_size)];
    }

    for (i = 1; i <= get(max); i++) {
        count_buffer[i] += count_buffer[i - 1];
    }

    for (i = (int) elem_count - 1; i >= 0; i--) {
        copy_bytes(array + i * elem_size, temp_array + (count_buffer[get(array + i * elem_size)] - 1) * elem_size, elem_size);
        count_buffer[get(array + i * elem_size)]--;
    }

    copy_bytes(temp_array, array, elem_size * elem_count);
}

// Функция сортировка слиянием, использует сигнатуру qsort.
// Выделяет буфер памяти через интерфейс IEcoMemoryAllocator1
int16_t ECOCALLMETHOD CEcoLab1Stable_qsort(
	struct IEcoLab1* me,
	void* pData,
	size_t elem_count,
    size_t elem_size,
    int __cdecl comp(const void *, const void *),
    int get(const char *)
) {
    CEcoLab1Stable* pCMe = (CEcoLab1Stable*)me;
    int16_t index;
    /* Выделение памяти под необходимые элементы */
    int* count_buffer;
    char* temp = pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, sizeof(int));
    char* max = pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, sizeof(int));
    size_t i;
    char *array = (char *) pData;
    char *temp_array = pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, elem_count * elem_size);

    /* Проверка указателей */
    if (me == 0 || pData == 0) {
        return -1;
    }

    for (i = 0; i < elem_count; ++i) {
        if (comp(array + (i * elem_size), max) == 1) {
            copy_bytes(array + (i * elem_size), max, elem_size);
        }
    }

    count_buffer = pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, (get(max) + 1) * sizeof(int));

    /* Запуск сортировки */
    counting_sort(count_buffer, pData, temp_array, temp, elem_count, elem_size, max, comp, get);

    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, count_buffer);
    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, temp_array);
    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, temp);
    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, max);
    return 0;
}

// Функция QueryInterface для интерфейса IEcoCalculatorX
int16_t ECOCALLMETHOD CEcoLab1_IX_QueryInterface(/* in */ struct IEcoCalculatorX* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1Stable* pCMe = (CEcoLab1Stable*)((uint64_t)me - sizeof(struct IEcoLab1*));
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return result;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoLab1) ) {
		*ppv = &pCMe->m_pVTblIEcoLab1;
		pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
	else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorX) ) {
        if (pCMe->m_pInnerUnknown != 0) {
            // Запрашиваем интерфейс IEcoCalculatorX внутреннего компонента
            return pCMe->m_pInnerUnknown->pVTbl->QueryInterface(pCMe->m_pInnerUnknown, riid, ppv);
        }
        else {
			*ppv = &pCMe->m_pVTblIX;
		}
		pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorY) ) {
        *ppv = &pCMe->m_pVTblIY;
		pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        if (pCMe->m_pInnerUnknown != 0) {
            // Запрашиваем интерфейс IEcoCalculatorX внутреннего компонента
            return pCMe->m_pInnerUnknown->pVTbl->QueryInterface(pCMe->m_pInnerUnknown, riid, ppv);
        }
        else {
			*ppv = &pCMe->m_pVTblIX;
		}
		pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else {
        *ppv = 0;
        return -1;
    }
    return 0;
}

// Функция AddRef для интерфейса IEcoCalculatorX
uint32_t ECOCALLMETHOD CEcoLab1_IX_AddRef(/* in */ struct IEcoCalculatorX* me) {
    CEcoLab1Stable* pCMe = (CEcoLab1Stable*)((uint64_t)me - sizeof(struct IEcoLab1*));

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    return ++pCMe->m_cRef;
}

// Функция Release для интерфейса IEcoCalculatorX
uint32_t ECOCALLMETHOD CEcoLab1_IX_Release(/* in */ struct IEcoCalculatorX* me) {
    CEcoLab1Stable* pCMe = (CEcoLab1Stable*)((uint64_t)me - sizeof(struct IEcoLab1*));

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

	/* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        if ( pCMe->m_pInnerUnknown != 0 ) {
            // Предотвращение рекурсивного вызова
            if (pCMe->m_pInnerUnknown->pVTbl->Release(pCMe->m_pInnerUnknown) == 0) {
                pCMe->m_pInnerUnknown = 0;
            }
            else {
                pCMe->m_cRef = 1;
            }
        }
        if ( pCMe->m_cRef == 0 ) {
            deleteCEcoLab1Stable((IEcoLab1*)pCMe);
        }
        return 0;
    }
    return pCMe->m_cRef;
}

// Функция Addition
int32_t ECOCALLMETHOD CEcoLab1_IX_Addition(/* in */ struct IEcoCalculatorX* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1Stable* pCMe = (CEcoLab1Stable*)((uint64_t)me - sizeof(struct IEcoLab1*));
    int32_t result = 0;

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

	if (pCMe->m_pIX != 0) {
		result = pCMe->m_pIX->pVTbl->Addition(pCMe->m_pIX, a, b);
	}

    return result;
}

// Функция Subtraction
int16_t ECOCALLMETHOD CEcoLab1_IX_Subtraction(/* in */ struct IEcoCalculatorX* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1Stable* pCMe = (CEcoLab1Stable*)((uint64_t)me - sizeof(struct IEcoLab1*));
    int16_t result = 0;

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

    if (pCMe->m_pIX != 0) {
		result = pCMe->m_pIX->pVTbl->Subtraction(pCMe->m_pIX, a, b);
	}

    return result;
}


// Функция QueryInterface для интерфейса IEcoCalculatorY
int16_t ECOCALLMETHOD CEcoLab1_IY_QueryInterface(/* in */ struct IEcoCalculatorY* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1Stable* pCMe = (CEcoLab1Stable*)((uint64_t)me - sizeof(struct IEcoCalculatorX*) - sizeof(struct IEcoLab1*));
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return result;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoLab1) ) {
		*ppv = &pCMe->m_pVTblIEcoLab1;
		pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
	else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorX) ) {
        if (pCMe->m_pInnerUnknown != 0) {
            // Запрашиваем интерфейс IEcoCalculatorX внутреннего компонента
            return pCMe->m_pInnerUnknown->pVTbl->QueryInterface(pCMe->m_pInnerUnknown, riid, ppv);
        }
        else {
			*ppv = &pCMe->m_pVTblIX;
		}
		pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorY) ) {
        *ppv = &pCMe->m_pVTblIY;
		pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIY;
		pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else {
        *ppv = 0;
        return -1;
    }
    return 0;
}

// Функция AddRef для интерфейса IEcoCalculatorY
uint32_t ECOCALLMETHOD CEcoLab1_IY_AddRef(/* in */ struct IEcoCalculatorY* me) {
    CEcoLab1Stable* pCMe = (CEcoLab1Stable*)((uint64_t)me - sizeof(struct IEcoCalculatorX*) - sizeof(struct IEcoLab1*));

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    return ++pCMe->m_cRef;
}

// Функция Release для интерфейса IEcoCalculatorY
uint32_t ECOCALLMETHOD CEcoLab1_IY_Release(/* in */ struct IEcoCalculatorY* me) {
    CEcoLab1Stable* pCMe = (CEcoLab1Stable*)((uint64_t)me - sizeof(struct IEcoCalculatorX*) - sizeof(struct IEcoLab1*));

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

	/* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        if ( pCMe->m_pInnerUnknown != 0 ) {
            // Предотвращение рекурсивного вызова
            if (pCMe->m_pInnerUnknown->pVTbl->Release(pCMe->m_pInnerUnknown) == 0) {
                pCMe->m_pInnerUnknown = 0;
            }
            else {
                pCMe->m_cRef = 1;
            }
        }
        if ( pCMe->m_cRef == 0 ) {
            deleteCEcoLab1Stable((IEcoLab1*)pCMe);
        }
        return 0;
    }
    return pCMe->m_cRef;
}

// Функция Multiplication
int32_t ECOCALLMETHOD CEcoLab1_IY_Multiplication(/* in */ struct IEcoCalculatorY* me, /* in */ int16_t a, /* in */ int16_t b) {
	CEcoLab1Stable* pCMe = (CEcoLab1Stable*)((uint64_t)me - sizeof(struct IEcoCalculatorX*) - sizeof(struct IEcoLab1*));
    int32_t result = 0;

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

	if (pCMe->m_pIY != 0) {
		result = pCMe->m_pIY->pVTbl->Multiplication(pCMe->m_pIY, a, b);
	}

    return result;
}

// Функция Division
int16_t ECOCALLMETHOD CEcoLab1_IY_Division(/* in */ struct IEcoCalculatorY* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1Stable* pCMe = (CEcoLab1Stable*)((uint64_t)me - sizeof(struct IEcoCalculatorX*) - sizeof(struct IEcoLab1*));
    int16_t result = 0;

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

    if (pCMe->m_pIY != 0) {
		result = pCMe->m_pIY->pVTbl->Division(pCMe->m_pIY, a, b);
	}

    return result;
}

// Виртуальная таблица методов для IEcoLab1
IEcoLab1VTbl g_x277FC00C35624096AFCFC125B94EEC90VTbl = {
    CEcoLab1Stable_QueryInterface,
    CEcoLab1Stable_AddRef,
    CEcoLab1Stable_Release,
    CEcoLab1Stable_qsort
};

// Виртуальная таблица методов для IEcoCalculatorX
IEcoCalculatorXVTbl g_x9322111622484742AE0682819447843DVTbl = {
    CEcoLab1_IX_QueryInterface,
    CEcoLab1_IX_AddRef,
    CEcoLab1_IX_Release,
    CEcoLab1_IX_Addition,
    CEcoLab1_IX_Subtraction
};

// Виртуальная таблица методов для IEcoCalculatorY
IEcoCalculatorYVTbl g_xBD6414C29096423EA90C04D77AFD1CADVTbl = {
    CEcoLab1_IY_QueryInterface,
    CEcoLab1_IY_AddRef,
    CEcoLab1_IY_Release,
    CEcoLab1_IY_Multiplication,
    CEcoLab1_IY_Division
};


// Функция инициализации экземпляра
int16_t ECOCALLMETHOD initCEcoLab1Stable(/*in*/ struct IEcoLab1* me, /* in */ struct IEcoUnknown *pIUnkSystem) {
    CEcoLab1Stable* pCMe = (CEcoLab1Stable*)me;
    IEcoInterfaceBus1* pIBus = 0;
	IEcoUnknown* pOuterUnknown = (IEcoUnknown*)me;
    int16_t result = -1;

    if (me == 0 ) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

	// Получение интерфейса по работе с умножением и делением у компонента "E" агрегирующего компонент "B"
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (void**) &pCMe->m_pIY);
    if (result == 0 && pCMe->m_pIY != 0) {
        // Получение интерфейса по работе со сложением и вычитанием у компонента "B"
        result = pCMe->m_pIY->pVTbl->QueryInterface(pCMe->m_pIY, &IID_IEcoCalculatorX, (void**) &pCMe->m_pIX);
        if (result != 0 || pCMe->m_pIX == 0) {
            // Получение интерфейса по работе со сложением и вычитанием у компонента "A"
            result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (void**) &pCMe->m_pIX);
        }
    }
    else {
        // Получение интерфейса по работе с умножением и делением у компонента "D" включающего компонент "A"
        result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (void**) &pCMe->m_pIY);
        if (result == 0 && pCMe->m_pIY != 0) {
            // Получение интерфейса по работе со сложением и вычитанием у компонента "A"
            result = pCMe->m_pIY->pVTbl->QueryInterface(pCMe->m_pIY, &IID_IEcoCalculatorX, (void**) &pCMe->m_pIX);
        }
        else {
			// Получение интерфейса по работе со сложением и вычитанием у компонента "B" c поддержкой агрегирования
			result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorB, pOuterUnknown, &IID_IEcoUnknown, (void**) &pCMe->m_pInnerUnknown);
            if (result != 0 || pCMe->m_pInnerUnknown == 0) {
                // Получение интерфейса по работе со сложением и вычитанием у компонента "A"
                result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (void**) &pCMe->m_pIX);
            }
        }
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);
	
    return result;
}

//  Функция создания экземпляра
int16_t ECOCALLMETHOD createCEcoLab1Stable(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab1** ppIEcoLab1) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoLab1Stable* pCMe = 0;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;
	
    /* Проверка указателей */
    if (ppIEcoLab1 == 0 || pIUnkSystem == 0) {
        return result;
    }

    /* Получение системного интерфейса приложения */
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem1, (void **)&pISys);

    /* Проверка */
    if (result != 0 && pISys == 0) {
        return result;
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

	/* Получение идентификатора компонента для работы с памятью */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Получение интерфейса распределителя памяти */
    pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 && pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        pISys->pVTbl->Release(pISys);
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoLab1Stable*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1Stable));

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = pISys;

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoLab1 */
    pCMe->m_pVTblIEcoLab1 = &g_x277FC00C35624096AFCFC125B94EEC90VTbl;

	// Создание таблицы функций интерфейса IEcoCalculatorX
	pCMe->m_pVTblIX = &g_x9322111622484742AE0682819447843DVTbl;

	// Создание таблицы функций интерфейса IEcoCalculatorY
	pCMe->m_pVTblIY = &g_xBD6414C29096423EA90C04D77AFD1CADVTbl;

    /* Инициализация данных */
	pCMe->m_pIX = 0;
	pCMe->m_pIY = 0;
	pCMe->m_pInnerUnknown = 0;
    pCMe->m_Name = 0;

    /* Возврат указателя на интерфейс */
    *ppIEcoLab1 = (IEcoLab1*)pCMe;

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

    return 0;
}

//  Функция освобождения экземпляра
void ECOCALLMETHOD deleteCEcoLab1Stable(/* in */ IEcoLab1* pIEcoLab1) {
    CEcoLab1Stable* pCMe = (CEcoLab1Stable*)pIEcoLab1;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1 != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if ( pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}