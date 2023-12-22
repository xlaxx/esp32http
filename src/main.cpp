#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <Wire.h>

#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>

#define OLED_RESET    13

Adafruit_SSD1306 display(128, 32, &Wire, OLED_RESET);

const char* ssid = "CMCC-cmve";
const char* password = "asdfghjkl";

// 定义
String url = "http://47.94.97.70:8080/show";
//String city = "changsha";
//String key = "SophKN_ITTHq0LEyO";
//String language = "zh-Hans";
//String unit = "c";


void setup() {
  Serial.begin(115200);

  // 连接 WiFi
  WiFi.begin(ssid, password);

  Serial.print("正在连接 Wi-Fi");
  
  // 检测是否连接成功
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }

  Serial.println("连接成功");
  Serial.print("IP 地址：");
  Serial.println(WiFi.localIP());

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.setTextColor(WHITE);//开像素点发光

  display.clearDisplay();//清屏S

  //display.setTextSize(1); //设置字体大小

}


void loop() {
  
  // 创建 HTTPClient 对象
  HTTPClient http;

  // 发送GET请求
  //http.begin(url+"?key="+key+"&location="+city+"&language="+language+"&unit="+unit);
  http.begin(url);

  int httpCode = http.GET();

  // 获取响应状态码
  Serial.printf("HTTP 状态码: %d", httpCode);

  // 获取响应正文
  String response = http.getString();
  Serial.println("响应数据");
  Serial.println(response);

  http.end();

  // 创建 DynamicJsonDocument 对象
  DynamicJsonDocument doc(1024);

  // 解析 JSON 数据
  deserializeJson(doc, response);

  // 从解析后的 JSON 文档中获取值
  //unsigned int temp = doc["results"][0]["now"]["temperature"].as<unsigned int>();
  String info = doc["receivedString"].as<String>();

  Serial.printf("白天天气: %s\n", info);

display.setTextSize(1);
  display.clearDisplay();//清屏S

  display.setCursor(0, 0);
  display.println("Web Message:");

display.setTextSize(2);
  // 在第二行显示温度
  display.setCursor(0, 16);
  display.println("" + String(info));

  display.display();

delay(2500);
}
