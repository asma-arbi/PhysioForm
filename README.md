# 🩺 PhysioForm — Application Desktop de Gestion & Rééducation Intelligente

**PhysioForm** est une application desktop moderne et robuste conçue pour la gestion complète et intelligente des cabinets de kinésithérapie, de physiothérapie et de rééducation fonctionnelle. 

Développée en **C++** avec le framework **Qt**, l'application combine la puissance d'un système de gestion de base de données d'entreprise (Oracle via QODBC) avec des technologies innovantes d'intelligence artificielle (vision par ordinateur en Python/OpenCV), de communication SMS (API Twilio) et de matériel embarqué (IoT avec Arduino/RFID).

---

## 🚀 Fonctionnalités Clés

### 👤 1. Gestion des Patients
* **Dossier Clinique Complet (CRUD) :** Enregistrement des données personnelles, antécédents médicaux, contacts d'urgence.
* **Recherche & Filtres Avancés :** Recherche rapide par ID ou nom et consultation instantanée de l'historique thérapeutique.
* **Historique des Soins :** Suivi chronologique complet de toutes les séances et interventions reçues par le patient.

### 💼 2. Gestion des Employés
* **Gestion du Personnel :** Suivi des profils administratifs et médicaux des kinésithérapeutes et secrétaires.
* **Sécurité & Rôles :** Attribution de droits d'accès sélectifs selon le secteur d'activité (Kinésithérapeute, Secrétaire, Comptable).
* **Double Authentification :** Connexion sécurisée classique par ID/Mot de passe ou via biométrie.

### 📅 3. Planification & Réservations
* **Agenda des Séances :** Planification, modification et annulation de rendez-vous pour les séances de rééducation.
* **Attribution des Praticiens :** Affectation automatique ou manuelle d'un patient à un kinésithérapeute disponible.
* **Prévention des conflits d'horaires :** Vérification de la disponibilité en temps réel pour éviter les doublons de réservation.

### 🩺 4. Suivi des Traitements
* **Fiche Thérapeutique :** Création de programmes de réhabilitation personnalisés avec objectifs cliniques.
* **Suivi de la Progression :** Évaluation chiffrée et textuelle de l'amélioration de la mobilité du patient au fil des séances.

### 💳 5. Gestion Financière & Statistiques
* **Enregistrement des Paiements :** Suivi en temps réel des règlements (espèces, chèques, virements) avec statuts (Payé, En attente).
* **Exportation de Rapports :**
  * Génération automatique de rapports de facturation en **PDF** (`.pdf`) avec mise en page soignée.
  * Exportation complète des tableaux financiers au format **Excel** (`.xlsx`) pour la comptabilité.
* **Statistiques & Visualisation :** Génération de graphiques statistiques interactifs (diagrammes circulaires via `Qt Charts`) pour analyser la répartition des revenus.
* **Notifications SMS Automatiques :** Envoi de messages de confirmation de paiement et rappels de rendez-vous sur le mobile du patient via l'API **Twilio**.

### 💬 6. Messagerie Interne (Chat)
* **Messagerie Instantanée :** Système de discussion intégré permettant au personnel (kinésithérapeutes et secrétariat) de collaborer et d'échanger des informations cliniques en temps réel en interne.

---

## 🧠 Fonctionnalités Intelligentes & Innovations

### 🔍 A. Connexion par Reconnaissance Faciale (IA)
* Intégration d'un script de vision par ordinateur en **Python/OpenCV** (`reconnaissance_faciale.py`).
* Utilisation de cascades de Haar pour détecter le visage via la webcam et de la corrélation de gabarits (*Template Matching*) avec des images de référence pour identifier l'employé et le connecter automatiquement sans saisie de mot de passe.

### 🚶‍♂️ B. Détection de Mouvement par Webcam
* Script de détection dynamique en temps réel (`motion_detection.py`) s'appuyant sur OpenCV.
* Analyse les variations de trame en temps réel (*Frame Differencing*) pour détecter et encadrer les mouvements du patient pendant ses séances d'exercice. Cet outil sert d'appui aux praticiens pour valider la réalisation correcte des mouvements physiques.

### 🏷️ C. Authentification Physique par RFID (Arduino & IoT)
* Intégration matérielle avec une carte **Arduino** via le port série (`QSerialPort`).
* Permet aux employés de s'authentifier instantanément et de déverrouiller leur espace de travail en scannant simplement leur badge RFID physique sur le lecteur du cabinet.

### 🌓 D. Thémage Dynamique (Dark/Light Mode)
* Changement de thème en temps réel à l'aide de fichiers de style CSS dédiés (`darkmode.css` et `lightmode.css`). L'ensemble de l'interface graphique s'adapte instantanément pour assurer le confort visuel du praticien de jour comme de nuit.

---

## 🛠️ Stack Technique

* **Langage Principal :** C++ (norme C++11 ou ultérieure)
* **Framework UI :** Qt 5 / Qt 6 (Qt Widgets, QML pour l'affichage de cartes `map.qml`, Qt Charts pour l'analyse financière)
* **Vision par Ordinateur :** Python 3.x, OpenCV (`cv2`)
* **Matériel Embarqué :** Arduino C/C++, Communication série standard
* **Base de données :** Connexion SQL via **QODBC** connecté à un serveur de base de données (Oracle/SQL Server)
* **Services Cloud / API :** API REST Twilio pour l'envoi de SMS (`QNetworkAccessManager`, `QNetworkRequest`)

---

## 📂 Structure du Projet

```bash
├── 📁 photos/                     # Ressources graphiques et icônes de l'interface
├── 🐍 reconnaissance_faciale.py   # Script de reconnaissance faciale en Python/OpenCV
├── 🐍 motion_detection.py         # Script d'analyse de mouvements en Python/OpenCV
├── 💻 main.cpp                    # Point d'entrée de l'application Qt
├── 💻 connexion.h / connexion.cpp # Initialisation et gestion du pilote QODBC pour la Base de Données
├── 🎨 lightmode.css / darkmode.css# Feuilles de style pour les thèmes clair et sombre
├── 🗺️ map.qml                     # Module QML pour l'intégration de cartes interactives
├── 📑 integration.pro             # Fichier de configuration du projet Qt (qmake)
│
├── 📝 MODULES D'INTERFACE (UI, En-têtes, Sources) :
│   ├── chat.ui / chat.h / chat.cpp               # Chat interne
│   ├── employe.ui / employe.h / employe.cpp      # Interface d'administration RH
│   ├── employeC.h / employeC.cpp                 # Logique métier & requêtes SQL Employés
│   ├── patient.ui / patient.h / patient.cpp      # Interface d'administration Patients
│   ├── patientC.h / patientc.cpp                 # Logique métier & requêtes SQL Patients
│   ├── paiement.ui / paiement.h / paiement.cpp   # Interface de facturation & SMS
│   ├── paiementC.h / paiementC.cpp               # Logique métier & exportations (PDF/Excel/Stats)
│   ├── reservation.ui / reservation.h / ...      # Gestion de l'agenda
│   └── traitement.ui / traitement.h / ...        # Suivi thérapeutique des patients
└── 📝 README.md                   # Documentation globale du projet (Ce fichier)
```

---

## ⚙️ Installation & Configuration

### Prérequis
1. **Qt Creator** avec la suite de compilation MinGW ou MSVC (compatible Qt 5 ou Qt 6).
2. **Python 3.x** installé et ajouté au PATH de votre système, avec la bibliothèque OpenCV :
   ```bash
   pip install opencv-python numpy
   ```
3. **Pilote ODBC** configuré sur votre système Windows avec une source de données système (DSN) nommée `projet`.
4. **Arduino IDE** (optionnel) si vous utilisez le lecteur de badge RFID.

### Lancement du Projet
1. Ouvrez le fichier `integration.pro` dans **Qt Creator**.
2. Configurez les kits de compilation.
3. Modifiez si nécessaire les informations de connexion de base de données dans [connexion.cpp](file:///c:/Users/asmaa/Downloads/physioform-app-desktop-en-c-et-Qt-designer-master/connexion.cpp) (Nom d'utilisateur et mot de passe de la base de données).
4. Configurez vos identifiants Twilio dans [paiement.cpp](file:///c:/Users/asmaa/Downloads/physioform-app-desktop-en-c-et-Qt-designer-master/paiement.cpp#L303) pour activer l'envoi de SMS.
5. Compilez et lancez l'application en cliquant sur le bouton **Exécuter** (icône verte de lecture).

---

## 👥 Membres de l'équipe Projet

Ce projet a été réalisé en collaboration par une équipe d'ingénieurs passionnés :
* 💳 **Ali** : Spécialiste module Financier (Gestion des Paiements, Facturation Excel/PDF, Statistiques de revenus et Module SMS Twilio).
* 💼 **Malek** : Concepteur système RH & IA (Gestion des Employés, Système d'Authentification par Reconnaissance Faciale via Python/OpenCV).
* 👤 **Asma** : Conceptrice Clinique (Gestion complète du fichier Patient, Historique médical et relations d'administration clinique).
* 🩺 **Feriel** & **Abdoo** & **Halima** : Équipe de développement et d'intégration (Modules de Suivi des Traitements, Réservations, Chat interne, Thémage CSS Clair/Sombre, et intégration matérielle Arduino/RFID).
