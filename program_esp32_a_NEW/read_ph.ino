void baca_ph(){
  static unsigned long timepoint = millis();
  if (millis() - timepoint > 1000U) {
    timepoint = millis();
    voltage = ads.readADC_SingleEnded(0) / 10;
    //Serial.print("voltage:");
    //Serial.println(voltage, 4);
    //    temperature = readTemperature();
    //Serial.print("temperature:");
    //Serial.print(temperature, 1);
    //Serial.println("^C");
    phValue = ph.readPH(voltage, temperature);
    phValue = phValue + calibration_ph;
    //Serial.print("pH:");
    //Serial.print(phValue, 4);
  }
  ph.calibration(voltage, temperature);
}
