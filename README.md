# IoT Smoke Detection System (ESP32)

Questo progetto consiste in un sistema di monitoraggio ambientale intelligente basato su microcontrollore **ESP32**, progettato per il rilevamento tempestivo di fumo e gas nocivi. Il sistema integra hardware IoT, protocolli di comunicazione standard e servizi cloud per garantire la sicurezza domestica o industriale.

## Caratteristiche principali

*   **Rilevamento Real-time:** Monitoraggio costante dei livelli di gas tramite sensore analogico MQ-2.
*   **Allerta Locale:** Attivazione immediata di una sirena (buzzer) al superamento delle soglie di sicurezza.
*   **Connettività MQTT:** Pubblicazione dei dati su un broker MQTT per una consultazione rapida e leggera tramite topic dedicati.
*   **Integrazione Cloud:** Salvataggio storico dei valori su **Firebase Realtime Database**.
*   **Notifiche Smart:** Invio automatico di e-mail di emergenza in caso di rilevamento anomalie.

## Stack Tecnologico

*   **Hardware:** ESP32, Sensore MQ-2, Buzzer attivo.
*   **Linguaggio:** C++ (Arduino Framework).
*   **Protocolli:** MQTT, HTTP.
*   **Servizi:** Firebase (Database), Server SMTP (Email).

## Architettura del Sistema

Il dispositivo si connette alla rete Wi-Fi locale e inizia il campionamento dei dati. I valori vengono processati internamente per verificare il superamento della soglia critica. Parallelamente alla gestione dell'allarme hardware, l'ESP32 agisce come:
1.  **MQTT Publisher**: invia i dati al broker per il monitoraggio live.
2.  **Firebase Client**: sincronizza i dati per l'analisi storica.

## Struttura del Repository

*   `src/`: Contiene il codice sorgente (.ino / .cpp).
*   `docs/`: Documentazione tecnica e schemi del circuito.

---
*Progetto realizzato per scopi didattici e prototipazione IoT.*
