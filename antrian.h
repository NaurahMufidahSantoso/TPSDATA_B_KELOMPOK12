#ifndef ANTRIAN_H
#define ANTRIAN_H

#include <iostream>
#include <iomanip>
#include "pasien.h"
#include "data_manager.h"

using namespace std;

/* =====================================================
   STRUCT NODE ANTRIAN (ASLI)
   ===================================================== */
struct NodeAntrian {
    Pasien data;
    NodeAntrian* next;

    // ===== TAMBAHAN ATRIBUT (TIDAK MENGGANTI YANG LAMA) =====
    bool keluarAntrian;     // sesuai instruksi: keluar setelah pilih waktu
    bool darurat;           // layanan darurat
    string waktuDipilih;    // waktu layanan

    NodeAntrian() {
        next = NULL;
        keluarAntrian = false;
        darurat = false;
        waktuDipilih = "-";
    }

    NodeAntrian(const Pasien& p) {
        data = p;
        next = NULL;
        keluarAntrian = false;
        darurat = false;
        waktuDipilih = "-";
    }
};

/* =====================================================
   CLASS ANTRIAN (CIRCULAR LINKED LIST)
   ===================================================== */
class Antrian {
private:
    NodeAntrian* rear;

    bool kosong() const {
        return rear == NULL;
    }

    int jumlahNode() const {
        if (kosong()) return 0;

        int count = 0;
        NodeAntrian* cur = rear->next;
        do {
            count++;
            cur = cur->next;
        } while (cur != rear->next);

        return count;
    }

    NodeAntrian* cariNode(int id) const {
        if (kosong()) return NULL;

        NodeAntrian* cur = rear->next;
        do {
            if (cur->data.id == id)
                return cur;
            cur = cur->next;
        } while (cur != rear->next);

        return NULL;
    }

    // ================= CLEAR ANTRIAN =================
    void clear() {
        if (kosong()) return;

        NodeAntrian* cur = rear->next;
        NodeAntrian* temp;

        while (cur != rear) {
            temp = cur;
            cur = cur->next;
            delete temp;
        }

        delete rear;
        rear = NULL;
    }

public:
    Antrian() {
        rear = NULL;
    }

    // ================= ENQUEUE (CIRCULAR) =================
    void enqueue(const Pasien& p) {
        NodeAntrian* baru = new NodeAntrian(p);

        if (kosong()) {
            rear = baru;
            rear->next = rear;
        } else {
            baru->next = rear->next;
            rear->next = baru;
            rear = baru;
        }
    }

    // ================= LOAD DARI FILE (ASLI) =================
    void loadDariFile(const string& layanan) {
        clear();

        Pasien* data;
        int n;
        DataManager::load(data, n);

        for (int i = 0; i < n; i++) {
            if (data[i].layanan == layanan &&
                data[i].status == MENUNGGU) {
                enqueue(data[i]);
            }
        }

        if (n > 0) delete[] data;
    }

    // ================= TAMPIL (DITAMBAH INFO BARU) =================
    void tampil() const {
        if (kosong()) {
            cout << "Antrian kosong\n";
            return;
        }

        cout << left
             << setw(5)  << "ID"
             << setw(12) << "Nama"
             << setw(12) << "Jenis"
             << setw(10) << "Status"
             << setw(10) << "Waktu"
             << setw(10) << "Info"
             << endl;
        cout << "---------------------------------------------------------\n";

        NodeAntrian* cur = rear->next;
        do {
            cout << setw(5)  << cur->data.id
                 << setw(12) << cur->data.nama
                 << setw(12) << cur->data.jenis
                 << setw(10) << cur->data.statusToString()
                 << setw(10) << cur->waktuDipilih
                 << setw(10) << (cur->darurat ? "DARURAT" : "-")
                 << endl;
            cur = cur->next;
        } while (cur != rear->next);
    }

    /* =====================================================
       TAMBAHAN LOGIKA SESUAI INSTRUKSI
       ===================================================== */

    // ===== PILIH WAKTU ? KELUAR DARI ANTRIAN =====
    void pilihWaktu(int id, const string& waktu, bool isDarurat) {
        NodeAntrian* node = cariNode(id);
        if (node == NULL) {
            cout << "Pasien tidak ditemukan\n";
            return;
        }

        node->waktuDipilih = waktu;
        node->darurat = isDarurat;
        node->keluarAntrian = true;

        cout << "Waktu dipilih. Pasien keluar dari antrian.\n";
    }

    // ===== TAMPIL KHUSUS DARURAT =====
    void tampilDarurat() const {
        if (kosong()) return;

        cout << "\n=== PASIEN DARURAT ===\n";
        NodeAntrian* cur = rear->next;
        bool ada = false;

        do {
            if (cur->darurat) {
                cout << cur->data.nama
                     << " | Waktu: " << cur->waktuDipilih << endl;
                ada = true;
            }
            cur = cur->next;
        } while (cur != rear->next);

        if (!ada)
            cout << "Tidak ada pasien darurat\n";
    }

    // ================= LAYANI PASIEN (TETAP) =================
    void layani() {
        if (kosong()) {
            cout << "Tidak ada pasien\n";
            return;
        }

        NodeAntrian* front = rear->next;
        int id = front->data.id;

        if (front == rear) {
            delete front;
            rear = NULL;
        } else {
            rear->next = front->next;
            delete front;
        }

        DataManager::ubahStatus(id, DIPERIKSA);
        DataManager::ubahLayanan(id, "Pemeriksaan");

        cout << "Pasien masuk pemeriksaan\n";
    }

    // ================= CARI PASIEN (TETAP) =================
    void cariPasien() const {
        int id;
        cout << "Masukkan ID: ";
        cin >> id;

        NodeAntrian* node = cariNode(id);
        if (node == NULL) {
            cout << "Pasien tidak ditemukan\n";
            return;
        }

        node->data.tampilLengkap();
    }

    // ================= INFO =================
    void infoJumlah() const {
        cout << "Jumlah pasien dalam antrian: "
             << jumlahNode() << endl;
    }

    // ================= MENU (DITAMBAH OPSI) =================
    void menu() {
        int pilih;
        do {
            cout << "\n=== MENU ANTRIAN ===\n";
            cout << "1. Muat Antrian\n";
            cout << "2. Tampil Antrian\n";
            cout << "3. Layani Pasien\n";
            cout << "4. Cari Pasien\n";
            cout << "5. Jumlah Antrian\n";
            cout << "6. Pilih Waktu (Keluar Antrian)\n";
            cout << "7. Tampil Pasien Darurat\n";
            cout << "0. Kembali\n";
            cout << "Pilih: ";
            cin >> pilih;

            switch (pilih) {
                case 1:
                    loadDariFile("Pendaftaran");
                    cout << "Antrian dimuat ulang\n";
                    break;
                case 2: tampil(); break;
                case 3: layani(); break;
                case 4: cariPasien(); break;
                case 5: infoJumlah(); break;
                case 6: {
                    int id;
                    string waktu;
                    int d;
                    cout << "ID Pasien: ";
                    cin >> id;
                    cout << "Waktu (mis. 09:00): ";
                    cin >> waktu;
                    cout << "Darurat? (1=Ya, 0=Tidak): ";
                    cin >> d;
                    pilihWaktu(id, waktu, d == 1);
                    break;
                }
                case 7: tampilDarurat(); break;
            }
        } while (pilih != 0);
    }
};

#endif
