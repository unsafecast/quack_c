file(GLOB src
    "*.c"
    "*.h"
)

add_library(quack STATIC ${src})
target_include_directories(quack PUBLIC "../")
