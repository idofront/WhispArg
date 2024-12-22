# WhispArg
A lightweight, single-header C++ library for quickly parsing command-line arguments in throwaway or prototype code.

---

## 1. What is WhispArg?

**WhispArg** is a C++ command-line argument parsing library designed for “throwaway” code, offering a **lightweight** approach in a **single header** file.  
The name originates from “Whisper,” conveying the notion of something that quietly fades away once it’s no longer needed.

> **Name Origin**  
> - “Whisp” = A shortened form of “Whisper,” evoking a small, temporary presence.  
> - “Arg” = Short for “Arguments.”  

---

## 2. Features

- **Designed for Throwaway Code**  
  - Ideal for prototyping or test scripts, easily integrated and just as easily discarded when no longer required.
- **Single Header Only**  
  - Simply `#include "WhispArg.hpp"`—no external dependencies needed.
- **Focus on Essential Functionality**  
  - Parses basic command-line structures (key-value pairs, flags, `--help`) without advanced or complex subcommand features.
- **Simple API**  
  - Easy-to-understand argument definitions and straightforward error messages make it quick to get started.

> **Note**  
> WhispArg is intended for lightweight, temporary usage. For large-scale CLI needs, please consider more feature-rich libraries.

---

## 3. Installation

1. Download or copy `WhispArg.hpp` from the [WhispArg repository](https://github.com/yourname/WhispArg).  
2. Place `WhispArg.hpp` in the same directory as your project source code, or in any header include path of your choice.  
3. Include it in your code via `#include "WhispArg.hpp"`.

---

## 4. Sample Code

For examples and usage details, see the sample code located under `./main/` in the repository.

---

## 5. Concept & Usage Scenarios

WhispArg is optimized for scenarios like small test code, educational scripts, or prototypes. It is not intended for large-scale, long-term usage.

- **Quick Parameter Testing**  
- **Educational Sample Projects**  
- **Lightweight Command-Line Support for Test Code**

> This library is a simple, lightweight tool that does not support complex subcommands or intricate option structures.  
> **“Quick to implement, quick to discard.”** For more advanced command-line parsing needs, please use a more robust library.

---

## 6. Development Policy & Future Plans

- **Keep It Minimal**  
  - We will preserve its throwaway-oriented concept by keeping feature requests minimal.
- **Open-Source**  
  - GitHub issues and pull requests are welcome, but note that it is a simple utility and not intended to grow into a full-featured tool.

---

## 7. License

This project is licensed under the [MIT License](LICENSE). See the `LICENSE` file in the repository for details.

---

## 8. Summary

WhispArg is a lightweight, single-header C++ library for simple command-line argument parsing.  
If you need to quickly implement a small-scale CLI for a temporary or prototype project, give WhispArg a try.

**Enjoy your ephemeral CLI parsing with WhispArg!**
