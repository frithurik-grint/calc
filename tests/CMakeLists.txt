add_executable("calc-test" "main.c")
target_link_libraries("calc-test" PUBLIC lib)

install(TARGETS "calc-test" DESTINATION "out/bin")
