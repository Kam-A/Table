#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/user/learning_temp/c_plus/final_project/final_table
  "/Library/Internet Plug-Ins/JavaAppletPlugin.plugin/Contents/Home/bin/java" -jar /Users/user/learning_temp/c_plus/final_project/final_table/antlr-4.9.2-complete.jar Formula.g4 -o /Users/user/learning_temp/c_plus/final_project/final_table/build/antlr4cpp_generated_src/Formula -no-listener -Dlanguage=Cpp -listener
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/user/learning_temp/c_plus/final_project/final_table
  "/Library/Internet Plug-Ins/JavaAppletPlugin.plugin/Contents/Home/bin/java" -jar /Users/user/learning_temp/c_plus/final_project/final_table/antlr-4.9.2-complete.jar Formula.g4 -o /Users/user/learning_temp/c_plus/final_project/final_table/build/antlr4cpp_generated_src/Formula -no-listener -Dlanguage=Cpp -listener
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/user/learning_temp/c_plus/final_project/final_table
  "/Library/Internet Plug-Ins/JavaAppletPlugin.plugin/Contents/Home/bin/java" -jar /Users/user/learning_temp/c_plus/final_project/final_table/antlr-4.9.2-complete.jar Formula.g4 -o /Users/user/learning_temp/c_plus/final_project/final_table/build/antlr4cpp_generated_src/Formula -no-listener -Dlanguage=Cpp -listener
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/user/learning_temp/c_plus/final_project/final_table
  "/Library/Internet Plug-Ins/JavaAppletPlugin.plugin/Contents/Home/bin/java" -jar /Users/user/learning_temp/c_plus/final_project/final_table/antlr-4.9.2-complete.jar Formula.g4 -o /Users/user/learning_temp/c_plus/final_project/final_table/build/antlr4cpp_generated_src/Formula -no-listener -Dlanguage=Cpp -listener
fi

