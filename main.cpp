#include<iostream>
#define MAX_SIZE 100
using namespace std;

typedef struct {
    int pid;
    int burts_time;
    int priority;
} Process;

typedef struct {
    Process elements[MAX_SIZE];
    int front = -1;
    int rear = -1;
    int count = 0;
    bool finish = false;
} Queue;

int T = 20;
Process process[MAX_SIZE];
Queue queue[4];
int waiting_time[4][MAX_SIZE];

int process_time = 0;

void printProcesses(Process process[], int n);
void enqueue(Process process[], Queue queue[], int n);
int isEmpty(Queue *queue);
int isFull(Queue *queue);
void printQueue(Queue queue[]);
int RR(Queue &queue,int (*waiting_time)[MAX_SIZE]);
int SJF1(Queue &queue,int (*waiting_time)[MAX_SIZE]);
int SJF2(Queue &queue,int (*waiting_time)[MAX_SIZE]);
int FIFO(Queue &queue,int (*waiting_time)[MAX_SIZE]);

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    for (int i = 0; i < n; ++i) {
        cout << "Enter the process id : ";
        cin >> process[i].pid;
        cout << "Enter the process burst time : ";
        cin >> process[i].burts_time;
        cout << "Enter the priority(0,1,2,3) : ";
        cin >> process[i].priority;
        cout << "<----------------------------------->" << endl;
    }

    printProcesses(process, n);
    enqueue(process, queue, n);
    printQueue(queue);

    cout << "<------------------------- Scheduling ------------------------>" << endl;
    int current_queue = 0;
    while(true) {
        switch (current_queue) {
        case 0:
            cout << "<------------------------- Round Robin ------------------------>" << endl;
            process_time=RR(queue[current_queue],waiting_time);
            break;
        case 1:
            cout << "<------------------------- Shortest Job First ------------------------>" << endl;
            SJF1(queue[current_queue],waiting_time);
            break;
        case 2:
            cout << "<------------------------- Shortest Job First ------------------------>" << endl;
            SJF2(queue[current_queue],waiting_time);
            break;
        case 3:
            cout << "<------------------------- First In First Out ------------------------>" << endl;
            FIFO(queue[current_queue],waiting_time);
            break;
        }
        current_queue = (current_queue + 1) %4;

        bool Check_finished = true;
        for (int i =0 ; i<4;++i){
            if(!queue[i].finish){
                Check_finished = false;
                break;
            }
        }
        if(Check_finished)
            break;
    }
}

int isFull(Queue *queue) {
    return (queue->rear + 1) % MAX_SIZE == queue->front;
}

int isEmpty(Queue *queue) {
    return queue->front == -1;
}

void enqueue(Process process[], Queue queue[], int n) {
    for (int i = 0; i < n; i++) {
        if (i >= n || process[i].pid == -1)
            break;

        int priority = process[i].priority;
        if (isFull(&queue[priority])) {
            cout << "Queue " << priority << " overflow" << endl;
            continue;
        }
        if (isEmpty(&queue[priority])) {
            queue[priority].front = 0;
        }
        queue[priority].rear = (queue[priority].rear + 1) % MAX_SIZE;
        queue[priority].elements[queue[priority].rear] = process[i];
        queue[priority].count++;
    }
}

void printProcesses(Process process[], int n) {
    cout << "\tProcess id \tBurst time \tpriority\n";
    for (int i = 0; i < n; i++) {
        cout << "\t\t" << process[i].pid << "\t\t" << process[i].burts_time << "\t" << process[i].priority << endl;
    }
}

void printQueue(Queue queue[]) {
    for (int i = 0; i < 4; ++i) {
        cout << "Queue" << i << ":" << endl;
        if (isEmpty(&queue[i])) {
            cout << "Empty:(" << endl;
            continue;
        }
        int front = queue[i].front;
        int rear = queue[i].rear;
        cout << "Pid\tburst time\tpriority" << endl;
        while (front != rear) {
            cout << queue[i].elements[front].pid << "\t " << queue[i].elements[front].burts_time << "\t " << queue[i].elements[front].priority << endl;
            front = (front + 1) % MAX_SIZE;
        }
        cout << queue[i].elements[rear].pid << "\t " << queue[i].elements[rear].burts_time << "\t " << queue[i].elements[rear].priority << endl;
    }
}

int RR(Queue &queue,int (*waiting_time)[MAX_SIZE]) {
    waiting_time[0][queue.count];
    Process re_bt[queue.count], re_tat[queue.count];
    int n = queue.count;

    for (int i = 0; i < n; ++i) {
        re_bt[i] = queue.elements[i];
        re_tat[i] = queue.elements[i];
    }

    int t = 0;
    while (1) {
        bool state = true;
        for (int i = 0; i < n; i++) {
            if (re_bt[i].burts_time > 0) {
                state = false;
                if (re_bt[i].burts_time > T) {
                    t += T;
                    re_bt[i].burts_time -= T;
                }
                else {
                    t += re_bt[i].burts_time;
                    waiting_time[0][i] = t - queue.elements[i].burts_time;
                    re_bt[i].burts_time = 0;
                }
            }
        }
        queue.finish = true;
        if (state == true)
            break;
    }

    int tat[n];
    for (int i = 0; i < n; ++i) {
        tat[i] = waiting_time[0][i] + re_tat[i].burts_time;
    }

    cout << "Queue 0 > " << endl;
    cout << "PID\tWaiting time\tTourn Around time\n";
    for (int i = 0; i < n; i++) {
        cout << queue.elements[i].pid << "\t" << waiting_time[0][i] << "\t\t" << tat[i] << endl;
    }

    for (int i =0;i<n;i++){
        process_time += re_tat[i].burts_time; 
    }
    //cout << process_time << endl;
    return process_time;
}

int SJF1(Queue &queue,int (*waiting_time)[MAX_SIZE]) {
    Process k, p;
    int n = queue.count;
    waiting_time[1][n];

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (queue.elements[i].burts_time > queue.elements[j].burts_time) {
                k = queue.elements[i];
                queue.elements[i] = queue.elements[j];
                queue.elements[j] = k;
            }
        }
    }

    //int t = 0;
    waiting_time[1][0] = process_time;

    int tot_wt = 0;
    for (int i = 0; i < n; i++) {
        tot_wt += queue.elements[i].burts_time;
        waiting_time[1][i + 1] = tot_wt+waiting_time[0][i]+process_time;
    }

    queue.finish = true;
    cout << "Queue 1 > " << endl;
    cout << "PID\tWaiting time\n";
    for (int i = 0; i < n; i++) {
        cout << queue.elements[i].pid << "\t" << waiting_time[1][i] << endl;
    }
    for (int i =0;i<n;i++){
        process_time += queue.elements->burts_time; 
    }
    //cout << process_time;
    return process_time;
    
}

int SJF2(Queue &queue,int (*waiting_time)[MAX_SIZE]) {
    Process k, p;
    int n = queue.count;
    waiting_time[2][n];

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (queue.elements[i].burts_time > queue.elements[j].burts_time) {
                k = queue.elements[i];
                queue.elements[i] = queue.elements[j];
                queue.elements[j] = k;
            }
        }
    }

    //int t = 0;
    waiting_time[2][0] = process_time;

    int tot_wt = 0;
    for (int i = 0; i < n; i++) {
        tot_wt += queue.elements[i].burts_time;
        waiting_time[2][i + 1] = tot_wt+waiting_time[0][i]+waiting_time[1][i]+process_time;
    }

    queue.finish = true;

    cout << "Queue 2 > " << endl;
    cout << "PID\tWaiting time\n";
    for (int i = 0; i < n; i++) {
        cout << queue.elements[i].pid << "\t" << waiting_time[2][i] << endl;
    }
    for (int i =0;i<n;i++){
        process_time += queue.elements->burts_time; 
    }
    //cout << process_time;
    return process_time;
}

int FIFO(Queue &queue,int (*waiting_time)[MAX_SIZE]) {
    int n = queue.count;
    waiting_time[3][n];

    

    int tat[n];
    for (int i = 0; i < n; i++) {
        tat[i] = queue.elements[i].burts_time + waiting_time[3][i];
    }

    waiting_time[3][0] = process_time;
    for (int i = 1; i < n; i++)
        waiting_time[3][i] = queue.elements[i - 1].burts_time + waiting_time[3][i - 1] + waiting_time[0][i-1] + waiting_time[1][i-1] + waiting_time[2][i-1]+process_time;


    queue.finish = true;
    int t = 0;
    cout << "Queue 3 > " << endl;
    cout << "PID\tWaiting time\tTourn Around time\n";
    for (int i = 0; i < n; i++) {
        cout << queue.elements[i].pid << "\t" << waiting_time[3][i] << "\t\t" << tat[i] << endl;
    }
    for (int i = 0; i < n; i++) {
        t += tat[i];
    }
    for (int i =0;i<n;i++){
        process_time += queue.elements->burts_time; 
    }
    //cout << process_time;
    return process_time;
}
