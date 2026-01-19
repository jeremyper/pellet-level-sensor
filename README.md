# 🔥 Pellet Monitor Pro v3.0

Capteur de niveau de granulés pour poêle Moretti Design avec interface web moderne et mise à jour OTA.

![ESP8266](https://img.shields.io/badge/ESP8266-Compatible-blue)
![ESPHome](https://img.shields.io/badge/ESPHome-2024.x-green)
![License](https://img.shields.io/badge/License-MIT-yellow)

---

## 📦 Matériel Requis

| Composant | Description | Prix approximatif |
|-----------|-------------|-------------------|
| ESP8266 ESP-12F | Ou NodeMCU / Wemos D1 Mini | ~3€ |
| GY-VL53L0X | Capteur laser ToF I2C | ~4€ |
| Câbles Dupont | 4x Femelle-Femelle | ~1€ |
| Alimentation | 5V USB | ~2€ |

**Coût total : ~10€**

---

## 🔌 Câblage

```
GY-VL53L0X          ESP8266
──────────          ───────
   VCC    ────────►  3.3V
   GND    ────────►  GND
   SDA    ────────►  GPIO4 (D2)
   SCL    ────────►  GPIO5 (D1)
```

### Schéma

```
        ┌─────────────────┐
        │   GY-VL53L0X    │
        │  ┌───────────┐  │
        │  │  CAPTEUR  │  │
        │  └───────────┘  │
        │ VCC GND SDA SCL │
        └──┬───┬───┬───┬──┘
           │   │   │   │
           │   │   │   └──► GPIO5 (D1) SCL
           │   │   └──────► GPIO4 (D2) SDA
           │   └──────────► GND
           └──────────────► 3.3V
                    │
              ┌─────┴─────┐
              │  ESP8266  │
              │  ESP-12F  │
              └───────────┘
```

---

## 🖥️ Installation du Serveur ESPHome (LXC Proxmox)

### 1. Créer le container LXC

Dans Proxmox :
- **Create CT**
- Template : `debian-12` ou `ubuntu-24.04`
- Disk : 8 GB
- CPU : 2 cores
- RAM : 2048 MB
- Network : DHCP ou IP fixe

### 2. Installer ESPHome

```bash
# Connexion au container
ssh root@IP_CONTAINER

# Mise à jour système
apt update && apt upgrade -y

# Installer les dépendances
apt install -y python3 python3-pip python3-venv git

# Créer l'environnement virtuel
python3 -m venv /opt/esphome

# Activer l'environnement
source /opt/esphome/bin/activate

# Installer ESPHome
pip install esphome

# Créer le dossier projets
mkdir -p /root/esphome-projects
cd /root/esphome-projects
```

### 3. Créer le service systemd (Dashboard)

```bash
cat > /etc/systemd/system/esphome.service << 'EOF'
[Unit]
Description=ESPHome Dashboard
After=network.target

[Service]
Type=simple
ExecStart=/opt/esphome/bin/esphome dashboard /root/esphome-projects --port 6052 --address 0.0.0.0
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
EOF

# Activer et démarrer le service
systemctl daemon-reload
systemctl enable esphome
systemctl start esphome

# Vérifier le statut
systemctl status esphome
```

**Dashboard accessible sur : `http://IP_CONTAINER:6052`**

---

## 📥 Installation du Capteur

### 1. Créer les fichiers de configuration

```bash
cd /root/esphome-projects

# Créer le fichier YAML principal
nano pellet-level-sensor.yaml
```

Colle le contenu du fichier `pellet-level-sensor.yaml` fourni.

**⚠️ N'oublie pas de modifier le WiFi :**
```yaml
wifi:
  ssid: "TON_SSID_WIFI"
  password: "TON_MOT_DE_PASSE"
```

```bash
# Créer le fichier HTML
nano pellet_page.h
```

Colle le contenu du fichier `pellet_page.h` fourni.

### 2. Vérifier les fichiers

```bash
ls -la /root/esphome-projects/
# Doit afficher :
# pellet-level-sensor.yaml
# pellet_page.h
```

### 3. Compiler le firmware

```bash
cd /root/esphome-projects
source /opt/esphome/bin/activate

# Compiler (première fois = ~3-5 min)
esphome compile pellet-level-sensor.yaml
```

### 4. Télécharger le fichier .bin

```bash
# Aller dans le dossier du firmware compilé
cd /root/esphome-projects/.esphome/build/pellet-level-sensor/.pioenvs/pellet-level-sensor/

# Vérifier que le fichier existe
ls -lh firmware.bin

# Lancer un serveur web temporaire pour télécharger
python3 -m http.server 8080
```

Sur ton PC, ouvre : `http://IP_CONTAINER:8080/firmware.bin`

Télécharge le fichier, puis **Ctrl+C** pour arrêter le serveur.

### 5. Flasher l'ESP (premier flash USB)

1. Va sur **https://web.esphome.io** (Chrome uniquement)
2. Connecte l'ESP8266 en USB à ton PC
3. Clique **CONNECT** → Sélectionne le port COM
4. Clique **INSTALL**
5. Sélectionne le fichier `firmware.bin` téléchargé
6. Attends la fin du flash (~2 min)

### 6. Accéder à l'interface

Une fois flashé, l'ESP se connecte à ton WiFi.

**Accès :** 
- `http://pellet-level-sensor.local`
- Ou `http://IP_DE_L_ESP`

---

## 🔄 Mise à jour OTA (Over-The-Air)

Après le premier flash, tu peux mettre à jour sans fil !

### Option 1 : Via l'interface web du capteur

1. **Compiler le nouveau firmware :**
```bash
cd /root/esphome-projects
source /opt/esphome/bin/activate
esphome compile pellet-level-sensor.yaml
```

2. **Récupérer le .bin :**
```bash
cd /root/esphome-projects/.esphome/build/pellet-level-sensor/.pioenvs/pellet-level-sensor/
python3 -m http.server 8080
# Télécharge firmware.bin depuis http://IP_CONTAINER:8080/firmware.bin
# Ctrl+C pour arrêter
```

3. **Uploader via l'interface :**
   - Va sur `http://IP_ESP` → Page **Système**
   - Glisse le fichier `.bin` dans la zone d'upload
   - Clique **🚀 Lancer la mise à jour**
   - Attends le redémarrage (~30 sec)

### Option 2 : Via ESPHome directement

```bash
cd /root/esphome-projects
source /opt/esphome/bin/activate

# Flash OTA (sans USB)
esphome run pellet-level-sensor.yaml
# Choisis "Over The Air" quand demandé
```

---

## 🌐 Interface Web

### Pages disponibles

| Page | Description |
|------|-------------|
| 🏠 **Accueil** | Jauge de niveau, stats, alertes |
| ⚙️ **Capteur** | Calibration, dimensions, seuils |
| 📡 **Réseau** | Config WiFi, IP statique |
| 🔧 **Système** | Infos, maintenance, OTA |

### Fonctionnalités

- ✅ Design glassmorphism moderne
- ✅ Jauge circulaire animée
- ✅ Visualisation 3D du réservoir
- ✅ Calibration automatique (vide/plein)
- ✅ Estimation autonomie en jours
- ✅ Alertes niveau critique
- ✅ Configuration IP statique
- ✅ Mise à jour OTA intégrée
- ✅ Reset usine
- ✅ Responsive mobile

---

## ⚙️ Configuration

### Paramètres ajustables depuis l'interface

| Paramètre | Description | Défaut |
|-----------|-------------|--------|
| Hauteur réservoir | Distance fond → capteur | 400 mm |
| Offset capteur | Distance capteur → granulés (plein) | 30 mm |
| Capacité | Poids max de granulés | 15 kg |
| Consommation | Estimation journalière | 1.5 kg/j |
| Seuil alerte | Niveau bas | 50 mm |

### Calibration

1. **Réservoir VIDE** : Clique sur "📭 Vide" → calcule la hauteur
2. **Réservoir PLEIN** : Clique sur "📬 Plein" → calcule l'offset

### IP Statique

Pour configurer une IP fixe, modifie le fichier YAML :

```yaml
wifi:
  ssid: "TON_WIFI"
  password: "TON_MDP"
  manual_ip:
    static_ip: 192.168.1.100
    gateway: 192.168.1.1
    subnet: 255.255.255.0
    dns1: 8.8.8.8
```

Puis recompile et reflash.

---

## 📡 Mode Point d'Accès (Secours)

Si l'ESP ne trouve pas le WiFi, il crée son propre réseau :

| Paramètre | Valeur |
|-----------|--------|
| SSID | `pellet-level-sensor-AP` |
| Mot de passe | `pellet1234` |
| IP | `192.168.4.1` |

---

## 🛠️ Commandes Utiles

### Gestion du service ESPHome

```bash
# Statut
systemctl status esphome

# Redémarrer
systemctl restart esphome

# Arrêter
systemctl stop esphome

# Voir les logs
journalctl -u esphome -f
```

### Compilation et flash

```bash
# Activer l'environnement
source /opt/esphome/bin/activate

# Valider la configuration
esphome config pellet-level-sensor.yaml

# Compiler seulement
esphome compile pellet-level-sensor.yaml

# Compiler et flasher (USB ou OTA)
esphome run pellet-level-sensor.yaml

# Voir les logs en direct
esphome logs pellet-level-sensor.yaml
```

### Serveur web temporaire (téléchargement .bin)

```bash
cd /root/esphome-projects/.esphome/build/pellet-level-sensor/.pioenvs/pellet-level-sensor/
python3 -m http.server 8080
# Accès : http://IP_CONTAINER:8080/firmware.bin
# Ctrl+C pour arrêter
```

---

## 🔧 Dépannage

### Le capteur VL53L0X n'est pas détecté

```bash
# Vérifier les logs
esphome logs pellet-level-sensor.yaml
```

- Vérifie les connexions SDA (GPIO4) et SCL (GPIO5)
- Vérifie l'alimentation **3.3V** (pas 5V)
- L'adresse I2C par défaut est `0x29`

### Impossible de se connecter au WiFi

1. Vérifie SSID et mot de passe dans le YAML
2. Connecte-toi au point d'accès de secours :
   - WiFi : `pellet-level-sensor-AP`
   - Mot de passe : `pellet1234`
   - URL : `http://192.168.4.1`

### La page web ne s'affiche pas

1. Vérifie que l'ESP est connecté (LED bleue)
2. Trouve l'IP dans ta box/routeur
3. Essaie `http://IP_ESP` au lieu de `.local`

### Erreur de compilation

```bash
# Nettoyer et recompiler
esphome clean pellet-level-sensor.yaml
esphome compile pellet-level-sensor.yaml
```

### Reset complet

Depuis l'interface : **Système** → **Réinitialisation usine**

Ou via le bouton flash de l'ESP (maintenir 10 sec au démarrage).

---

## 🏠 Intégration Home Assistant (optionnel)

L'ESP est automatiquement découvert par Home Assistant.

1. **Paramètres** → **Appareils et services**
2. L'appareil devrait apparaître automatiquement
3. Sinon : **Ajouter** → **ESPHome** → `pellet-level-sensor.local`

### Entités disponibles

- `sensor.niveau_granules` - Niveau en %
- `sensor.niveau_mm` - Niveau en mm
- `sensor.stock` - Stock en kg
- `sensor.autonomie` - Jours restants
- `binary_sensor.alerte_niveau_bas` - Alerte
- `button.redemarrer` - Redémarrage

---

## 📊 Spécifications Techniques

| Paramètre | Valeur |
|-----------|--------|
| Portée capteur | 30 - 2000 mm |
| Précision | ±3% |
| Consommation | ~80mA @ 5V |
| Fréquence mesure | 10 secondes |
| Interface I2C | 400 kHz |
| Flash ESP | 4 MB |

---

## 📄 Licence

MIT - Libre d'utilisation et modification.

---

## 🙏 Crédits

- [ESPHome](https://esphome.io/)
- [VL53L0X Library](https://github.com/pololu/vl53l0x-arduino)
- Interface développée avec ❤️

---

**Bon chauffage ! 🔥**
