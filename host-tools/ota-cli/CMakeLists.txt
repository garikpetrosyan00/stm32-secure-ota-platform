add_executable(ota-cli
    src/main.c
)

target_compile_options(ota-cli PRIVATE
    -Wall -Wextra -Werror
)
