#ifndef _DISPENSE_TICKET_H_
//
// ticket dispensing function
//
  void dispense_ticket(int opto_signal_pin, int dispenser_motor, int tickets_dispensed,
                       int tickets_requested, int opto_signal_status, int current_status,
                       int previous_status, int last_opto_signal, int key) {
    setTime(0);
    previous_status = digitalRead(opto_signal_pin);
    motorOn(dispenser_motor);
    while (tickets_dispensed < tickets_requested) {

      if (tickets_requested == 1) {
        delay(490);   // varies based on length of each ticket - longer ticket = higher delay
        cancelDispensing(dispenser_motor);
      }

      // 1 = HIGH = no ticket gap detected
      // 0 = LOW = ticket gap detected
      opto_signal_status = digitalRead(opto_signal_pin);

      // state machine conditions
      if (opto_signal_status == 1) {
        current_status = 1;
        previous_status = 1;
      }

      if (opto_signal_status == 0) {
        current_status = 0;
      }

      // count tickets dispensed
      for (;current_status == 0 && previous_status == 1;) {

        if (tickets_dispensed == (tickets_requested - 1)) {
          delay(100);   // varies based on length of each ticket - longer ticket = higher delay
          printMatrix(tickets_requested);
          cancelDispensing(dispenser_motor);
        }

        if (digitalRead(opto_signal_pin) == 1) {
          countTicket(tickets_dispensed, last_opto_signal, previous_status);
        }

        // reset if *
        //key = keypad.getKey();
        if (key == '*') {
          cancelDispensing(dispenser_motor);
        }
      }
//
// end of roll catch
//
      int current_time = now();
      int no_ticket_reset = current_time - last_opto_signal;
      if (no_ticket_reset >= 3) {
        cancelDispensing(dispenser_motor);
      }

      //key = keypad.getKey();
      if (key == '*') {
        cancelDispensing(dispenser_motor);
      }
    }
  }
#endif
