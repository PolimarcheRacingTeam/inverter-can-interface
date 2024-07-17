# Implementazione di un CAN bus per ricezione ed invio dati a due inverter (e motori) per una vettura di Formula SAE

## Introduzione

Il progetto consiste nello sviluppare la comunicazione tra un microcontrollore e un inverter tramite protocollo CAN nell'ambito delle gare automobilistiche, in particolare per una vettura di Formula SAE.

## Installazione e Configurazione

I passi per la configurazione necessari sono:

### 1. collegamento degli inverter

Il primo step è quello di effettuare 
- il collegamento degli inverter ai motori (cavo encoder) e 
- il collegamento in EtherCAT ai vari inverter. Viene collegato il PC al primo degli inverter tramite cavo Ethernet. Ogni inverter è collegato direttamente tramite un cavo Ethernet al dispositivo successivo. Questi cavi Ethernet trasportano i dati in tempo reale tra i dispositivi EtherCAT.
- un'altro collegamento necessario al funzionamento del tutto è quello di alimentazione, nel nostro caso è necessario anche solo l'alimentazione 24V che permette il funzionamento della componente logica.

### 2. configurazione degli inverter

E' possibile tramite AMK Aipex Pro configurare tutti i parametri relativi agli inverter.
E' possibile utilizzare 2 configurazioni CAN differenti:
- **Free configuration** in cui si ha il massimo livello di personalizzazione dei pacchetti CAN (non utilizzata in questo progetto)
- **Fixed configuration** in cui si utilizzano dei pacchetti CAN già definiti (2 in ricezione e 1 in invio) nel seguente modo e che vedremo in dettagli più avanti:

![can_packet](/img/can_packet.png)

In particolare, ogni inverter è identificato da un NODE_ADDRESS che poi andrà a costituire parte dei vari CAN Identifier ovvero gli ID dei pacchetti. E' importante settare i NODE_ADDRESS come specificato da AMK (o in altro modo ma evitando conflitti tra ID):

![can_ids](/img/can_ids.png)

all'interno del parametro User list 3 (ID34091):

![Node_address](/img/node_setting.jpeg)

L'output rate utilizzato è 50 ms il che significa che ogni inverter, ogni 50 ms costruisce un pacchetto e lo invia sul CAN bus con gli ID specificati da Actual Values 1 e 2. Il transmission rate è impostato al default (500 kBit/s)

Gli altri parametri dell'inverter che non riguardano il CAN possono essere impostati in base alle specifiche del proprio progetto.

### 3. collegamento del can bus alla scheda

Nel caso in cui l'inverter è pronto (status LED green) si può procedere al collegamento della scheda all'inverter tramite (abbiamo utilizzato dei jumper almeno per la fase di testing).

![can_linking](/img/can_link.jpeg)

In foto:
- il nero è il GND collegato al verde
- giallo collegato al bianco è CAN_HIGH
- giallo collegato al marrone è CAN_LOW

![can_linking](/img/can_link2.png)

Inoltre la scheda deve essere alimentata, nel nostro caso l'alimentazione viene data dal pc (cavo usb-c) da cui viene effettuato l'upload del codice.

### 4. Installazione di PlatformIO IDE e upload del codice

Una volte scritto lo script PlatformIO IDE fornisce funzionalità di build e di upload sulla scheda. Se, come nel nostro caso per scopi di testing ci si collega al seriale si possono anche visualizzare i dati inviati e ricevuti (i pacchetti CAN).
