#include <iostream>
#include <iomanip>

void qsort(double** a, int l, int r) {
    int i = l;
    int j = r;
    double m = a[(l + r) / 2][0];
    while (i <= j) {
        while (a[i][0] < m)
            i++;
        while (a[j][0] > m)
            j--;
        if (i <= j) {
            double t = a[i][1];
            a[i][1] = a[j][1];
            a[j][1] = t;
            t = a[i][0];
            a[i][0] = a[j][0];
            a[j][0] = t;
            i++;
            j--;
        }
    }
    if (i < r)
        qsort(a, i, r);
    if (j > l)
        qsort(a, l, j);
}

void swap(int& a, int& b) {
    int t = a;
    a = b;
    b = t;
}

int getRandExceptFor(int ex, int n) {
    if (ex != -1) {
        int* list = new int[n - 1];
        for (int i = 0; i < n; i++) {
            if (i < ex)
                list[i] = i;
            if (i > ex)
                list[i - 1] = i;
        }
        return list[rand() % (n - 1)];
    }
    return rand() % n;
}

void deleteMin(int** a, int n, int m) {
    int imin = 0;
    for (int i = 1; i < n; i++)
        if (a[imin][m] > a[i][m])
            imin = i;
    n--;
    for (int i = imin; i < n; i++)
        for (int j = 0; j < m + 2; j++)
            a[i][j] = a[i + 1][j];
}

void localUpgrade(int* a, int* w, int* v, int n, int p) {
    double** wvRatio = new double*[n];
    for (int i = 0; i < n; i++)
        wvRatio[i] = new double[2];
    for (int i = 0; i < n; i++) {
        wvRatio[i][0] = w[i] / (double)v[i];
        wvRatio[i][1] = i;
    }
    qsort(wvRatio, 0, n - 1);
    int i = 0;
    while (i < n && (a[(int)wvRatio[i][1]] == 1 || (w[(int)wvRatio[i][1]] + a[n + 1] > p)))
        i++;
    if (i < n) {
        i = (int)wvRatio[i][1];
        a[i] = 1;
        a[n] = a[n] + v[i];
        a[n + 1] = a[n + 1] + w[i];
    }
    for (int i = 0; i < n; i++)
        delete[] wvRatio[i];
    delete[] wvRatio;
}

void copyArr(int* a, int* b, int n) {
    for (int i = 0; i < n; i++)
        b[i] = a[i];
}

int main() {
    srand(time(0));
    int** a = new int*[1000];
    int n = 100;
    int p = 150;
    int* weight = new int[n];
    int* val = new int[n];
    for (int i = 0; i < n; i++) {
        weight[i] = rand() % 4 + 1;
        val[i] = rand() % 8 + 2;
    }
    std::cout << "Weights:" << std::endl;
    for (int i = 0; i < n; i++)
        std::cout << weight[i] << " ";
    std::cout << std::endl;
    std::cout << "Values:" << std::endl;
    for (int i = 0; i < n; i++)
        std::cout << val[i] << " ";
    std::cout << std::endl;
    for (int i = 0; i < 1000; i++)
        a[i] = new int[n + 2];
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < n + 2; j++)
            a[i][j] = 0;
        if (i < n) {
            a[i][n - i - 1] = 1;
            a[i][n] = val[n - i - 1];
            a[i][n + 1] = weight[n - i - 1];
        }
    }
    int step = 0;
    int bestSolution = 0;
    for (int i = 1; i <= n; i++)
        if (a[i][n] > a[bestSolution][n])
            bestSolution = i;
    do {
        int j = getRandExceptFor(bestSolution, n);
        int* f = new int[n + 2];
        f[n] = 0;
        f[n + 1] = 0;
        for (int i = 0; i < n; i++) {
            int rnd = rand() % 2;
            if (rnd == 0)
                f[i] = a[bestSolution][i];
            else
                f[i] = a[j][i];
            f[n] += val[i] * f[i];
            f[n + 1] += weight[i] * f[i];
        }
        if (f[n + 1] <= p) {
            int l = getRandExceptFor(-1, n);
            int r = getRandExceptFor(l, n);
            double rnd = rand() % 100;
            if (rnd < 10 && f[l] != f[r]) {
                if (f[l] == 0 && f[r] == 1 && f[n + 1] - weight[r] + weight[l] <= p) {
                    swap(f[l], f[r]);
                    f[n] = f[n] - val[r] + val[l];
                    f[n + 1] = f[n + 1] - weight[r] + weight[l];
                }
                else {
                    if (f[l] == 1 && f[r] == 0 && f[n + 1] - weight[l] + weight[r] <= p) {
                        swap(f[l], f[r]);
                        f[n] = f[n] - val[l] + val[r];
                        f[n + 1] = f[n + 1] - weight[l] + weight[r];
                    }
                }
            }
            localUpgrade(f, weight, val, n, p);
            for (int i = 0; i < n + 2; i++)
                a[n][i] = f[i];
            deleteMin(a, n + 1, n);
        }
        delete[] f;
        bestSolution = 0;
        for (int i = 1; i <= n; i++)
            if (a[i][n] > a[bestSolution][n])
                bestSolution = i;
        step++;
    } while (step <= 1000);
    std::cout << "Best chromosome:" << std::endl;
    for (int i = 0; i < n + 2; i++)
        std::cout << a[bestSolution][i] << " ";
    return 0;
}