cmake_minimum_required(VERSION 3.0)
# 工程名
project(app)

# 选择生成方式
set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")

# 清空默认参数
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")

# 设定工具链
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)

# cpu
set(CPU -mcpu=cortex-m4)

# fpu
set(FPU -mfpu=fpv4-sp-d16)

# float-abi
set(FLOAT_ABI -mfloat-abi=hard)

# mcu
set(MCU "${CPU} -mthumb ${FPU} ${FLOAT_ABI}")

# 优化等级
set(OPT "-O0")
#set(OPT "-O3")

# 编译标志
set(CMAKE_C_FLAGS "${MCU} -Wall -fdata-sections -ffunction-sections")
set(CMAKE_C_FLAGS_DEBUG "${OPT} -g -gdwarf-2")
set(CMAKE_C_FLAGS_RELEASE "${OPT}")

# 编译宏
add_definitions("-DGD32C10X -DGD32C103V_EVAL")

# 头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR}/Core/main/Inc 
    ${PROJECT_SOURCE_DIR}/Core/BSP/include 
    ${PROJECT_SOURCE_DIR}/Core/FATFS/include 
    ${PROJECT_SOURCE_DIR}/Core/Firmware/GD32C10x/Include  
    ${PROJECT_SOURCE_DIR}/Core/freeRTOS/include 
    ${PROJECT_SOURCE_DIR}/User/include
    )

# 使能汇编语言
ENABLE_LANGUAGE(ASM)

######################################
# 源代码
######################################
# C 语言代码
aux_source_directory(${PROJECT_SOURCE_DIR}/Core/Firmware/GD32C10x/Source/ UAPI_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/Core/freeRTOS/src/ RTOS_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/Core/BSP/SRC/ BSP_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/Core/FATFS/SRC/ FATFS_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/User/SRC/ UAPP_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/Core/main/Src/ C_SOURCES)
# 汇编代码 
set(ASM_SRC ${PROJECT_SOURCE_DIR}/Core/startup.s)

# 链接脚本及链接器设置
set(LINKER_SCRIPT ${PROJECT_SOURCE_DIR}/Core/GD32C103.ld)
set(SPECS "-specs=nosys.specs -specs=nano.specs")
set(CMAKE_EXE_LINKER_FLAGS
    "${MCU} -T${LINKER_SCRIPT} ${SPECS} -lm -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map,--cref -Wl,--gc-sections"
)

#生成Free RTOS库
# add_library(freertos ${RTOS_SRC})



#生成板级支持
#add_library(BSP ${BSP_SRCS})

#生成FATFS文件系统库
# add_library(fatfs ${FATFS_SRC})

#生成app
add_library(app ${UAPP_SRC} ${UAPI_SRC} ${BSP_SRCS} ${RTOS_SRC} ${FATFS_SRC})

#生成二进制文件
add_executable(${PROJECT_NAME}.elf ${C_SOURCES} ${ASM_SRC} )

# 链接生成最终文件
target_link_libraries(
    ${PROJECT_NAME}.elf 
    # freertos 
    # fatfs 
    app 
    )

# 设定生成文件位置
set(ELF_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf)
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
## 生成hex和bin
add_custom_command(TARGET "${PROJECT_NAME}.elf" POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${ELF_FILE} ${BIN_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Oihex  ${ELF_FILE} ${HEX_FILE}
    COMMENT "Building ${PROJECT_NAME}.bin and ${PROJECT_NAME}.hex"

    COMMAND ${CMAKE_COMMAND} -E copy ${HEX_FILE} "${PROJECT_SOURCE_DIR}/out/${PROJECT_NAME}.hex"
    COMMAND ${CMAKE_COMMAND} -E copy ${BIN_FILE} "${PROJECT_SOURCE_DIR}/out/${PROJECT_NAME}.bin"

    COMMAND ${CMAKE_SIZE} --format=berkeley ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMENT "Invoking: Cross ARM GNU Print Size"
)
