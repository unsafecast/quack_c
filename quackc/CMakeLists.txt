file(GLOB src
    "*.c"
    "*.h"
)

add_executable(quackc ${src})
target_link_libraries(quackc PUBLIC quack)
