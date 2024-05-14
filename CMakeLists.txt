cmake_minimum_required(VERSION 3.10)
project(MyFreeRTOSProject C)

set(CMAKE_C_STANDARD 99)

# Adicione os diretórios de inclusão
include_directories(FreeRTOS/FreeRTOS-Kernel/include)
include_directories(FreeRTOS/FreeRTOS-Kernel/portable/GCC/Posix)

# Adicione o executável
add_executable(train src/train.c)

# Adicione as fontes do FreeRTOS
target_sources(train PRIVATE FreeRTOS/FreeRTOS-Kernel/croutine.c)
target_sources(train PRIVATE FreeRTOS/FreeRTOS-Kernel/event_groups.c)
target_sources(train PRIVATE FreeRTOS/FreeRTOS-Kernel/list.c)
target_sources(train PRIVATE FreeRTOS/FreeRTOS-Kernel/queue.c)
target_sources(train PRIVATE FreeRTOS/FreeRTOS-Kernel/tasks.c)
target_sources(train PRIVATE FreeRTOS/FreeRTOS-Kernel/timers.c)
target_sources(train PRIVATE FreeRTOS/FreeRTOS-Kernel/portable/MemMang/heap_4.c)
target_sources(train PRIVATE FreeRTOS/FreeRTOS-Kernel/portable/GCC/Posix/port.c)

# Vincule a biblioteca pthread
target_link_libraries(train pthread)

