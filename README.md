# Finance Desk

A banking game for the Windows console, written in C++ to practice the language.

```text
Welcome to the Bank!
FinanceDesk v1
 $100 |                 _        /
  $90 |             ___/ \      /
  $80 |            /      \_   /
  $70 |       _   /         \_/
  $60 |   ___/ \_/
  $50 |__/
      |----------------------------
```

## Running

Open `FinanceDesk.sln` in Visual Studio 2022 and run `Debug | x64`. SQLite is included in the repo.

## Features

- You can create an account and log in.
- You can pay another account by username or account number.
- You can see every transaction your account has sent or received.

## How the code is organized

Headers are in `lib/` and implementations are in `src/`, in matching folders. `ApplicationPages` handles menus and input, `GameDatabaseManager` holds the SQL, and `SQLiteDB` wraps the SQLite C API in classes.
