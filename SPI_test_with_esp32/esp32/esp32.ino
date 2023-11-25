// ====================================================================
// Arduino code example for SPI Slave Mode
// Read unsigned short (two bytes) from SPI, send word to serial port
// On 16 MHz Arduino, can work at > 500 words per second
// J.Beale July 19 2011
// ====================================================================
#include <SPI.h>
#include <ESP32SPISlave.h>
#include <pins_arduino.h>

ESP32SPISlave slave;

static constexpr uint32_t BUFFER_SIZE {32};
uint8_t spi_slave_tx_buf[BUFFER_SIZE];
uint8_t spi_slave_rx_buf[BUFFER_SIZE];


void setup() {
    Serial.begin(115200);
    // note: the default pins are different depending on the board
    // please refer to README Section "SPI Buses and SPI PinsV" for more details
    slave.setDataMode(SPI_MODE0);
    slave.begin(VSPI);
}

void loop() {
    spi_slave_tx_buf[0] = 33;
    // block until the transaction comes from master
    slave.wait(spi_slave_rx_buf, spi_slave_tx_buf, BUFFER_SIZE);

    // if transaction has completed from master,
    // available() returns the number of completed transactions,
    // and `spi_slave_rx_buf` is automatically updated
    while (slave.available()) {
        // do something with `spi_slave_rx_buf`
        
        Serial.print("Data Received: ");
        Serial.println((char*)spi_slave_rx_buf);
        Serial.println();

        slave.pop();
        
    }
}

