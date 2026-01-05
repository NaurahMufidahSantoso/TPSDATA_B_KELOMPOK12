#ifndef LAPORAN_PEMERIKSAAN_H
#define LAPORAN_PEMERIKSAAN_H

#include <fstream>
#include <iostream>
#include <ctime>
#include "pasien.h"
#include "data_manager.h"

using namespace std;

class LaporanPemeriksaan {
private:
    static void cetakGaris(ofstream& file) {
        file << "==============================================\n";
    }

    static void cetakTanggal(ofstream& file) {
        time_t now = time(0);
        tm* ltm = localtime(&now);

        file << "Tanggal        : "
             << ltm->tm_mday << "-"
             << (1 + ltm->tm_mon) << "-"
             << (1900 + ltm->tm_year) << endl;
    }

public:
    /* =====================================================
       LAPORAN PEMERIKSAAN (ASLI – TIDAK DIUBAH)
       ===================================================== */
    static void simpan(const Pasien& p,
                       const string& hasil,
                       const string& tindakan) {
        ofstream file("laporan_pemeriksaan.txt", ios::app);

        cetakGaris(file);
        file << "        LAPORAN PEMERIKSAAN PASIEN\n";
        file << "          KLINIK HEWAN CERIA PETS\n";
        cetakGaris(file);

        cetakTanggal(file);
        file << "Dokter         : Dokter Jaga\n";
        file << "----------------------------------------------\n";
        file << "ID Pasien      : " << p.id << endl;
        file << "Nama Hewan     : " << p.nama << endl;
        file << "Jenis Hewan    : " << p.jenis << endl;
        file << "Layanan        : Pemeriksaan\n";
        file << "Prioritas      : " << p.prioritas << endl;
        file << "----------------------------------------------\n";
        file << "Keluhan:\n";
        file << "- " << p.keluhan << endl;
        file << "----------------------------------------------\n";
        file << "Hasil Pemeriksaan:\n";
        file << "- " << hasil << endl;
        file << "----------------------------------------------\n";
        file << "Tindakan Medis:\n";
        file << "- " << tindakan << endl;
        file << "----------------------------------------------\n";
        file << "Status Akhir   : SELESAI\n";
        cetakGaris(file);
        file << endl;

        file.close();
    }

    /* =====================================================
       TAMBAHAN MATERI: LAPORAN ENKRIPSI (HUFFMAN + AVL)
       ===================================================== */

    // Simpan laporan enkripsi KTP
    static void simpanLaporanEnkripsi(const string& ktpAsli) {
        ofstream file("laporan_enkripsi.txt", ios::app);

        int bitBefore = DataManager::bitSebelum(ktpAsli);
        int bitAfter  = DataManager::bitSesudah(ktpAsli);
        int efisiensi = 100 - (bitAfter * 100 / bitBefore);

        file << "==============================================\n";
        file << "        LAPORAN ENKRIPSI DATA PASIEN\n";
        file << "==============================================\n";
        cetakTanggal(file);
        file << "Data           : No KTP Pasien\n";
        file << "KTP Asli       : " << ktpAsli << endl;
        file << "Bit Sebelum    : " << bitBefore << " bit\n";
        file << "Bit Sesudah    : " << bitAfter  << " bit\n";
        file << "Efisiensi      : " << efisiensi << " %\n";
        file << "Metode         : Huffman Coding (Binary Tree)\n";
        file << "----------------------------------------------\n";
        file << endl;

        file.close();
    }

    // Simpan hasil enkripsi ke AVL Tree (simbolik)
    static DataManager::AVLNode* simpanKeAVL(
        DataManager::AVLNode* root,
        const string& dataTerenkripsi
    ) {
        root = DataManager::insertAVL(root, dataTerenkripsi);
        return root;
    }

    // Tampilkan isi AVL Tree (admin)
    static void tampilAVL(DataManager::AVLNode* root) {
        cout << "\n=== DATA TERENKRIPSI (AVL TREE) ===\n";
        DataManager::inorderAVL(root);
    }
};

#endif
