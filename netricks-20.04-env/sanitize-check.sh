o_files=$(find . -name "*.o")
if [ -n "$o_files" ]; then
    echo "Found .o files:"
    echo "$o_files"
    exit 1
fi

so_files=$(find . -name "*.so")
if [ -n "$so_files" ]; then
    echo "Found .so files:"
    echo "$so_files"
    exit 1
fi

a_files=$(find . -name "*.a")
if [ -n "$a_files" ]; then
    echo "Found .a files:"
    echo "$a_files"
    exit 1
fi