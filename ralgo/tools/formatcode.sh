find . -regex '.*\.\(cpp\|hpp\|cc\|c\|h\|cxx\)' -exec clang-format -style=file -i {} \;
