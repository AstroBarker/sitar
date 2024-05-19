# Output libname matches target name, with the usual extensions on your system
#add_library(libsaha Saha.cpp Saha.hpp Solve.hpp Abstractions.hpp Constants.hpp )
add_library(sahalib atom.cpp saha.cpp check_state.cpp )

# Link each target with other targets or add options, etc.

# Adding something we can run - Output name matches target name
add_executable(saha main.cpp)

# Make sure you link your targets with this command. It can also link libraries and
# even flags, so linking a target that does not exist will not give a configure-time error.
target_link_libraries(saha PRIVATE sahalib)
#target_compile_features(saha PUBLIC cxx_std_11)

