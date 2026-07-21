# ConsoleApplication3 — Bank Client Management System

![Language](https://img.shields.io/badge/language-C%2B%2B-00599C)
![Build System](https://img.shields.io/badge/build-Visual%20Studio%20(MSBuild)-5C2D91)
![Platform](https://img.shields.io/badge/platform-Windows-0078D6)
![License](https://img.shields.io/badge/license-Not%20specified-lightgrey)

A console-based bank client management system with multi-user login and permission-based access control, built in C++.

## Overview

`ConsoleApplication3` is a text/console C++ application that manages bank clients and their accounts. It supports full CRUD operations on client records (show, add, delete, update, find), basic banking transactions (deposit and withdraw), and a separate user-management system with login authentication and granular, bitmask-based permissions controlling which menu operations each user is allowed to perform. All data (clients and users) is persisted to local delimited text files rather than a database.

## Features

- **User login system**: on startup, the user must authenticate with a username and password (validated against stored user records) before the main menu is shown; login retries until valid credentials are entered.
- **Permission-based access control**: each user has a `permissions` bitmask covering List Clients, Add Client, Delete Client, Update Client, Find Client, Transactions, and Manage Users; a special "all permissions" value grants unrestricted access. Every main-menu action checks the current user's permissions and shows "Access Denied" if the required bit is not set.
- **Client management**:
  - **Show Clients** — lists all clients in a formatted table (account number, pin code, name, phone, balance).
  - **Add Client** — prompts for account number (checked for uniqueness), pin code, name, phone, and starting balance, and appends the new record to the clients file; supports adding multiple clients in a row.
  - **Delete Client** — looks up a client by account number, confirms with the user, and marks the record for deletion (soft-delete flag) before rewriting the file to exclude it.
  - **Update Client** — looks up a client by account number and lets the user re-enter pin code, name, phone, and balance.
  - **Find Client** — looks up and displays a single client by account number.
- **Transactions menu**:
  - **Deposit** — adds a user-entered amount to a client's balance.
  - **Withdraw** — subtracts a user-entered amount from a client's balance, re-prompting until the amount does not exceed the current balance.
  - **Total Balance** — menu option present (clears the screen) but not yet implemented (see [Known Limitations](#known-limitations)).
- **User management menu** (itself permission-gated):
  - **List Users** — lists all stored users in a formatted table (username, password).
  - **Add New User** — prompts for username, password, and a yes/no answer per permission category, computing the resulting permissions bitmask, then appends the record to the users file.
  - **Delete User** — looks up a user by username, confirms, and marks the record for deletion before rewriting the file to exclude it.
  - **Update User** — looks up a user by username and lets the user re-enter the password.
  - **Find User** — looks up and displays a single user by username.
- **File-based persistence**: client records are stored in `MyFile2.txt` and user records in `B.txt`, using a custom `#//#` field delimiter, with simple load/save/append operations on each menu action.
- **Console screen reset**: a `ResetScreen()` helper clears the console and resets its color between menu screens using Windows-specific `system()` calls.
- **Logout**: exits the application from the main menu.

## Screenshots

_No screenshots are included in the provided project files. This is a text-based console application._

## Demo

Not specified.

## Tech Stack

| Category | Technology |
|---|---|
| Language | C++ |
| Standard Library | `<iostream>`, `<fstream>`, `<string>`, `<vector>`, `<iomanip>` |
| Build System / IDE | Visual Studio 2022 (Solution/Project files: `.sln`, `.vcxproj`) |
| Toolset | MSVC `v143` platform toolset |
| Character Set | Unicode |
| Target Platforms | Win32 and x64 (Debug and Release configurations) |
| Application Type | Windows Console Application |
| Data Storage | Local plain-text files (`MyFile2.txt`, `B.txt`) with a custom `#//#` delimiter — no database engine |

## Architecture

The application is a single-file, procedural C++ console program (`ConsoleApplication3.cpp`) with no classes — all state is modeled using plain structs, and behavior is implemented as a large set of free functions organized under comment-delimited sections (`BASIC`, `CONVERSION`, `STRING UTIL`, `FILE`, `DISPLAY`, `LOGIC`, `ADD CLIENT`, `DELETE HELPERS`, `UPDATE`, `FIND`, `TRANSACTIONS`, `USER CONVERSION`, `LOAD USERS`, `DISPLAY USERS`, `ADD USER`, `DELETE USER HELPERS`, `DELETE USER`, `ENUM PERMISSIONS`, `MANAGE USERS MENU`, `CHECK LOGIN`, `LOGIN`, `MAIN MENU`).

**Data structures:**
- `stClient` — a bank client's account number, pin code, name, phone, balance, and a soft-delete flag (`markForDelete`).
- `stUser` — a system user's username, password, integer permissions bitmask, and a soft-delete flag.
- `sUsers` — an alternate, smaller user struct (username + permissions) that is declared but not used anywhere in the provided code (see [Known Limitations](#known-limitations)).
- `enMainMenuOptions` / `enTransactionsMenuOptions` — enums naming the numbered main-menu and transactions-menu choices.
- `enPermissions` — a bitmask enum (`pListClients = 1`, `pAddClient = 2`, `pDeleteClient = 4`, `pUpdateClient = 8`, `pFindClient = 16`, `pTransactions = 32`, `pManageUsers = 64`, `pAll = -1`) used to gate access to each menu action.

**Core functions:**
- `ConvertClientToLine` / `ConvertLineToClient` and `ConvertUserToLine` / `ConvertLineToUser` — serialize/deserialize client and user records to/from the `#//#`-delimited text file format.
- `SplitString` — a generic string-splitting utility used when parsing lines read from the data files.
- `LoadClientsFromFile` / `SaveClientsToFile` and `LoadUsersFromFile` / `SaveUsersToFile` — read all records from, and rewrite all (non-deleted) records to, the respective text files.
- `IsAccountExist`, `GetClientByAccountNumber`, `MarkClientForDeletion` — client lookup/mutation helpers, mirrored for users by `GetUserByUsername` and `MarkUserForDeletion`.
- `PrintClientRecord` / `PrintAllClientsData` and `PrintUserRecord` / `PrintAllUsers` — formatted console table output.
- `AddNewClient`, `DeleteClient`, `UpdateClientInfo`, `FindClient` — top-level client CRUD flows driven by console prompts.
- `Deposit` / `DepositAmount` and `Withdraw` / `WithdrawAmount` — transaction flows for adjusting a client's balance.
- `AddNewUser` / `ReadNewUserData`, `DeleteUser`, `UpdateUser` / `UpdateUserData`, `FindUser` — top-level user CRUD flows, including the permission-selection prompts used when creating a user.
- `CheckLogin`, `Login`, `GetCurrentUser` — authenticate a username/password pair against the stored users file and load the authenticated user's full record into the global `CurrentUser`.
- `ShowTransactionsMenu`, `ManageUsersMenu`, `SeeAllMenue` (main menu, note the typo) — menu-loop functions that print options, read a numeric choice, and dispatch to the relevant action (each gated by `CurrentUser.permissions` where applicable).
- `main()` — logs the user in, resets the screen, and enters the main menu loop.

### Notable implementation details

- Access control is enforced per-action inside `SeeAllMenue()` by checking `CurrentUser.permissions == pAll || (CurrentUser.permissions & p<Action>)` before performing each menu action.
- "Deletion" is implemented as a soft-delete: `markForDelete` is set to `false` for the record to keep, then `SaveClientsToFile`/`SaveUsersToFile` only re-writes records where `markForDelete == true` — meaning the flag's default value (`true`, set at struct definition) means "keep," and it is set to `false` to mark a record as excluded on the next save.
- Console color/screen changes rely on Windows-specific `system("cls")` / `system("color 0F")` / `system("pause>0")` calls, making the program Windows-only as written.
- Filenames are inconsistent: client data uses the named constant `CLIENTS_FILE_NAME` (`"MyFile2.txt"`), while all user-file operations use the hard-coded literal `"B.txt"` directly instead of a similar named constant.
- Comments in the source are a mix of English and Arabic (e.g., explanatory notes on `GetCurrentUser`'s case-sensitive field lookup, and on `sUsers.Permissions`).

## Project Structure

```
ConsoleApplication3/
├── ConsoleApplication3.sln    # Visual Studio solution file
├── ConsoleApplication3.vcxproj # MSBuild/Visual Studio C++ project file
└── ConsoleApplication3.cpp    # Single-file source: all client/user management logic and console I/O
```

> **Note:** Only the solution, project, and single `.cpp` source file were provided. Standard companion files for a Visual Studio C++ project (e.g., `.vcxproj.filters`, `.vcxproj.user`) were not included. The data files the application reads/writes at runtime (`MyFile2.txt`, `B.txt`) are created/managed by the application itself and are not part of the source project.

## Requirements

- Windows OS (the program uses Windows-specific `system("cls")`, `system("color ...")`, and `system("pause>0")` calls).
- Visual Studio 2022 (or compatible), with the **Desktop development with C++** workload, since the project targets the `v143` platform toolset.
- No external libraries or third-party dependencies — only the C++ standard library is used.
- At least one user record must exist in the `B.txt` users file before the application can be logged into, since `Login()` loops until valid credentials are entered and there is no built-in default/admin account or first-run setup flow.

## Installation

1. Clone or download the repository.
2. Open `ConsoleApplication3.sln` in Visual Studio 2022.
3. Select a configuration/platform (`Debug` or `Release`; `Win32` or `x64`).
4. Build the solution (**Build → Build Solution**, or `Ctrl+Shift+B`).

Alternatively, from a **Developer Command Prompt for VS 2022**, you can build via MSBuild:

```
msbuild "ConsoleApplication3.sln" /p:Configuration=Release /p:Platform=x64
```

## Configuration

Not specified via configuration files — the two data file names are hard-coded in the source (`MyFile2.txt` for clients via the `CLIENTS_FILE_NAME` constant, and `B.txt` for users via a hard-coded string literal). No environment variables or external config files are used.

## Running the Project

Run the built executable (e.g., `ConsoleApplication3.exe` from the `x64\Release` or `x64\Debug` output folder) from a console/terminal, or press `F5`/`Ctrl+F5` in Visual Studio to build and run directly.

Because the application requires an existing user account to log in, and there is no seeded/default user, a user record must first be present in `B.txt` in the application's working directory (formatted as `username#//#password#//#permissions`) before the application can be used interactively.

Once running:
1. Enter a username and password at the login prompt (repeats until valid).
2. Use the numbered main menu to show/add/delete/update/find clients, perform transactions, manage users, or log out.
3. Follow the on-screen prompts for each selected action.

## Development

Open `ConsoleApplication3.sln` in Visual Studio and edit `ConsoleApplication3.cpp` directly; there is a single source file with no additional modules.

## Build

Build via Visual Studio (**Build Solution**) or MSBuild as shown in [Installation](#installation). Four build configurations are defined: `Debug|Win32`, `Debug|x64`, `Release|Win32`, and `Release|x64`.

## Testing

Not specified. No test project or testing framework is referenced in the provided files.

## API Documentation

Not applicable. This is a standalone console application with no exposed APIs or network endpoints.

## Database

No database engine is used. Persistence is implemented via two plain-text, delimiter-separated files:

| File | Purpose | Format |
|---|---|---|
| `MyFile2.txt` | Client records | One client per line: `accountNumber#//#pinCode#//#name#//#phone#//#accountBalance` |
| `B.txt` | User records | One user per line: `userName#//#password#//#permissions` |

Both files are fully re-read into memory and fully re-written on most operations (no incremental/indexed access), and "deleted" records are simply omitted when the file is next saved.

## Deployment

Not specified. No Docker, CI/CD, or packaging/installer configuration was provided. The application is deployed by distributing the compiled `.exe` (and ensuring the working directory contains, or can create, the `MyFile2.txt`/`B.txt` data files).

## Security Notes

- **Plain-text password storage**: user passwords are stored and compared as plain text in `B.txt`, with no hashing, salting, or encryption.
- **Plain-text client PINs**: client `pinCode` values are likewise stored as plain text in `MyFile2.txt` and are not used anywhere in the code to authorize client-specific actions.
- **No input sanitization**: the custom `#//#` delimiter is used for both parsing and serialization; a user-entered value containing that exact substring could corrupt the record format (not validated against in the provided code).
- **Local-only, single-user-session design**: there is no session management, encryption in transit, or protection against concurrent file access from multiple running instances.
- Access control (permissions bitmask) governs which menu actions are available to a logged-in user, but this is an application-level authorization scheme only, not a security boundary against direct file access.

## Performance Notes

Not applicable. This is a lightweight, synchronous console application; each operation loads and (if needed) rewrites the entire relevant data file, which is adequate for small datasets but would not scale well to a very large number of client or user records.

## Known Limitations

- **`Total Balance` transaction menu option is unimplemented**: selecting it in `ShowTransactionsMenu()` only clears the screen and does nothing further.
- **No seeded/default admin user**: since `Login()` requires an existing match in `B.txt`, there is no way to access the system on a fresh install without a pre-existing user record (created some other way, e.g., manually).
- **`sUsers` struct is declared but unused**: the codebase defines two different user-representation structs (`sUsers` and `stUser`); only `stUser` is actually used.
- **Inconsistent file-name handling**: `MyFile2.txt` uses a named constant while `B.txt` is hard-coded as a string literal in multiple places.
- **Windows-only**: relies on `system("cls")`, `system("color ...")`, and `system("pause>0")`, which are not portable to other operating systems.
- **Recursive menu navigation**: `SeeAllMenue()` calls itself recursively after every action instead of using a loop, which could eventually exhaust the call stack during a very long session.
- **No error handling for malformed/corrupt data files**: `ConvertLineToClient`/`ConvertLineToUser` assume a fixed number of `#//#`-delimited fields and will throw/crash on malformed lines (e.g., via out-of-range vector access or failed `stod`/`stoi` conversions).
- **Plain-text password/PIN storage** (see [Security Notes](#security-notes)).
- No automated tests are present.

## Future Improvements

- Implement the "Total Balance" transactions menu option.
- Add a first-run/setup flow to create an initial admin user if `B.txt` is empty or missing.
- Remove the unused `sUsers` struct, or consolidate it with `stUser`.
- Replace the hard-coded `"B.txt"` literal with a named constant, matching the pattern used for `CLIENTS_FILE_NAME`.
- Replace recursive menu navigation (`SeeAllMenue()` calling itself) with an iterative loop.
- Add validation and error handling around file parsing and numeric conversions.
- Hash and salt stored passwords instead of storing them in plain text.
- Replace Windows-specific `system()` calls with a cross-platform approach to support non-Windows builds.
- Add unit tests for the client/user CRUD and permission-checking logic.

## Contributing

Contributions are welcome. To contribute:

1. Fork the repository.
2. Create a feature branch (`git checkout -b feature/my-feature`).
3. Make your changes and commit them with clear messages.
4. Push to your fork and open a Pull Request describing your changes.

Please keep changes consistent with the existing procedural C++ style used in this project.

## License

Not specified. No license file was included in the provided project files.

## Author

Baraa Mardini
