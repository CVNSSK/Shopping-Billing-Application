# QuickBill

QuickBill is a cross-platform, terminal-based billing application written in C.  

It supports admin and user logins, product management, bill generation, and maintains a history of transactions in the `data/` folder.


## Features

- **Admin Login**
  - Review bills
  - Change admin password
  - Edit billers

- **User Login**
  - Process customer bills
  - Automatically save bill details with date

- **Billing**
  - Prints professional receipt layout
  - Auto-increments bill/customer ID
  - Saves bills in `data/` as `MM_DD_YYYY.txt`

- **Product Management**
  - Products loaded from `data/products.csv`
  - Supports discounts, GST, and net cost calculation

- **Cross-Platform**
  - Works on macOS, Linux, and Windows
  - Uses platform-specific `getch()` for hidden password entry


## Project Structure

```
QuickBill/
├── build/               # Compiled object files (created locally, not in GitHub)
├── data/                # Data and runtime files
│   ├── products.csv     # Product list
│   ├── password.txt     # Login credentials
│   ├── cidupdate.txt    # Auto-generated bill counter
│   ├── MM_DD_YYYY.txt   # Auto-generated daily bills
├── src/                 # Source code
│   ├── main.c
│   ├── admin.c
│   ├── biller.c
│   ├── utils.c
│   └── quickbill.h
├── Makefile
├── README.md
└── .gitignore
```


## Getting Started

### Prerequisites
- **macOS / Linux**: `gcc` (or any C compiler), `make`
- **Windows**: MinGW or MSVC

### Build
```bash
make
```

This will compile the source files and generate the executable:

```bash
./quickbill        # macOS / Linux
quickbill.exe      # Windows
```

### Cleaning Build Files
```bash
make clean
```

This removes all compiled object files and the executable, keeping the workspace clean.

### Why Makefile?
We use a `Makefile` to:
- Automate the compilation process.
- Ensure consistent build commands across platforms.
- Avoid manually typing long `gcc` commands.
- Allow easy rebuilds and cleanup (`make`, `make clean`).

### Data Files
- `data/password.txt` - Stores credentials in `username,password` format.
- `data/products.csv` - Stores product list in CSV format.
- `data/cidupdate.txt` - Tracks the last bill/customer ID (auto-generated).
- `data/MM_DD_YYYY.txt` - Stores each day's bills (auto-generated).

### Login Defaults

| Role   | Username | Password   |
|:------:|:--------:|:----------:|
| Admin  | admin    | admin@123  |
| User   | 1001     | pass1      |
| User   | 1002     | pass2      |


## Why This Project?

Originally started as a **simple C-based billing system** for learning purposes, QuickBill was first developed and completed by *[Chandu Venkata Naga Sai Saran Kumar Velpuri](https://github.com/CVNSSK)* around **2022**, with initial input from *[Wineel Wilson Dasari](https://github.com/WINEEL)*.  

In **2025**, *[Wineel](https://github.com/WINEEL)* revived, cleaned, and updated the project to make it:
- Cross-platform (macOS, Linux, Windows)
- Easier to build using a Makefile
- More organized with separate modules for admin, billing, and utilities
- Equipped with cleaner file handling and modern C practices

It now serves as both a **learning project** for beginners in C and a **functional terminal-based billing system** that can be adapted for small shops or educational use.


## Future Improvements
- Add graphical interface for better usability  
- Enhance product search and filtering  
- Implement database backend (SQLite/MySQL) for scalability  


## License

This project is **open-source** and licensed under the **MIT License**.  
See the [LICENSE](LICENSE) file for details.  
Feel free to modify and improve upon it!


## Authors

**Initial Development (2022):**  
- *[Chandu Venkata Naga Sai Saran Kumar Velpuri](https://github.com/CVNSSK)* and *[Wineel Wilson Dasari](https://github.com/WINEEL)* - Started the project together.
- **[Chandu](https://github.com/CVNSSK)** completed the original working version.

**Recent Updates (2025):**  
- **[Wineel](https://github.com/WINEEL)** updated and enhanced the project with improvements to code quality, cross-platform compatibility, Makefile integration, and file handling, with valuable assistance from GPT.

