#define TRIGGER D3
#define ECHO D4  // Pin received echo pulse

class HCSR04 {
   public:
    void start();
    void setDelay(unsigned long);
    void SET_TEMPERATURE(float temperature);
    void HANDLE_MEASURE_DELAYS();
    void MEASURE_DISTANCE();
    float getDistance();

   private:
    unsigned long preMills, curMills, delaytime, maxTimeoutMicroSec = 0;
    unsigned short maxDistanceCm = 200;
    float air_temp = 19.307;  //Using the approximate formula 19.307°C results in roughly 343m/s which is the commonly used value for air.
    float distance = 0.0;
    bool use_function = false;
    bool isenabled = true;
    int trigger_state = LOW;
};

void HCSR04::start() {
    preMills = millis();
    digitalWrite(TRIGGER, LOW);
    Serial.println("HCSR04: STARTED");
}

float HCSR04::getDistance() {
    return distance;
}

void HCSR04::SET_TEMPERATURE(float temperature) {
    air_temp = temperature;
}

void HCSR04::MEASURE_DISTANCE() {
    curMills = micros();
    if (trigger_state == LOW) {
        if (curMills - preMills >= 2) {
            preMills = curMills;
            trigger_state = HIGH;
            digitalWrite(TRIGGER, trigger_state);
        }
    } else if (trigger_state == HIGH) {
        if (curMills - preMills >= 10) {
            preMills = curMills;
            trigger_state = LOW;
            digitalWrite(TRIGGER, trigger_state);

            unsigned long maxDistanceDurationMicroSec;

            float speedOfSoundInCmPerMicroSec = 0.03313 + 0.0000606 * air_temp;  // Cair ≈ (331.3 + 0.606 ⋅ ϑ) m/s
            /* Compute max delay based on max distance with 25% margin in microseconds */
            maxDistanceDurationMicroSec = 2.5 * maxDistanceCm / speedOfSoundInCmPerMicroSec;
            if (maxTimeoutMicroSec > 0) {
                maxDistanceDurationMicroSec = min(maxDistanceDurationMicroSec, maxTimeoutMicroSec);
            }

            // Measure the length of echo signal, which is equal to the time needed for sound to go there and back.
            unsigned long durationMicroSec = pulseIn(ECHO, HIGH, maxDistanceDurationMicroSec);  // can't measure beyond max distance

            float distanceCm = durationMicroSec / 2.0 * speedOfSoundInCmPerMicroSec;
            if (distanceCm == 0 || distanceCm > maxDistanceCm) {
                distance = -1.0;
            } else {
                distance = distanceCm;
            }
        }
    }
}
