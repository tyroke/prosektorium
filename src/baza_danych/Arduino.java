package baza_danych;

import java.io.PrintWriter;
import com.fazecast.jSerialComm.SerialPort;
import java.io.IOException;
import java.io.InputStream;

public class Arduino {

    static SerialPort chosenPort;

    void send(String message) throws InterruptedException {

        if (Arduino.chosenPort.openPort()) {
            PrintWriter output = new PrintWriter(chosenPort.getOutputStream());

            output.print(message);
            Thread.sleep(2000);
            output.flush();

        } else {
            // disconnect from the serial port
            chosenPort.closePort();

        }
    }

    char read() throws InterruptedException, IOException {

        char message;
        if (Arduino.chosenPort.openPort()) {
            InputStream input = chosenPort.getInputStream();
            message = (char) input.read();
            //input.close();
            Thread.sleep(100);

        } else {
            // disconnect from the serial port
            chosenPort.closePort();
            message = 'x';

        }
        return message;
    }

    Arduino() {
        Arduino.chosenPort = SerialPort.getCommPort("COM1");
        //Arduino.chosenPort.setComPortTimeouts(SerialPort.TIMEOUT_SCANNER, 0, 0);

    }

}
