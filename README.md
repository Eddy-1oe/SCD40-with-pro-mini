# SCD40 and HM10 Bluetooth module with-pro-mini 
#Automated system with SCD40, LDR, BUZZER and LEDS


#This code takes readings from the environment (co2 in ppm, Temperature in degrees Celsius,
#and Humidity in percent) which controls three leds and a buzzer depending on the threshold
#value set in the code. Ldr also determines whether a fourth led should be on or of depending 
#on the light intensity and all these data are transmitted to another controller via Bluetooth.
# The file label HELMET is used to interface the project using labview.
# note: used the arduino code labelled SLAVE if you only want to interface with the arduino ide and slaveupd when you want to use Labview on the receiver side.
