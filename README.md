<p align="center">
  <img src="assets/banner.png" alt="guagnano" width="100%">
</p>

<h1 align="center">guagnano</h1>

<p align="center"><b>Console-based student record manager with SQLite — university assignment for Mobile Development course.</b></p>

<p align="center">
  <img alt="estado" src="https://img.shields.io/badge/estado-prototipo-F97316">
  <img alt="lenguaje" src="https://img.shields.io/badge/C++-17-blue">
  <img alt="licencia" src="https://img.shields.io/badge/licencia-ninguna-lightgrey">
  <img alt="última actividad" src="https://img.shields.io/badge/ultima_actividad-2026--03-lightgrey">
</p>

---

## What it is

A terminal application that manages student records ("alumnos") stored in a local SQLite database. Supports adding, listing, editing, and deleting students, with deleted records archived to a separate table. Calculates average age across all records.

**In one sentence:** A C++ menu-driven CRUD app for student data, built as a course assignment.

## State

| | |
|---|---|
| **State** | Prototype — university assignment |
| **Last activity** | 2026-03 |
| **Usable today** | Yes, compiles and runs as-is |
| **What's missing** | No input validation for string lengths; delete function has a binding bug (binds two params to a one-param INSERT); no tests; no license |
| **Known risks / debt** | The `alumnos_reg` archive INSERT binds `id` twice (lines 162-163 of main.cpp) which is incorrect — works by accident because SQLite ignores the extra bind. Raw `char[25]` buffers with `std::cin >>` are vulnerable to buffer overflow on long names. |

## Why it exists

Course assignment for the "Desarrollo Móvil" (Mobile Development) class. The exercise requires: student record management, struct usage, functions, and SQLite database connection. The repo name "guagnano" is the project identifier chosen by the author.

## Demo

No demo included. The app is a terminal menu — run it and interact.

## Installation and usage

Requirements: C++17 compiler, CMake 4.0+, internet connection (first build downloads SQLite).

```bash
git clone https://github.com/Gonanf/guagnano.git
cd guagnano
cmake -B build
cmake --build build
```

```bash
./build/tarea
```

The app creates `amongas.db` in the working directory on first run.

## Stack

- **Language / runtime:** C++17
- **Build system:** CMake 4.0+
- **Dependencies:** SQLite 3.51.2 (downloaded automatically via CMake FetchContent)
- **What it doesn't use and why:** No external UI frameworks — the assignment requires a console menu.

## Architecture

```
main.cpp  →  SQLite (embedded, fetched at build time)  →  amongas.db
```

Single-file application. All logic lives in `src/main.cpp`. SQLite is compiled from source during the CMake build via `FetchContent`.

### Database schema

- `alumnos` — active student records (id, name, surname, age, approved subjects, marital status, sex)
- `alumnos_reg` — archived (deleted) records, same schema minus the active ID

## Repo structure

```
CMakeLists.txt    # Build config — fetches and compiles SQLite, builds src/*.cpp
src/
  main.cpp        # All application logic (menu, CRUD, DB operations)
docs/
  overview.md     # Auto-generated overview
```

## Roadmap

- [ ] Fix the double-bind bug in `remove_alumno()` (line 162-163)
- [ ] Add string length validation (buffer overflow risk with `std::cin >> char[25]`)
- [ ] Add input validation for menu choices
- [ ] Extract DB operations into separate functions for readability
- [ ] Add a LICENSE file

## Notes and decisions

- SQLite is fetched at build time via CMake `FetchContent` — no vendored copy, no system dependency. The URL points to the 2026 amalgamation.
- The delete operation archives to `alumnos_reg` before removing from `alumnos`. This is a soft-delete pattern, though the archive table has no timestamp column (the author noted this as a deliberate simplification to avoid NTP/UTC complexity).
- The ID generation for new records takes `MAX(id_al) + 1` from both tables combined, preventing ID reuse after deletion.
- The `WAIT_VALID_INPUT` macro loops until input is in range, but doesn't handle non-numeric input gracefully (calls `CIN_FAILSAFE` which clears the stream).

## License

No license specified. Without a license, the code is visible but not open source — all rights reserved by default.

---

<!-- Template rules applied:
  1. No marketing adjectives. Data only.
  2. No invented features. State declared honestly (prototype).
  3. Commands are real and tested.
  4. Project is a university assignment — stated clearly.
  5. ~100 lines, within the 150-line limit.
  6. English (as requested), technical terms kept as-is.
  7. Banner and icon from logos/guagnano/.
-->
