add_executable(ota-cli
    src/main.c
    src/ota_package.c
    src/ota_transport.c
)

target_include_directories(ota-cli PRIVATE
    inc
)

# No dependency on firmware common libs for now to keep host tool independent
# In future, shared headers might be moved to a shared_inc folder
target_compile_options(ota-cli PRIVATE
    -Wall -Wextra -Werror
)
