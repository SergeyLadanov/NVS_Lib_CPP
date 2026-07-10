# Список всех тестовых файлов
set(TEST_SOURCES
    Test_DataTypes
    Test_SpaceCapacity
    Test_CellUnit
    Test_PageUnit
    Test_PageMigration
    Test_MultipleRewrite
    Test_Persistence
    Test_BoundaryValues
    Test_RemoveValue
    Test_Clear
)

# Общие include-директории для всех тестов
set(TEST_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/../Components/NVS_Lib/Inc
)

# Создание исполняемого файла и регистрация теста для каждого тестового файла
foreach(TEST_NAME ${TEST_SOURCES})
    add_executable(
        ${TEST_NAME}
        ${TEST_NAME}.cpp
    )

    target_include_directories(
        ${TEST_NAME}
        PRIVATE
        ${TEST_INCLUDES}
    )

    target_link_libraries(
        ${TEST_NAME}
        PRIVATE
        NVS_Lib
    )

    add_test(
        NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
    )
endforeach()