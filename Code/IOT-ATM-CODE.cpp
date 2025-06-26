#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#define TEMP_SENSOR A0
#define Password_Length 5
#include <SoftwareSerial.h>
int redLed = 13;
int greenLed = 12;
int idTransit;
int row = 0;
int col = 0;
int x = 0;
long duration;

int motorPin = 10;
SoftwareSerial bluetooth(8, 9);
// for the lcd pins
int rs = 11, en = A1, d4 = A2, d5 = A3, d6 = A4, d7 = A5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

bool isEnteredId;
int status = 0;
int pos = 0;
int dataCount = 0;
String Data;
//set user 
String Masters[4] = {"1111", "2222", "3333", "4444"};
String nameMasters[4] = {"Xuan Giang", "Tan Thanh", "Tuan Anh", "Dinh Tien"};
byte data_count = 0, master_count = 0;
long balances[4] = {10000, 20000, 30000, 40000};
bool Pass_is_good;
bool isSignIn = false;
bool isEnteredAmount;
char customKey;
long amount = 0;
bool isCleared = false;
bool isFirstTime = true;
int WrongPass = 0;
int indexOfUser = 0;

const byte ROWS = 4;
const byte COLS = 4;
//declaring keypad
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte rowPins[ROWS] = {0, 1, 2, 3};
byte colPins[COLS] = {4, 5, 6, 7};
Keypad customKeypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{   
    bluetooth.begin(9600);
    pinMode(motorPin, OUTPUT);
    digitalWrite(motorPin,LOW);
    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    lcd.begin(16, 2);
    lcd.print("Welcome to ATM");
    loading("Loading");

    lcd.clear();
}
//loading first
void loading(char msg[])
{
    lcd.setCursor(0, 1);
    lcd.print(msg);

    for (int i = 0; i < 9; i++) //loading...........
    {
        delay(100);
        lcd.print(".");
    }
}

void clearData()
{
    while (data_count != 0)
    {
        Data[data_count--] = 0;
    }
    return;
}

//LOOP  
void loop()
{
    if (bluetooth.available()) 
    {
        char customKey = bluetooth.read();
        handleKey(customKey); 
    }

    if (isSignIn)
    {
        if (status == 99) 
        {
            showTemp(); // Hiển thị nhiệt độ liên tục
        }
        else 
        {
            // Các trạng thái khác của chương trình
            if (status == 1) 
                status1(indexOfUser);
            else if (status == 2)
                status2();
            else if (status == 3)
                status3();
            else if (status == 7)
                deposit(indexOfUser);
            else if (status == 11)
                withdraw(indexOfUser);
            else if (status == 17)
                transit(indexOfUser);
        }
    }
    else 
    {
        SignIn();
    }
}
 
//BLUETOOTH

void handleKey(char key)
{
    switch (key)
    {
        case '1': case '2': case '3': case 'A':
        case '4': case '5': case '6': case 'B':
        case '7': case '8': case '9': case 'C':
        case '*': case '0': case '#': case 'D':
            if (!isSignIn) {
                SignIn(); // Nếu chưa đăng nhập, xử lý nhập mật khẩu
            } else {
                updateStatus(key);  // Nếu đã đăng nhập, chuyển trạng thái
            }
            break;
    }
}
void updateStatus(char key)
{
    switch (key)
    {
        case 'A':
            status = 7;  // Đổi trạng thái để loop() xử lý nạp tiền
            break;
        case 'B':
            status = 11; // Đổi trạng thái để loop() xử lý rút tiền
            break;
        case 'C':
            status = 17; // Đổi trạng thái để loop() xử lý chuyển khoản
            break;
        case '*':
            status = 99; // Hiển thị nhiệt độ ngay lập tức
            showTemp();
            break;
        case 'D':
            lcd.clear();
            loading("Logging out...");
            delay(2000); 
            isSignIn = false; // Đánh dấu là chưa đăng nhập
            indexOfUser = -1; // Xóa thông tin người dùng
            status = 0;  // Quay về trạng thái ban đầu
            data_count = 0;
            Data = "";
            lcd.clear();
            break;
        default:
            
            break;
    }
}



//XỬ LÝ ĐĂNG NHẬP
void SignIn() // Cho phép nhập ID và mật khẩu từ cả Bluetooth và bàn phím vật lý
{
    bluetooth.flush(); // xóa bộ nhớ đệm của bluetooth
    
    lcd.clear();
    lcd.setCursor(0, 0);
    
    if (WrongPass == 3) // Nếu nhập sai 3 lần, khóa thẻ và thoát
    {
        lcd.print("Card is Locked");
        digitalWrite(redLed, HIGH);
        tone(redLed, 500, 2500);
        digitalWrite(redLed, LOW);
        delay(2000);
        return;
    }

    lcd.print("Enter ID:");
    String enteredID = "";
    int idIndex = -1;

    while (true) // Vòng lặp nhập ID (cho phép nhập từ cả Bluetooth và bàn phím)
    {
        char key = getKeyInput(); // Nhận phím từ Bluetooth hoặc bàn phím vật lý
        if (key >= '0' && key <= '9')
        {
            enteredID += key;
            lcd.setCursor(enteredID.length() - 1, 1);
            lcd.print(key);
        }

        if (enteredID.length() == 1) // Chỉ cho nhập 1 số (ID từ 0 - 3)
        {
            idIndex = enteredID.toInt(); // Chuyển ID sang số nguyên

            if (idIndex >= 0 && idIndex < 4) // Kiểm tra ID hợp lệ
            {
                lcd.clear();
                lcd.print("ID found: ");
                lcd.setCursor(4, 1);
                lcd.print(nameMasters[idIndex]);
                delay(2300);
                lcd.clear();
                lcd.print("Enter Password:");
                break; // Thoát vòng lặp nhập ID, sang nhập mật khẩu
            }
            else
            {
                lcd.clear();
                lcd.print("ID Not Exist!");
                delay(1500);
                lcd.clear();
                lcd.print("Enter ID:");
                enteredID = ""; // Reset ID nhập vào
            }
        }
    }

    // --- Nhập mật khẩu từ cả Bluetooth và bàn phím vật lý ---
    Data = "";
    data_count = 0;

    while (true) // Vòng lặp nhập mật khẩu
    {
        char customKey = getKeyInput(); // Nhận phím từ Bluetooth hoặc bàn phím
        if (customKey >= '0' && customKey <= '9')
        {
            Data += customKey;
            lcd.setCursor(data_count, 1);
            lcd.print(Data[data_count]);
            delay(200);
            lcd.setCursor(data_count, 1);
            lcd.print("*");
            data_count++;
        }

        if (data_count == Password_Length - 1) // Khi nhập đủ mật khẩu
        {
            if (Data == Masters[idIndex]) // So sánh với mật khẩu của ID nhập vào
            {
                lcd.clear();
                lcd.print("    Welcome");
                lcd.setCursor(4, 1);
                lcd.print(nameMasters[idIndex]);
                isSignIn = true;
                WrongPass = 0; // Reset số lần nhập sai

                digitalWrite(greenLed, HIGH);
                tone(greenLed, 900, 1000);
                delay(2000);
                digitalWrite(greenLed, LOW);

                indexOfUser = idIndex;
                status++;
                return;
            }
            else
            {
                WrongPass++; // Tăng số lần nhập sai
                lcd.clear();
                lcd.print("Wrong Password!");
                digitalWrite(redLed, HIGH);
                tone(redLed, 400 + WrongPass * 100, 500);
                digitalWrite(redLed, LOW);
                delay(1000);

                if (WrongPass == 3) // Nếu sai 3 lần, khóa thẻ
                {
                    lcd.clear();
                    lcd.print("Card is Locked");
                    digitalWrite(redLed, HIGH);
                    tone(redLed, 500, 2500);
                    digitalWrite(redLed, LOW);
                    delay(2000);
                    return; // Thoát khỏi hàm, không cho nhập tiếp
                }

                // Nếu chưa sai 3 lần, yêu cầu nhập lại mật khẩu
                lcd.clear();
                lcd.print("Enter Password:");
                Data = "";
                data_count = 0;
            }
        }
    }
}

// Hàm lấy dữ liệu từ Bluetooth hoặc bàn phím
char getKeyInput() {
    // Kiểm tra nếu có dữ liệu từ Bluetooth (HC-05)
    if (bluetooth.available() > 0) {
        return bluetooth.read();
    }

    // Kiểm tra nếu có phím từ bàn phím vật lý
    char key = customKeypad.getKey();
    if (key) return key;

    return '\0'; // Không có phím nhấn
}


//TRẠNG THÁI
void status1(int index) // Money orginal
{
    lcd.clear();
    lcd.print("Account balance:");
    lcd.setCursor(0, 1);
    lcd.print(balances[index]);
    delay(5000);
    status++;
}

void status2() // 4 choices: Deposit, Withdraw, Transit, Exit
{
    lcd.clear();
    lcd.print("A.Depo ");

    lcd.setCursor(8, 0);
    lcd.print("C.Trans");
    lcd.setCursor(0, 1);
    lcd.print("B.Withd");
    lcd.setCursor(8, 1);
    lcd.print("D.Exit"); // Thêm lựa chọn D. Exit
    status++;
}

void status3() // Get the input key
{
    customKey = customKeypad.getKey();
    if (customKey)
    {
        if (customKey == 'A')
        {
            isFirstTime = true;
            status = 7; // Deposit
        }
        else if (customKey == 'B')
        {
            isFirstTime = true;
            status = 11; // Withdraw
        }
        else if (customKey == 'C')
        {
            isFirstTime = true;
            status = 17; // Transit
        }
        else if (customKey == 'D') 
        {
            lcd.clear();
            loading("Logging out");
            delay(2000); // Hiển thị "Logging..." trong 2 giây
            isSignIn = false; // Đánh dấu chưa đăng nhập
            status = 0; // Quay về trạng thái đăng nhập ban đầu
            data_count = 0; // Reset dữ liệu nhập
            Data = ""; // Xóa dữ liệu nhập trước đó
            lcd.clear(); // Xóa màn hình để hiển thị lại giao diện đăng nhập
        }
        else if (customKey == '*') // Nhấn * để hiển thị nhiệt độ
        {
            status = 99; // Trạng thái đặc biệt để hiển thị nhiệt độ
        }
    }
}
//SHOW NHIỆT ĐỘ
void showTemp() 
{
    status = 99; // Đánh dấu trạng thái đang hiển thị nhiệt độ
    lcd.clear();

    while (status == 99) 
    {
        // Đọc nhiệt độ từ cảm biến LM35
        int sensorValue = analogRead(TEMP_SENSOR); 
        float voltage = sensorValue * (5.0 / 1023.0); 
        float temperatureC = voltage * 100.0; 

        // Hiển thị nhiệt độ trên LCD
        lcd.setCursor(0, 0);
        lcd.print("Temp: ");
        lcd.print(temperatureC);
        lcd.print(" C  "); // Dấu cách để xóa số dư
        
        lcd.setCursor(0, 1);
        lcd.print("Press C to back ");

        // Điều khiển động cơ nếu nhiệt độ > 30C
        if(temperatureC > 30)
        {
          digitalWrite(motorPin,HIGH);
        }else{
          digitalWrite(motorPin,LOW);
        }
        
        delay(400); // Cập nhật mỗi 500ms

        // Kiểm tra phím bấm từ bàn phím vật lý và Bluetooth
        char key = customKeypad.getKey(); 
        
        if (bluetooth.available()) 
        {
            key = bluetooth.read(); 
        }

        // Nếu nhấn 'C', thoát khỏi chế độ hiển thị nhiệt độ
        if (key == 'C') 
        {
            lcd.clear();
            status = 2; // Quay lại menu chính
            return;
        }
    }
}



//XỬ LÝ NẠP CHUYỂN RÚT
// Có 2 cách xử lý là sau khi thực hiện giao dịch xong đưa chúng ta về màn menu chọn hình thức giao dịch tiếp theo hoặc sử dụng goOn để tiếp tục chọn A để tiếp tục chọn B để thoát
char readCustomKey() {
    if (bluetooth.available() > 0) {  
        char key = bluetooth.read();
        while (bluetooth.available()){
          bluetooth.read(); // Xóa buffer để tránh lag
        }
        return key;
    } 
    return customKeypad.getKey(); 
}

void deposit(int index) {
    if (isFirstTime) {
        lcd.clear();
        lcd.print("Deposit Amount:");
        isFirstTime = false;
        isEnteredAmount = false;
        amount = 0;
        dataCount = 0;
    }

    customKey = readCustomKey();

    if (!isEnteredAmount) {
        if (customKey >= '0' && customKey <= '9') {
            amount = amount * 10 + (customKey - '0');
            lcd.setCursor(dataCount, 1);
            lcd.print(customKey);
            dataCount++;
        }

        if (customKey == '#') {  // Xác nhận nạp tiền
            balances[index] += amount;
            lcd.clear();
            lcd.print("Deposit Done!");
            lcd.setCursor(0, 1);
            lcd.print("New Bal: ");
            lcd.print(balances[index]);
            delay(2000);
            
            // Reset biến trạng thái
            amount = 0;
            dataCount = 0;
            isEnteredAmount = false;
            isFirstTime = true;
            status = 1; // Quay lại menu chính
        } 
        else if (customKey == 'C') { // Hủy giao dịch
            amount = 0;
            dataCount = 0;
            isEnteredAmount = false;
            isFirstTime = true;
            status = 2;
        }
    }

    
}


void withdraw(int index) {
    if (isFirstTime) {
        lcd.clear();
        lcd.print("Withdraw Amount:");
        isFirstTime = false;
        isEnteredAmount = false;
        amount = 0;
        dataCount = 0;
    }

    customKey = readCustomKey();

    if (!isEnteredAmount) {
        if (customKey >= '0' && customKey <= '9') {
            amount = amount * 10 + (customKey - '0');
            lcd.setCursor(dataCount, 1);
            lcd.print(customKey);
            dataCount++;
        }

        if (customKey == '#') {  // Xác nhận rút tiền
            if (amount > balances[index]) {
                lcd.clear();
                lcd.print("Not Enough!");
                delay(2000);
            } else {
                balances[index] -= amount;
                lcd.clear();
                lcd.print("Withdraw Done!");
                lcd.setCursor(0, 1);
                lcd.print("New Bal: ");
                lcd.print(balances[index]);
                delay(2000);
            }

            // Reset biến trạng thái
            amount = 0;
            dataCount = 0;
            isEnteredAmount = false;
            isFirstTime = true;
            status = 1; 
        } 
        else if (customKey == 'C') { // Hủy giao dịch
            amount = 0;
            dataCount = 0;
            isEnteredAmount = false;
            isFirstTime = true;
            status = 2;
        }
    }
}


void transit(int index) 
{
    if (isFirstTime)
    {
        lcd.clear();
        lcd.print("Enter Id:");
        isFirstTime = false;
        isEnteredId = false;
    }
    if (!isEnteredId) //not enter id
    {
        customKey = readCustomKey();
        if (customKey >= '0' && customKey <= '9')
        {
            lcd.setCursor(0, 1);
            lcd.print(customKey);
            delay(500);
            if (customKey == '0' || customKey == '1' || customKey == '2' || customKey == '3')
            {

                if (int(customKey) - 48 == index) //ascii, check enter id is current user
                {
                    lcd.clear();
                    lcd.print("Invalid Id");
                    delay(5000);
                    status=2;
                    return;
                }
                else
                {
                    lcd.clear();
                    isEnteredId = true;
                    isEnteredAmount = false;
                    idTransit = int(customKey) - 48; //user receive money
                    lcd.print(isEnteredId);
                }
            }
            else
            {
                lcd.clear();
                lcd.print("Wrong Id");
                delay(5000);
                status = 2;
                return;
            }
        }
    }
    if (isEnteredId) //id valid
    {
        if (!isEnteredAmount) //not enter amount
        {
            lcd.clear();
            lcd.print("Enter Amount:");
            isEnteredAmount = true;
        }
        customKey = readCustomKey();
        if (customKey >= '0' && customKey <= '9')
        {
            amount = amount * 10 + customKey - '0';
            lcd.setCursor(dataCount, 1);
            lcd.print(customKey);
            dataCount++;
        }
        if (amount > balances[index]) //check enough
        {
            lcd.clear();
            lcd.print("Not Enough");
            delay(5000);
            amount = 0;
            status=2; 
            return;
        }
        else //enough
        {
            if (customKey == '#') //handle transit
            {
                balances[idTransit] += amount;
                balances[index] -= amount;
                lcd.clear();
                lcd.print("Trans Done!");
                lcd.setCursor(0, 1);
                lcd.print("New Bal: ");
                lcd.print(balances[index]);
                delay(2000);
                amount = 0;
                dataCount = 0;
                status = 1; 
                delay(3000);
                isEnteredId = false;
                isFirstTime = true;
            }
            if (customKey == 'C') //return status 2
            {
                amount = 0;
                dataCount = 0;
                status = 2;
                isFirstTime = true;
            }
        }
    }
}
