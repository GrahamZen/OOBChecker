# Array Out-Of-Bound Static Analysis
## Team Members
- Tongwei Dai
- Gehan Zheng

## Introduction
This project (abbreviated as `OOBChecker`) is to implement an [LLVM](https://llvm.org/) pass to detect array out-of-bound errors in C programs. The pass is implemented in C++ using the LLVM API and relies on the theories of dataflow analysis. Using the chaotic iteration algorithm on the interval domain, we are able to effectively catch array out-of-bound errors in both direct access and indirect access cases.

## Project Structure
```
└── Project Root
    ├── .devcontainer   // docker configuration
    ├── .vscode         // vscode configuration
    ├── include         // OOB Checker headers 
    ├── src             // OOB Checker source files
    ├── test            // test C programs to demonstrate usage
    └── unit_test       // internal unit test for OOB Checker functionalities
```

## How to Build
### Prerequisites
- [Docker](https://www.docker.com/)
- [Visual Studio Code](https://code.visualstudio.com/)

### Build Steps
1. Clone this repository to your local machine.
2. Install the [Remote - Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) extension for Visual Studio Code.
3. Open the project folder in Visual Studio Code.
4. Click the green button at the bottom left corner of the window and select `Reopen in Container`.
5. Wait for the container to build and start.
6. Open a terminal in Visual Studio Code.
7. Run the following commands to create a build directory.
    ```bash
    mkdir build && cd build
    ```
8. Run the following commands to build the project.
    ```bash
    cmake ..
    make
    ```

## How to Run & Example Output
1. Follow the build steps to build the project.

### Running Example Test Programs
Run the following command to run the pass on a test program.
```bash
cd test
make test[1-10]
```
- Replace `[1-10]` is the test program number.
- Two files will be generated: `test*.out` and `test*.err`, where `test*.out` is the output of the pass and `test*.err` contains array out-of-bound errors detected by the pass.

### Debugging the Pass
1. If not already, install the [command variable extension](https://marketplace.visualstudio.com/items?itemName=rioj7.command-variable) for Visual Studio Code.
2. Press F5 to select any test program from the dropdown menu and run the pass with the debugger attached.

### Running Unit Tests
Run the following command to run the unit test.
```bash
cd unit_test
make main
./main
```

### Running on Your Own Program
suppose your program is named `test.c` and contains the following code:
```c
int main() {
    int a[20];
    int* b = a + 10;
    int c = 2;
    int d = 3; 
    b[c < d] = 0; // out-of-bounds access
    return 0;
}
```

run the following command to run the pass on your program.
```bash
cd build
clang -emit-llvm -S -fno-discard-value-names -Xclang -disable-O0-optnone -c -o test.ll [path/to/test.c]
opt -load ./OOBChecker.so -OOBChecker test.ll 
```

---
`test.err` will contain the following line, if everything works correctly.
    ```
    Potential array out of bounds error:   %arrayidx = getelementptr inbounds i32, i32* %0, i64 %idxprom
    ```
- which is the IR instruction corresponding to the out-of-bounds access `b[c < d] = 0;`.
---
`test.out` will show how the interval domain is updated at each program point.
- for example, this snippet shows the comparison result `c < d` is deduced to be `[1, 1]` at the end of the comparison instruction `%cmp = icmp slt i32 %1, %2`, which is correct.
```
IN                   | OUT
%d       |-> [3, 3]  | %cmp     |-> [1, 1] 
%1       |-> [2, 2]  | %d       |-> [3, 3] 
%2       |-> [3, 3]  | %1       |-> [2, 2] 
%retval  |-> [0, 0]  | %2       |-> [3, 3] 
%c       |-> [2, 2]  | %retval  |-> [0, 0] 
                     | %c       |-> [2, 2] 
```