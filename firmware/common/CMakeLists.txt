add_library(common STATIC
    src/common_utils.c
    src/flash_if.c
    src/image_metadata.c
    src/boot_confirm.c
)

target_include_directories(common PUBLIC
    inc
)

target_compile_options(common PRIVATE
    -Wall -Wextra -Werror
)
