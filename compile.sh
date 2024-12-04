mkdir -p build  # Ensure the build directory exists

find . -name "*.cpp" | while read src; do
    # Create object file path in the build directory, replacing .cpp with .o
    obj="build/$(basename ${src%.cpp}.o)"
    # Compile the source file into the build directory
    g++ -std=c++20 -c "$src" -I/usr/include/SFML -I./include -o "$obj"
done

