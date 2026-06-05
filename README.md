# Holynano Text Editor

Holynano adalah text editor sederhana berbasis terminal yang dibuat dengan bahasa C. Program ini memakai doubly linked list sebagai text buffer, sehingga teks disimpan per baris di dalam node.

## Struktur Data

Program memakai dua struktur utama:

```c
typedef struct Node {
  char *info;
  struct Node *prev;
  struct Node *next;
} Node;
```

`Node` menyimpan satu baris teks pada `info`. Pointer `prev` menunjuk ke baris sebelumnya, sedangkan `next` menunjuk ke baris berikutnya.

```c
typedef struct Buffer {
  Node *head;
  Node *tail;
  size_t length;
} Buffer;
```

`Buffer` menyimpan alamat node pertama (`head`), node terakhir (`tail`), dan jumlah node/baris (`length`).

## Pembagian File

- `main.c`
  - titik awal program
  - membaca input keyboard
  - menangani shortcut
  - memanggil fungsi dari modul lain

- `dhafin.c` / `dhafin.h`
  - membuat dan mengelola buffer linked list
  - membuat node
  - menambah node
  - menyisipkan node
  - menghapus node
  - membersihkan buffer
  - mengaktifkan raw mode terminal
  - clear screen

- `danish.c` / `danish.h`
  - load file ke buffer
  - save buffer ke file
  - redraw screen
  - insert character pada posisi cursor

- `zaky.c` / `zaky.h`
  - menyimpan posisi cursor
  - memindahkan cursor
  - backspace character
  - delete line

## Compile

Program ditujukan untuk dijalankan di Linux/WSL.

```bash
gcc main.c dhafin.c danish.c zaky.c -o main
```

Dengan warning:

```bash
gcc -Wall -Wextra -std=c11 main.c dhafin.c danish.c zaky.c -o main
```

## Run

Menjalankan editor tanpa file:

```bash
./main
```

Menjalankan editor dengan file:

```bash
./main nama_file.txt
```

Contoh:

```bash
./main danish.c
```

Jika program dijalankan tanpa argumen file, hasil save default akan memakai:

```text
output.txt
```

## Shortcut

- `Ctrl + S`
  - menyimpan isi buffer ke file

- `Ctrl + X`
  - keluar dari program

- `Ctrl + T`
  - delete line





