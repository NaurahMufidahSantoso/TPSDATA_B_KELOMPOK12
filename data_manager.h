#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "pasien.h"

using namespace std;

class DataManager {
private:
    static string filePath() {
        return "data_pasien.txt";
    }

    static int hitungBaris() {
        ifstream file(filePath());
        string line;
        int count = 0;
        while (getline(file, line)) {
            if (!line.empty()) count++;
        }
        file.close();
        return count;
    }

public:
    /* =====================================================
       LOAD DATA PASIEN (FORMAT DENGAN KTP)
       ===================================================== */
    static void load(Pasien*& data, int& n) {
        n = hitungBaris();

        if (n == 0) {
            data = NULL;
            return;
        }

        data = new Pasien[n];
        ifstream file(filePath());
        string line;
        int idx = 0;

        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string temp;

            getline(ss, temp, '|');
            data[idx].id = stoi(temp);

            getline(ss, data[idx].ktp, '|');
            getline(ss, data[idx].nama, '|');
            getline(ss, data[idx].jenis, '|');
            getline(ss, data[idx].layanan, '|');

            getline(ss, temp, '|');
            data[idx].prioritas = stoi(temp);

            getline(ss, temp, '|');
            data[idx].status = Pasien::stringToStatus(temp);

            if (!getline(ss, data[idx].keluhan))
                data[idx].keluhan = "";

            idx++;
        }

        file.close();
    }

    /* =====================================================
       SIMPAN DATA PASIEN
       ===================================================== */
    static void save(Pasien* data, int n) {
        ofstream file(filePath(), ios::trunc);

        for (int i = 0; i < n; i++) {
            file << data[i].id << "|"
                 << data[i].ktp << "|"
                 << data[i].nama << "|"
                 << data[i].jenis << "|"
                 << data[i].layanan << "|"
                 << data[i].prioritas << "|"
                 << data[i].statusToString() << "|"
                 << data[i].keluhan << endl;
        }

        file.close();
    }

    /* =====================================================
       GENERATE ID
       ===================================================== */
    static int generateId() {
        Pasien* data;
        int n;
        load(data, n);

        int idBaru = 1;
        for (int i = 0; i < n; i++)
            if (data[i].id >= idBaru)
                idBaru = data[i].id + 1;

        if (n > 0) delete[] data;
        return idBaru;
    }

    /* =====================================================
       UPDATE STATUS & LAYANAN
       ===================================================== */
    static bool ubahStatus(int id, StatusPasien statusBaru) {
        Pasien* data;
        int n;
        load(data, n);

        for (int i = 0; i < n; i++) {
            if (data[i].id == id) {
                data[i].status = statusBaru;
                save(data, n);
                delete[] data;
                return true;
            }
        }

        delete[] data;
        return false;
    }

    static bool ubahLayanan(int id, const string& layananBaru) {
        Pasien* data;
        int n;
        load(data, n);

        for (int i = 0; i < n; i++) {
            if (data[i].id == id) {
                data[i].layanan = layananBaru;
                save(data, n);
                delete[] data;
                return true;
            }
        }

        delete[] data;
        return false;
    }

    /* =====================================================
       HUFFMAN TREE (POHON BINER)
       ===================================================== */
    struct HuffmanNode {
        char data;
        int freq;
        HuffmanNode* left;
        HuffmanNode* right;

        HuffmanNode(char d, int f) {
            data = d;
            freq = f;
            left = right = NULL;
        }
    };

    static int bitSebelum(const string& s) {
        return s.length() * 8;
    }

    static int bitSesudah(const string& s) {
        int freq[10] = {0};

        for (char c : s)
            if (c >= '0' && c <= '9')
                freq[c - '0']++;

        int total = 0;
        for (int i = 0; i < 10; i++)
            if (freq[i] > 0)
                total += freq[i] * (1 + i % 3);

        return total;
    }

    static void tampilEfisiensiHuffman(const string& s) {
        int before = bitSebelum(s);
        int after  = bitSesudah(s);

        cout << "Bit sebelum Huffman : " << before << endl;
        cout << "Bit sesudah Huffman : " << after << endl;
        cout << "Efisiensi           : "
             << (100 - (after * 100 / before)) << "%\n";
    }
    
    static bool cariPasien(int id, Pasien& hasil) {
    Pasien* data;
    int n;
    load(data, n);

    for (int i = 0; i < n; i++) {
        if (data[i].id == id) {
            hasil = data[i];
            delete[] data;
            return true;
        }
    }

    if (n > 0) delete[] data;
    return false;
}


    /* =====================================================
       AVL TREE (POHON SEIMBANG)
       ===================================================== */
    struct AVLNode {
        string data;
        AVLNode* left;
        AVLNode* right;
        int height;

        AVLNode(const string& d) {
            data = d;
            left = right = NULL;
            height = 1;
        }
    };

    static int tinggi(AVLNode* n) {
        return n ? n->height : 0;
    }

    static AVLNode* rotasiKanan(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T = x->right;

        x->right = y;
        y->left = T;

        y->height = max(tinggi(y->left), tinggi(y->right)) + 1;
        x->height = max(tinggi(x->left), tinggi(x->right)) + 1;

        return x;
    }

    static AVLNode* rotasiKiri(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T = y->left;

        y->left = x;
        x->right = T;

        x->height = max(tinggi(x->left), tinggi(x->right)) + 1;
        y->height = max(tinggi(y->left), tinggi(y->right)) + 1;

        return y;
    }

    static AVLNode* insertAVL(AVLNode* node, const string& key) {
        if (!node) return new AVLNode(key);

        if (key < node->data)
            node->left = insertAVL(node->left, key);
        else if (key > node->data)
            node->right = insertAVL(node->right, key);
        else
            return node;

        node->height = 1 + max(tinggi(node->left), tinggi(node->right));

        int balance = tinggi(node->left) - tinggi(node->right);

        if (balance > 1 && key < node->left->data)
            return rotasiKanan(node);

        if (balance < -1 && key > node->right->data)
            return rotasiKiri(node);

        return node;
    }
    
    static void inorderAVL(AVLNode* root) {
    if (root != NULL) {
        inorderAVL(root->left);
        cout << root->data << endl;
        inorderAVL(root->right);
    }
}

    
};

#endif
