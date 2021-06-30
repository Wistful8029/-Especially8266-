//基于点灯科技
#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT
#define BLINKER_PRINT Serial
//#include <Arduino.h>
#include <Blinker.h>
#include <Servo.h>  //sg90舵机
#include <DHT.h>  //温度检测模块
Servo myservo1; //舵机1信号   由d1控制
Servo myservo2; //舵机2信号   由d2控制
//////////////////////改这里

char auth[] = "XXXX";   // 点灯科技申请到的设备ID
char ssid[] = "XXXX";   // wlan名称
char pswd[] = "XXXX";    //wlan密码

////////////////////////////////////
char dsad[] = "前进";
const int IN1 = D5;   //d5-d8控制电机  d0控制继电器信号  d1和d2控制舵机
const int IN2 = D6;
const int IN3 = D7;
const int IN4 = D8;
const int IN5 = D3;
const int IN6 = D1;
const int IN7 = D2;
float cm;
int TrigPin = 1;//定义Trig为接口1  接tx
int EchoPin = 3;//定义Echo接口为3  接rx

//float duration;
float distance;
/////////////////////////
//滑块用来无极调节舵机角度（1-180°）
#define Slider_1 "SliderKey1" //滑块1控制舵机1
#define Slider_2 "SliderKey2" //滑块2控制舵机2
BlinkerSlider Slider1(Slider_1);//滑块1
BlinkerSlider Slider2(Slider_2);//滑块2
int i=0,j=0;
//////////////////////////
/*
D5-D8控制两个电机
D1和D2控制两个舵机
D0控制继电器的信号
D4接温度检测模块
 */
// 新建blinker组件对象
//用法：在点灯科技app中新建组件，名称改为“”内容
BlinkerNumber Number1("num-abc"); //显示wifi信号强度
BlinkerNumber HUMI("humi");    //定义湿度数据键名
BlinkerNumber TEMP("temp");    //定义温度数据键名
BlinkerNumber DIST("meter");    //定义距离键名
BlinkerButton Button1("btn");     //控制舵机1    信号接D1
BlinkerButton Button2("btn-1"); //控制舵机2      信号接D2
BlinkerButton Button3("btn-2"); //
//BlinkerButton Button4("btn-3"); //控制激光发射  接SD3
BlinkerButton Button5("btn-4"); //控制继电器    接D3
BlinkerButton Button6("up"); //控制前进  D5-D8
BlinkerButton Button7("down"); //控制后退
BlinkerButton Button8("left"); //控制左转
BlinkerButton Button9("right"); //控制右转
BlinkerButton Button10("stop"); //停止
#define DHTPIN 2   //D4引脚接DHT11的数据线
#define DHTTYPE DHT11   // 使用DHT 11温度湿度模块
DHT dht(DHTPIN, DHTTYPE);    //定义dht
float humi_read = 0, temp_read = 0;
void heartbeat()  //心跳包，回传数据
{
   // HUMI.print(humi_read);        //给blinkerapp回传湿度数据
    //TEMP.print(temp_read);        //给blinkerapp回传温度数据
}
void slider1_callback(int32_t value)  //控制舵机1
{
    BLINKER_LOG("get slider value: ", value);
    i=value;
    Slider1.color("#1E90FF");
    myservo1.write(90-value);         //控制舵机1旋转到i度
}
void slider2_callback(int32_t value)  //控制舵机2
{
    BLINKER_LOG("get slider value: ", value);
    j=value;
    Slider2.color("#1E90FF");
    myservo2.write(90-value);         //控制舵机2旋转到j度
}
// 按下按键即会执行该函数
/*这一段用于替换滑块，若滑块无法控制，则启用该段代码
void button1_callback(const String & state) {   //控制舵机1旋转到左前方
    BLINKER_LOG("get button state: ", state);
    myservo1.write(180);         //控制舵机旋转到180度
    delay(3000);                //延迟3000毫秒
    myservo1.write(0);           //控制舵机旋转到0度
}
void button2_callback(const String & state) {  //控制舵机1旋转到右前方
    BLINKER_LOG("get button state: ", state);
    myservo1.write(180);         //控制舵机旋转到180度
    delay(3000);                //延迟3000毫秒
    myservo2.write(0);           //控制舵机旋转到0度
}
*/
void button3_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    digitalWrite(IN3, !digitalRead(IN3));
}
/*void button4_callback(const String & state) { //控制激光
    BLINKER_LOG("get button state: ", state);
    digitalWrite(LIGHT, !digitalRead(LIGHT));
}*/
void button5_callback(const String & state) {  //控制继电器
    BLINKER_LOG("get button state: ", state);
    digitalWrite(IN5, !digitalRead(IN5));
}
void button6_callback(const String & state) {   //控制前进，需要d5 和d7同时输出高电平，d6和d8输出低电平
    BLINKER_LOG("get button state: ", state);  
    digitalWrite(IN2, LOW);
    digitalWrite(IN4, LOW);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN3, HIGH);
    }
void button7_callback(const String & state) {   // 控制后退  与上相反
    BLINKER_LOG("get button state: ", state);
    digitalWrite(IN1, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN4, HIGH);
}
void button8_callback(const String & state) {   //控制左转  d5和d8输出高电平，d6和d7输出低电平
    BLINKER_LOG("get button state: ", state);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN4, HIGH);
    
}
void button9_callback(const String & state) {   //控制右转  与上相反
    BLINKER_LOG("get button state: ", state);
    digitalWrite(IN1, LOW);
    digitalWrite(IN4, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    
}
void button10_callback(const String & state) {   //控制停止
    BLINKER_LOG("get button state: ", state);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}
// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
}
void setup() {  //该部分代码在8266上电时只执行一次，为初始化部分
    // 初始化串口
    Serial.begin(115200);
    #if defined(BLINKER_PRINT)
        BLINKER_DEBUG.stream(BLINKER_PRINT);
    #endif
    BLINKER_DEBUG.stream(Serial);
    BLINKER_DEBUG.debugAll();
    myservo1.attach(5);            //设置舵机1信号输出引脚编号为5（D1）
    myservo1.write(90);             //控制舵机旋转到90度
    myservo2.attach(4);            //设置舵机2信号输出引脚编号为4（D2）
    myservo2.write(90);             //控制舵机旋转到90度
    //以下是各引脚的初始化
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    pinMode(IN1, OUTPUT);
    digitalWrite(IN1, LOW);
    pinMode(IN2, OUTPUT);
    digitalWrite(IN2, LOW);
    pinMode(IN3, OUTPUT);
    digitalWrite(IN3, LOW);
    pinMode(IN4, OUTPUT);
    digitalWrite(IN4, LOW);
    pinMode(IN5, OUTPUT);
    digitalWrite(IN5, HIGH);
    pinMode(IN6, OUTPUT);
    pinMode(IN7, OUTPUT);

    pinMode(TrigPin,OUTPUT);//设置2为输出接口
  pinMode(EchoPin,INPUT);//设置3为输入接口
    ///////////////////
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Blinker.attachHeartbeat(heartbeat);
    dht.begin();
    //下面这一串表示在点灯科技按键执行的操作
    //Button1.attach(button1_callback);
    //Button2.attach(button2_callback);
    Button3.attach(button3_callback);
//    Button4.attach(button4_callback);
    Button5.attach(button5_callback);
    Button6.attach(button6_callback);
    Button7.attach(button7_callback);
    Button8.attach(button8_callback);
    Button9.attach(button9_callback);
    Button10.attach(button10_callback);
    Slider1.attach(slider1_callback);
    Slider2.attach(slider2_callback);
}
void loop() { //该部分在上电期间会一直执行
    Blinker.run();//用于保持设备间连接和处理数据，调用最频繁
    Number1.print(WiFi.RSSI());  //回传wifi信号强度
    ////////////////////////////////////
    //负责获取环境温度和湿度
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t))
    {
        BLINKER_LOG("Failed to read from DHT sensor!");
    }
    else
    {
        BLINKER_LOG("Humidity: ", h, " %");
        BLINKER_LOG("Temperature: ", t, " *C");
        humi_read = h;
        temp_read = t;
    }
    ////////////////////////////////////
    digitalWrite(TrigPin,LOW);//将2置为低电平
  delay(2);                 //延时
  digitalWrite(TrigPin,HIGH);//将2置为高电平
  delay(10);                 //延时
  digitalWrite(TrigPin,LOW);//将2置为低电平
  cm = pulseIn(EchoPin,HIGH) / 58.0;//计算距离
  cm = (int(cm)) / 100.0;//单位为米
    HUMI.print(humi_read);
    TEMP.print(temp_read);
    DIST.print(cm);
    Blinker.delay(10000);
}
