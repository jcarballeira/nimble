# CMake generated Testfile for 
# Source directory: /home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp
# Build directory: /home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(ament_index_cpp_utest "/usr/bin/python3" "-u" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_test/share/ament_cmake_test/cmake/run_test.py" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/test_results/ament_index_cpp/ament_index_cpp_utest.gtest.xml" "--package-name" "ament_index_cpp" "--output-file" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/ament_cmake_gtest/ament_index_cpp_utest.txt" "--command" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/ament_index_cpp_utest" "--gtest_output=xml:/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/test_results/ament_index_cpp/ament_index_cpp_utest.gtest.xml")
set_tests_properties(ament_index_cpp_utest PROPERTIES  LABELS "gtest" REQUIRED_FILES "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/ament_index_cpp_utest" TIMEOUT "60" WORKING_DIRECTORY "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp" _BACKTRACE_TRIPLES "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_test/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_gtest/share/ament_cmake_gtest/cmake/ament_add_gtest_test.cmake;86;ament_add_test;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_gtest/share/ament_cmake_gtest/cmake/ament_add_gtest.cmake;93;ament_add_gtest_test;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp/CMakeLists.txt;42;ament_add_gtest;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp/CMakeLists.txt;0;")
add_test(copyright "/usr/bin/python3" "-u" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_test/share/ament_cmake_test/cmake/run_test.py" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/test_results/ament_index_cpp/copyright.xunit.xml" "--package-name" "ament_index_cpp" "--output-file" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/ament_copyright/copyright.txt" "--command" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_copyright/bin/ament_copyright" "--xunit-file" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/test_results/ament_index_cpp/copyright.xunit.xml")
set_tests_properties(copyright PROPERTIES  LABELS "copyright;linter" TIMEOUT "200" WORKING_DIRECTORY "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp" _BACKTRACE_TRIPLES "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_test/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_copyright/share/ament_cmake_copyright/cmake/ament_copyright.cmake;51;ament_add_test;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_copyright/share/ament_cmake_copyright/cmake/ament_cmake_copyright_lint_hook.cmake;22;ament_copyright;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_copyright/share/ament_cmake_copyright/cmake/ament_cmake_copyright_lint_hook.cmake;0;;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_core/share/ament_cmake_core/cmake/core/ament_execute_extensions.cmake;48;include;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_lint_auto/share/ament_lint_auto/cmake/ament_lint_auto_package_hook.cmake;21;ament_execute_extensions;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_lint_auto/share/ament_lint_auto/cmake/ament_lint_auto_package_hook.cmake;0;;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_core/share/ament_cmake_core/cmake/core/ament_execute_extensions.cmake;48;include;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_core/share/ament_cmake_core/cmake/core/ament_package.cmake;66;ament_execute_extensions;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp/CMakeLists.txt;49;ament_package;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp/CMakeLists.txt;0;")
add_test(cppcheck "/usr/bin/python3" "-u" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_test/share/ament_cmake_test/cmake/run_test.py" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/test_results/ament_index_cpp/cppcheck.xunit.xml" "--package-name" "ament_index_cpp" "--output-file" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/ament_cppcheck/cppcheck.txt" "--command" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cppcheck/bin/ament_cppcheck" "--xunit-file" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/test_results/ament_index_cpp/cppcheck.xunit.xml" "--include_dirs" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp/include" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp/include")
set_tests_properties(cppcheck PROPERTIES  LABELS "cppcheck;linter" TIMEOUT "300" WORKING_DIRECTORY "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp" _BACKTRACE_TRIPLES "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_test/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_cppcheck/share/ament_cmake_cppcheck/cmake/ament_cppcheck.cmake;66;ament_add_test;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_cppcheck/share/ament_cmake_cppcheck/cmake/ament_cmake_cppcheck_lint_hook.cmake;87;ament_cppcheck;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_cppcheck/share/ament_cmake_cppcheck/cmake/ament_cmake_cppcheck_lint_hook.cmake;0;;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_core/share/ament_cmake_core/cmake/core/ament_execute_extensions.cmake;48;include;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_lint_auto/share/ament_lint_auto/cmake/ament_lint_auto_package_hook.cmake;21;ament_execute_extensions;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_lint_auto/share/ament_lint_auto/cmake/ament_lint_auto_package_hook.cmake;0;;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_core/share/ament_cmake_core/cmake/core/ament_execute_extensions.cmake;48;include;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_core/share/ament_cmake_core/cmake/core/ament_package.cmake;66;ament_execute_extensions;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp/CMakeLists.txt;49;ament_package;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp/CMakeLists.txt;0;")
add_test(cpplint "/usr/bin/python3" "-u" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_test/share/ament_cmake_test/cmake/run_test.py" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/test_results/ament_index_cpp/cpplint.xunit.xml" "--package-name" "ament_index_cpp" "--output-file" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/ament_cpplint/cpplint.txt" "--command" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cpplint/bin/ament_cpplint" "--xunit-file" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/test_results/ament_index_cpp/cpplint.xunit.xml")
set_tests_properties(cpplint PROPERTIES  LABELS "cpplint;linter" TIMEOUT "120" WORKING_DIRECTORY "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp" _BACKTRACE_TRIPLES "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_test/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_cpplint/share/ament_cmake_cpplint/cmake/ament_cpplint.cmake;68;ament_add_test;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_cpplint/share/ament_cmake_cpplint/cmake/ament_cmake_cpplint_lint_hook.cmake;39;ament_cpplint;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_cpplint/share/ament_cmake_cpplint/cmake/ament_cmake_cpplint_lint_hook.cmake;0;;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_core/share/ament_cmake_core/cmake/core/ament_execute_extensions.cmake;48;include;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_lint_auto/share/ament_lint_auto/cmake/ament_lint_auto_package_hook.cmake;21;ament_execute_extensions;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_lint_auto/share/ament_lint_auto/cmake/ament_lint_auto_package_hook.cmake;0;;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_core/share/ament_cmake_core/cmake/core/ament_execute_extensions.cmake;48;include;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_core/share/ament_cmake_core/cmake/core/ament_package.cmake;66;ament_execute_extensions;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp/CMakeLists.txt;49;ament_package;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp/CMakeLists.txt;0;")
add_test(lint_cmake "/usr/bin/python3" "-u" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_test/share/ament_cmake_test/cmake/run_test.py" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/test_results/ament_index_cpp/lint_cmake.xunit.xml" "--package-name" "ament_index_cpp" "--output-file" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/ament_lint_cmake/lint_cmake.txt" "--command" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_lint_cmake/bin/ament_lint_cmake" "--xunit-file" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/test_results/ament_index_cpp/lint_cmake.xunit.xml")
set_tests_properties(lint_cmake PROPERTIES  LABELS "lint_cmake;linter" TIMEOUT "60" WORKING_DIRECTORY "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp" _BACKTRACE_TRIPLES "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_test/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_lint_cmake/share/ament_cmake_lint_cmake/cmake/ament_lint_cmake.cmake;47;ament_add_test;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_lint_cmake/share/ament_cmake_lint_cmake/cmake/ament_cmake_lint_cmake_lint_hook.cmake;21;ament_lint_cmake;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_lint_cmake/share/ament_cmake_lint_cmake/cmake/ament_cmake_lint_cmake_lint_hook.cmake;0;;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_core/share/ament_cmake_core/cmake/core/ament_execute_extensions.cmake;48;include;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_lint_auto/share/ament_lint_auto/cmake/ament_lint_auto_package_hook.cmake;21;ament_execute_extensions;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_lint_auto/share/ament_lint_auto/cmake/ament_lint_auto_package_hook.cmake;0;;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_core/share/ament_cmake_core/cmake/core/ament_execute_extensions.cmake;48;include;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_core/share/ament_cmake_core/cmake/core/ament_package.cmake;66;ament_execute_extensions;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp/CMakeLists.txt;49;ament_package;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp/CMakeLists.txt;0;")
add_test(uncrustify "/usr/bin/python3" "-u" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_test/share/ament_cmake_test/cmake/run_test.py" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/test_results/ament_index_cpp/uncrustify.xunit.xml" "--package-name" "ament_index_cpp" "--output-file" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/ament_uncrustify/uncrustify.txt" "--command" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_uncrustify/bin/ament_uncrustify" "--xunit-file" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/test_results/ament_index_cpp/uncrustify.xunit.xml")
set_tests_properties(uncrustify PROPERTIES  LABELS "uncrustify;linter" TIMEOUT "60" WORKING_DIRECTORY "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp" _BACKTRACE_TRIPLES "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_test/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_uncrustify/share/ament_cmake_uncrustify/cmake/ament_uncrustify.cmake;70;ament_add_test;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_uncrustify/share/ament_cmake_uncrustify/cmake/ament_cmake_uncrustify_lint_hook.cmake;43;ament_uncrustify;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_uncrustify/share/ament_cmake_uncrustify/cmake/ament_cmake_uncrustify_lint_hook.cmake;0;;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_core/share/ament_cmake_core/cmake/core/ament_execute_extensions.cmake;48;include;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_lint_auto/share/ament_lint_auto/cmake/ament_lint_auto_package_hook.cmake;21;ament_execute_extensions;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_lint_auto/share/ament_lint_auto/cmake/ament_lint_auto_package_hook.cmake;0;;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_core/share/ament_cmake_core/cmake/core/ament_execute_extensions.cmake;48;include;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_core/share/ament_cmake_core/cmake/core/ament_package.cmake;66;ament_execute_extensions;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp/CMakeLists.txt;49;ament_package;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp/CMakeLists.txt;0;")
add_test(xmllint "/usr/bin/python3" "-u" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_test/share/ament_cmake_test/cmake/run_test.py" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/test_results/ament_index_cpp/xmllint.xunit.xml" "--package-name" "ament_index_cpp" "--output-file" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/ament_xmllint/xmllint.txt" "--command" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_xmllint/bin/ament_xmllint" "--xunit-file" "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/build/ament_index_cpp/test_results/ament_index_cpp/xmllint.xunit.xml")
set_tests_properties(xmllint PROPERTIES  LABELS "xmllint;linter" TIMEOUT "60" WORKING_DIRECTORY "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp" _BACKTRACE_TRIPLES "/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_test/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_xmllint/share/ament_cmake_xmllint/cmake/ament_xmllint.cmake;50;ament_add_test;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_xmllint/share/ament_cmake_xmllint/cmake/ament_cmake_xmllint_lint_hook.cmake;18;ament_xmllint;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_xmllint/share/ament_cmake_xmllint/cmake/ament_cmake_xmllint_lint_hook.cmake;0;;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_core/share/ament_cmake_core/cmake/core/ament_execute_extensions.cmake;48;include;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_lint_auto/share/ament_lint_auto/cmake/ament_lint_auto_package_hook.cmake;21;ament_execute_extensions;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_lint_auto/share/ament_lint_auto/cmake/ament_lint_auto_package_hook.cmake;0;;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_core/share/ament_cmake_core/cmake/core/ament_execute_extensions.cmake;48;include;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/install/ament_cmake_core/share/ament_cmake_core/cmake/core/ament_package.cmake;66;ament_execute_extensions;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp/CMakeLists.txt;49;ament_package;/home/jaime/MotorControl/uros_ws/firmware/dev_ws/ament/ament_index/ament_index_cpp/CMakeLists.txt;0;")
subdirs("gtest")
