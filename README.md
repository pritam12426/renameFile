# fix-name

`fix-name` is a command-line utility for sanitizing and standardizing file and directory names. It cleans up names by removing special characters, converting to lowercase, and ensuring consistent formatting, making them more shell-friendly and easier to work with.

## Features

- **Lowercase Conversion:** Converts all characters in the name to lowercase.
- **Character Sanitization:** Removes non-ASCII characters.
- **Whitespace Handling:** Replaces spaces and other whitespace with underscores.
- **Special Character Cleanup:** Intelligently handles and removes redundant special characters like `!`, `@`, `#`, `$`, `%`, `^`, `&`, `*`, `(`, `)`.
- **Dry Run Mode:** Preview changes without actually renaming any files using the `--dry-run` or `-n` flag.
- **Ignore Patterns:** Specify files and directories to ignore during processing.
- **Verbose Control:** Control the verbosity of the output.
- **Safe by Default:** Prevents accidental renaming of files directly in the user's home directory unless explicitly enabled with the `--force` flag.

## Usage

```bash
fix-name [options] <file_or_directory_paths...>
```

### Options

- `-V`, `--no-verbose`: Disable verbose output. By default, the tool prints every renaming operation.
- `-F`, `--force`: Allow the tool to operate within the user's home directory. This is a safety feature to prevent accidental renaming of important files.
- `-s`, `--summary`: Print a summary of all operations at the end. (Note: This feature is intended for future implementation).
- `-n`, `--dry-run`: Perform a trial run without making any actual changes to the filesystem. This is useful for previewing the renames.
- `-i <name>`, `--ignore <name>`: Ignore a specific file or directory name. This option can be used multiple times. Default ignored names include `.git`, `.gitignore`, `CMakeCache.txt`, and `CMakeLists.txt`.

## Building the Project

The project uses CMake for building.

### Prerequisites

- A C++ compiler (supporting C++17 or later)
- CMake 3.16 or later

### Build Steps

1. **Clone the repository:**
   ```bash
   git clone <repository_url>
   cd renameFile
   ```

2. **Create a build directory:**
   ```bash
   mkdir build && cd build
   ```

3. **Configure the project with CMake:**
   ```bash
   cmake ..
   ```

4. **Compile the project:**
   ```bash
   cmake --build .
   ```

The executable `fix-name` will be created in the `build` directory.

## Example

Suppose you have a file named `My File (1).txt`.

Running `fix-name` on it:
```bash
./fix-name "My File (1).txt"
```

This will rename the file to `my_file_1.txt`.

Using the dry-run feature:
```bash
./fix-name -n "My File (1).txt"
```

Output:
```
[DRY] Would rename /path/to/My File (1).txt -> /path/to/my_file_1.txt
```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
