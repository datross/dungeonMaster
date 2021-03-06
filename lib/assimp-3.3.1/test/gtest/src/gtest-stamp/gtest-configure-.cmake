

set(command "/usr/bin/cmake;-DCMAKE_BUILD_TYPE=;-Dgtest_force_shared_crt=ON;-Dgtest_disable_pthreads:BOOL=OFF;-DBUILD_GTEST=ON;-GUnix Makefiles;/home/vanessa/Bureau/dungeonMaster/lib/assimp-3.3.1/test/gtest/src/gtest")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/home/vanessa/Bureau/dungeonMaster/lib/assimp-3.3.1/test/gtest/src/gtest-stamp/gtest-configure-out.log"
  ERROR_FILE "/home/vanessa/Bureau/dungeonMaster/lib/assimp-3.3.1/test/gtest/src/gtest-stamp/gtest-configure-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /home/vanessa/Bureau/dungeonMaster/lib/assimp-3.3.1/test/gtest/src/gtest-stamp/gtest-configure-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "gtest configure command succeeded.  See also /home/vanessa/Bureau/dungeonMaster/lib/assimp-3.3.1/test/gtest/src/gtest-stamp/gtest-configure-*.log")
  message(STATUS "${msg}")
endif()
