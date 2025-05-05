# LeOS - A 32-bit Operating System

LeOS is a 32-bit operating system featuring a graphical user interface and file management capabilities.

## Project Structure

```
LeOS/
├── boot/           # Bootloader and early initialization
├── kernel/         # Core kernel functionality
├── drivers/        # Hardware drivers
├── fs/             # File system implementation
├── gui/            # Graphical user interface
└── lib/            # System libraries
```

## Requirements

- NASM (Netwide Assembler)
- GCC cross-compiler for i386
- QEMU for testing
- Make

## Building

1. Install dependencies:
   ```bash
   sudo apt-get install nasm gcc-multilib qemu-system-i386 make
   ```

2. Build the system:
   ```bash
   make
   ```

3. Run in QEMU:
   ```bash
   make run
   ```

## Features

- 32-bit protected mode
- Graphical user interface
- File system support
- Basic memory management
- Hardware abstraction layer
- Device drivers

## Development Status

This project is currently in early development. Major components are being implemented. 
