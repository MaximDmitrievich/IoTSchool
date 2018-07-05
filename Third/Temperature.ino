float getTemperature() {
    float temp;
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
    return temp;
}
