#include <iostream>
#include <vector>
#include <cstdio>
using namespace std;

struct Resource {
    char name[10];
    int n;
};

int *maxn, *alloc, *need;
Resource *avai;
int m, n;

void dispose() {
    delete [] avai;
    delete [] maxn;
    delete [] alloc;
    delete [] need;
}

void inputResources() {
    cout << "Resource Count:" << endl;
    cin >> m;
    avai = new Resource[m];
    for (int i = 0; i < m; ++i) {
        cout << "Resource " << i << " Name and Available Count:" << endl;
        cin >> avai[i].name >> avai[i].n;
    }
}

void inputJobs() {
    cout << "Job Count:" << endl;
    cin >> n;
    maxn = new int[n * m];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << "Job " << i << " Max Need:" << endl;
            cin >> maxn[i * m + j];
        }
    }
}

void inputAlloc() {
    cout << "Allocation Matrix: " << endl;
    alloc = new int[n * m];
    bool ok = false;
    while (!ok) {
        ok = true;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                cin >> alloc[i * m + j];
                if (alloc[i * m + j] > maxn[i * m + j]) {
                    printf("Invalid Request (Request %d %d > Max Need %d %d)\n", i, j, i, j);
                    ok = false;
                }
            }
        }
    }

    need = new int[n * m];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            need[i * m + j] = maxn[i * m + j] - alloc[i * m + j];
        }
    }
}

void printInfo() {
    cout << "------------" << endl;
    cout << "=== Avai ===" << endl;
    for (int j = 0; j < m; ++j) {
        cout << avai[j].name << " avai: " << avai[j].n << endl;
    }
    cout << "=== Alloc ===" << endl;
    for (int i = 0; i < n; ++i) {
        cout << "Job " << i << ": \t";
        for (int j = 0; j < m; ++j) {
            cout << avai[j].name << ": " << alloc[i * m + j] << "\t";
        }
        cout << endl;
    }
    cout << "=== Need ===" << endl;
    for (int i = 0; i < n; ++i) {
        cout << "Job " << i << ": \t";
        for (int j = 0; j < m; ++j) {
            cout << avai[j].name << ": " << need[i * m + j] << "\t";
        }
        cout << endl;
    }
}

bool check() {
    cout << "------------" << endl;
    int *work = new int[m];
    char *finish = new char[n];

    for (int j = 0; j < m; ++j) {
        work[j] = avai[j].n;
    }
    for (int i = 0; i < n; ++i) {
        finish[i] = 0;
    }
    
    bool found = true;
    while (found) {
        found = false;
        for (int i = 0; i < n; ++i) {
            if (finish[i]) continue;
            int j;
            for (j = 0; j < m; ++j) {
                if (need[i * m +j] > work[j]) break;
            }
            if (j == m) {
                found = true;
                finish[i] = 1;
                cout << "=== Work ===" << endl;
                for (j = 0; j < m; ++j) {
                    work[j] += alloc[i * m + j];
                    cout << work[j] << endl;
                }
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        if (finish[i] == 0) {
            found = true;
            break;
        }
    }

    delete [] finish;
    delete [] work;
    return !found;
}

void request() {
    int i;
    int *r = new int[m];

    cout << "------------" << endl;
    cout << "Request Job: " << endl;   
    cin >> i;

    cout << "Request Resource Vector: " << endl;

    bool ok = true;
    for (int j = 0; j < m; ++j) {
        cin >> r[j];
        if (r[j] > need[i * m + j]) {
            printf("Invalid Request (Request %d %d > Need %d %d)\n", i, j, i, j);
            ok = false;
        }
        if (r[j] > avai[j].n) {
            printf("Please Wait... (Request %d %d > Avai %d)\n", i, j, j);
            ok = false;
        }
    }

    if (ok) {
        for (int j = 0; j < m; ++j) {
            avai[j].n -= r[j];
            alloc[i * m + j] += r[j];
            need[i * m + j] -= r[j];
        }
        bool safe = check();
        if (safe) {
            cout << "Successfully Requested (Safe Now)" << endl;
        } else {
            cout << "Failed to Request (Unsafe Now)" << endl;
            for (int j = 0; j < m; ++j) {
                avai[j].n += r[j];
                alloc[i * m + j] -= r[j];
                need[i * m + j] += r[j];
            }
        }
    }

    delete [] r;
}

int main() {
    inputResources();
    inputJobs();
    inputAlloc();
    printInfo();
    if (check()) {
        cout << "Safe Now" << endl;
    } else {
        cout << "Unsafe Now!!" << endl;
    }
    int req;
    cout << "Request Count: " << endl;
    cin >> req;
    for (int i = 0; i < req; ++i) {
        request();
        printInfo();
    }
    dispose();

    return 0;
}
