#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#define MAX 30

// deklarasi struct untuk single linkedlist
struct Node
{
    char hurufDepanPlat[MAX];
    int noPlat;
    char hurufBelakangPlat[MAX];
    char warnaKendaraan[MAX];
    int durasi;
    char golongan[MAX];
    struct Node *next;
};

// deklarasi struct untuk queue
struct Queue
{
    struct Node *front, *rear, *temp;
};

int count = 0;

void menu()
{
    printf("------------------------------------------------------------------\n");
    printf("          SISTEM PEMBAYARAN PARKIR KENDARAAN PERTOKOAN X          \n");
    printf("------------------------------------------------------------------\n");
    printf("1. Memasukkan data kendaraan baru ke dalam antrean pembayaran\n");     // terdapat dua pilihan, yaitu enqueue pada queue dan add front pada single linkedlist
    printf("2. Hitung biaya parkir, lalu hapus data ketika pembayaran selesai\n"); // dequeue pada queue
    printf("3. Tampilkan seluruh data antrean kendaraan\n");
    printf("4. Cari data kendaraan berdasarkan No Plat-nya\n");
    printf("5. Exit\n");
    printf("------------------------------------------------------------------\n");
}

int currentTanggal()
{
    time_t rawtime = time(NULL);
    struct tm *ptm = localtime(&rawtime);
    int tanggal = ptm->tm_mday;
    return tanggal;
}

int currentHari()
{
    time_t rawtime = time(NULL);
    struct tm *ptm = localtime(&rawtime);
    int hari = ptm->tm_wday;
    return hari;
}

int compareString(char value1[], char value2[])
{
    int result;
    result = strcmp(value1, value2);
    return result;
}

struct Node *createNode(char newPlatDepan[], int newPlatNo, char newPlatBelakang[], char newWarna[], int newDur, char newGol[])
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (newNode != NULL)
    {
        strcpy(newNode->hurufDepanPlat, newPlatDepan);
        newNode->noPlat = newPlatNo;
        strcpy(newNode->hurufBelakangPlat, newPlatBelakang);
        strcpy(newNode->warnaKendaraan, newWarna);
        newNode->durasi = newDur;
        strcpy(newNode->golongan, newGol);
        newNode->next = NULL;
    }
    else if (newNode == NULL)
    {
        printf("Alokasi memori gagal!");
        exit(EXIT_FAILURE);
    }
    return newNode;
}

struct Queue *createQueue()
{
    struct Queue *Q = (struct Queue *)malloc(sizeof(struct Queue));
    Q->front = Q->rear = NULL;
    return Q;
}

void enqueue(struct Queue *Q, char newPlatDepan[], int newPlatNo, char newPlatBelakang[], char newWarna[], int newDur, char newGol[])
{
    struct Node *newNode = createNode(newPlatDepan, newPlatNo, newPlatBelakang, newWarna, newDur, newGol);
    if (newNode != NULL)
    {
        if (Q->rear == NULL)
        {
            Q->front = newNode;
            Q->rear = newNode;
        }
        else
        {
            Q->rear->next = newNode;
            Q->rear = newNode;
        }
    }
    else if (newNode == NULL)
    {
        printf("Alokasi memori gagal!");
        exit(EXIT_FAILURE);
    }
}

void addDataPrioritas(struct Queue *Q, char newPlatDepan[], int newPlatNo, char newPlatBelakang[], char newWarna[], int newDur, char newGol[])
{
    struct Node *newNode = createNode(newPlatDepan, newPlatNo, newPlatBelakang, newWarna, newDur, newGol);
    if (newNode != NULL)
    {
        if (Q->front == NULL)
        {
            Q->front = newNode;
            Q->rear = newNode;
        }
        else
        {
            newNode->next = Q->front;
            Q->front = newNode;
        }
    }
    else if (newNode == NULL)
    {
        printf("Alokasi memori gagal!");
        exit(EXIT_FAILURE);
    }
}

int diskonGanjilGenap(int platNo, int biaya)
{
    int tanggalCur = currentTanggal();

    // tanggal dan plat nomor sama-sama ganjil atau sama-sama genap
    if ((tanggalCur % 2 == 0 && platNo % 2 == 0) || (tanggalCur % 2 != 0 && platNo % 2 != 0))
    {
        biaya = biaya * 0.5;
    }

    // tanggal dan plat nomor tidak sama-sama ganjil atau tidak sama-sama genap
    if ((tanggalCur % 2 != 0 && platNo % 2 == 0) || (tanggalCur % 2 == 0 && platNo % 2 != 0))
    {
        biaya = biaya;
    }

    return biaya;
}

int biayaParkir(int durasiParkir, char golKendaraan[], int platNo)
{
    int biaya, biayaSetelahCekDiskon;
    double pembulatanDurasi;
    if (durasiParkir <= 5)
    {
        biaya = 0;
    }
    else if (durasiParkir > 5 && durasiParkir <= 60)
    {
        if ((compareString(golKendaraan, "R2")) == 0)
        {
            biaya = 5000;
        }
        if ((compareString(golKendaraan, "R4")) == 0)
        {
            biaya = 10000;
        }
    }
    else if (durasiParkir > 60)
    {
        pembulatanDurasi = ceil(durasiParkir / 60.0);
        if ((compareString(golKendaraan, "R2")) == 0)
        {
            biaya = 5000 + ((pembulatanDurasi - 1) * 1000);
        }
        if ((compareString(golKendaraan, "R4")) == 0)
        {
            biaya = 10000 + ((pembulatanDurasi - 1) * 1000);
        }
    }

    biayaSetelahCekDiskon = diskonGanjilGenap(platNo, biaya);
    return biayaSetelahCekDiskon;
}

void dequeue(struct Queue *Q)
{
    struct Node *temp = Q->front;
    int biaya;
    if (Q->front == NULL)
    {
        printf("Belum ada data yang tersedia!");
        return;
    }
    else if (Q->front == Q->rear)
    {
        Q->front = Q->rear = NULL;
    }
    else
    {
        Q->front = Q->front->next;
    }
    biaya = biayaParkir(temp->durasi, temp->golongan, temp->noPlat);
    printf("\n\nData kendaraan yang akan dibayar:\n");
    printf("-------------------------------------------------------------------------");
    printf("\n|\t%s %d %s\t|\t%s\t|\t%d\t|\t%s\t|\n", temp->hurufDepanPlat, temp->noPlat, temp->hurufBelakangPlat, temp->warnaKendaraan, temp->durasi, temp->golongan);
    printf("-------------------------------------------------------------------------");
    printf("\n\nBiaya parkir yang harus dibayar: Rp. %d", biaya);
    free(temp);
}

void printNode(struct Node *node)
{
    printf("\n|\t%s %d %s\t|\t%s\t|\t%d\t|\t%s\t|\n", node->hurufDepanPlat, node->noPlat, node->hurufBelakangPlat, node->warnaKendaraan, node->durasi, node->golongan);
}

void display(struct Queue *Q)
{
    struct Node *temp = Q->front;
    if ((Q->front == NULL) && (Q->rear == NULL))
    {
        printf("\nBelum ada data yang tersedia!");
        return;
    }
    printf("\n\nData kendaraan yang sedang antre:\n");
    printf("-------------------------------------------------------------------------");
    printf("\n|\tPlat Nomor\t|\tWarna\t|\tDurasi\t|\tGol\t|\n");
    printf("-------------------------------------------------------------------------");
    while (temp != NULL)
    {
        printNode(temp);
        temp = temp->next;
    }
}

void search(struct Queue *Q, char platDepan[], int platNo, char platBelakang[])
{
    struct Node *temp = Q->front;
    int found = 0;
    if ((Q->front == NULL) && (Q->rear == NULL))
    {
        printf("\nBelum ada data yang tersedia!");
        return;
    }
    while (temp != NULL)
    {
        if ((compareString(temp->hurufDepanPlat, platDepan) == 0) && (temp->noPlat == platNo) && (compareString(temp->hurufBelakangPlat, platBelakang) == 0))
        {
            found = 1;
            break;
        }
        temp = temp->next;
    }
    if (found == 1)
    {
        printf("\n\nData kendaraan yang dicari:\n");
        printf("-------------------------------------------------------------------------");
        printf("\n|\t%s %d %s\t|\t%s\t|\t%d\t|\t%s\t|\n", temp->hurufDepanPlat, temp->noPlat, temp->hurufBelakangPlat, temp->warnaKendaraan, temp->durasi, temp->golongan);
        printf("-------------------------------------------------------------------------");
    }
    else
    {
        printf("\n\nData kendaraan yang dicari tidak ditemukan!");
    }
}

int main()
{
    char platDepan[MAX], platBelakang[MAX], warna[MAX], gol[MAX], warnaPrioritasHariIni[MAX];
    int platNo, dur, pilihMenu;

    struct Queue *Q = createQueue();

    do
    {
        system("cls");
        menu();
        printf("\n\nPilih menu yang tersedia: ");
        scanf("%d", &pilihMenu);
        switch (pilihMenu)
        {
        case 1:
            system("cls");
            printf("Masukkan plat no kendaraan (ex: L<space>1234<space>LL): ");
            scanf("%s %d %s", &platDepan, &platNo, &platBelakang);

            // warna kendaraan
            printf("Masukkan warna kendaraan (1. Hitam, 2. Putih, 3. Lainnya): ");
            scanf("%s", &warna);
            if (compareString(warna, "1") == 0)
            {
                strcpy(warna, "Hitam");
            }
            if (compareString(warna, "2") == 0)
            {
                strcpy(warna, "Putih");
            }
            if (compareString(warna, "3") == 0)
            {
                strcpy(warna, "Lainnya");
            }

            printf("Masukkan durasi parkir kendaraan (dalam menit): ");
            scanf("%d", &dur);

            // golongan kendaraan
            printf("Masukkan golongan kendaraan (1. R2, 2. R4): ");
            scanf("%s", &gol);
            if (compareString(gol, "1") == 0)
            {
                strcpy(gol, "R2");
            }
            if (compareString(gol, "2") == 0)
            {
                strcpy(gol, "R4");
            }

            // warna prioritas berdasarkan hari
            if (currentHari() == 0)
            { // sunday
                strcpy(warnaPrioritasHariIni, "Hitam");
            }
            else if (currentHari() == 1)
            { // monday
                strcpy(warnaPrioritasHariIni, "Hitam");
            }
            else if (currentHari() == 2)
            { // tuesday
                strcpy(warnaPrioritasHariIni, "Putih");
            }
            else if (currentHari() == 3)
            { // wednesday
                strcpy(warnaPrioritasHariIni, "Lainnya");
            }
            else if (currentHari() == 4)
            { // thursday
                strcpy(warnaPrioritasHariIni, "Hitam");
            }
            else if (currentHari() == 5)
            { // friday
                strcpy(warnaPrioritasHariIni, "Putih");
            }
            else if (currentHari() == 6)
            { // saturday
                strcpy(warnaPrioritasHariIni, "Lainnya");
            }

            if (compareString(warna, warnaPrioritasHariIni) == 0)
            {
                addDataPrioritas(Q, platDepan, platNo, platBelakang, warna, dur, gol);
            }
            else
            {
                enqueue(Q, platDepan, platNo, platBelakang, warna, dur, gol);
            }

            system("cls");
            break;
        case 2:
            system("cls");
            dequeue(Q);
            getch();
            system("cls");
            break;
        case 3:
            system("cls");
            display(Q);
            getch();
            system("cls");
            break;
        case 4:
            system("cls");
            printf("Masukkan Plat Nomor yang Datanya Ingin Dicari");
            printf("\nMasukkan plat no kendaraan (ex: L<space>1234<space>LL): ");
            scanf("%s %d %s", &platDepan, &platNo, &platBelakang);
            search(Q, platDepan, platNo, platBelakang);
            getch();
            system("cls");
            break;
        case 5:
            exit(0);
            break;
        default:
            system("cls");
            printf("Menu yang dipilih salah, tekan enter untuk memasukkan ulang menu!");
            getch();
        }
    } while (pilihMenu != 5);
}
