
# Consider dependencies only in project.
set(CMAKE_DEPENDS_IN_PROJECT_ONLY OFF)

# The set of languages for which implicit dependencies are needed:
set(CMAKE_DEPENDS_LANGUAGES
  "ASM"
  )
# The set of files for implicit dependencies of each language:
set(CMAKE_DEPENDS_CHECK_ASM
  "/home/josh/zephyrproject/zephyr/arch/arm/core/nmi_on_reset.S" "/home/josh/cmpt433/work/blinky/build/zephyr/arch/arch/arm/core/CMakeFiles/arch__arm__core.dir/nmi_on_reset.S.obj"
  )
set(CMAKE_ASM_COMPILER_ID "GNU")

# Preprocessor definitions for this target.
set(CMAKE_TARGET_DEFINITIONS_ASM
  "KERNEL"
  "K_HEAP_MEM_POOL_SIZE=0"
  "PICOLIBC_DOUBLE_PRINTF_SCANF"
  "__LINUX_ERRNO_EXTENSIONS__"
  "__PROGRAM_START"
  "__ZEPHYR_SUPERVISOR__"
  "__ZEPHYR__=1"
  )

# The include file search paths:
set(CMAKE_ASM_TARGET_INCLUDE_PATH
  "/home/josh/zephyrproject/zephyr/kernel/include"
  "/home/josh/zephyrproject/zephyr/arch/arm/include"
  "zephyr/include/generated/zephyr"
  "/home/josh/zephyrproject/zephyr/include"
  "zephyr/include/generated"
  "/home/josh/zephyrproject/zephyr/soc/ti/k3"
  "/home/josh/zephyrproject/zephyr/lib/open-amp/."
  "/home/josh/zephyrproject/zephyr/soc/ti/k3/am6x/."
  "/home/josh/zephyrproject/zephyr/soc/ti/k3/am6x/r5"
  "/home/josh/zephyrproject/modules/hal/cmsis/CMSIS/Core_R/Include"
  "/home/josh/zephyrproject/zephyr/modules/cmsis/."
  "modules/libmetal/libmetal/lib/include"
  "/home/josh/zephyrproject/modules/lib/open-amp/open-amp/lib/include"
  "/home/josh/zephyrproject/zephyr/lib/libc/common/include"
  )

# The set of dependency files which are needed:
set(CMAKE_DEPENDS_DEPENDENCY_FILES
  "/home/josh/zephyrproject/zephyr/arch/arm/core/fatal.c" "zephyr/arch/arch/arm/core/CMakeFiles/arch__arm__core.dir/fatal.c.obj" "gcc" "zephyr/arch/arch/arm/core/CMakeFiles/arch__arm__core.dir/fatal.c.obj.d"
  "/home/josh/zephyrproject/zephyr/arch/arm/core/nmi.c" "zephyr/arch/arch/arm/core/CMakeFiles/arch__arm__core.dir/nmi.c.obj" "gcc" "zephyr/arch/arch/arm/core/CMakeFiles/arch__arm__core.dir/nmi.c.obj.d"
  "/home/josh/zephyrproject/zephyr/arch/arm/core/tls.c" "zephyr/arch/arch/arm/core/CMakeFiles/arch__arm__core.dir/tls.c.obj" "gcc" "zephyr/arch/arch/arm/core/CMakeFiles/arch__arm__core.dir/tls.c.obj.d"
  )

# Targets to which this target links.
set(CMAKE_TARGET_LINKED_INFO_FILES
  )

# Fortran module output directory.
set(CMAKE_Fortran_TARGET_MODULE_DIR "")
