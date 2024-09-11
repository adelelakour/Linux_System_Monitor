This project is a Linux system monitoring tool built using C++ and the NCurses library. It displays real-time system information such as CPU usage, memory usage, running processes, and more. The tool provides a command-line interface similar to popular system monitoring tools like htop or top.


**Features**

* Real-Time Monitoring: Get updated statistics on CPU usage, memory consumption, and running processes.
* CPU Usage: Displays the overall CPU usage and per-core statistics.
* Memory Usage: Shows the total and used memory.
* Process List: Displays information about running processes, including PID, user, CPU usage, RAM usage, uptime, and command.
* NCurses Interface: Uses NCurses to create a user-friendly terminal interface with color support.

**Dependencies**

This project relies on the following dependencies:

    NCurses: Used for creating the user interface.
    C++17: Required for the `filesystem` header and other modern C++ features.
