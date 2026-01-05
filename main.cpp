#include <iostream>
#include <string>

#include "pasien.h"
#include "data_manager.h"
#include "pendaftaran.h"
#include "antrian.h"
#include "antrian_prioritas.h"
#include "pemeriksaan.h"
#include "rawat_inap.h"
#include "laporan_pemeriksaan.h"

using namespace std;

/* =====================================================
   GLOBAL CIRCULAR LINKED LIST
   ===================================================== */
NodePasien* headPasien = NULL;

/* =====================================================
   TAMBAH PASIEN KE CIRCULAR LINKED LIST
   ===================================================== */
void tambahPasienCLL(const Pasien& p) {
    NodePasien* baru = new NodePasien();
    baru->data = p;

    if (headPasien == NULL) {
        headPasien = baru;
        baru->next = baru;
    } else {
        NodePasien* cur = headPasien;
        while (cur->next != headPasien)
            cur = cur->next;

        cur->next = baru;
        baru->next = headPasien;
    }
}

/* =====================================================
   LOGIN CUSTOMER BERDASARKAN KTP
   ===================================================== */
NodePasien* loginCustomerByKTP(const string& ktp) {
    if (headPasien == NULL) return NULL;

    NodePasien* cur = headPasien;
    do {
        if (cur->data.ktp == ktp)
            return cur;
        cur = cur->next;
    } while (cur != headPasien);

    return NULL;
}

/* =====================================================
   LOAD DATA DARI FILE KE CLL
   ===================================================== */
void loadPasienCLLDariFile() {
    // bersihkan CLL lama
    headPasien = NULL;

    Pasien* data;
    int n;
    DataManager::load(data, n);

    for (int i = 0; i < n; i++) {
        tambahPasienCLL(data[i]);
    }

    if (n > 0) delete[] data;
}

/* ================= HEADER ================= */
void header() {
    cout << "============================================================\n";
    cout << "            SISTEM KLINIK HEWAN CERIA PETS                  \n";
    cout << "============================================================\n";
}

/* ================= LOGIN ADMIN ================= */
bool loginAdmin() {
    string user, pass;
    cout << "Username Admin : ";
    cin >> user;
    cout << "Password Admin : ";
    cin >> pass;
    return (user == "admin" && pass == "admin123");
}

/* ================= LOGIN CUSTOMER ================= */
bool loginCustomer(string& ktp) {
    cout << "Masukkan No KTP : ";
    cin >> ktp;
    return (loginCustomerByKTP(ktp) != NULL);
}

/* ================= MENU UTAMA ================= */
void menuUtama() {
    cout << "\nMENU UTAMA\n";
    cout << "1. Pendaftaran Pasien\n";
    cout << "2. Antrian Pasien\n";
    cout << "3. Antrian Prioritas\n";
    cout << "4. Pemeriksaan\n";
    cout << "5. Rawat Inap\n";
    cout << "0. Keluar\n";
    cout << "Pilih menu: ";
}

/* ================= MAIN ================= */
int main() {
    loadPasienCLLDariFile();   // load awal

    int role;
    string ktpCustomer;

    /* ================= LOGIN ================= */
    do {
        header();
        cout << "LOGIN SISTEM\n";
        cout << "1. Login Admin\n";
        cout << "2. Login Customer\n";
        cout << "0. Keluar\n";
        cout << "Pilih: ";
        cin >> role;

        if (role == 1) {
            if (loginAdmin()) {
                cout << "Login admin berhasil\n";
                break;
            } else {
                cout << "Login admin gagal\n";
            }
        }
        else if (role == 2) {
            if (loginCustomer(ktpCustomer)) {
                cout << "Login customer berhasil\n";
                break;
            } else {
                cout << "Login customer gagal (KTP tidak ditemukan)\n";
            }
        }
        else if (role == 0) {
            return 0;
        }

        cout << "\nTekan ENTER untuk ulang login...";
        cin.ignore();
        cin.get();

    } while (true);

    /* ================= MENU UTAMA ================= */
    int pilih;
    do {
        header();
        menuUtama();
        cin >> pilih;

        switch (pilih) {
            case 1: {
                Pendaftaran p;
                p.menu();

                // ?? PENTING: sync ulang CLL setelah pendaftaran
                loadPasienCLLDariFile();
                break;
            }
            case 2: {
                Antrian a;
                a.menu();
                break;
            }
            case 3: {
                AntrianPrioritas ap;
                ap.menu();
                break;
            }
            case 4: {
                Pemeriksaan pr;
                pr.menu();
                break;
            }
            case 5: {
                RawatInap ri;
                ri.menu();
                break;
            }
            case 0:
                cout << "Terima kasih telah menggunakan sistem klinik\n";
                break;
            default:
                cout << "Pilihan tidak valid\n";
        }

        if (pilih != 0) {
            cout << "\nTekan ENTER untuk kembali ke menu utama...";
            cin.ignore();
            cin.get();
        }

    } while (pilih != 0);

    return 0;
}
