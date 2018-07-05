#include <EEPROM.h>

void readCredentials() { //connection string
    int ssidAddr = 0;
    int passAddr = ssidAddr + SSID_LEN;
    int connectionStringAddr = passAddr + SSID_LEN;
  
    ssidString = (char *)malloc(SSID_LEN);
    passString = (char *)malloc(PASS_LEN);
    connectionString = (char *)malloc(CONNECTION_STRING_LEN);
  
    int ssidLength = EEPROMread(ssidAddr, ssidString);
    int passLength = EEPROMread(passAddr, passString);
    int connectionStringLength = EEPROMread(connectionStringAddr, connectionString);
  
    //strcpy(ssidString, D_SSID);
    strcpy(ssidString, M_SSID);
    EEPROMWrite(ssidAddr, ssidString, strlen(ssidString));
  
    //strcpy(passString, D_PASSWORD);
    strcpy(passString, M_PASSWORD);
    EEPROMWrite(passAddr, passString, strlen(passString));
    
    strcpy(connectionString, DEVICE_CONNECTION_STRING);
    EEPROMWrite(connectionStringAddr, connectionString, strlen(connectionString));
}

#define EEPROM_END 0
#define EEPROM_START 1

void EEPROMWrite(int addr, char *data, int size) { //Write EEPROM
    EEPROM.begin(EEPROM_SIZE);
    
    // write the start marker
    EEPROM.write(addr, EEPROM_START);
    addr++;
    
    for (int i = 0; i < size; i++) {
        EEPROM.write(addr, data[i]);
        addr++;
    }
    
    EEPROM.write(addr, EEPROM_END);
    EEPROM.commit();
    EEPROM.end();
}

int EEPROMread(int addr, char *buf) { //Read EEPROM
    EEPROM.begin(EEPROM_SIZE);
    int count = -1;
    char c = EEPROM.read(addr);
    addr++;
    
    if (c != EEPROM_START) {
        return 0;
    }
    
    while (c != EEPROM_END && count < EEPROM_SIZE) {
        c = (char)EEPROM.read(addr);
        count++;
        addr++;
        buf[count] = c;
    }
    
    EEPROM.end();
    
    return count;
}


