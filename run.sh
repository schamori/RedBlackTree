echo "Compiling main.cpp..."
g++ -std=c++17 -o main main.cpp

# Check if compilation succeeded
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

echo "Compilation successful. Running the program..."
echo "--------------------------------------------"

# Run the compiled program
./main

# Exit with the program's return code
exit $?