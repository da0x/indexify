# File Index Generator

## Introduction
This program, named `indexify`, scans the current directory and generates an HTML file (`index.html`) listing all regular files. It displays each file's name, size (in a human-readable format like KB, MB, etc.), and last modification time.

## Requirements
- C++20 compliant compiler: This program uses features introduced in C++20.
- Makefile (provided): For easy compilation and installation.

## Installation

### Compiling the Program
To compile `indexify`, use the provided Makefile. Run the following command in the directory containing your source code and Makefile:

```bash
make
```

This will generate the `indexify` executable.

### Installation
To install `indexify` to your system, use the `install` target in the Makefile:

```bash
make install
```

This will copy the `indexify` binary to `/usr/local/bin`, allowing it to be run from any directory.

## Usage
After installation, you can run `indexify` in any directory:

```bash
indexify
```

This command will create an `index.html` file in the current directory, listing all the regular files with their names, sizes, and last modified times.

### Running Without Installation
If you prefer not to install, you can still run `indexify` directly after compilation:

```bash
make run
```

## Features
- Scans the current directory for regular files.
- Generates an HTML file listing file names, sizes, and last modification times.
- File sizes are displayed in a human-readable format (B, KB, MB, GB).

## Cleaning Up
To clean up the build artifacts, use:

```bash
make clean
```

## Uninstallation
To uninstall `indexify` from your system:

```bash
make uninstall
```

## Contributing
Contributions are welcome! Feel free to fork this repository and submit pull requests with any enhancements or bug fixes.

## License
This project is licensed under the Apache License 2.0. See the `LICENSE` file in this repository for the full license text.



