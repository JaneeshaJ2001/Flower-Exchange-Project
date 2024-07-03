# Flower-Exchange-Project

This repository hosts an efficient C++ trading application designed for a flower exchange system. The application processes incoming orders against existing orders in the Order Book, executing full or partial trades and generating an Execution Report for each order.

In contrast to a socket-based architecture, this project employs a straightforward approach where the trader and exchange components communicate via direct function calls and method invocations.

## To get started

1. Clone the repository:

   ```bash
   git clone https://github.com/JaneeshaJ2001/Flower-Exchange-Project.git

2. Run the executable file of application.cpp
    ```bash
    .\application.exe 

## Input and Output

Input parameters are provided via a Comma-Separated Values (CSV) file in the absence of a graphical user interface (GUI). The Execution Report is meticulously logged in another CSV file.

## Implementation Details

- Object-oriented approach is central to the implementation.
- Key components include structures and a main class (Order and OrderBook).

## Optimization techniques:

- Initial use of a hashmap to streamline order book creation.
- Utilization of a priority_queue for efficient order management.
- Vector-based aggregation of execution reports to minimize system calls.
