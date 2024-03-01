#include <Wire.h>

int selected_reg;
char registers[10];

#define MAX_REG sizeof(registers)

#define CMD_SELECT   (char)0x01
#define CMD_WRITE    (char)0x02
#define CMD_READ     (char)0x03

void setup() {
  Wire.begin(0x08);
  Wire.onReceive(writeEvent);
  Wire.onRequest(requestEvent);

  Serial.begin(9600);

  memset(registers, 0, sizeof(registers));

  selected_reg = -1;
}

void loop() {
  delay(1000);

  for (int i = 0; i < MAX_REG; i++)
    if (registers[i] <= 254)
      registers[i]++;
    else
      registers[i] = 0;
}

void requestEvent() {
  char msg[20];

  if (selected_reg != -1 && selected_reg < MAX_REG)
    Wire.write(registers[selected_reg]);
  else
    Wire.write(255);

  selected_reg = -1;
}

void writeEvent(int howMany) {
  char c, value;
  char msg[20];

  c = Wire.read();

  switch (c) {
    case CMD_SELECT:
      selected_reg = Wire.read();

      sprintf(msg, "CMD_SELECT(%d)", selected_reg);
      Serial.println(msg);

      break;

    case CMD_WRITE:
      value = Wire.read();

      sprintf(msg, "CMD_WRITE(%d, %d)", selected_reg, value);
      Serial.println(msg);

      if (selected_reg > 0 && selected_reg < MAX_REG)
        registers[selected_reg] = value;

      selected_reg = -1;
      break;

    case CMD_READ:
      sprintf(msg, "CMD_READ(%d) = %d", selected_reg, registers[selected_reg]);
      Serial.println(msg);

      Wire.write(registers[selected_reg]);
      break;
  }
}
