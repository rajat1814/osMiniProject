# osMiniProject
A simple and interactive CPU Scheduling Simulator built in C.
This project demonstrates how different CPU scheduling algorithms work by allowing you to input processes and see how each algorithm handles them in real time.

Perfect for students, OS lab work, mini-projects, and anyone learning how schedulers behave under different conditions.

Features

 First Come First Serve (FCFS)
 Shortest Job First — Non-Preemptive
 Shortest Remaining Time First (SJF Preemptive)
 Priority Scheduling (Non-Preemptive)
 Round Robin (with custom time quantum)

 Automatic calculation of:

Completion Time

Turnaround Time

Waiting Time

Average TAT & WT

 How It Works

Enter the number of processes.

Provide Arrival Time, Burst Time, and Priority for each process.

Choose a scheduling algorithm.

Instantly view the scheduling results in a clean tabular format.

Switch between algorithms easily without re-entering processes.

 Compilation & Execution
gcc -o cpu_scheduler cpu_scheduler.c
./cpu_scheduler


Works on Linux, macOS (with GCC), and Windows (using MinGW/WSL).

 Project Structure
cpu_scheduler.c   --> Main source file
README.md          --> Project documentation



Pick an algorithm and you’ll see calculated CT, TAT, WT, and averages.

Why This Project?

This project was created to make CPU scheduling easy to visualize and understand. Instead of reading dry theory, you can actually run the algorithms, compare them, and build intuition about how schedulers impact process performance.

Contributing

Feel free to fork, improve, or add new algorithms (like Priority Preemptive or Multilevel Queue).
PRs and suggestions are always welcome!

📜 License

This project is open-source under the MIT License.

Just tell me!
