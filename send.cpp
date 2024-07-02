#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0xD4, 0xD4, 0xDA, 0x5E, 0x07, 0xA0};

typedef struct struct_message {
  char data[200]; 
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

// Função callback para status do envio de dados
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);

  // Configura o dispositivo como uma estação Wi-Fi
  WiFi.mode(WIFI_STA);

  // Inicializa o ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Registra a função de callback para o status de envio
  esp_now_register_send_cb(OnDataSent);

  // Registra informações do peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Adiciona o peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Define os valores a serem enviados
  String mensagem = "{\"TPS\":" + String(random(0, 100)) + ",\"MAP\":" +
                    String(random(0, 100)) + ",\"AirTemp\":" +
                    String(random(0, 100)) + ",\"EngineTemp\":" +
                    String(random(0, 100)) + ",\"ExhaustO2\":" +
                    String(random(0, 2)) + ",\"RPM\":" +
                    String(random(0, 14000)) + ",\"OilTemp\":" +
                    String(random(0, 100)) + ",\"PitLimit\":" +
                    String(random(0, 100)) + ",\"OilPressure\":" +
                    String(random(0, 100)) + ",\"FuelPressure\":" +
                    String(random(0, 100)) + ",\"WaterPressure\":" +
                    String(random(0, 100)) + ",\"Gear\":" +
                    String(random(0, 100)) + "} \n";

  // Copia o conteúdo da string para o campo da struct
  mensagem.toCharArray(myData.data, sizeof(myData.data));

  // Envia a mensagem via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}
