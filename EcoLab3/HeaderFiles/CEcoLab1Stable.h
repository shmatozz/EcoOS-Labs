/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает реализацию компонента CEcoLab1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __C_ECOLAB1STABLE_H__
#define __C_ECOLAB1STABLE_H__

#include "IEcoLab1.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

// X Y
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"

typedef struct CEcoLab1Stable {

    /* Таблица функций интерфейса IEcoLab1 */
    IEcoLab1VTbl* m_pVTblIEcoLab1;

	// Таблица функций интерфейса IEcoCalculatorX
	IEcoCalculatorXVTbl* m_pVTblIX;

	// Таблица функций интерфейса IEcoCalculatorY
	IEcoCalculatorYVTbl* m_pVTblIY;

    /* Счетчик ссылок */
    uint32_t m_cRef;

    /* Интерфейс для работы с памятью */
    IEcoMemoryAllocator1* m_pIMem;

    /* Системный интерфейс */
    IEcoSystem1* m_pISys;

	// Указатель на интерфейс IEcoCalculatorX включаемого компонента
    IEcoCalculatorX* m_pIX;

	// Указатель на интерфейс IEcoCalculatorY включаемого компонента
    IEcoCalculatorY* m_pIY;

	// Указатель на IEcoUnknown внутреннего компонента
    IEcoUnknown* m_pInnerUnknown;

    /* Данные экземпляра */
    char_t* m_Name;

} CEcoLab1Stable, *CEcoLab1StablePtr;

/* Инициализация экземпляра */
int16_t ECOCALLMETHOD initCEcoLab1Stable(/*in*/ struct IEcoLab1* me, /* in */ IEcoUnknown *pIUnkSystem);
/* Создание экземпляра */
int16_t ECOCALLMETHOD createCEcoLab1Stable(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab1** ppIEcoLab1);
/* Удаление */
void ECOCALLMETHOD deleteCEcoLab1Stable(/* in */ IEcoLab1* pIEcoLab1);

#endif /* __C_ECOLAB1STABLE_H__ */
