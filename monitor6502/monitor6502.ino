const char ADDR[] = {22, 24, 26, 28, 30, 32, 34, 36,
                     38, 40, 42, 44, 52, 50, 48, 46};
const char DATA[] = {39, 41, 43, 45, 47, 49, 51, 53};

const char CLOCK = 2;
const char READ_WRITE = 3;

void onClock() {
    char buffer[64] = {};

    unsigned int addr = 0;
    for (int i = 0; i < 16; i++) {
        int bit = digitalRead(ADDR[i]) ? 1 : 0;
        buffer[i] = bit ? '1' : '0';
        addr |= bit << i;
    }

    buffer[16] = ' ';

    unsigned int data = 0;
    for (int i = 0; i < 8; i++) {
        int bit = digitalRead(DATA[i]) ? 1 : 0;
        buffer[i + 17] = bit ? '1' : '0';
        data |= bit << i;
    }

    buffer[25] = ' ';

    char rw = digitalRead(READ_WRITE) ? 'R' : 'W';
    sprintf(buffer + 25, "| 0x%04x -(%c)> 0x%02x | ", addr, rw, data);

    Serial.println(buffer);
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    for (int i = 0; i < 16; i++) {
        pinMode(ADDR[i], INPUT);
    }
    for (int i = 0; i < 8; i++) {
        pinMode(DATA[i], INPUT);
    }

    pinMode(CLOCK, INPUT);
    pinMode(READ_WRITE, INPUT);
    attachInterrupt(digitalPinToInterrupt(CLOCK), onClock, RISING);
    Serial.begin(57600);
}

void loop() {}