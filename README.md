# park_sensor_IOT
 A parking sensor built using NodeMcu and HC-SR04 Ultrasonic Distance Sensor 

The project calculates the distance by measuring the echo duration from the HC-SR04 sensor. Based on the calculated distance, the following actions are triggered:

Distance < 10 cm:

The red LED lights up.
The buzzer sounds.
Distance 10-30 cm:

The yellow LED lights up.
The buzzer remains off.
Distance > 30 cm:

The green LED lights up.
The buzzer remains off.
The code also displays the distance on the Serial Monitor once every second.



![serial_port](https://github.com/user-attachments/assets/cd8bbf69-8581-41d5-8c85-40f9becc2026)


Also you can see the distance in website.


![website](https://github.com/user-attachments/assets/b03a895c-c497-47ad-9312-5f7f452b710a)



Components Used

* ESP8266 or any compatible microcontroller with WIFI modüle
* HC-SR04 Ultrasonic Sensor
* LEDs (Red, Yellow, Green)
* Buzzer
* Resistors
* Jumper wires
* Breadboard


![Shematic](https://github.com/user-attachments/assets/2df1b26d-a5e2-4a4e-82fa-2bc5793224fa)


How to Use

1) Set up the hardware according to the circuit diagram above.
2) Open the code file in Arduino IDE and select the correct board (e.g., NodeMCU 1.0) and port.
3) Upload the code. Once done, observe the distance readings on the Serial Monitor and test the LEDs/buzzer based on different distances.
Contribution

If you'd like to improve this project or add new features, feel free to submit a pull request or open an issue to share your suggestions.

License

This project is licensed under the MIT License. See the LICENSE file for more details.








