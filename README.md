# Table

Self-imlemented simple Excel-like table:
 - support text, formula cells. Formula may contain references to another cells
 - for formula evaluation use ANTLR

Build:
 - ANTLR distr in project folder
 - change CMakeLists.txt target_link_libraries(spreadsheet "path to libantlr4-runtime.a")
 - from build folder
	cmake --build .
	cmake ../ -DCMAKE_CXX_COMPILER=clang -G "Xcode" 
