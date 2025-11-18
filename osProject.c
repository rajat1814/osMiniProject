#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXP 100
#define INF 1000000000

typedef struct
{
  int pid;
  int at;
  int bt;
  int rt;
  int priority;
  int ct;
  int tat;
  int wt;
  int start;
} Process;

void swap_proc(Process *a, Process *b)
{
  Process t = *a;
  *a = *b;
  *b = t;
}

int compare_arrival(const void *a, const void *b)
{
  Process *p1 = (Process *)a;
  Process *p2 = (Process *)b;
  if (p1->at != p2->at)
    return p1->at - p2->at;
  return p1->pid - p2->pid;
}

void print_table(Process proc[], int n, char *hdr)
{
  int i;
  double tot_tat = 0, tot_wt = 0;
  printf("\n--- %s ---\n", hdr);
  printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
  for (i = 0; i < n; ++i)
  {
    tot_tat += proc[i].tat;
    tot_wt += proc[i].wt;
    printf("%d\t%d\t%d\t%d\t%d\t%d\n", proc[i].pid, proc[i].at, proc[i].bt, proc[i].ct, proc[i].tat, proc[i].wt);
  }
  printf("Average TAT = %.2f\tAverage WT = %.2f\n", tot_tat / n, tot_wt / n);
}

void fcfs(Process proc[], int n)
{
  int time = 0, i;
  for (i = 0; i < n; ++i)
  {
    if (time < proc[i].at)
      time = proc[i].at;
    proc[i].start = time;
    time += proc[i].bt;
    proc[i].ct = time;
    proc[i].tat = proc[i].ct - proc[i].at;
    proc[i].wt = proc[i].tat - proc[i].bt;
  }
  print_table(proc, n, "FCFS Scheduling");
}

void sjf_nonpreemptive(Process proc[], int n)
{
  int completed = 0, time = 0;
  int done[MAXP] = {0};
  while (completed < n)
  {
    int idx = -1;
    int min_bt = INF;
    for (int i = 0; i < n; ++i)
    {
      if (!done[i] && proc[i].at <= time)
      {
        if (proc[i].bt < min_bt)
        {
          min_bt = proc[i].bt;
          idx = i;
        }
      }
    }
    if (idx == -1)
    {
      time++;
      continue;
    }
    proc[idx].start = time;
    time += proc[idx].bt;
    proc[idx].ct = time;
    proc[idx].tat = proc[idx].ct - proc[idx].at;
    proc[idx].wt = proc[idx].tat - proc[idx].bt;
    done[idx] = 1;
    completed++;
  }
  print_table(proc, n, "SJF Non-preemptive");
}

void sjf_preemptive(Process proc[], int n)
{
  int completed = 0, time = 0;
  int done[MAXP] = {0};
  for (int i = 0; i < n; ++i)
    proc[i].rt = proc[i].bt;
  while (completed < n)
  {
    int idx = -1;
    int min_rt = INF;
    for (int i = 0; i < n; ++i)
    {
      if (!done[i] && proc[i].at <= time)
      {
        if (proc[i].rt < min_rt)
        {
          min_rt = proc[i].rt;
          idx = i;
        }
      }
    }
    if (idx == -1)
    {
      time++;
      continue;
    }
    proc[idx].rt--;
    time++;
    if (proc[idx].rt == 0)
    {
      proc[idx].ct = time;
      proc[idx].tat = proc[idx].ct - proc[idx].at;
      proc[idx].wt = proc[idx].tat - proc[idx].bt;
      done[idx] = 1;
      completed++;
    }
  }
  print_table(proc, n, "SJF Preemptive (SRTF)");
}

void priority_nonpreemptive(Process proc[], int n)
{
  int completed = 0, time = 0;
  int done[MAXP] = {0};
  while (completed < n)
  {
    int idx = -1;
    int best_pr = INF;
    for (int i = 0; i < n; ++i)
    {
      if (!done[i] && proc[i].at <= time)
      {
        if (proc[i].priority < best_pr)
        {
          best_pr = proc[i].priority;
          idx = i;
        }
      }
    }
    if (idx == -1)
    {
      time++;
      continue;
    }
    proc[idx].start = time;
    time += proc[idx].bt;
    proc[idx].ct = time;
    proc[idx].tat = proc[idx].ct - proc[idx].at;
    proc[idx].wt = proc[idx].tat - proc[idx].bt;
    done[idx] = 1;
    completed++;
  }
  print_table(proc, n, "Priority Non-preemptive (Lower value = higher priority)");
}

void round_robin(Process proc[], int n, int quantum)
{
  int time = 0;
  int remaining = n;
  for (int i = 0; i < n; ++i)
    proc[i].rt = proc[i].bt;
  int q[MAXP], front = 0, rear = 0;
  int inq[MAXP] = {0};
  for (;;)
  {
    for (int i = 0; i < n; ++i)
      if (!inq[i] && proc[i].at <= time && proc[i].rt > 0)
      {
        q[rear++] = i;
        inq[i] = 1;
      }
    if (front == rear)
    {
      int all_done = 1;
      for (int i = 0; i < n; ++i)
        if (proc[i].rt > 0)
        {
          all_done = 0;
          break;
        }
      if (all_done)
        break;
      time++;
      continue;
    }
    int idx = q[front++];
    int exec = proc[idx].rt < quantum ? proc[idx].rt : quantum;
    proc[idx].rt -= exec;
    time += exec;
    for (int i = 0; i < n; ++i)
      if (!inq[i] && proc[i].at <= time && proc[i].rt > 0)
      {
        q[rear++] = i;
        inq[i] = 1;
      }
    if (proc[idx].rt > 0)
      q[rear++] = idx;
    else
    {
      proc[idx].ct = time;
      proc[idx].tat = proc[idx].ct - proc[idx].at;
      proc[idx].wt = proc[idx].tat - proc[idx].bt;
    }
  }
  print_table(proc, n, "Round Robin");
}

void read_processes(Process proc[], int n)
{
  for (int i = 0; i < n; ++i)
  {
    proc[i].pid = i + 1;
    printf("Process %d: Arrival Time Burst Time Priority: ", proc[i].pid);
    scanf("%d %d %d", &proc[i].at, &proc[i].bt, &proc[i].priority);
    proc[i].rt = proc[i].bt;
    proc[i].ct = proc[i].tat = proc[i].wt = proc[i].start = 0;
  }
  qsort(proc, n, sizeof(Process), compare_arrival);
}

void copy_procs(Process src[], Process dest[], int n)
{
  for (int i = 0; i < n; ++i)
    dest[i] = src[i];
}

int main()
{
  Process original[MAXP];
  Process proc[MAXP];
  int n;
  printf("Enter number of processes: ");
  if (scanf("%d", &n) != 1 || n <= 0 || n > MAXP)
    return 0;
  read_processes(original, n);
  for (;;)
  {
    printf("\nChoose Algorithm:\n1. FCFS\n2. SJF Non-preemptive\n3. SJF Preemptive (SRTF)\n4. Priority Non-preemptive\n5. Round Robin\n6. Exit\nChoice: ");
    int ch;
    if (scanf("%d", &ch) != 1)
      break;
    if (ch == 6)
      break;
    copy_procs(original, proc, n);
    if (ch == 1)
      fcfs(proc, n);
    else if (ch == 2)
      sjf_nonpreemptive(proc, n);
    else if (ch == 3)
      sjf_preemptive(proc, n);
    else if (ch == 4)
      priority_nonpreemptive(proc, n);
    else if (ch == 5)
    {
      int q;
      printf("Enter time quantum: ");
      scanf("%d", &q);
      copy_procs(original, proc, n);
      round_robin(proc, n, q);
    }
    else
      printf("Invalid choice\n");
  }
  printf("Exiting\n");
  return 0;
}
