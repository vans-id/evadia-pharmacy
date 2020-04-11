#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <dirent.h>
#include <windows.h>
#include <conio.h>
#include <dos.h>

char file_name[] = "list_data.txt";
char checkoutFile[] = "checkout.txt";
char delim[] = ";";
char data_barang[200][3][200];
int jml_data_barang = 0;
int iterator = 0;
char shoppingCart[200][3][200];

char *substring(char *string, int position, int length)
{
  char *pointer;
  int c;

  pointer = malloc(length + 1);

  if (pointer == NULL)
  {
    printf("Unable to allocate memory.\n");
    exit(EXIT_FAILURE);
  }

  for (c = 0; c < length; c++)
  {
    *(pointer + c) = *(string + position - 1);
    string++;
  }

  *(pointer + c) = '\0';

  return pointer;
}

void getData()
{
  FILE *fp = fopen(file_name, "r");
  char line[200];

  memset(data_barang, 0, sizeof(data_barang));
  jml_data_barang = 0;

  if (fp == NULL)
  {
    printf("Gagal membuka file %s", file_name);
    exit(EXIT_FAILURE);
  }
  else
  {
    int i = 0;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
      char *token = strtok(line, delim);

      int j = 0;
      while (token != NULL)
      {
        strcpy(data_barang[i][j], token);
        token = strtok(NULL, delim);
        j++;
      }

      i++;
      jml_data_barang++;
    }
  }
  fclose(fp);
}

void peekData()
{
  getData();
  system("@cls||clear");
  printf("============================================\n");
  printf("Menu Lihat Data Barang\n");
  printf("============================================\n");
  if (jml_data_barang > 0)
  {
    printf("No\tNama Barang\tHarga\tStok\n");
    printf("============================================\n");
    int i;
    for (i = 0; i < jml_data_barang; i++)
    {
      char nama_brg[100];
      strcpy(nama_brg, data_barang[i][0]);
      if (strlen(nama_brg) > 15)
      {
        strcpy(nama_brg, substring(nama_brg, 1, 15));
      }
      else if (strlen(nama_brg) < 8)
      {
        snprintf(nama_brg, sizeof nama_brg, "%s%s", data_barang[i][0], "\t");
      }
      int harga_brg = atoi(data_barang[i][1]);
      int stok_brg = atoi(data_barang[i][2]);

      printf("%d\t%s\t%d\t%d \n", i + 1, nama_brg, harga_brg, stok_brg);
    }
  }
  else
  {
    printf("Tidak ada data barang!\n");
  }
  printf("============================================\n");
}
// Hilangkan bug
void menu_utama();
void viewData()
{
  peekData();
  system("pause");
  menu_utama();
}

bool addToFile(char inputan[], char fileName[])
{
  FILE *fp = fopen(fileName, "a+");
  if (fp == NULL)
  {
    return false;
  }
  else
  {
    fputs(inputan, fp);
  }
  fclose(fp);
  return true;
}

void tambahData()
{
  scanf("%c");
  system("@cls||clear");
  printf("============================================\n");
  printf("Menu Tambah Data Barang\n");
  printf("============================================\n");

  char nama[100], harga[50], stok[50], hasil[200];

  printf("Masukkan Nama Barang : ");
  gets(nama);
  if (strlen(nama) > 15)
  {
    strcpy(nama, substring(nama, 1, 15));
  }

  printf("Masukkan Harga Barang : ");
  gets(harga);
  int hargaa = atoi(harga);
  if (hargaa < 1000)
    hargaa = 1000;
  else if (hargaa > 9999999)
    hargaa = 9999999;

  printf("Masukkan Stok Barang : ");
  gets(stok);
  int stokk = atoi(stok);
  if (stokk < 1)
    stokk = 1;
  else if (stokk > 10000)
    stokk = 10000;

  printf("============================================\n");
  snprintf(hasil, sizeof hasil, "%s;%d;%d;\n", nama, hargaa, stokk);
  if (addToFile(hasil, file_name))
  {
    printf("Sukses tambah data barang!\n");
  }
  else
  {
    printf("Gagal tambah data barang!\n");
  }
  printf("============================================\n");
  system("pause");
  menu_utama();
}

void hapusData()
{
  scanf("%c");
  system("@cls||clear");
  printf("============================================\n");
  printf("Menu Hapus Data Barang\n");
  printf("============================================\n");
  char nama[100];
  printf("Masukkan Nama Barang : ");
  gets(nama);
  if (jml_data_barang > 0)
  {
    FILE *fp = fopen(file_name, "r");
    char line[200];

    if (fp == NULL)
    {
      printf("Gagal membuka file %s", file_name);
      exit(EXIT_FAILURE);
    }
    else
    {
      bool ada = false, ada2 = false;
      int i = 0, j = 0, k = 0;
      char tampung[100][200];
      while (fgets(line, sizeof(line), fp) != NULL)
      {
        char *token = strtok(line, delim);
        j = 0;
        ada = false;
        while (token != NULL)
        {
          if (j == 0)
          {
            if (strcmp(data_barang[i][j], nama) == 0)
            {
              ada = true;
              ada2 = true; //memastikan jika ketemu
            }
          }
          token = strtok(NULL, delim);
          j++;
        }

        if (!ada)
        {
          //data barang yg bukan dihapus akan ditampung di array tampung
          snprintf(tampung[k], sizeof tampung[k], "%s;%s;%s;\n", data_barang[i][0], data_barang[i][1], data_barang[i][2]);
          k++;
        }

        i++;
      }

      fclose(fp);

      printf("============================================\n");
      if (ada2)
      {
        fp = fopen(file_name, "w"); //reset file jadi kosong
        fclose(fp);
        if (k > 0)
        {
          for (i = 0; i < k; i++)
          {
            addToFile(tampung[i], file_name);
          }
        }
        printf("Sukses hapus (%s) dari data barang!\n", nama);
      }
      else
      {
        printf("Nama barang (%s) tidak ditemukan!\n", nama);
      }
    }
  }
  else
  {
    printf("============================================\n");
    printf("Tidak ada data barang!\n");
  }

  printf("============================================\n");
  system("pause");
  menu_utama();
}

void transactionMenu()
{
  scanf("%c");
  system("@cls||clear");
  peekData();

  // Shopping cart
  int choice = 0;

  while (choice != 3)
  {
    FILE *fp = fopen(file_name, "r");
    printf("\nPilih transaksi :\n");
    printf("1. Beli\n");
    printf("2. Checkout\n");
    printf("3. Cancel\n");
    printf("\nPilihanmu: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
      int menu, ammount;
      // Add item to shopping cart
      scanf("%c");
      system("@cls||clear");
      peekData();
      printf("\nPilih index item untuk dibeli : ");
      scanf("%d", &menu);
      printf("Masukan jumlah item yang ingin dibeli : ");
      scanf("%d", &ammount);

      int remains = atoi(data_barang[menu - 1][2]);

      if (menu > jml_data_barang)
      {
        printf("\n--------------------------------------------");
        printf("\nGagal menambahkan ke keranjang belanja!");
        printf("\nIndex barang tidak ditemukan");
        printf("\n--------------------------------------------\n");
      }
      else if (ammount > remains)
      {
        printf("\n--------------------------------------------");
        printf("\nGagal menambahkan ke keranjang belanja!");
        printf("\nJumlah barang tidak valid");
        printf("\n--------------------------------------------\n");
      }
      else
      {
        strcpy(shoppingCart[iterator][0], data_barang[menu - 1][0]);
        strcpy(shoppingCart[iterator][1], data_barang[menu - 1][1]);
        sprintf(shoppingCart[iterator][2], "%d", ammount);

        iterator++;
        printf("\n--------------------------------------------\n");
        printf("Berhasil menambahkan ke keranjang belanja!\n");
        printf("--------------------------------------------\n");
      }

      system("pause");
      transactionMenu();
    }
    else if (choice == 2)
    {

      char bill[200], date[50], store[10][200];

      scanf("%c");
      system("@cls||clear");
      printf("======================================================\n");
      printf("Checkout Details\n");
      printf("======================================================\n");

      if (iterator > 0)
      {
        printf("No\tNama Barang\tHarga\tJumlah\tTotal Harga\n");
        printf("======================================================\n");
        int i, totalPrice = 0;

        for (i = 0; i < iterator; i++)
        {
          char displayName[100];
          strcpy(displayName, shoppingCart[i][0]);

          if (strlen(displayName) > 15)
          {
            strcpy(displayName, substring(displayName, 1, 15));
          }
          else if (strlen(displayName) < 8)
          {
            snprintf(displayName, sizeof displayName, "%s%s", shoppingCart[i][0], "\t");
          }

          char itemName[100];
          strcpy(itemName, shoppingCart[i][0]);
          int itemPrice = atoi(shoppingCart[i][1]);
          int itemOrder = atoi(shoppingCart[i][2]);
          int itemTotal = itemPrice * itemOrder;

          totalPrice += itemTotal;

          printf("%d\t%s\t%d\t%d\t%d \n", i + 1, displayName, itemPrice, itemOrder, itemTotal);

          // insert bill
          snprintf(bill, sizeof bill, "%s;%d;%d;%d;\n", itemName, itemPrice, itemOrder, itemTotal);

          strcpy(store[i], bill);
        }

        printf("======================================================\n");
        printf("\nTotal Harga : Rp %d", totalPrice);

        int payment, paymentReturn;

        printf("\nBayar : Rp ");
        scanf("%d", &payment);

        paymentReturn = payment - totalPrice;

        if (paymentReturn < 0)
        {
          printf("\nUang pembayaran kurang!\n");
          system("pause");
          transactionMenu();
        }
        printf("\nPembayaran Sukses!");
        printf("\nKembalian : Rp %d", paymentReturn);

        // Get local time
        time_t now;
        time(&now);
        strcpy(date, "\n\n");
        strcat(date, ctime(&now));

        // Insert into bills
        addToFile(date, checkoutFile);
        int r;
        for (r = 0; r < i; r++)
        {
          addToFile(store[r], checkoutFile);
        }

        char converted[100];

        sprintf(converted, "%d", payment);
        strcat(converted, "\n");
        addToFile(converted, checkoutFile);

        sprintf(converted, "%d", paymentReturn);
        strcat(converted, "\n");
        addToFile(converted, checkoutFile);

        int k;
        fp = fopen(file_name, "w");
        char newItem[200];
        char input[jml_data_barang][200];

        // copy array
        for (k = 0; k < jml_data_barang; k++)
        {
          int itemStock = atoi(data_barang[k][2]);

          snprintf(newItem, sizeof newItem, "%s;%s;%d;\n", data_barang[k][0], data_barang[k][1], itemStock);
          strcpy(input[k], newItem);
        }
        // Filter array
        for (k = 0; k < jml_data_barang; k++)
        {
          int l;

          for (l = 0; l < jml_data_barang; l++)
          {
            if (strcmp(shoppingCart[k][0], data_barang[l][0]) == 0)
            {
              int itemCart = atoi(shoppingCart[k][2]);
              int itemStock = atoi(data_barang[l][2]);

              int handleStock = itemStock - itemCart;

              snprintf(newItem, sizeof newItem, "%s;%s;%d;\n", data_barang[l][0], data_barang[l][1], handleStock);

              strcpy(input[l], newItem);
            }
          }
        }
        // Cut stock
        for (k = 0; k < jml_data_barang; k++)
        {
          addToFile(input[k], file_name);
        }
      }
      else
      {
        printf("Tidak ada data barang!\n");
      }
      printf("\n\n======================================================\n");
      iterator = 0;
      system("pause");
      transactionMenu();
    }

    iterator = 0;
    fclose(fp);
    menu_utama();
  }
}

void menu(int pil)
{
  if (pil == 1)
    viewData();
  else if (pil == 2)
    tambahData();
  else if (pil == 3)
    hapusData();
  else if (pil == 4)
  {
    transactionMenu();
  }
}

void menu_utama()
{
  system("Color 0E");
  int pilihan = 0;
  while (pilihan == 0)
  {
    system("@cls||clear");
    printf("============================================\n");
    printf("MENU UTAMA\n");
    printf("============================================\n");
    printf("1. Lihat Data Barang\n");
    printf("2. Tambah Data Barang\n");
    printf("3. Hapus Data Barang\n");
    printf("4. Transaksi\n");
    printf("5. Exit\n");
    printf("Pilih Menu : ");
    scanf("%d", &pilihan);
    if (pilihan <= 0 || pilihan > 6)
    {
      pilihan = 0;
      system("@cls||clear");
      printf("Pilihan Menu Salah!");
      system("pause");
    }
  }

  if (pilihan == 5)
    exit(EXIT_SUCCESS);
  else
    menu(pilihan);
}

// Helper for login func
int gotoxy(int x, int y), loading(int x, int y);

int gotoxy(int x, int y)
{

  HANDLE handle;
  COORD coord;
  coord.X = x;
  coord.Y = y;
  handle = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleCursorPosition(handle, coord);
}

int loading(int x, int y)
{
  system("cls");
  int i;
  gotoxy(x, y - 1);
  printf("Loading...");

  for (i = x; i <= x + 19; i++)
  {
    gotoxy(i, y);
    printf("%c", 196);
    gotoxy(i, y + 1);
    printf("%c", 196);
  }
  for (i = y; i <= y + 1; i++)
  {
    gotoxy(x, i);
    printf("%c", 179);
    gotoxy(x + 19, i);
    printf("%c", 179);
  }
  gotoxy(x, y);
  printf("%c", 218);
  gotoxy(x + 19, y);
  printf("%c", 191);
  gotoxy(x, y + 1);
  printf("%c", 192);
  gotoxy(x + 19, y + 1);
  printf("%c", 217);
  for (i = x; i <= x + 19; i++)
  {
    gotoxy(i, y);
    printf("%c", 220);
    gotoxy(i, y + 1);
    printf("%c", 223);
    Sleep(70);
  }
  system("cls");
}

void login()
{
  system("Color 02");
  char user[20], pass[20];
  int x;
  double y;
  char ch, currentUserName[20], currentPassword[20];
  int i, attempts = 1, userNameCorrect = 1;
  int maxTry = 3;
  char password[20] = "evadia123";
  char userName[20] = "admin";
  char salam[] = "\t\t\t           BY EVAN & NADIA\n";
sini:
  while (attempts <= maxTry)
  {
    system("cls");
    gotoxy(22, 6);
    printf("+---------------------------------+");
    gotoxy(33, 7);
    printf("SELAMAT DATANG\n");
    gotoxy(39, 8);
    printf("DI\n");
    gotoxy(30, 9);
    printf("=== APOTEK EVADIA ===\n");
    gotoxy(16, 10);
    printf("+---------------------------------------------+");
    gotoxy(20, 11);
    printf("SILAHKAN MASUKKAN USERNAME DAN PASSWORD\n");
    gotoxy(16, 20);
    printf("+---------------------------------------------+");
    gotoxy(62, 11);
    printf("|");
    gotoxy(62, 12);
    printf("|");
    gotoxy(62, 13);
    printf("|");
    gotoxy(62, 14);
    printf("|");
    gotoxy(62, 15);
    printf("|");
    gotoxy(62, 16);
    printf("|");
    gotoxy(62, 17);
    printf("|");
    gotoxy(62, 18);
    printf("|");
    gotoxy(62, 19);
    printf("|");
    gotoxy(16, 11);
    printf("|");
    gotoxy(16, 12);
    printf("|");
    gotoxy(16, 13);
    printf("|");
    gotoxy(16, 14);
    printf("|");
    gotoxy(16, 15);
    printf("|");
    gotoxy(16, 16);
    printf("|");
    gotoxy(16, 17);
    printf("|");
    gotoxy(16, 18);
    printf("|");
    gotoxy(16, 19);
    printf("|");
    struct tm *Sys_T;
    time_t Tval;
    Tval = time(NULL);
    Sys_T = localtime(&Tval);
    gotoxy(28, 13);
    printf("%s", asctime(Sys_T));

    ch = '\0';
    if (userNameCorrect)
    {
      gotoxy(33, 15);
      printf("Username : ");
      gets(currentUserName);
      gotoxy(33, 17);
      printf("Password : ");
      i = 0;
      while (ch != 13)
      {
        ch = getch();
        if (ch != 13 && ch != 8)
        {
          putch('*');
          currentPassword[i] = ch;
          i++;
        }
      }
      currentPassword[i] = '\0';
    }

    if (strcmp(currentUserName, userName) == 0 && strcmp(currentPassword, password) == 0)
    {
      userNameCorrect = 0;
      gotoxy(25, 22);
      printf("Authentication Successfull!\n");
      for (x = 0; salam[x] != 0; x++)
      {
        Sleep(40);
        gotoxy(15 + x, 24);
        printf("%c", salam[x]);
        for (y = 0; y <= 9999999; y++)
        {
        }
      }
      loading(30, 12);
      menu_utama();
    }
    else
    {
      if (attempts < 3)
      {
        gotoxy(25, 22);
        printf("Username atau Password salah!");
        getch();
      }
      else
      {
        gotoxy(15, 22);
        printf("Anda telah mencoba beberapa kali, tutup program!!!\n\n");
        Sleep(5);
        exit(1);
        getch();
      }
      attempts++;
    }
  }
}

void main()
{
  getData();
  login();
}
