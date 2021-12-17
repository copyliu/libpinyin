add_subdirectory(include)
add_subdirectory(storage)
add_subdirectory(lookup)

add_executable(
    test_pinyin
    test_pinyin.cpp
    compat/test_utf8_init.cpp
)

target_link_libraries(
    test_pinyin
    pinyin
)

add_executable(
    test_phrase
    test_phrase.cpp
    compat/test_utf8_init.cpp
)

target_link_libraries(
    test_phrase
    pinyin
)

add_executable(
    test_chewing
    test_chewing.cpp
    compat/test_utf8_init.cpp
)

target_link_libraries(
    test_chewing
    pinyin
)
