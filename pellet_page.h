#include "esphome.h"

const char PELLET_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no">
<meta name="theme-color" content="#0a0a0f">
<meta name="apple-mobile-web-app-capable" content="yes">
<title>Pellet Monitor Pro</title>
<link rel="icon" href="data:image/svg+xml,<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 100 100'><text y='.9em' font-size='90'>🔥</text></svg>">
<style>
@import url('https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700;800&display=swap');

:root {
  --bg-dark: #0a0a0f;
  --bg-card: rgba(255,255,255,0.03);
  --bg-glass: rgba(255,255,255,0.05);
  --border-glass: rgba(255,255,255,0.08);
  --accent: #6366f1;
  --accent-glow: rgba(99,102,241,0.4);
  --orange: #f97316;
  --orange-glow: rgba(249,115,22,0.4);
  --green: #22c55e;
  --yellow: #eab308;
  --red: #ef4444;
  --text: #ffffff;
  --text-secondary: rgba(255,255,255,0.6);
  --text-muted: rgba(255,255,255,0.4);
}

* { margin:0; padding:0; box-sizing:border-box; }

body {
  font-family: 'Inter', -apple-system, BlinkMacSystemFont, sans-serif;
  background: var(--bg-dark);
  background-image: 
    radial-gradient(ellipse at top, rgba(99,102,241,0.1) 0%, transparent 50%),
    radial-gradient(ellipse at bottom right, rgba(249,115,22,0.08) 0%, transparent 50%);
  min-height: 100vh;
  color: var(--text);
  overflow-x: hidden;
  -webkit-font-smoothing: antialiased;
}

/* Scrollbar */
::-webkit-scrollbar { width: 6px; }
::-webkit-scrollbar-track { background: transparent; }
::-webkit-scrollbar-thumb { background: rgba(255,255,255,0.1); border-radius: 3px; }

.app {
  max-width: 500px;
  margin: 0 auto;
  padding: 20px;
  padding-bottom: 100px;
}

/* Glass Card */
.glass {
  background: var(--bg-glass);
  backdrop-filter: blur(20px);
  -webkit-backdrop-filter: blur(20px);
  border: 1px solid var(--border-glass);
  border-radius: 24px;
}

/* Header */
.header {
  text-align: center;
  padding: 30px 20px;
  position: relative;
}

.logo {
  font-size: 3rem;
  margin-bottom: 10px;
  animation: float 3s ease-in-out infinite;
}

@keyframes float {
  0%, 100% { transform: translateY(0); }
  50% { transform: translateY(-10px); }
}

.header h1 {
  font-size: 1.5rem;
  font-weight: 700;
  letter-spacing: -0.5px;
  margin-bottom: 4px;
}

.header .tagline {
  font-size: 0.8rem;
  color: var(--text-muted);
  font-weight: 400;
}

.connection-status {
  display: inline-flex;
  align-items: center;
  gap: 8px;
  margin-top: 16px;
  padding: 8px 16px;
  border-radius: 100px;
  font-size: 0.75rem;
  font-weight: 500;
  background: rgba(34,197,94,0.1);
  border: 1px solid rgba(34,197,94,0.2);
  color: var(--green);
}

.connection-status.offline {
  background: rgba(239,68,68,0.1);
  border-color: rgba(239,68,68,0.2);
  color: var(--red);
}

.connection-status::before {
  content: '';
  width: 8px;
  height: 8px;
  border-radius: 50%;
  background: currentColor;
  animation: blink 2s infinite;
}

@keyframes blink {
  0%, 100% { opacity: 1; }
  50% { opacity: 0.4; }
}

/* Alert */
.alert-banner {
  background: linear-gradient(135deg, rgba(239,68,68,0.2), rgba(239,68,68,0.1));
  border: 1px solid rgba(239,68,68,0.3);
  border-radius: 16px;
  padding: 16px 20px;
  margin: 20px 0;
  display: none;
  align-items: center;
  gap: 14px;
  animation: alertPulse 2s infinite;
}

.alert-banner.show { display: flex; }

@keyframes alertPulse {
  0%, 100% { box-shadow: 0 0 0 0 rgba(239,68,68,0.4); }
  50% { box-shadow: 0 0 20px 5px rgba(239,68,68,0.2); }
}

.alert-banner .icon { font-size: 1.8rem; }
.alert-banner .content h4 { font-size: 0.9rem; font-weight: 600; color: var(--red); }
.alert-banner .content p { font-size: 0.75rem; color: var(--text-secondary); margin-top: 2px; }

/* Main Gauge Card */
.gauge-section {
  margin: 20px 0;
  padding: 30px 20px;
  position: relative;
  overflow: hidden;
}

.gauge-section::before {
  content: '';
  position: absolute;
  top: -100px;
  left: 50%;
  transform: translateX(-50%);
  width: 300px;
  height: 300px;
  background: radial-gradient(circle, var(--orange-glow) 0%, transparent 70%);
  opacity: 0.3;
  pointer-events: none;
}

.gauge-container {
  display: flex;
  justify-content: center;
  align-items: center;
  gap: 40px;
  position: relative;
  z-index: 1;
}

/* Circular Gauge */
.circular-gauge {
  position: relative;
  width: 200px;
  height: 200px;
}

.circular-gauge svg {
  transform: rotate(-90deg);
  width: 100%;
  height: 100%;
}

.gauge-track {
  fill: none;
  stroke: rgba(255,255,255,0.05);
  stroke-width: 12;
}

.gauge-progress {
  fill: none;
  stroke-width: 12;
  stroke-linecap: round;
  transition: stroke-dashoffset 1s cubic-bezier(0.4, 0, 0.2, 1), stroke 0.5s;
  filter: drop-shadow(0 0 12px var(--orange-glow));
}

.gauge-center {
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
  text-align: center;
}

.gauge-value {
  font-size: 3.5rem;
  font-weight: 800;
  line-height: 1;
  background: linear-gradient(180deg, #fff 0%, rgba(255,255,255,0.7) 100%);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
}

.gauge-label {
  font-size: 0.7rem;
  color: var(--text-muted);
  text-transform: uppercase;
  letter-spacing: 3px;
  margin-top: 8px;
}

/* Tank Visualization */
.tank-wrapper {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 10px;
}

.tank-3d {
  width: 60px;
  height: 140px;
  position: relative;
  perspective: 500px;
}

.tank-body {
  width: 100%;
  height: 100%;
  background: linear-gradient(90deg, 
    rgba(255,255,255,0.02) 0%, 
    rgba(255,255,255,0.08) 50%, 
    rgba(255,255,255,0.02) 100%);
  border: 2px solid rgba(255,255,255,0.15);
  border-radius: 12px;
  position: relative;
  overflow: hidden;
  box-shadow: 
    inset 0 0 30px rgba(0,0,0,0.5),
    0 20px 40px rgba(0,0,0,0.3);
}

.tank-level {
  position: absolute;
  bottom: 0;
  left: 0;
  right: 0;
  background: linear-gradient(180deg, 
    var(--yellow) 0%, 
    var(--orange) 50%, 
    #ea580c 100%);
  transition: height 1s cubic-bezier(0.4, 0, 0.2, 1);
  box-shadow: 
    0 -10px 30px var(--orange-glow),
    inset 0 5px 15px rgba(255,255,255,0.2);
}

.tank-level::before {
  content: '';
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  height: 20px;
  background: linear-gradient(180deg, rgba(255,255,255,0.3), transparent);
}

.tank-shine {
  position: absolute;
  top: 0;
  left: 5px;
  width: 8px;
  height: 100%;
  background: linear-gradient(180deg, 
    rgba(255,255,255,0.1) 0%, 
    rgba(255,255,255,0.02) 100%);
  border-radius: 4px;
}

.tank-scale {
  position: absolute;
  right: -35px;
  top: 0;
  bottom: 0;
  display: flex;
  flex-direction: column;
  justify-content: space-between;
  font-size: 0.6rem;
  color: var(--text-muted);
  padding: 5px 0;
}

/* Status Badge */
.status-display {
  text-align: center;
  margin-top: 30px;
}

.status-pill {
  display: inline-flex;
  align-items: center;
  gap: 8px;
  padding: 14px 32px;
  border-radius: 100px;
  font-size: 0.9rem;
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: 2px;
  transition: all 0.3s;
}

.status-pill.full { background: linear-gradient(135deg, var(--green), #16a34a); color: #000; }
.status-pill.good { background: linear-gradient(135deg, #3b82f6, #2563eb); }
.status-pill.medium { background: linear-gradient(135deg, var(--yellow), #ca8a04); color: #000; }
.status-pill.low { background: linear-gradient(135deg, var(--orange), #ea580c); }
.status-pill.critical { 
  background: linear-gradient(135deg, var(--red), #dc2626); 
  animation: criticalPulse 1s infinite;
}

@keyframes criticalPulse {
  0%, 100% { transform: scale(1); }
  50% { transform: scale(1.02); }
}

/* Stats Grid */
.stats-grid {
  display: grid;
  grid-template-columns: repeat(2, 1fr);
  gap: 12px;
  margin: 20px 0;
}

.stat-card {
  padding: 20px 16px;
  text-align: center;
  transition: transform 0.2s, border-color 0.2s;
}

.stat-card:hover {
  transform: translateY(-4px);
  border-color: rgba(99,102,241,0.3);
}

.stat-icon {
  font-size: 1.5rem;
  margin-bottom: 12px;
  display: block;
}

.stat-value {
  font-size: 1.8rem;
  font-weight: 700;
  line-height: 1;
  margin-bottom: 6px;
}

.stat-label {
  font-size: 0.65rem;
  color: var(--text-muted);
  text-transform: uppercase;
  letter-spacing: 1px;
}

/* Navigation */
.bottom-nav {
  position: fixed;
  bottom: 0;
  left: 0;
  right: 0;
  background: rgba(10,10,15,0.95);
  backdrop-filter: blur(20px);
  -webkit-backdrop-filter: blur(20px);
  border-top: 1px solid var(--border-glass);
  display: flex;
  justify-content: center;
  padding: 12px 0 calc(12px + env(safe-area-inset-bottom));
  z-index: 1000;
}

.nav-item {
  flex: 1;
  max-width: 100px;
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 4px;
  padding: 8px;
  color: var(--text-muted);
  text-decoration: none;
  transition: color 0.2s;
  cursor: pointer;
  border: none;
  background: none;
  font-family: inherit;
}

.nav-item.active { color: var(--accent); }
.nav-item .icon { font-size: 1.3rem; }
.nav-item .label { font-size: 0.6rem; text-transform: uppercase; letter-spacing: 0.5px; }

/* Pages */
.page { display: none; }
.page.active { display: block; animation: pageIn 0.3s ease; }

@keyframes pageIn {
  from { opacity: 0; transform: translateY(20px); }
  to { opacity: 1; transform: translateY(0); }
}

/* Settings */
.settings-group {
  margin: 20px 0;
  padding: 20px;
}

.settings-group-title {
  display: flex;
  align-items: center;
  gap: 10px;
  font-size: 0.75rem;
  font-weight: 600;
  color: var(--text-secondary);
  text-transform: uppercase;
  letter-spacing: 1px;
  margin-bottom: 16px;
  padding-bottom: 12px;
  border-bottom: 1px solid var(--border-glass);
}

.setting-item {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 16px 0;
  border-bottom: 1px solid rgba(255,255,255,0.03);
}

.setting-item:last-child { border-bottom: none; }

.setting-info h4 {
  font-size: 0.9rem;
  font-weight: 500;
  margin-bottom: 3px;
}

.setting-info p {
  font-size: 0.7rem;
  color: var(--text-muted);
}

.setting-control {
  display: flex;
  align-items: center;
  gap: 10px;
}

.input-field {
  width: 90px;
  padding: 10px 14px;
  background: rgba(0,0,0,0.3);
  border: 1px solid var(--border-glass);
  border-radius: 12px;
  color: var(--text);
  font-size: 0.9rem;
  font-family: inherit;
  text-align: center;
  transition: border-color 0.2s, box-shadow 0.2s;
}

.input-field:focus {
  outline: none;
  border-color: var(--accent);
  box-shadow: 0 0 0 3px var(--accent-glow);
}

.input-field.wide { width: 160px; text-align: left; }

.input-unit {
  font-size: 0.75rem;
  color: var(--text-muted);
  min-width: 30px;
}

/* Toggle Switch */
.toggle {
  width: 50px;
  height: 28px;
  background: rgba(255,255,255,0.1);
  border-radius: 100px;
  position: relative;
  cursor: pointer;
  transition: background 0.3s;
}

.toggle.active { background: var(--accent); }

.toggle::after {
  content: '';
  position: absolute;
  top: 3px;
  left: 3px;
  width: 22px;
  height: 22px;
  background: #fff;
  border-radius: 50%;
  transition: transform 0.3s;
  box-shadow: 0 2px 8px rgba(0,0,0,0.3);
}

.toggle.active::after { transform: translateX(22px); }

/* Buttons */
.btn {
  display: inline-flex;
  align-items: center;
  justify-content: center;
  gap: 10px;
  padding: 14px 28px;
  border: none;
  border-radius: 14px;
  font-size: 0.9rem;
  font-weight: 600;
  font-family: inherit;
  cursor: pointer;
  transition: transform 0.15s, box-shadow 0.15s;
  width: 100%;
}

.btn:active { transform: scale(0.97); }

.btn-primary {
  background: linear-gradient(135deg, var(--accent), #4f46e5);
  color: #fff;
  box-shadow: 0 8px 24px var(--accent-glow);
}

.btn-secondary {
  background: var(--bg-glass);
  border: 1px solid var(--border-glass);
  color: var(--text);
}

.btn-danger {
  background: linear-gradient(135deg, var(--red), #dc2626);
  color: #fff;
}

.btn-success {
  background: linear-gradient(135deg, var(--green), #16a34a);
  color: #fff;
}

.btn-group {
  display: grid;
  grid-template-columns: repeat(2, 1fr);
  gap: 12px;
  margin-top: 16px;
}

/* Calibration Card */
.calibration-card {
  background: linear-gradient(135deg, rgba(99,102,241,0.1), rgba(99,102,241,0.05));
  border: 1px solid rgba(99,102,241,0.2);
  border-radius: 20px;
  padding: 24px;
  margin: 20px 0;
  text-align: center;
}

.calibration-card h3 {
  font-size: 1rem;
  margin-bottom: 8px;
  color: var(--accent);
}

.calibration-card p {
  font-size: 0.8rem;
  color: var(--text-secondary);
  margin-bottom: 16px;
}

.calibration-value {
  font-size: 3rem;
  font-weight: 800;
  color: var(--accent);
  margin: 20px 0;
  text-shadow: 0 0 30px var(--accent-glow);
}

/* Info Card */
.info-section {
  margin: 20px 0;
  padding: 20px;
}

.info-row {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 14px 0;
  border-bottom: 1px solid rgba(255,255,255,0.03);
  font-size: 0.85rem;
}

.info-row:last-child { border-bottom: none; }
.info-row .label { color: var(--text-muted); }
.info-row .value { font-weight: 500; font-family: 'SF Mono', monospace; }

/* Toast */
.toast {
  position: fixed;
  bottom: 100px;
  left: 50%;
  transform: translateX(-50%) translateY(100px);
  background: rgba(0,0,0,0.9);
  backdrop-filter: blur(20px);
  -webkit-backdrop-filter: blur(20px);
  color: #fff;
  padding: 14px 28px;
  border-radius: 14px;
  font-size: 0.85rem;
  font-weight: 500;
  opacity: 0;
  transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
  z-index: 2000;
  display: flex;
  align-items: center;
  gap: 10px;
}

.toast.show {
  transform: translateX(-50%) translateY(0);
  opacity: 1;
}

.toast.success { border-left: 4px solid var(--green); }
.toast.error { border-left: 4px solid var(--red); }
.toast.info { border-left: 4px solid var(--accent); }

/* OTA Upload Styles */
.ota-upload-zone {
  border: 2px dashed var(--border-glass);
  border-radius: 16px;
  padding: 30px 20px;
  text-align: center;
  cursor: pointer;
  transition: all 0.3s;
  background: rgba(0,0,0,0.2);
}

.ota-upload-zone:hover,
.ota-upload-zone.dragover {
  border-color: var(--accent);
  background: rgba(99,102,241,0.1);
}

.ota-upload-content {
  pointer-events: none;
}

.ota-icon {
  font-size: 3rem;
  display: block;
  margin-bottom: 12px;
}

.ota-text {
  font-size: 0.9rem;
  color: var(--text-secondary);
  margin-bottom: 4px;
}

.ota-hint {
  font-size: 0.7rem;
  color: var(--text-muted);
}

.ota-file-info {
  display: flex;
  align-items: center;
  gap: 12px;
  padding: 14px 16px;
  background: rgba(34,197,94,0.1);
  border: 1px solid rgba(34,197,94,0.2);
  border-radius: 12px;
  margin-top: 12px;
}

.file-icon {
  font-size: 1.5rem;
}

.file-details {
  flex: 1;
}

.file-name {
  font-size: 0.85rem;
  font-weight: 500;
  color: var(--green);
}

.file-size {
  font-size: 0.7rem;
  color: var(--text-muted);
}

.btn-icon {
  width: 32px;
  height: 32px;
  border: none;
  background: rgba(255,255,255,0.1);
  border-radius: 8px;
  color: var(--text);
  cursor: pointer;
  font-size: 1rem;
  display: flex;
  align-items: center;
  justify-content: center;
}

.ota-progress {
  margin-top: 16px;
}

.progress-bar {
  height: 8px;
  background: rgba(255,255,255,0.1);
  border-radius: 4px;
  overflow: hidden;
}

.progress-fill {
  height: 100%;
  width: 0%;
  background: linear-gradient(90deg, var(--accent), var(--green));
  border-radius: 4px;
  transition: width 0.3s;
}

.progress-text {
  font-size: 0.8rem;
  color: var(--text-secondary);
  text-align: center;
  margin-top: 10px;
}

/* Network Config */
.network-status {
  display: flex;
  align-items: center;
  gap: 12px;
  padding: 16px;
  background: rgba(34,197,94,0.1);
  border: 1px solid rgba(34,197,94,0.2);
  border-radius: 12px;
  margin-bottom: 20px;
}

.network-status.warning {
  background: rgba(234,179,8,0.1);
  border-color: rgba(234,179,8,0.2);
}

.network-status .icon { font-size: 1.5rem; }
.network-status .info { flex: 1; }
.network-status .info h4 { font-size: 0.85rem; font-weight: 600; }
.network-status .info p { font-size: 0.7rem; color: var(--text-secondary); }

/* IP Input Group */
.ip-input-group {
  display: flex;
  gap: 8px;
  align-items: center;
}

.ip-input-group input {
  width: 55px;
  padding: 10px 8px;
  background: rgba(0,0,0,0.3);
  border: 1px solid var(--border-glass);
  border-radius: 10px;
  color: var(--text);
  font-size: 0.9rem;
  font-family: 'SF Mono', monospace;
  text-align: center;
}

.ip-input-group input:focus {
  outline: none;
  border-color: var(--accent);
}

.ip-input-group span {
  color: var(--text-muted);
  font-weight: 600;
}
</style>
</head>
<body>
<div class="app">
  <!-- HOME PAGE -->
  <div id="page-home" class="page active">
    <header class="header">
      <div class="logo">🔥</div>
      <h1>Pellet Monitor Pro</h1>
      <p class="tagline">Poêle Moretti Design</p>
      <div id="connStatus" class="connection-status">Connecté</div>
    </header>

    <div id="alertBanner" class="alert-banner">
      <span class="icon">⚠️</span>
      <div class="content">
        <h4>Niveau Critique</h4>
        <p>Le réservoir doit être rechargé rapidement</p>
      </div>
    </div>

    <section class="gauge-section glass">
      <div class="gauge-container">
        <div class="circular-gauge">
          <svg viewBox="0 0 200 200">
            <circle class="gauge-track" cx="100" cy="100" r="85"/>
            <circle id="gaugeProgress" class="gauge-progress" cx="100" cy="100" r="85" 
                    stroke="var(--orange)" stroke-dasharray="534" stroke-dashoffset="534"/>
          </svg>
          <div class="gauge-center">
            <div id="gaugeValue" class="gauge-value">--</div>
            <div class="gauge-label">Niveau</div>
          </div>
        </div>

        <div class="tank-wrapper">
          <div class="tank-3d">
            <div class="tank-body">
              <div id="tankLevel" class="tank-level" style="height: 0%"></div>
              <div class="tank-shine"></div>
            </div>
            <div class="tank-scale">
              <span>100</span>
              <span>75</span>
              <span>50</span>
              <span>25</span>
              <span>0</span>
            </div>
          </div>
        </div>
      </div>

      <div class="status-display">
        <div id="statusPill" class="status-pill good">● BON</div>
      </div>
    </section>

    <div class="stats-grid">
      <div class="stat-card glass">
        <span class="stat-icon">📏</span>
        <div id="statHeight" class="stat-value">--</div>
        <div class="stat-label">Hauteur (mm)</div>
      </div>
      <div class="stat-card glass">
        <span class="stat-icon">⚖️</span>
        <div id="statKg" class="stat-value">--</div>
        <div class="stat-label">Stock (kg)</div>
      </div>
      <div class="stat-card glass">
        <span class="stat-icon">📅</span>
        <div id="statDays" class="stat-value">--</div>
        <div class="stat-label">Autonomie (j)</div>
      </div>
      <div class="stat-card glass">
        <span class="stat-icon">🎯</span>
        <div id="statDistance" class="stat-value">--</div>
        <div class="stat-label">Distance (mm)</div>
      </div>
    </div>

    <section class="info-section glass">
      <div class="info-row">
        <span class="label">Signal WiFi</span>
        <span id="infoWifi" class="value">-- dBm</span>
      </div>
      <div class="info-row">
        <span class="label">Adresse IP</span>
        <span id="infoIp" class="value">--</span>
      </div>
      <div class="info-row">
        <span class="label">Dernière MAJ</span>
        <span id="infoUpdate" class="value">--</span>
      </div>
    </section>
  </div>

  <!-- SETTINGS PAGE -->
  <div id="page-settings" class="page">
    <header class="header">
      <div class="logo">⚙️</div>
      <h1>Paramètres</h1>
      <p class="tagline">Configuration du capteur</p>
    </header>

    <div class="calibration-card">
      <h3>🎯 Calibration</h3>
      <p>Distance actuelle mesurée par le capteur</p>
      <div id="calibDist" class="calibration-value">-- mm</div>
      <div class="btn-group">
        <button class="btn btn-secondary" onclick="calibrateEmpty()">
          📭 Vide
        </button>
        <button class="btn btn-secondary" onclick="calibrateFull()">
          📬 Plein
        </button>
      </div>
    </div>

    <div class="settings-group glass">
      <div class="settings-group-title">
        <span>📐</span> Dimensions
      </div>
      <div class="setting-item">
        <div class="setting-info">
          <h4>Hauteur réservoir</h4>
          <p>Distance du fond au capteur</p>
        </div>
        <div class="setting-control">
          <input type="number" id="cfgHeight" class="input-field" min="100" max="1000" step="10">
          <span class="input-unit">mm</span>
        </div>
      </div>
      <div class="setting-item">
        <div class="setting-info">
          <h4>Offset capteur</h4>
          <p>Distance capteur → granulés (plein)</p>
        </div>
        <div class="setting-control">
          <input type="number" id="cfgOffset" class="input-field" min="0" max="200" step="5">
          <span class="input-unit">mm</span>
        </div>
      </div>
    </div>

    <div class="settings-group glass">
      <div class="settings-group-title">
        <span>⚖️</span> Capacité
      </div>
      <div class="setting-item">
        <div class="setting-info">
          <h4>Capacité réservoir</h4>
          <p>Poids maximum de granulés</p>
        </div>
        <div class="setting-control">
          <input type="number" id="cfgCapacity" class="input-field" min="5" max="50" step="1">
          <span class="input-unit">kg</span>
        </div>
      </div>
      <div class="setting-item">
        <div class="setting-info">
          <h4>Consommation</h4>
          <p>Estimation journalière moyenne</p>
        </div>
        <div class="setting-control">
          <input type="number" id="cfgConso" class="input-field" min="0.5" max="10" step="0.5">
          <span class="input-unit">kg/j</span>
        </div>
      </div>
    </div>

    <div class="settings-group glass">
      <div class="settings-group-title">
        <span>🚨</span> Alertes
      </div>
      <div class="setting-item">
        <div class="setting-info">
          <h4>Seuil niveau bas</h4>
          <p>Alerte si niveau inférieur</p>
        </div>
        <div class="setting-control">
          <input type="number" id="cfgAlert" class="input-field" min="10" max="200" step="10">
          <span class="input-unit">mm</span>
        </div>
      </div>
    </div>

    <button class="btn btn-primary" onclick="saveSettings()" style="margin: 20px 0;">
      💾 Enregistrer
    </button>
  </div>

  <!-- NETWORK PAGE -->
  <div id="page-network" class="page">
    <header class="header">
      <div class="logo">📡</div>
      <h1>Réseau</h1>
      <p class="tagline">Configuration réseau</p>
    </header>

    <div id="netStatus" class="network-status">
      <span class="icon">✅</span>
      <div class="info">
        <h4>Connecté</h4>
        <p id="netStatusText">DHCP - IP automatique</p>
      </div>
    </div>

    <div class="settings-group glass">
      <div class="settings-group-title">
        <span>📶</span> Connexion actuelle
      </div>
      <div class="info-row">
        <span class="label">Réseau WiFi</span>
        <span id="netSsid" class="value">--</span>
      </div>
      <div class="info-row">
        <span class="label">Adresse IP</span>
        <span id="netIp" class="value">--</span>
      </div>
      <div class="info-row">
        <span class="label">Adresse MAC</span>
        <span id="netMac" class="value">--</span>
      </div>
      <div class="info-row">
        <span class="label">Signal</span>
        <span id="netSignal" class="value">-- dBm</span>
      </div>
    </div>

    <div class="settings-group glass">
      <div class="settings-group-title">
        <span>🔧</span> Configuration IP
      </div>
      <div class="setting-item">
        <div class="setting-info">
          <h4>Utiliser IP statique</h4>
          <p>Désactiver le DHCP</p>
        </div>
        <div id="toggleStatic" class="toggle" onclick="toggleStaticIp()"></div>
      </div>
      
      <div id="staticIpConfig" style="display: none;">
        <div class="setting-item">
          <div class="setting-info">
            <h4>Adresse IP</h4>
          </div>
          <input type="text" id="cfgStaticIp" class="input-field wide" placeholder="192.168.1.100">
        </div>
        <div class="setting-item">
          <div class="setting-info">
            <h4>Passerelle</h4>
          </div>
          <input type="text" id="cfgGateway" class="input-field wide" placeholder="192.168.1.1">
        </div>
        <div class="setting-item">
          <div class="setting-info">
            <h4>Masque</h4>
          </div>
          <input type="text" id="cfgSubnet" class="input-field wide" placeholder="255.255.255.0">
        </div>
        <div class="setting-item">
          <div class="setting-info">
            <h4>DNS</h4>
          </div>
          <input type="text" id="cfgDns" class="input-field wide" placeholder="8.8.8.8">
        </div>
      </div>
    </div>

    <div class="settings-group glass">
      <div class="settings-group-title">
        <span>ℹ️</span> Information
      </div>
      <p style="font-size: 0.8rem; color: var(--text-secondary); line-height: 1.6;">
        ⚠️ <strong>Note :</strong> La configuration IP statique nécessite de modifier le fichier YAML d'ESPHome et de recompiler le firmware. Les paramètres ci-dessus sont sauvegardés pour référence.
      </p>
    </div>

    <button class="btn btn-primary" onclick="saveNetworkConfig()" style="margin: 20px 0;">
      💾 Sauvegarder config réseau
    </button>
  </div>

  <!-- SYSTEM PAGE -->
  <div id="page-system" class="page">
    <header class="header">
      <div class="logo">🔧</div>
      <h1>Système</h1>
      <p class="tagline">Informations & maintenance</p>
    </header>

    <div class="settings-group glass">
      <div class="settings-group-title">
        <span>📊</span> Informations
      </div>
      <div class="info-row">
        <span class="label">Version ESPHome</span>
        <span id="sysVersion" class="value">--</span>
      </div>
      <div class="info-row">
        <span class="label">Uptime</span>
        <span id="sysUptime" class="value">--</span>
      </div>
      <div class="info-row">
        <span class="label">Mémoire libre</span>
        <span id="sysMem" class="value">-- KB</span>
      </div>
      <div class="info-row">
        <span class="label">Signal WiFi</span>
        <span id="sysWifi" class="value">-- dBm</span>
      </div>
    </div>

    <div class="settings-group glass">
      <div class="settings-group-title">
        <span>🛠️</span> Maintenance
      </div>
      <div class="btn-group">
        <button class="btn btn-secondary" onclick="fetchData()">
          🔄 Actualiser
        </button>
        <button class="btn btn-danger" onclick="restartDevice()">
          🔌 Redémarrer
        </button>
      </div>
      <button class="btn btn-secondary" onclick="factoryReset()" style="margin-top: 12px;">
        ⚠️ Réinitialisation usine
      </button>
    </div>

    <div class="settings-group glass">
      <div class="settings-group-title">
        <span>📦</span> Mise à jour OTA
      </div>
      <p style="font-size: 0.8rem; color: var(--text-secondary); margin-bottom: 16px; line-height: 1.6;">
        Téléversez un nouveau firmware (.bin) pour mettre à jour le capteur sans fil.
      </p>
      
      <div class="ota-upload-zone" id="otaDropZone">
        <input type="file" id="otaFile" accept=".bin" style="display: none;" onchange="handleFileSelect(event)">
        <div class="ota-upload-content" onclick="document.getElementById('otaFile').click()">
          <span class="ota-icon">📤</span>
          <p class="ota-text">Cliquez ou glissez un fichier .bin</p>
          <p class="ota-hint">firmware.bin</p>
        </div>
      </div>
      
      <div id="otaProgress" class="ota-progress" style="display: none;">
        <div class="progress-bar">
          <div id="progressFill" class="progress-fill"></div>
        </div>
        <p id="progressText" class="progress-text">Préparation...</p>
      </div>
      
      <div id="otaFileInfo" class="ota-file-info" style="display: none;">
        <span class="file-icon">📄</span>
        <div class="file-details">
          <p id="fileName" class="file-name">firmware.bin</p>
          <p id="fileSize" class="file-size">0 KB</p>
        </div>
        <button class="btn-icon" onclick="clearFile()">✕</button>
      </div>
      
      <button id="otaUploadBtn" class="btn btn-success" onclick="startOtaUpload()" style="margin-top: 16px; display: none;">
        🚀 Lancer la mise à jour
      </button>
    </div>

    <div class="settings-group glass">
      <div class="settings-group-title">
        <span>📱</span> À propos
      </div>
      <p style="font-size: 0.75rem; color: var(--text-muted); text-align: center; line-height: 1.8;">
        Pellet Monitor Pro v3.0<br>
        ESP8266 + VL53L0X<br>
        Interface développée avec ❤️
      </p>
    </div>
  </div>
</div>

<!-- Navigation -->
<nav class="bottom-nav">
  <button class="nav-item active" onclick="showPage('home')">
    <span class="icon">🏠</span>
    <span class="label">Accueil</span>
  </button>
  <button class="nav-item" onclick="showPage('settings')">
    <span class="icon">⚙️</span>
    <span class="label">Capteur</span>
  </button>
  <button class="nav-item" onclick="showPage('network')">
    <span class="icon">📡</span>
    <span class="label">Réseau</span>
  </button>
  <button class="nav-item" onclick="showPage('system')">
    <span class="icon">🔧</span>
    <span class="label">Système</span>
  </button>
</nav>

<!-- Toast -->
<div id="toast" class="toast"></div>

<script>
const $ = id => document.getElementById(id);
let data = {};
let staticIpEnabled = false;

// Navigation
function showPage(page) {
  document.querySelectorAll('.page').forEach(p => p.classList.remove('active'));
  document.querySelectorAll('.nav-item').forEach(n => n.classList.remove('active'));
  $('page-' + page).classList.add('active');
  event.currentTarget.classList.add('active');
}

// Toast
function toast(msg, type = 'success') {
  const t = $('toast');
  t.innerHTML = (type === 'success' ? '✓' : type === 'error' ? '✗' : 'ℹ') + ' ' + msg;
  t.className = 'toast show ' + type;
  setTimeout(() => t.classList.remove('show'), 3000);
}

// Fetch data from ESP
async function fetchData() {
  try {
    const res = await fetch('/states');
    const states = await res.json();
    data = {};
    states.forEach(s => data[s.id] = s.value);
    updateUI();
    $('connStatus').className = 'connection-status';
    $('connStatus').textContent = 'Connecté';
  } catch (e) {
    $('connStatus').className = 'connection-status offline';
    $('connStatus').textContent = 'Hors ligne';
  }
}

function updateUI() {
  // Gauge
  const pct = parseFloat(data.pellet_level_percent) || 0;
  $('gaugeValue').textContent = Math.round(pct) + '%';
  
  // Progress ring (circumference = 2 * PI * 85 = 534)
  const offset = 534 - (pct / 100 * 534);
  const prog = $('gaugeProgress');
  prog.style.strokeDashoffset = offset;
  prog.style.stroke = pct > 50 ? 'var(--green)' : pct > 25 ? 'var(--yellow)' : 'var(--red)';
  
  // Tank
  $('tankLevel').style.height = pct + '%';
  
  // Status pill
  const pill = $('statusPill');
  let cls = 'good', txt = '● BON';
  if (pct > 75) { cls = 'full'; txt = '✓ PLEIN'; }
  else if (pct > 50) { cls = 'good'; txt = '● BON'; }
  else if (pct > 25) { cls = 'medium'; txt = '◐ MOYEN'; }
  else if (pct > 10) { cls = 'low'; txt = '◔ BAS'; }
  else { cls = 'critical'; txt = '⚠ CRITIQUE'; }
  pill.className = 'status-pill ' + cls;
  pill.textContent = txt;
  
  // Alert
  const levelMm = parseFloat(data.pellet_level_mm) || 0;
  const threshold = parseFloat(data.min_level) || 50;
  $('alertBanner').classList.toggle('show', levelMm < threshold && levelMm > 0);
  
  // Stats
  $('statHeight').textContent = Math.round(levelMm);
  $('statKg').textContent = (parseFloat(data.kg_remaining) || 0).toFixed(1);
  $('statDays').textContent = (parseFloat(data.days_remaining) || 0).toFixed(1);
  $('statDistance').textContent = Math.round(parseFloat(data.distance_raw) || 0);
  
  // Info
  $('infoWifi').textContent = Math.round(parseFloat(data.wifi_signal_db) || 0) + ' dBm';
  $('infoIp').textContent = data.current_ip || '--';
  $('infoUpdate').textContent = new Date().toLocaleTimeString('fr-FR');
  
  // Calibration
  $('calibDist').textContent = Math.round(parseFloat(data.distance_raw) || 0) + ' mm';
  
  // Settings values
  $('cfgHeight').value = parseFloat(data.reservoir_height) || 400;
  $('cfgOffset').value = parseFloat(data.sensor_offset) || 30;
  $('cfgCapacity').value = parseFloat(data.capacity_kg) || 15;
  $('cfgConso').value = parseFloat(data.consumption_per_day) || 1.5;
  $('cfgAlert').value = parseFloat(data.min_level) || 50;
  
  // Network
  $('netSsid').textContent = data.wifi_ssid || '--';
  $('netIp').textContent = data.current_ip || '--';
  $('netMac').textContent = data.wifi_mac || '--';
  $('netSignal').textContent = Math.round(parseFloat(data.wifi_signal_db) || 0) + ' dBm';
  
  // System
  $('sysVersion').textContent = data.esphome_version || '--';
  $('sysMem').textContent = Math.round(parseFloat(data.free_memory) || 0) + ' KB';
  $('sysWifi').textContent = Math.round(parseFloat(data.wifi_signal_db) || 0) + ' dBm';
  
  // Uptime
  const up = parseFloat(data.uptime_sensor) || 0;
  const d = Math.floor(up / 86400);
  const h = Math.floor((up % 86400) / 3600);
  const m = Math.floor((up % 3600) / 60);
  $('sysUptime').textContent = d > 0 ? `${d}j ${h}h ${m}m` : h > 0 ? `${h}h ${m}m` : `${m}m`;
}

// Save sensor settings
async function saveSettings() {
  const settings = [
    ['reservoir_height', $('cfgHeight').value],
    ['sensor_offset', $('cfgOffset').value],
    ['capacity_kg', $('cfgCapacity').value],
    ['consumption_per_day', $('cfgConso').value],
    ['min_level', $('cfgAlert').value]
  ];
  
  try {
    await Promise.all(settings.map(([id, val]) => 
      fetch(`/number/${id}/set?value=${val}`, { method: 'POST' })
    ));
    toast('Paramètres enregistrés');
    setTimeout(fetchData, 500);
  } catch (e) {
    toast('Erreur de sauvegarde', 'error');
  }
}

// Toggle static IP
function toggleStaticIp() {
  staticIpEnabled = !staticIpEnabled;
  $('toggleStatic').classList.toggle('active', staticIpEnabled);
  $('staticIpConfig').style.display = staticIpEnabled ? 'block' : 'none';
  
  if (staticIpEnabled) {
    $('netStatus').classList.add('warning');
    $('netStatusText').textContent = 'Configuration IP statique';
  } else {
    $('netStatus').classList.remove('warning');
    $('netStatusText').textContent = 'DHCP - IP automatique';
  }
}

// Save network config
function saveNetworkConfig() {
  const config = {
    staticIp: staticIpEnabled,
    ip: $('cfgStaticIp').value,
    gateway: $('cfgGateway').value,
    subnet: $('cfgSubnet').value,
    dns: $('cfgDns').value
  };
  
  localStorage.setItem('pellet_network_config', JSON.stringify(config));
  toast('Configuration réseau sauvegardée', 'info');
  
  // Show instructions
  if (staticIpEnabled) {
    alert(
      'Pour appliquer l\'IP statique, modifiez le fichier YAML :\n\n' +
      'wifi:\n' +
      '  manual_ip:\n' +
      '    static_ip: ' + config.ip + '\n' +
      '    gateway: ' + config.gateway + '\n' +
      '    subnet: ' + config.subnet + '\n' +
      '    dns1: ' + config.dns + '\n\n' +
      'Puis recompilez et reflashez le firmware.'
    );
  }
}

// Load saved network config
function loadNetworkConfig() {
  try {
    const saved = JSON.parse(localStorage.getItem('pellet_network_config'));
    if (saved) {
      if (saved.staticIp) toggleStaticIp();
      $('cfgStaticIp').value = saved.ip || '';
      $('cfgGateway').value = saved.gateway || '';
      $('cfgSubnet').value = saved.subnet || '';
      $('cfgDns').value = saved.dns || '';
    }
  } catch (e) {}
}

// Calibration
async function calibrateEmpty() {
  if (!confirm('Le réservoir est VIDE ?\n\nCette action va recalculer la hauteur du réservoir.')) return;
  try {
    await fetch('/button/calibrate_empty/press', { method: 'POST' });
    toast('Calibration vide effectuée');
    setTimeout(fetchData, 500);
  } catch (e) {
    toast('Erreur', 'error');
  }
}

async function calibrateFull() {
  if (!confirm('Le réservoir est PLEIN ?\n\nCette action va recalculer l\'offset du capteur.')) return;
  try {
    await fetch('/button/calibrate_full/press', { method: 'POST' });
    toast('Calibration plein effectuée');
    setTimeout(fetchData, 500);
  } catch (e) {
    toast('Erreur', 'error');
  }
}

// System actions
async function restartDevice() {
  if (!confirm('Redémarrer le capteur ?')) return;
  try {
    await fetch('/button/restart_btn/press', { method: 'POST' });
    toast('Redémarrage...', 'info');
  } catch (e) {
    toast('Erreur', 'error');
  }
}

async function factoryReset() {
  if (!confirm('⚠️ ATTENTION ⚠️\n\nCette action va réinitialiser tous les paramètres.\n\nContinuer ?')) return;
  try {
    await fetch('/button/factory_reset_btn/press', { method: 'POST' });
    toast('Réinitialisation...', 'info');
  } catch (e) {
    toast('Erreur', 'error');
  }
}

// Init
loadNetworkConfig();
fetchData();
setInterval(fetchData, 10000);
initOtaUpload();

// OTA Upload Functions
let selectedFile = null;

function initOtaUpload() {
  const dropZone = $('otaDropZone');
  
  ['dragenter', 'dragover', 'dragleave', 'drop'].forEach(evt => {
    dropZone.addEventListener(evt, preventDefaults, false);
  });
  
  function preventDefaults(e) {
    e.preventDefault();
    e.stopPropagation();
  }
  
  ['dragenter', 'dragover'].forEach(evt => {
    dropZone.addEventListener(evt, () => dropZone.classList.add('dragover'), false);
  });
  
  ['dragleave', 'drop'].forEach(evt => {
    dropZone.addEventListener(evt, () => dropZone.classList.remove('dragover'), false);
  });
  
  dropZone.addEventListener('drop', handleDrop, false);
}

function handleDrop(e) {
  const files = e.dataTransfer.files;
  if (files.length > 0) {
    processFile(files[0]);
  }
}

function handleFileSelect(e) {
  const files = e.target.files;
  if (files.length > 0) {
    processFile(files[0]);
  }
}

function processFile(file) {
  if (!file.name.endsWith('.bin')) {
    toast('Fichier invalide. Sélectionnez un fichier .bin', 'error');
    return;
  }
  
  selectedFile = file;
  
  $('fileName').textContent = file.name;
  $('fileSize').textContent = formatFileSize(file.size);
  
  $('otaDropZone').style.display = 'none';
  $('otaFileInfo').style.display = 'flex';
  $('otaUploadBtn').style.display = 'block';
}

function clearFile() {
  selectedFile = null;
  $('otaFile').value = '';
  $('otaDropZone').style.display = 'block';
  $('otaFileInfo').style.display = 'none';
  $('otaUploadBtn').style.display = 'none';
  $('otaProgress').style.display = 'none';
}

function formatFileSize(bytes) {
  if (bytes < 1024) return bytes + ' B';
  if (bytes < 1024 * 1024) return (bytes / 1024).toFixed(1) + ' KB';
  return (bytes / (1024 * 1024)).toFixed(2) + ' MB';
}

async function startOtaUpload() {
  if (!selectedFile) {
    toast('Aucun fichier sélectionné', 'error');
    return;
  }
  
  if (!confirm('⚠️ Mise à jour du firmware\n\nLe capteur va redémarrer après la mise à jour.\n\nContinuer ?')) {
    return;
  }
  
  $('otaUploadBtn').style.display = 'none';
  $('otaProgress').style.display = 'block';
  $('progressFill').style.width = '0%';
  $('progressText').textContent = 'Envoi du firmware...';
  
  const formData = new FormData();
  formData.append('update', selectedFile);
  
  try {
    const xhr = new XMLHttpRequest();
    
    xhr.upload.addEventListener('progress', (e) => {
      if (e.lengthComputable) {
        const percent = Math.round((e.loaded / e.total) * 100);
        $('progressFill').style.width = percent + '%';
        $('progressText').textContent = `Envoi: ${percent}%`;
      }
    });
    
    xhr.addEventListener('load', () => {
      if (xhr.status === 200) {
        $('progressFill').style.width = '100%';
        $('progressText').textContent = '✓ Mise à jour réussie ! Redémarrage...';
        $('progressFill').style.background = 'var(--green)';
        toast('Firmware mis à jour ! Redémarrage en cours...', 'success');
        
        // Attendre le redémarrage et recharger
        setTimeout(() => {
          $('progressText').textContent = 'Reconnexion...';
          setTimeout(() => {
            location.reload();
          }, 10000);
        }, 3000);
      } else {
        throw new Error('Erreur HTTP: ' + xhr.status);
      }
    });
    
    xhr.addEventListener('error', () => {
      throw new Error('Erreur réseau');
    });
    
    xhr.open('POST', '/update');
    xhr.send(formData);
    
  } catch (e) {
    $('progressFill').style.width = '100%';
    $('progressFill').style.background = 'var(--red)';
    $('progressText').textContent = '✗ Erreur: ' + e.message;
    toast('Erreur lors de la mise à jour', 'error');
    
    setTimeout(() => {
      clearFile();
    }, 3000);
  }
}
</script>
</body>
</html>
)rawliteral";
