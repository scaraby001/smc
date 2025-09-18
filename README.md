# Импульс. Аппаратно-программный комплекс для изучения МК семейства STM32. SWD01

![Setup](images/Impuls.jpg)

## Быстрый запуск

### Файл конфигурации CubeMX
* Открыть файл [`smc.ioc`](/smc.ioc) в CubeMX (вер. 6.14.1).
* Перейти на вкладку Project Manager.
* Toolchain / IDE выбрать STM32CubeIDE.
* Firmware Packadge Name and Version выбрать STM32Cube FW_F4 V1.28.0.
* Выполнить команду GENERATE CODE

### Основная программа

* В файле 'main.c' добавить следующие строки:

```c
/* USER CODE BEGIN Includes */

#include "tasks.h"

/* USER CODE END Includes */
```

```c
  /* USER CODE BEGIN 2 */

  tasks.power->state = CASE_START;

  /* USER CODE END 2 */
```

```c
  tasks_loop();

/* USER CODE END WHILE */
```
    