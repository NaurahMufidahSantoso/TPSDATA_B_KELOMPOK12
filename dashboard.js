/* ================== ROLE CHECK ================== */
const role = localStorage.getItem("role");
if (!role) location.href = "login.html";

/* ================== GLOBAL STATE ================== */
let selectedPasienId = null;

/* ================== DATABASE ================== */
let pasienDB = JSON.parse(localStorage.getItem("pasienDB")) || [];
let antrianDB = JSON.parse(localStorage.getItem("antrianDB")) || [];
let rekamMedisDB = JSON.parse(localStorage.getItem("rekamMedisDB")) || [];

/* ================== DATA DOKTER (STATIS) ================== */
const dokterData = [
  {
    nama: "Dr. Budi Santoso",
    spesialis: "Spesialis Hewan Kecil",
    kategori: "kecil",
    jam: "09:00 – 13:00",
    status: "available",
    tanggal: "2025-12-12"
  },
  {
    nama: "Dr. Siti Aminah",
    spesialis: "Dokter Hewan Umum",
    kategori: "umum",
    jam: "09:00 – 13:00",
    status: "full",
    tanggal: "2025-12-12"
  },
  {
    nama: "Dr. Andi Pratama",
    spesialis: "Reptil & Eksotis",
    kategori: "eksotis",
    jam: "14:00 – 18:00",
    status: "available",
    tanggal: "2025-12-12"
  },
  {
    nama: "Dr. Rina Wijaya",
    spesialis: "Dokter Hewan Umum",
    kategori: "umum",
    jam: "14:00 – 18:00",
    status: "off",
    tanggal: "2025-12-13"
  }
];

/* ================== HELPERS ================== */
function saveAll() {
  localStorage.setItem("pasienDB", JSON.stringify(pasienDB));
  localStorage.setItem("antrianDB", JSON.stringify(antrianDB));
  localStorage.setItem("rekamMedisDB", JSON.stringify(rekamMedisDB));
}

function adminOnly() {
  if (role !== "admin") {
    alert("Akses khusus pegawai klinik");
    return false;
  }
  return true;
}

/* ================== PENDAFTARAN PASIEN ================== */
function enqueuePatient() {
  const nama = document.getElementById("namaPasien")?.value;
  const jenis = document.getElementById("jenisPasien")?.value;
  const priority =
    document.querySelector('input[name="priority"]:checked')?.value || "3";

  if (!nama || !jenis) {
    alert("Lengkapi data pasien");
    return;
  }

  const pasien = {
    id: Date.now(),
    nama,
    jenis,
    status: "menunggu",
    priority
  };

  pasienDB.push(pasien);
  antrianDB.push(pasien);
  saveAll();

  document.getElementById("namaPasien").value = "";
  document.getElementById("jenisPasien").value = "";
  document
    .querySelectorAll('input[name="priority"]')
    .forEach(r => (r.checked = false));

  renderDashboard();
  renderAntrian();
}

/* ================== ANTRIAN ================== */
function renderAntrian() {
  const list = document.getElementById("queueList");
  if (!list) return;
  list.innerHTML = "";

  antrianDB.forEach((p, i) => {
    const li = document.createElement("li");
    li.innerHTML = `
      <b>${i + 1}. ${p.nama}</b> (${p.jenis})
      <button class="mini-btn">Pilih</button>
    `;
    li.querySelector("button").onclick = () => {
      selectedPasienId = p.id;
      alert(`Pasien ${p.nama} dipilih`);
    };
    list.appendChild(li);
  });
}

function callNext() {
  if (!adminOnly()) return;
  if (!antrianDB.length) return alert("Antrian kosong");

  const next = antrianDB.shift();
  next.status = "diperiksa";
  selectedPasienId = next.id;

  pasienDB = pasienDB.map(p => (p.id === next.id ? next : p));
  saveAll();

  renderDashboard();
  renderAntrian();
}

/* ================== DASHBOARD ================== */
function renderDashboard() {
  const current = pasienDB.find(p => p.status === "diperiksa");

  const currentPatientEl = document.getElementById("currentPatient");
  if (currentPatientEl)
    currentPatientEl.innerText = current
      ? `${current.nama} (${current.jenis})`
      : "-";

  const queueCountEl = document.getElementById("queueCount");
  if (queueCountEl) queueCountEl.innerText = antrianDB.length;

  const waitingText = document.getElementById("waitingText");
  if (waitingText)
    waitingText.innerText = antrianDB.length
      ? "Menunggu: " + antrianDB.map(p => p.nama).join(", ")
      : "Tidak ada antrian";

  if (role === "user") {
    document.querySelectorAll(".admin-only").forEach(el => {
      el.style.display = "none";
    });
  }
}

/* ================== DIAGNOSA → REKAM MEDIS ================== */
function simpanDiagnosaForm() {
  if (!selectedPasienId) {
    alert("Pilih pasien dulu");
    return;
  }

  const diagnosis = document.getElementById("diagDiagnosis")?.value;
  const tindakan = document.getElementById("diagTindakan")?.value;
  const dokter = "Dr. Hana";

  if (!diagnosis) {
    alert("Diagnosis wajib diisi");
    return;
  }

  const pasien = pasienDB.find(p => p.id === selectedPasienId);
  if (!pasien) return;

  rekamMedisDB.unshift({
    pasienId: pasien.id,
    nama: pasien.nama,
    tanggal: new Date().toISOString().split("T")[0],
    diagnosis,
    tindakan,
    dokter
  });

  pasien.status = "selesai";
  saveAll();

  renderRekamMedis();
  renderDashboard();

  alert("Diagnosa berhasil disimpan");
}

/* ================== REKAM MEDIS ================== */
function renderRekamMedis() {
  const timeline = document.getElementById("rekamTimeline");
  if (!timeline) return;
  timeline.innerHTML = "";

  const keyword =
    document.getElementById("searchRekam")?.value.toLowerCase() || "";
  const tanggal =
    document.getElementById("filterRekamTanggal")?.value || "";

  rekamMedisDB
    .filter(r => {
      const cocokNama = r.nama.toLowerCase().includes(keyword);
      const cocokTanggal = tanggal ? r.tanggal === tanggal : true;
      return cocokNama && cocokTanggal;
    })
    .forEach(r => {
      const div = document.createElement("div");
      div.className = "timeline-item";
      div.innerHTML = `
        <span class="timeline-dot"></span>
        <div class="timeline-card done">
          <div class="timeline-date">${r.tanggal}</div>
          <ul>
            <li><b>Pasien:</b> ${r.nama}</li>
            <li><b>Diagnosis:</b> ${r.diagnosis}</li>
            <li><b>Tindakan:</b> ${r.tindakan || "-"}</li>
            <li><b>Dokter:</b> ${r.dokter}</li>
          </ul>
        </div>
      `;
      timeline.appendChild(div);
    });
}

/* ================== RAWAT INAP ================== */
function masukkanRawatInap() {
  if (!selectedPasienId) {
    alert("Pilih pasien dulu");
    return;
  }

  const pasien = pasienDB.find(p => p.id === selectedPasienId);
  if (!pasien) return;

  pasien.rawatInap = {
    ruangan: "Observasi",
    tanggalMasuk: new Date().toLocaleDateString()
  };

  saveAll();
  alert("Pasien masuk rawat inap");
}

/* ================== JADWAL DOKTER ================== */
function renderJadwalDokter() {
  const grid = document.querySelector(".jadwal-grid");
  const countEl = document.getElementById("doctorCount");
  if (!grid) return;

  const tanggalInput = document.getElementById("filterTanggal");
  const kategoriInput = document.getElementById("filterDokter");

  const tanggal = tanggalInput && tanggalInput.value ? tanggalInput.value : null;
  const kategori = kategoriInput ? kategoriInput.value : "all";

  const hasil = dokterData.filter(d => {
    const cocokTanggal = tanggal ? d.tanggal === tanggal : true;
    const cocokKategori = kategori === "all" ? true : d.kategori === kategori;
    return cocokTanggal && cocokKategori;
  });

  grid.innerHTML = "";

  hasil.forEach(d => {
    const card = document.createElement("div");
    card.className = `dokter-card ${d.status}`;
    card.innerHTML = `
      <div class="dokter-info">
        <h3>${d.nama}</h3>
        <p>${d.spesialis}</p>
        <span>${d.jam}</span>
      </div>
      <span class="status">${d.status}</span>
    `;
    grid.appendChild(card);
  });

  if (countEl) countEl.innerText = hasil.length;
}

document.getElementById("filterTanggal")
  ?.addEventListener("change", renderJadwalDokter);

document.getElementById("filterDokter")
  ?.addEventListener("change", renderJadwalDokter);


/* ================== NAVIGASI ================== */
document.querySelectorAll(".sidebar li[data-page]").forEach(item => {
  item.onclick = () => {
    document.querySelectorAll(".page").forEach(p =>
      p.classList.remove("active")
    );
    document.getElementById(item.dataset.page)?.classList.add("active");

    document.querySelectorAll(".sidebar li").forEach(li =>
      li.classList.remove("active")
    );
    item.classList.add("active");
  };
});

/* ================== LOGOUT ================== */
function logout() {
  localStorage.clear();
  location.href = "index.html";
}

/* ================== GRAFIK KUNJUNGAN ================== */
function renderKunjunganChart() {
  const canvas = document.getElementById("kunjunganChart");
  if (!canvas) return;

  const ctx = canvas.getContext("2d");
  ctx.clearRect(0, 0, canvas.width, canvas.height);

  // Ambil data 7 hari terakhir
  const days = [];
  const counts = [];

  for (let i = 6; i >= 0; i--) {
    const d = new Date();
    d.setDate(d.getDate() - i);
    const key = d.toISOString().split("T")[0];

    days.push(d.toLocaleDateString("id-ID", { weekday: "short" }));

    const total = pasienDB.filter(p => {
      if (!p.id) return false;
      const created = new Date(p.id).toISOString().split("T")[0];
      return created === key;
    }).length;

    counts.push(total);
  }

  // Konfigurasi grafik
  const max = Math.max(...counts, 1);
  const width = canvas.width;
  const height = canvas.height;
  const padding = 30;
  const barWidth = (width - padding * 2) / counts.length;

  // Axis
  ctx.strokeStyle = "#cbd5e1";
  ctx.beginPath();
  ctx.moveTo(padding, padding);
  ctx.lineTo(padding, height - padding);
  ctx.lineTo(width - padding, height - padding);
  ctx.stroke();

  // Bars
  counts.forEach((val, i) => {
    const barHeight = (val / max) * (height - padding * 2);
    const x = padding + i * barWidth + 10;
    const y = height - padding - barHeight;

    ctx.fillStyle = "#6f89a6";
    ctx.fillRect(x, y, barWidth - 20, barHeight);

    // Label angka
    ctx.fillStyle = "#334155";
    ctx.font = "12px Inter";
    ctx.fillText(val, x + 4, y - 6);

    // Label hari
    ctx.fillText(days[i], x + 2, height - 10);
  });
}

/* ================== INIT ================== */
renderDashboard();
renderAntrian();
renderRekamMedis();
renderJadwalDokter();
renderKunjunganChart();

