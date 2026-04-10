# Holynano Text Editor
## Menjalankan Program

Gunakan perintah berikut di terminal:

### Membuka editor kosong

```bash
./holynano
```

### Membuka file tertentu

```bash
./holynano [nama_file]
```

### Mode hanya lihat (read-only)

```bash
./holynano --view
```

## Shortcut Keyboard

| Shortcut | Fungsi |
| --- | --- |
| Ctrl + S | Menyimpan file |
| Ctrl + X | Keluar dari editor |
| Ctrl + T | Menghapus baris |
| Ctrl + Y | Menyalin baris |
| Ctrl + K | Memotong baris |
| Ctrl + P | Menempelkan baris |


- Mode `--view` view only, tidak bisa mengedit
- Tidak ada autosave, jadi gunakan Ctrl + S setelah melakukan perubahan.

## Contoh Penggunaan

```bash
./holynano notes.txt
```



