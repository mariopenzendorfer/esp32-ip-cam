#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "esp_camera.h"
#include "wifi_config.h"

#define MAX_FPS 20

camera_config_t camera_config = {
    .pin_pwdn = 32,
    .pin_reset = -1,
    .pin_xclk = 0,
    .pin_sccb_sda = 26,
    .pin_sccb_scl = 27,
    .pin_d7 = 35,
    .pin_d6 = 34,
    .pin_d5 = 39,
    .pin_d4 = 36,
    .pin_d3 = 21,
    .pin_d2 = 19,
    .pin_d1 = 18,
    .pin_d0 = 5,
    .pin_vsync = 25,
    .pin_href = 23,
    .pin_pclk = 22,

    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,
    .pixel_format = PIXFORMAT_JPEG,
    .frame_size = FRAMESIZE_QVGA,
    .jpeg_quality = 12,
    .fb_count = 2,
    .fb_location = CAMERA_FB_IN_DRAM,
    .grab_mode = CAMERA_GRAB_LATEST,
};

camera_fb_t *fb = NULL;

WebServer server(80);

void handle_mjpeg()
{
  WiFiClient client = server.client();

  const char *BOUNDARY = "\r\n--IMAGE_BOUNDARY\r\n";
  const size_t bdrLen = strlen(BOUNDARY);
  client.write("HTTP/1.1 200 OK\r\n");
  client.write("Content-Type: multipart/x-mixed-replace; boundary=IMAGE_BOUNDARY\r\n");
  client.write(BOUNDARY);

  Serial.println("Starting mjpeg stream");

  while (true)
  {
    if (!client.connected()) break;

    if (fb)
      esp_camera_fb_return(fb);
    fb = esp_camera_fb_get();

    client.write("Content-Type: image/jpeg\r\n\r\n");
    client.write((char *)fb->buf, fb->len);
    client.write(BOUNDARY, bdrLen);

    vTaskDelay((1000 / MAX_FPS) / portTICK_PERIOD_MS);
  }

  Serial.println("Stopping mjpeg stream");
}

void handle_jpg()
{
  WiFiClient client = server.client();

  if (fb)
    esp_camera_fb_return(fb);
  fb = esp_camera_fb_get();

  client.write("HTTP/1.1 200 OK\r\n");
  client.write("Content-Disposition: inline; filename=capture.jpg\r\n");
  client.write("Content-Type: image/jpeg\r\n\r\n");
  client.write((char *)fb->buf, fb->len);

  Serial.println("Sent jpg");
}

void handle_404()
{
  server.send(404, "text / plain", "Use /mjpeg for mjpeg stream or /jpg for single jpg image");
}

void setup()
{
  Serial.begin(115200);

  esp_camera_init(&camera_config);

  Serial.println("Camera initialized");

  IPAddress ip;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("WiFi initializing");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(F("."));
  }
  ip = WiFi.localIP();
  Serial.print("IP: ");
  Serial.println(ip);
  Serial.println("WiFi initialized");

  server.on("/mjpeg", HTTP_GET, handle_mjpeg);
  server.on("/jpg", HTTP_GET, handle_jpg);
  server.onNotFound(handle_404);
  server.begin();
  Serial.println("Server initialized");
}

void loop()
{
  server.handleClient();
}
