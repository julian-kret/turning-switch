#define LEFT_BUTTON_PIN 2                 // Номер піну кнопки ЛІВОГО повороту
#define RIGHT_BUTTON_PIN 3                // Номер піну кнопки ПРАВОГО повороту

#define LEFT_LAMP_PIN 10                  // Номер піну кнопки ЛІВОЇ лампи повороту
#define RIGHT_LAMP_PIN 11                 // Номер піну кнопки ПРАВОЇ лампи повороту

#define INDICATOR_LAMP_PIN 12             // Номер піну індикатора поворотів

#define READ_DELAY 250                    // Затримка у мс читання повторного натиснення (для нейтралізації дребіжжаня)

#define BLINK_DELAY 500                   // Період у мс мигання

#define AUTO_OFF_DELAY 30000              // Затримка автоматичного вимкнення повороту


// Стан увімкнення поворотів
boolean leftTurnEnable = false;           //      ПРАВОГО повороту
boolean rightTurnEnable = false;          //      ЛІВОГО повороту


void setup() {
  // put your setup code here, to run once:

  pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);

  pinMode(LEFT_LAMP_PIN, OUTPUT);
  pinMode(RIGHT_LAMP_PIN, OUTPUT);

  pinMode(INDICATOR_LAMP_PIN, OUTPUT);  

  // Гасимо лампи
  setLampState(LEFT_LAMP_PIN, false);
  setLampState(RIGHT_LAMP_PIN, false);
}

// Вмикає лампу на піні lampPin, у залежності від параметра lampState 
void setLampState(int lampPin, boolean lampState) {

  digitalWrite(lampPin, !lampState); 
  
  digitalWrite(INDICATOR_LAMP_PIN, lampState); 
  
}


boolean getButtonState(int buttonPin) {

  return !digitalRead(buttonPin);
  
}


boolean getLeftButtonState()  {

  static boolean buttonState = false;
  static boolean oldButtonState = false;

  static unsigned long buttonReadNextTime = 0;

  if(millis() >= buttonReadNextTime)  {
    
    buttonState = getButtonState(LEFT_BUTTON_PIN);
    
    if(buttonState != oldButtonState)
      buttonReadNextTime = millis() + READ_DELAY;

    oldButtonState = buttonState;  
    
  }

  return buttonState;
  
}


boolean getRightButtonState()  {

  static boolean buttonState = false;
  static boolean oldButtonState = false;

  static unsigned long buttonReadNextTime = 0;

  if(millis() >= buttonReadNextTime)  {
    
    buttonState = getButtonState(RIGHT_BUTTON_PIN);
    
    if(buttonState != oldButtonState)
      buttonReadNextTime = millis() + READ_DELAY;
      
    oldButtonState = buttonState;  
    
  }

  return buttonState;
  
}


void updateTurnStates()  {

  static boolean oldLeftButtonState = false;
  static boolean oldRightButtonState = false;

  boolean leftButtonState = getLeftButtonState();
  boolean rightButtonState = getRightButtonState();

  static unsigned long autoOffTime = 0;


  boolean bothPressed = leftButtonState && rightButtonState;

  
  if(!oldLeftButtonState && leftButtonState)  {
    leftTurnEnable = !leftTurnEnable;
    rightTurnEnable = bothPressed;
    
    if(leftTurnEnable)
      autoOffTime = millis() + AUTO_OFF_DELAY;
  }
  
  oldLeftButtonState = leftButtonState;


  
  if(!oldRightButtonState && rightButtonState)  {
    rightTurnEnable = !rightTurnEnable;
    leftTurnEnable = bothPressed;

    if(rightTurnEnable)
      autoOffTime = millis() + AUTO_OFF_DELAY;
  }
  
  oldRightButtonState = rightButtonState;


  // Якщо не увімкнуті обидва (не аварійка) - вимкнути
  if(!(leftTurnEnable && rightTurnEnable) && millis() >= autoOffTime)  {
    rightTurnEnable = false; 
    leftTurnEnable = false;
  }

}


void blinkLamps() {

  static boolean leftLampEnable = false;
  static boolean rightLampEnable = false;
  
  static unsigned long lampNextSwitch = 0;


  if(millis() > lampNextSwitch) {

    if(leftTurnEnable && rightTurnEnable)
      rightLampEnable = leftLampEnable;
      
    if(leftTurnEnable)
      leftLampEnable = !leftLampEnable;
    else
      leftLampEnable = false;
  
    if(rightTurnEnable)
      rightLampEnable = !rightLampEnable;
    else
      rightLampEnable = false;

    if(leftTurnEnable || rightTurnEnable)
      lampNextSwitch = millis() + BLINK_DELAY;
  }

  setLampState(LEFT_LAMP_PIN, leftLampEnable);
  setLampState(RIGHT_LAMP_PIN, rightLampEnable);
    
}


void loop() {
  // put your main code here, to run repeatedly:

  updateTurnStates();

  blinkLamps();
  
}
