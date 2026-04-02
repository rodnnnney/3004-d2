# HintonMarket - Deliverable 2

## Build & Run Instructions

1. Open `HintonMarket.pro` in Qt Creator
2. Build the project (Ctrl+B)
3. Run the application (Ctrl+R)

**Note:** The `hintonMarket.sqlite3` database file is automatically copied to the build directory on each build. If the database is not found, ensure the `.sqlite3` file is in the same directory as the `.pro` file.

## User Accounts

All accounts use the password: `password`

### Food Vendors
| Username  | Name           | Business Name         |
|-----------|----------------|-----------------------|
| fvendor1  | Alice Johnson  | Alice's Fresh Produce |
| fvendor2  | Bob Williams   | Bob's Bakery          |
| fvendor3  | Carol Davis    | Carol's Kitchen       |
| fvendor4  | Dan Miller     | Dan's Deli            |

### Artisan Vendors
| Username  | Name           | Business Name         |
|-----------|----------------|-----------------------|
| avendor1  | Eva Brown      | Eva's Pottery Studio  |
| avendor2  | Frank Garcia   | Frank's Woodcraft     |
| avendor3  | Grace Lee      | Grace's Jewelry       |
| avendor4  | Henry Wilson   | Henry's Candles       |

### Market Operator
| Username  | Name            |
|-----------|-----------------|
| operator  | Olivia Martinez |

### System Administrator
| Username  | Name           |
|-----------|----------------|
| admin     | Sam Thompson   |

## Features

### Vendor Features
- View available market weeks and stall availability
- Book a market stall (one active booking at a time)
- Cancel an existing booking
- Join a waitlist when stalls are full
- Remove self from waitlist
- Receive notifications when auto-booked from waitlist

### Market Operator Features
- Book a stall on behalf of a vendor
- Cancel a booking on behalf of a vendor
- Remove a vendor from a waitlist

### System Administrator Features
- View system overview (user count, booking count, waitlist count)
- View all registered users

## Business Rules
- Maximum 2 food vendor stalls and 2 artisan vendor stalls per market week
- Vendors can hold only one active booking at a time
- Waitlists are organized by vendor category (Food/Artisan) AND by market week
- Waitlists follow first-come, first-served (FIFO) order
- When a stall is cancelled, the first vendor on the matching waitlist is auto-booked and notified

## Technical Details
- Language: C++17
- Framework: Qt Widgets
- Database: SQLite (via Qt SQL module)
- Architecture: Layered (Models → Database → UI)
